using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GJTalkServer
{
    class UserAuth
    {
        Dictionary<string, long> idmap = new Dictionary<string, long>();

        public static string EncryptPassword(string password)
        {
            if (password == null)
                return null;
            return Crypto.Md5(Crypto.Md5(password) + password);
        }

        public long GetUserId(string username)
        {
            long id = -1;
            if (string.IsNullOrEmpty(username))
            {
                username = username.ToLower();
                if (!idmap.TryGetValue(username, out id))
                {
                    try
                    {
                        using (SqlDataContext dc = new SqlDataContext())
                        {
                            var user = dc.DatabaseUserItems.Where(x => string.Compare(x.Username, username, true) == 0 || string.Compare(x.Mail,
                                username, true) == 0).Select(x => new { x.Id, x.Mail, x.Username }).Single();
                            idmap.Add(user.Username.ToLower(), user.Id);
                            if (!string.IsNullOrEmpty(user.Mail))
                                idmap.Add(user.Mail.ToLower(), user.Id);
                        }
                    }
                    catch { }
                }
            }
            return id;
        }

        public User GetUser(string username)
        {
            if (string.IsNullOrEmpty(username))
                return null;
            try
            {
                using (SqlDataContext dc = new SqlDataContext())
                {
                    var user = dc.DatabaseUserItems.Single(x => string.Compare(x.Username, username, true) == 0
                || string.Compare(x.Mail, username) == 0);
                    return new User()
                    {
                        UserId = user.Id,
                        Username = user.Username,
                        Nickname = user.Nickname,
                        HeaderPic = user.HeaderImage,
                        Signature = user.Signature,
                        Status = UserOnlineStatus.Hidden,
                        Mail = user.Mail
                    };
                }
            }
            catch { return null; }

        }
        public string GetUserpassword(string username)
        {
            if (string.IsNullOrEmpty(username))
                return null;
            try
            {
                using (SqlDataContext dc = new SqlDataContext())
                {
                    return dc.DatabaseUserItems.Where(x => string.Compare(username, x.Username, true) == 0
                        || string.Compare(username, x.Mail, true) == 0)
                        .Select(x => x.Password).Single();
                }
            }
            catch { return null; }
        }
        public bool Auth(string username, string password)
        {
            return Auth(username, password, true);
        }
        public bool Auth(string username, string password, bool passwordEncrypted)
        {
            string userPassword = GetUserpassword(username);
            if (userPassword == null)
                return false;
            else if (passwordEncrypted)
                return EncryptPassword(userPassword) == password;
            else
                return userPassword == password;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GJTalkServer
{
    class UserAuth
    {


        public static string EncryptPassword(string password)
        {
            if (password == null)
                return null;
            return Crypto.Md5(Crypto.Md5(password) + password);
        }
        public User GetUser(string username)
        {
            return new User()
            {
                UserId = 0,
                Username = username,
                Nickname = "Test",
                HeaderPic = "test.png",
                Signature = "this is a test",
                Status = UserOnlineStatus.Hidden
            };
        }
        public string GetUserpassword(string username)
        {
            return "test";
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

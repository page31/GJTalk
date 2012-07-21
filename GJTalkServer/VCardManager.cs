using System;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using Matrix;
using Matrix.Xmpp.Vcard;

namespace GJTalkServer
{
    class VCardManager
    {
        private static VCardManager _instance;
        public static VCardManager Instance
        {

            get { if (_instance == null) _instance = new VCardManager(); return _instance; }
        }
        public Vcard Get(string username)
        {
            Vcard vcard = null;
            using (SqlDataContext dc=new SqlDataContext())
            {
                try
                {
                    var user = dc.DatabaseUserItems.Single(x => string.Compare(username, x.Username, true) == 0);
                    var data=HeaderManager.Load(username);
                    vcard = new Vcard();
                    vcard.Nickname = user.Nickname;
                    vcard.Description = user.Signature;
                    vcard.Jid = JIDEscaping.Escape(user.Username) + "@gjtalk.com";
                    vcard.AddEmail(new Email(user.Mail));
                    if (data != null)
                        vcard.Photo = new Photo(data, ImageFormat.Png);
                }
                catch (System.Exception ex)
                {
                	
                }
            }
            return vcard;
        }
        public Vcard Get(Jid jid)
        {
            if (jid == null)
                return null;
            return Get(JIDEscaping.Unescape(jid.User));
        }
        public bool Set(Jid jid, Vcard vcard)
        {
            return Set(JIDEscaping.Unescape(jid), vcard);
        }
        public bool Set(string username, Vcard vcard)
        {
            using (SqlDataContext dc=new SqlDataContext())
            {
                try
                {
                    var user = dc.DatabaseUserItems.Single(x => string.Compare(username, x.Username, true) == 0);
                    user.Nickname = vcard.Nickname;
                    user.Signature = vcard.Description;
                    
                }
                catch (System.Exception ex)
                {
                	return false;
                }
            }
            return true;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Matrix;

namespace GJTalkServer
{
    enum UserOnlineStatus
    {
        Available,
        Hidden,
        Busy,
        Mute,
        Away
    }
    class User
    {

        public long UserId { get; set; }
        public string Username { get; set; }
        public string Nickname { get; set; }
        public string HeaderPic { get; set; }
        public string Signature { get; set; }
        public UserOnlineStatus Status { get; set; }
        public Jid GetJid()
        {
            return new Jid(Username, "gjtalk.com", "");
        }

    }
}

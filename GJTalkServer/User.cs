using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GJTalkServer
{
    class User
    {
        public long UserId { get; set; }
        public string Username { get; set; }
        public string Nickname { get; set; }
        public string HeaderPic { get; set; }
        public string Signature { get; set; }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GJTalkServer
{
    class AddFriendRequest
    {
        public DateTime Time { get; set; }
        public string OwnerUsername { get; set; }
        public string FriendUsername { get; set; }
        public string GroupName { get; set; }
        public string Remark { get; set; } 
        public string Message { get; set; }
        
    }
}

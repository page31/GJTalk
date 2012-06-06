using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GJTalkServer
{
    class BuddyGroup
    {
        public string User { get; set; }
        public string GroupName { get; set; }
        public List<BuddyItem> Buddies { get; set; }
        public BuddyGroup()
        {
            Buddies = new List<BuddyItem>();
        }
    }
}

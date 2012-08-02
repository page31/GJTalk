using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Matrix.Xmpp.Roster;

namespace GJTalkServer
{
    class BuddyItem
    {
        public string Username { get; set; }
        public string Nickname { get; set; }
        public string Remark { get; set; }
        public string GroupName { get; set; }

        public static implicit operator RosterItem(BuddyItem buddyItem)
        {
            return buddyItem.ToRosterItem();
        }

        public RosterItem ToRosterItem()
        {
            return new RosterItem(JIDEscaping.Escape(Username) + "@gjtalk.com",
                Remark == null ? Nickname : Remark, GroupName);
        }
    }
}

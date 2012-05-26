using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Matrix.Xmpp.Base;

namespace GJTalkServer
{
    class MessageMangaer
    {
        GJTalkServer server;
        public MessageMangaer(GJTalkServer server)
        {
            this.server = server;
        }
        public bool HandleMessage(Session session,Message message)
        {

            return true;
        }
    }
}

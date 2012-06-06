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
        public bool HandleMessage(Session session, Message message)
        {
            var destSession = server.SessionManager.GetSession(message.To.User);
            if (destSession != null)
            {
                try
                {
                    destSession.Send(message);
                    return true;
                }
                catch { }
            } //destuser not online or send failed.
            server.OfflineMessageManager.Put(message.To.User, new Message[] { message }); //put to offline message 
            return true;
        }
    }
}

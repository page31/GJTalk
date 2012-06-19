using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Matrix.Xml;
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
            string destUser = message.To.User;
            if (string.IsNullOrEmpty(destUser))
                return false;
            if (!DeliverMessage(destUser, message))
                server.OfflineMessageManager.Put(destUser, new Message[] { message }); //put to offline message 
            return true;
        }
        public bool DeliverMessage(long to, XmppXElement element)
        {
            var session = server.SessionManager.GetSession(to);
            if (session != null)
                session.Send(element);
            else
                return false;
            return true;
        }
        public bool DeliverMessage(string to, XmppXElement element)
        {
            var session = server.SessionManager.GetSession(to);
            if (session != null)
                session.Send(element);
            else
                return false;
            return true;
        }
    }
}

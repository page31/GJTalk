using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GJTalkServer
{
    class SessionManager
    {
        Dictionary<string, Session> onlineUsers;
        LinkedList<Session> connectingUsers;
        GJTalkServer server;
        public SessionManager(GJTalkServer server)
        {
            onlineUsers = new Dictionary<string, Session>();
            connectingUsers = new LinkedList<Session>();
            this.server = server;
        }
        public Session Add(Session session)
        {
            connectingUsers.AddFirst(session);
            return session;
        }
        public bool IsOnline(string username)
        {
            return onlineUsers.ContainsKey(username.ToLower());
        }
        public Session AddConnected(Session session)
        {
            try
            {
                connectingUsers.Remove(session);
            }
            finally
            {
            }
            onlineUsers.Add(session.SessionUser.Username.ToLower(), session);
            return session;
        }
    }
}

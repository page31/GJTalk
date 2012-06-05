using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GJTalkServer
{
    class SessionManager
    {
        Dictionary<string, Session> onlineUsers;
        GJTalkServer server;
        object objLock = new object();
        public SessionManager(GJTalkServer server)
        {
            onlineUsers = new Dictionary<string, Session>();
            this.server = server;
        }
        public Session GetSession(string username)
        {
            lock (objLock)
            {
                Session session;
                if (onlineUsers.TryGetValue(username.ToLower(), out session))
                    return session;
                else
                    return null;
            }
        }
        public bool IsOnline(string username)
        {
            lock (objLock)
            {
                return onlineUsers.ContainsKey(username.ToLower());
            }
        }
        public Session Add(Session session)
        {
            lock (objLock)
            {
                onlineUsers.Add(session.SessionUser.Username.ToLower(), session);
                return session;
            }
        }
        public void Remove(Session session)
        {
            lock (objLock)
            {
                if (session.SessionUser != null)
                    onlineUsers.Remove(session.SessionUser.Username.ToLower());
            }
        }
    }
}

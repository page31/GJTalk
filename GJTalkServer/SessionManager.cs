using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GJTalkServer
{
    class SessionManager
    {
        Hashtable onlineUsers = new Hashtable();
        GJTalkServer server;
        object objLock = new object();
        public SessionManager(GJTalkServer server)
        {
            this.server = server;
        }
        public Session GetSession(long UserId)
        {
            lock (objLock)
            {
                return onlineUsers[UserId] as Session;
            }
        }
        public Session GetSession(string username)
        {
            if (username == null)
                return null;
            lock (objLock)
            {
                long userId = server.AuthManager.GetUserId(username);
                return GetSession(userId);
            }
        }
        public bool IsOnline(string username)
        {
            lock (objLock)
            {
                return IsOnline(server.AuthManager.GetUserId(username));
            }
        }
        public bool IsOnline(long userId)
        {
            return onlineUsers.ContainsKey(userId);
        }
        public Session Add(Session session)
        {
            if (session.SessionUser == null)
                return null;
            lock (objLock)
            {
                onlineUsers.Add(session.SessionUser.UserId, session);
                return session;
            }
        }
        public void Remove(Session session)
        {
            lock (objLock)
            {
                if (session.SessionUser != null)
                    onlineUsers.Remove(session.SessionUser.UserId);
            }
        }
    }
}

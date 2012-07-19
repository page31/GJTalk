using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using Matrix;
using Matrix.Xml;
using Matrix.Xmpp;
using Matrix.Xmpp.Component;
using Matrix.Xmpp.Sasl;
using Matrix.Xmpp.Stream;
using MxAuth = Matrix.Xmpp.Sasl.Auth;
using Xmpp = Matrix.Xmpp;
namespace GJTalkServer
{
    enum SessionOfflineReason
    {
        ServerDown,
        MultiLogin,
        Force
    }
    enum DataSentAction
    {
        Close,
        None
    }
    class Session
    {
        public User SessionUser { get; private set; }
        public Xmpp.Roster.Roster Roster { get; private set; }
        public bool IsOnline { get { return SessionUser != null; } }
        public List<BuddyGroup> Groups { get; private set; }
        public Jid Jid
        {
            get
            {
                if (SessionUser == null)
                    return null;
                return SessionUser.Username + "@gjtalk.com";
            }
        }

        GJTalkServer server;
        XmppStreamParser streamParser;
        Socket socket;
        AsyncCallback readCallback;
        Xmpp.Client.Presence presence;


        byte[] buffer;
        const int buff_size = 1024;

        public Session(GJTalkServer server, Socket socket)
        {
            this.server = server;
            this.socket = socket;
            // server.SessionManager.Add(this);
            readCallback = new AsyncCallback(OnRead);

            buffer = new byte[buff_size];
            streamParser = new XmppStreamParser();
            streamParser.OnError += streamParser_OnError;
            streamParser.OnStreamElement += streamParser_OnStreamElement;
            streamParser.OnStreamEnd += streamParser_OnStreamEnd;
            streamParser.OnStreamStart += streamParser_OnStreamStart;
            BeginRead();
            Roster = new Xmpp.Roster.Roster();
            Groups = new List<BuddyGroup>();
            Console.WriteLine("New Session");

        }


        void BeginRead()
        {
            if (socket == null)
                return;
            try
            {
                socket.BeginReceive(buffer, 0, buff_size, SocketFlags.None, readCallback, null);
            }
            catch
            {
                Close();
            }
        }
        public void Send(string data)
        {
            Send(data, null);
        }
        public void Send(string data, DataSentAction action)
        {
            Send(data, (object)action);
        }
        public void Send(string data, object userState)
        {
            if (string.IsNullOrEmpty(data) || socket == null)
                return;
            byte[] buffer = Encoding.UTF8.GetBytes(data);
            lock (socket)
            {
                try
                {
                    socket.BeginSend(buffer, 0, buffer.Length,
                         SocketFlags.None, OnSent, userState);
                }
                catch (System.Exception ex)
                {
                    Close();
                }

            }
        }
        public void Send(XmppXElement element)
        {
            Send(element.ToString());
        }

        public void Send(XmppXElement element, DataSentAction action)
        {
            Send(element.ToString(), action);
        }
        public void Send(XmppXElement element, object userState)
        {
            Send(element.ToString(), userState);
        }
        public void Reset()
        {
            streamParser.Reset();
        }
        void OnRead(IAsyncResult result)
        {
            int size = 0;
            try
            {
                size = socket.EndReceive(result);
            }
            catch { }
            if (size > 0)
            {
                string str = Encoding.UTF8.GetString(buffer, 0, size);
                Console.WriteLine(str);
                streamParser.Write(buffer, 0, size);
                BeginRead();
            }
            else
            {
                Close();
            }

        }
        void OnSent(IAsyncResult result)
        {
            try
            {
                int size = socket.EndSend(result);
                if (size == 0)
                {
                    Close();
                    return;
                }
            }
            catch
            {
                Close();
                return;
            }
            if (result.AsyncState != null)
            {
                if (result.AsyncState is DataSentAction)
                {
                    switch ((DataSentAction)result.AsyncState)
                    {
                        case DataSentAction.None:
                            break;
                        case DataSentAction.Close:
                            Close();
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        public void Stop()
        {
            Send("</stream:stream>", DataSentAction.Close);
        }
        public void Close()
        {
            SetPresence(PresenceType.unavailable, null, 0);

            if (socket == null)
                return;
            server.SessionManager.Remove(this);
            try
            {
                socket.Shutdown(SocketShutdown.Both);
            }
            catch { }
            socket.Close();
            socket = null;
        }
        void streamParser_OnStreamStart(object sender, Matrix.StanzaEventArgs e)
        {
            SendStreamHeader();
            //Send(new Matrix.Xmpp.Tls.StartTls());
            Send(BuildFeatures());
        }

        void streamParser_OnStreamEnd(object sender, Matrix.EventArgs e)
        {
            Close();
        }

        void streamParser_OnStreamElement(object sender, Matrix.StanzaEventArgs e)
        {
            server.PacketManager.Enqueue(new Packet()
            {
                Session = this,
                Stanza = e.Stanza,
                State = e.State,
                Server = server
            });
        }

        void streamParser_OnError(object sender, Matrix.ExceptionEventArgs e)
        {
            Stop();
        }

        void SendStreamHeader()
        {
            var stream = new Stream()
            {
                Version = "1.0",
                From = server.Domain,
                Id = Guid.NewGuid().ToString()
            };
            Send(stream.StartTag());
        }
        XmppXElement BuildFeatures()
        {
            var feat = new StreamFeatures();
            if (!IsOnline)
            {
                var mechs = new Mechanisms();
                mechs.AddMechanism(SaslMechanism.PLAIN);
                feat.Mechanisms = mechs;
            }
            else
            {
                feat.Bind = new Xmpp.Bind.Bind();
                feat.Session = new Xmpp.Session.Session();
            }

            return feat;
        }

        public void Offline(SessionOfflineReason reason)
        {
            server.SessionManager.Remove(this);
            Send(new Message(SessionUser.GetJid())
            {
                Subject = "ServerMessage",
                Body = "Offline:" + reason.ToString()
            });
            Stop();
        }
        public void UpdateSubscribers(XmppXElement element)
        {
            foreach (var item in Roster.GetRoster())
            {
                if (item.Subscription == Xmpp.Roster.Subscription.to
                    || item.Subscription == Xmpp.Roster.Subscription.both)
                {
                    server.MessageManager.DeliverMessage(item.Jid.User, element);
                }
            }
        }
        private void AddBuddyGroup(BuddyGroup group)
        {
            lock (Groups)
            {
                lock (Roster)
                {
                    BuddyGroup destGroup = group;
                    for (int i = 0; i < Groups.Count; i++)
                    {
                        if (group.GroupName == Groups[i].GroupName)
                        {
                            destGroup = Groups[i];
                            break;
                        }
                    }
                    foreach (var item in group.Buddies)
                    {
                        destGroup.Buddies.Add(item);
                        Roster.AddRosterItem(
                            new Xmpp.Roster.RosterItem(item.Username, item.Nickname, destGroup.GroupName));
                    }
                }
            }
        }
        public void SetPresence(Matrix.Xmpp.PresenceType type, string status, int priority)
        {
            if (this.Jid == null)
                return;
            this.presence = new Xmpp.Client.Presence();
            presence.Type = type;
            presence.Status = status;
            presence.Priority = priority;
            presence.From = this.Jid;
            var friends = FriendshipManager.Instance.GetAllBuddy(SessionUser.Username);
            foreach (var friend in friends)
            {
                var session = server.SessionManager.GetSession(friend.Username);
                if (session != null && session.Jid != null)
                {
                    presence.To = session.Jid;
                    session.Send(presence);
                }
            }
        }
        public Xmpp.Client.Presence GetPresence()
        {
            return presence;
        }
        public void SetOnline(string username)
        {
            var user = server.AuthManager.GetUser(username);
            lock (server.SessionManager)
            {
                Session oldSession = server.SessionManager.GetSession(user.UserId);
                if (oldSession != null)
                    oldSession.Offline(SessionOfflineReason.MultiLogin);
            }
            this.SessionUser = user;
            server.SessionManager.Add(this);
            Groups.AddRange(FriendshipManager.Instance.GetAllBuddyInGroup(user.Username));
            var offlineMsgs = server.OfflineMessageManager.Get(user.Username, true);
            if (offlineMsgs != null)
            {
                foreach (var msg in offlineMsgs)
                {
                    Send(msg);
                }
            }
        }
    }

}

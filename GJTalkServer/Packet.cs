using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Matrix.Xml;
using Matrix.Xmpp;
using Matrix.Xmpp.Roster;
using Matrix.Xmpp.Sasl;
using Matrix.Xmpp.Stream;
using XmppBase = Matrix.Xmpp.Base;
using MxAuth = Matrix.Xmpp.Sasl.Auth;
using XData = Matrix.Xmpp.XData;
using XmppClient = Matrix.Xmpp.Client;
using Matrix.Xmpp.Bind;
using Matrix.Xmpp.Client;
namespace GJTalkServer
{
    class Packet
    {
        public DateTime Time { get; set; }
        public Session Session { get; set; }
        public object State { get; set; }
        public XmppXElement Stanza { get; set; }
        public GJTalkServer Server { get; set; }
        public Packet()
        {
            Time = DateTime.Now;
        }

        void ProcessRosterIq(XmppBase.Iq iq)
        {

            if (iq.Type == IqType.get)
                ProcessGetRoster(iq);
            else if (iq.Type == IqType.set)
                ProcessSetRoster(iq);


        }
        void ProcessSessionIq(XmppBase.Iq iq)
        {
            iq.Type = IqType.result;
            iq.SwitchDirection();
            Session.Send(iq);
        }
        void ProcessVCardIq(XmppBase.Iq iq)
        {
            if (iq.Type == IqType.get)
            {
                iq.Type = IqType.result;
                Matrix.Xmpp.Vcard.Vcard vc = iq.Query as Matrix.Xmpp.Vcard.Vcard;
                var vcard = VCardManager.Instance.Get(iq.To);
                if (vcard != null)
                {

                    foreach (var mail in vcard.GetEmails())
                    {
                        vc.AddEmail(mail);
                    }
                    vc.Nickname = vcard.Nickname;
                    vc.Jid = vcard.Jid;
                    vc.Description = vcard.Description;
                    vc.Name = vcard.Name;
                    vc.Fullname = vcard.Fullname;
                    vc.Birthday = vcard.Birthday;
                    vc.Photo = vcard.Photo;
                    vc.Title = vcard.Title;
                    vc.Url = vcard.Url;
                }
            }
            else if (iq.Type == IqType.set)
            {
                iq.Type = IqType.result;

            }
            else
            {
                iq.Type = IqType.error;
            }
            iq.SwitchDirection();
            Session.Send(iq);
        }
        void ProcessBindIq(XmppBase.Iq iq)
        {
            iq.SwitchDirection();
            iq.Type = IqType.result;
            var roster = new RosterItem();
            Bind bind = iq.Query as Bind;
            bind.RemoveAll();
            bind.Jid = Session.SessionUser.Username + "@gjtalk.com";
            Session.Send(iq);
        }
        void ProcessInfoIq(XmppBase.Iq iq)
        {
            Matrix.Xmpp.Disco.Info info = iq.Query as Matrix.Xmpp.Disco.Info;
            if (iq.To.ToString().ToLower() == "gjtalk.com")
            {

                info.AddIdentity(new Matrix.Xmpp.Disco.Identity("im", "server", "gjtalk server"));
                info.AddIdentity(new Matrix.Xmpp.Disco.Identity("pep", "pubsub"));
            }
            else if (iq.To.ToString().ToLower() == "search.gjtalk.com")
            {
                info.AddIdentity(new Matrix.Xmpp.Disco.Identity("user", "User Search", "directory"));
                info.AddFeature(new Matrix.Xmpp.Disco.Feature("jabber:iq:search"));
                info.AddFeature(new Matrix.Xmpp.Disco.Feature("http://jabber.org/protocol/rsm"));
                info.AddFeature(new Matrix.Xmpp.Disco.Feature("http://jabber.org/protocol/disco#info"));
            }

            iq.SwitchDirection();
            iq.Type = IqType.result;
            Session.Send(iq);
        }
        void ProcessItemsIq(XmppBase.Iq iq)
        {
            iq.SwitchDirection();
            iq.Query.Add(new Matrix.Xmpp.Disco.Item("search.gjtalk.com", null, "User Search"));
            iq.Type = IqType.result;
            Session.Send(iq);
        }
        void ProcessSearchIq(XmppBase.Iq iq)
        {
            iq.SwitchDirection();
            Matrix.Xmpp.Search.Search search = iq.Query as Matrix.Xmpp.Search.Search;
            if (search.XData == null)
                search.XData = new XData.Data();
            if (iq.Type == IqType.get)
            {
                search.Instructions = "The following fields are available.";
                search.First = search.Last = search.Nick = search.Email = "";
                search.XData.Type = XData.FormType.form;
                search.XData.Title = "Advanced User Search";
                search.XData.Instructions = "The following fields are available.";
                search.XData.AddField(new XData.Field("FORM_TYPE", "jabber:iq:search") { Type = XData.FieldType.Hidden });
                var searchFiled = new XData.Field("search", "Search", XData.FieldType.Text_Single);
                var usernameField = new XData.Field("Username", "Username", XData.FieldType.Boolean);
                var nameField = new XData.Field("Name", "Name", XData.FieldType.Boolean);
                var emailField = new XData.Field("Email", "Email", XData.FieldType.Boolean);
                searchFiled.Add(new XData.Value(true));
                usernameField.Add(new XData.Value(true));
                nameField.Add(new XData.Value(true));
                emailField.Add(new XData.Value(true));
                search.XData.AddField(searchFiled);
                search.XData.AddField(usernameField);
                search.XData.AddField(nameField);
                search.XData.AddField(emailField);

            }
            else if (iq.Type == IqType.set && search.XData.Type == XData.FormType.submit)
            {
                var searchField = search.XData.GetField("search");
                var usernameField = search.XData.GetField("Username");
                var nameField = search.XData.GetField("Name");
                var emailField = search.XData.GetField("Email");
                if (searchField != null)
                {
                    bool bUsername = usernameField != null && usernameField.GetValueBool();
                    bool bName = nameField != null && nameField.GetValueBool();
                    bool bEmail = emailField != null && emailField.GetValueBool();
                    string keyword = searchField.Value;
                    if (!string.IsNullOrEmpty(keyword))
                    {
                        using (SqlDataContext dc = new SqlDataContext())
                        {
                            var query = dc.DatabaseUserItems.Where(x =>
                                (bUsername && string.Compare(x.Username, keyword, true) == 0) ||
                                (bName && string.Compare(x.Nickname, keyword, true) == 0) ||
                                (bEmail && string.Compare(x.Mail, keyword, true) == 0)
                                );
                            search.XData = new XData.Data();
                            search.XData.Type = XData.FormType.result;

                            search.XData.AddField(new XData.Field()
                            {
                                Var = "FORM_TYPE",
                                Type = XData.FieldType.Hidden
                            });
                            XData.Reported reported = new XData.Reported();
                            reported.Add(new XData.Field("jid", "JID", XData.FieldType.Jid_Single));
                            reported.Add(new XData.Field("Username", "Username", XData.FieldType.Text_Single));
                            reported.Add(new XData.Field("Name", "Name", XData.FieldType.Text_Single));
                            reported.Add(new XData.Field("Email", "Email", XData.FieldType.Text_Single));
                            reported.Add(new XData.Field("Signature", "Signature", XData.FieldType.Text_Single));
                            search.XData.Add(reported);
                            foreach (var user in query)
                            {
                                var item = new XData.Item();
                                item.Add(new XData.Field("Name", user.Nickname));
                                item.Add(new XData.Field("Email", user.Mail));
                                item.Add(new XData.Field("Username", user.Username));
                                item.Add(new XData.Field("jid", user.Username + "@gjtalk.com"));
                                item.Add(new XData.Field("Signature", user.Signature));
                                search.XData.AddItem(item);
                            }
                        }
                    }
                }
            }
            iq.Type = IqType.result;
            Session.Send(iq);
        }
        void ProcessIq(XmppBase.Iq iq)
        {
            if (iq.Query is Roster)
                ProcessRosterIq(iq);
            else if (iq.Query is Matrix.Xmpp.Session.Session)
                ProcessSessionIq(iq);
            else if (iq.Query is Matrix.Xmpp.Vcard.Vcard)
                ProcessVCardIq(iq);
            else if (iq.Query is Matrix.Xmpp.Bind.Bind)
                ProcessBindIq(iq);
            else if (iq.Query is Matrix.Xmpp.Disco.Info)
                ProcessInfoIq(iq);
            else if (iq.Query is Matrix.Xmpp.Search.Search)
                ProcessSearchIq(iq);
            else if (iq.Query is Matrix.Xmpp.Disco.Items)
                ProcessItemsIq(iq);
            else
            {
                iq.SwitchDirection();
                iq.Type = IqType.result;
                Session.Send(iq);
            }
        }
        void ProcessMessage(XmppBase.Message msg)
        {
            msg.From = Session.Jid;
            msg.Delay = new Matrix.Xmpp.Delay.Delay(DateTime.Now, msg.From);
            Server.MessageManager.HandleMessage(this.Session, msg);
        }
        void ProcessAuth(MxAuth auth)
        {
            string user = null, pass = null;
            Failure failure = null;
            if (auth.SaslMechanism == SaslMechanism.PLAIN)
            {
                byte[] buffer = Convert.FromBase64String(auth.Value);
                string sasl = Encoding.UTF8.GetString(buffer).Trim((char)0);
                string[] split = sasl.Split((char)0);
                if (split.Length == 3)
                {
                    user = split[1];
                    pass = split[2];
                }
                else if (split.Length == 2)
                {
                    user = split[0];
                    pass = split[1];
                }
                else
                {
                    failure = new Failure(FailureCondition.not_authorized);
                }
            }
            else
            {
                failure = new Failure(FailureCondition.invalid_mechanism);
            }
            if (failure == null)
            {
                user = JIDEscaping.Unescape(user);
                if (!Server.AuthManager.Auth(user, pass, pass.Length == 32))
                    failure = new Failure(FailureCondition.not_authorized);
            }
            if (failure == null)
            {
                Session.SetOnline(user);
                Session.Send(new Success());
                Session.Reset();
            }
            else
                Session.Send(failure, DataSentAction.Close);
        }


        void ProcessGetRoster(XmppBase.Iq iq)
        {
            List<Presence> presences = new List<Presence>();
            iq.SwitchDirection();
            iq.Type = IqType.result;
            foreach (var group in Session.Groups)
            {
                foreach (var buddy in group.Buddies)
                {
                    var ri = buddy.ToRosterItem();
                    //var ri = new RosterItem(JIDEscaping.Escape(buddy.Username) + "@gjtalk.com", buddy.Remark == null ? buddy.Nickname : buddy.Remark, group.GroupName);
                    ri.Subscription = Subscription.both;

                    iq.Query.Add(ri);
                    var buddySession = Server.SessionManager.GetSession(buddy.Username);
                    if (buddySession != null)
                    {
                        var curPresence = buddySession.GetPresence();
                        if (curPresence.Type != PresenceType.unavailable)
                        {

                            presences.Add(new Presence(curPresence.Show, curPresence.Status, curPresence.Priority)
                            {
                                Type = curPresence.Type,
                                From = buddySession.Jid,
                                To = Session.Jid
                            });
                        }
                    }
                }
            }
            Session.Send(iq);
            foreach (var item in presences)
            {
                Session.Send(item);
            }
            Session.SendOfflineMessages();
        }
        void ProcessSetRoster(XmppBase.Iq iq)
        {
            Roster roster = iq.Query as Roster;
            Roster respRoster = new Roster();
            foreach (var item in roster.GetRoster())
            {
                string owner = Session.SessionUser.Username;
                string username = JIDEscaping.Unescape(item.Jid.User);

                string group = null;
                if (item.HasGroups)
                    group = item.GetGroups()[0];

                if (item.Subscription == Subscription.remove)
                {
                    FriendshipManager.Instance.RemoveFriend(owner,
                        JIDEscaping.Unescape(item.Jid.User));
                    return;
                }


                if (FriendshipManager.Instance.IsFriend(
                    Session.SessionUser.Username,
                    username))
                {
                    FriendUpdateFlags flags = FriendUpdateFlags.UpdateRemark;
                    if (group != null)
                        flags |= FriendUpdateFlags.UpdateGroup;
                    FriendshipManager.Instance.UpdateFriend(owner, username,
                        null, group, item.Name, null, flags);
                    respRoster.Add(item);
                }
                else
                {
                    var sqlUser = Server.AuthManager.GetUser(username);
                    if (sqlUser == null)
                        return;
                    FriendshipManager.Instance.AddFriend(Session.SessionUser.Username,
                        group, username, item.Name, sqlUser.Nickname, Subscription.from);
                }
            }
            iq.Type = IqType.result;
            iq.Query = respRoster;
            iq.SwitchDirection();
            Session.Send(iq);
        }

        void ProcessSubscribeRequest(XmppBase.Presence presence)
        {
            var session = Server.SessionManager.GetSession(presence.To);
            if (session == null)
                Server.OfflineMessageManager.PutPresence(presence);
            else
                session.Send(presence);

        }
        void ProcessUnsubscribeRequest(XmppBase.Presence presence)
        {
            string owner = Session.SessionUser.Username;
            string friend = JIDEscaping.Unescape(presence.To.User);
            FriendshipManager.Instance.RemoveFriend(owner, friend);
            FriendshipManager.Instance.RemoveFriend(friend, owner);

        }
        void ProcessSubscribeResposne(XmppBase.Presence presence)
        {
            if (presence.To == null)
                return;

            string owner = JIDEscaping.Unescape(presence.To.User);
            string friend = Session.SessionUser.Username;

            var session = Server.SessionManager.GetSession(presence.To);
            if (session == null)
                Server.OfflineMessageManager.PutPresence(presence);
            else
                session.Send(presence);
            if (FriendshipManager.Instance.IsFriend(owner, friend))
            {
                FriendshipManager.Instance.UpdateFriend(owner, friend, null, null, null, Subscription.both,
                    FriendUpdateFlags.UpdateSubscription);
            }


        }
        void ProcessSubscription(XmppBase.Presence presence)
        {
            if (presence.Type == PresenceType.subscribe)
                ProcessSubscribeRequest(presence);
            else if (presence.Type == PresenceType.unsubscribe)
                ProcessUnsubscribeRequest(presence);
            else if (presence.Type == PresenceType.subscribed || presence.Type == PresenceType.unsubscribed)
                ProcessSubscribeResposne(presence);
        }

        void ProcessPresence(XmppBase.Presence presence)
        {
            if (presence.Type == PresenceType.probe)
            {
                foreach (var item in FriendshipManager.Instance.GetAllBuddy(Session.SessionUser.Username))
                {
                    var friendSession = Server.SessionManager.GetSession(item.Username);
                    if (friendSession != null)
                    {
                        if (friendSession.GetPresence() != null)
                            Session.Send(friendSession.GetPresence());
                    }
                }
            }
            else if (presence.Type == PresenceType.subscribe || presence.Type == PresenceType.subscribed
                || presence.Type == PresenceType.unsubscribe || presence.Type == PresenceType.unsubscribed)
            {
                ProcessSubscription(presence);
            }
            else
            {
                Session.SetPresence(presence.Type, presence.Status, presence.Priority);
            }
        }

        public void Handle()
        {
            Console.WriteLine("Handle " + Stanza.GetType().ToString());
            if (!Session.IsOnline && !(Stanza is MxAuth))
            {
                Session.Close();
            }
            if (Stanza is XmppBase.Iq)
            {
                ProcessIq(Stanza as XmppBase.Iq);
            }
            else if (Stanza is XmppBase.Message)
            {
                ProcessMessage(Stanza as XmppBase.Message);
            }
            else if (Stanza is XmppBase.Presence)
            {
                ProcessPresence(Stanza as XmppBase.Presence);
            }
            else if (Stanza is MxAuth)
            {
                ProcessAuth(Stanza as MxAuth);
            }
        }
    }
}

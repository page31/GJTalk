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
            {
                iq.SwitchDirection();
                iq.Type = IqType.result;
                foreach (var group in Session.Groups)
                {
                    foreach (var buddy in group.Buddies)
                    {
                        var ri = new RosterItem(buddy.Username + "@gjtalk.com", buddy.Nickname, group.GroupName);
                        ri.Subscription = Subscription.both;
                        ri.SetAttribute("remark", buddy.Remark);
                        
                        iq.Query.Add(ri);
                    }
                }
            }
            Session.Send(iq);
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
                            reported.Add(new XData.Field("jid","JID",XData.FieldType.Jid_Single));
                            reported.Add(new XData.Field("Username", "Username", XData.FieldType.Text_Single));
                            reported.Add(new XData.Field("Name", "Name", XData.FieldType.Text_Single));
                            reported.Add(new XData.Field("Email", "Email", XData.FieldType.Text_Single));
                            search.XData.Add(reported);
                            foreach (var user in query)
                            {
                                var item = new XData.Item();
                                item.Add(new XData.Field("Name", user.Nickname));
                                item.Add(new XData.Field("Email", user.Mail));
                                item.Add(new XData.Field("Username", user.Username));
                                item.Add(new XData.Field("jid", user.Username + "@gjtalk.com"));
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

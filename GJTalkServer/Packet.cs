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
                        var ri = new RosterItem(buddy.Username, buddy.Nickname, group.GroupName);
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

        }
        void ProcessIq(XmppBase.Iq iq)
        {
            if (iq.Query is Roster)
                ProcessRosterIq(iq);
            else if (iq.Query is Matrix.Xmpp.Session.Session)
                ProcessSessionIq(iq);
        }
        void ProcessMessage(XmppBase.Message msg)
        {
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
                user = user.Replace("\\40", "@");
                if (!Server.AuthManager.Auth(user, pass, pass.Length == 32))
                    failure = new Failure(FailureCondition.not_authorized);
            }
            if (failure == null)
            {
                Session.Send(new Success());
                Session.SetOnline(user);
            }
            else
                Session.Send(failure, DataSentAction.Close);
        }
        void ProcessPresence(XmppBase.Presence presence)
        {
        }
        public void Handle()
        {
            Console.WriteLine("Handle " + Stanza.GetType().ToString());
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

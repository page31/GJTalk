using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Matrix.Xml;
using Matrix.Xmpp;
using Matrix.Xmpp.Base;
using Matrix.Xmpp.Sasl;
using Matrix.Xmpp.Stream;
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
        void ProcessIq(Iq iq)
        {

        }
        void ProcessMessage(Message msg)
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
            if (failure == null && !Server.AuthManager.Auth(user, pass, true))
                failure = new Failure(FailureCondition.not_authorized);
            if (failure == null)
            {
                Session.Send(new Success());
                Session.SetOnline(user);
            }
            else
                Session.Send(failure);
        }
        void ProcessPresence(Presence presence)
        {
        }
        public void Handle()
        {
            if (Stanza is Iq)
            {
                ProcessIq(Stanza as Iq);
            }
            else if (Stanza is Message)
            {
                ProcessMessage(Stanza as Message);
            }
            else if (Stanza is Presence)
            {
                ProcessPresence(Stanza as Presence);
            }
            else if (Stanza is MxAuth)
            {
                ProcessAuth(Stanza as MxAuth);
            }
        }
    }
}

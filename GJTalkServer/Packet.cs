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
            Server.MessageManager.HandleMessage(this.Session,msg);
        }
        void ProcessAuth(MxAuth auth)
        {

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

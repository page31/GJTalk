using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Matrix.Xmpp.Client;

namespace GJTalkServerPressureTest
{
    class TestClient
    {
        static List<TestClient> clients = new List<TestClient>();
        string m_Name;
        string m_Server;
        bool authed = false;
        XmppClient client = new XmppClient();
        public TestClient(string name, string server)
        {
            m_Name = name;
            m_Server = server;
            client.SetUsername(name);
            client.SetXmppDomain(server);
            client.Password = "test";
            client.StartTls = false;
            client.Hostname = "localhost";
            client.Status = "ready for chat";
            client.Show = Matrix.Xmpp.Show.chat;

            client.OnLogin += client_OnLogin;
            client.OnMessage += client_OnMessage;
            client.OnRosterItem += client_OnRosterItem;
            client.OnIq += client_OnIq;
            client.OnAuthError += client_OnAuthError;
            client.OnBeforeSasl += client_OnBeforeSasl;
            client.OnSendXml += client_OnSendXml;
            client.OnBind += client_OnBind;
            client.OnRosterEnd += client_OnRosterEnd;
            clients.Add(this);
        }

        void client_OnRosterEnd(object sender, Matrix.EventArgs e)
        {
            authed = true;    
        }

        void client_OnBind(object sender, Matrix.JidEventArgs e)
        {
            Console.WriteLine("{0} bind:",m_Name,e.Jid.ToString());
        }

        void client_OnSendXml(object sender, Matrix.TextEventArgs e)
        {
            Console.WriteLine("{0} send:", m_Name, e.Text);
        }

        void client_OnBeforeSasl(object sender, Matrix.Xmpp.Sasl.SaslEventArgs e)
        {
            e.SaslMechanism = Matrix.Xmpp.Sasl.SaslMechanism.PLAIN;

        }

        void client_OnAuthError(object sender, Matrix.Xmpp.Sasl.SaslEventArgs e)
        {
            Console.WriteLine("{0} auth error.", m_Name);
        }

        void client_OnIq(object sender, IqEventArgs e)
        {
            Console.WriteLine("{0} iq:{1}", m_Name, e.Iq);
        }

        void client_OnRosterItem(object sender, Matrix.Xmpp.Roster.RosterEventArgs e)
        {
            Console.WriteLine("{0} roster:{1}", m_Name, e.RosterItem);
        }

        void client_OnMessage(object sender, MessageEventArgs e)
        {
            Console.WriteLine("{0} message:{1}", m_Name, e.Message);
        }

        void client_OnLogin(object sender, Matrix.EventArgs e)
        {
            Console.WriteLine("{0} login.", m_Name);
           // authed = true;
        }
        public void Start()
        {
            new Thread(DoTest).Start();
        }
        public void DoTest()
        {

            client.Open();
            while (true)
            {
                if (authed)
                {
                    // client.Send(new RosterIq(Matrix.Xmpp.IqType.get));
                    TestClient destClient = this;
                    Random rnd = new Random();
                    destClient = clients[rnd.Next(clients.Count)];
                    Message msg = new Message()
                    {
                        Id = DateTime.Now.Ticks.ToString(),
                        To = destClient.m_Name,
                        Body = "Test Message " + DateTime.Now
                    };
                    client.Send(msg);
                }
                Thread.Sleep(3000);
            }

        }
    }
}

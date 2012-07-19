using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace GJTalkServer
{
    class GJTalkServer
    {
        public SessionManager SessionManager { get; private set; }
        public PacketManager PacketManager { get; private set; }
        public MessageMangaer MessageManager { get; private set; }
        public UserAuth AuthManager { get; private set; }
        public OfflineMessageManager OfflineMessageManager { get;private set; }

        public int Port { get; set; }
        public IPAddress ListenAddress { get; set; }
        public string Domain { get; private set; }

        AsyncCallback acceptCallback;
        Socket listenSocket; 

        void BeginAccept(Socket socket)
        {
            listenSocket.BeginAccept(acceptCallback, socket);
        }
        void OnAccept(IAsyncResult result)
        {
            Socket socket = result.AsyncState as Socket;
            try
            {
                new Session(this, socket.EndAccept(result));
            }
            catch(Exception ex)
            {

            }
            BeginAccept(socket);
        }
        public void Start()
        {
            this.SessionManager = new SessionManager(this);
            this.MessageManager = new MessageMangaer(this);
            this.AuthManager = new UserAuth();
            this.OfflineMessageManager = new OfflineMessageManager();

            this.PacketManager = new PacketManager()
            {
                ThreadCount =1 /*Environment.ProcessorCount * 2*/
            };

            listenSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            listenSocket.Bind(new IPEndPoint(ListenAddress, Port));
            listenSocket.Listen(8);
            if (acceptCallback == null)
                acceptCallback = new AsyncCallback(OnAccept);
            BeginAccept(listenSocket);
            PacketManager.StartHandle();
            Domain = "GJTalk.com";

        }
        public GJTalkServer()
        {
            ListenAddress = IPAddress.Any;
            Port = 5222;
        }
        public void Stop()
        {
        }
    }
}

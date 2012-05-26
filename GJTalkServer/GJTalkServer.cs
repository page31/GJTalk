﻿using System;
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
        public MessageMangaer MessageManager { get;private set; }
        public int Port { get; set; }
        public IPAddress ListenAddress { get; set; }
        public string Domain { get; private set; }

        AsyncCallback acceptCallback;
        Socket listenSocket;
        Thread thListen;

        void ListenEntry()
        {
            listenSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            listenSocket.Bind(new IPEndPoint(ListenAddress, Port));
            listenSocket.Listen(8);
            if (acceptCallback == null)
                acceptCallback = new AsyncCallback(OnAccept);
            BeginAccept(listenSocket);
        }
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
            catch
            {
            }
            BeginAccept(socket);
        }
        public void Start()
        {
            this.SessionManager = new SessionManager(this);
            this.PacketManager = new PacketManager()
            {
                ThreadCount = Environment.ProcessorCount * 2 
            };
            this.MessageManager = new MessageMangaer(this);

            thListen = new Thread(ListenEntry);
            thListen.IsBackground = true;
            thListen.Start();
            PacketManager.StartHandle();
            Domain = "GJTalk.com";
        }
        public void Stop()
        {
        }
    }
}
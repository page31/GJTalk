using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace GJTalkServer
{
    class Program
    {
        static void Empty()
        {
            while (true)
                Thread.Sleep(1000);
        }
        static void Main(string[] args)
        {
            new Test().Start();
            GJTalkServer server = new GJTalkServer();
            server.Start();
            Console.WriteLine("Started.");
            new Thread(Empty).Start();
        }
    }
}

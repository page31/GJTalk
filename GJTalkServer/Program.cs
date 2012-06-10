using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
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
        static void HackMatrix()
        {
            var type = typeof(Matrix.License.LicenseManager);
            var filed = type.GetField("#=qwb4_$yjqjwwNZxtIavQJqQ==", BindingFlags.Static | BindingFlags.NonPublic);
            filed.SetValue(null, true);
        }
        static void Main(string[] args)
        {
            HackMatrix();
            new Test().Start();
            GJTalkServer server = new GJTalkServer();
            server.Start();
            Console.WriteLine("Started.");
            new Thread(Empty).Start();
        }
    }
}

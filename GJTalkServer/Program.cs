using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.IO;
using Matrix.Xmpp.Component;

namespace GJTalkServer
{
    class Program
    {
        static GJTalkServer server;
        static void Empty()
        {
            while (true)
                Thread.Sleep(1000);
        }
        static void HackMatrix()
        {
            var type = typeof(Matrix.License.LicenseManager);
            var field = type.GetField("#=qwb4_$yjqjwwNZxtIavQJqQ==", BindingFlags.Static | BindingFlags.NonPublic);
            field.SetValue(null, true);
        }
        static void TestFriend()
        {

            var groups = FriendshipManager.Instance.GetAllBuddyInGroup("test");

        }
        static void SetEnv()
        {
            HackMatrix();
            var process = Process.GetCurrentProcess();
            var file = process.MainModule.FileName;
            var dir = Path.GetDirectoryName(file);
            HeaderManager.SetPath(dir + "\\UserPic");

        }
        static void VirualOnline(string username)
        {

        }
        static void SetPresence()
        {
            Console.Write("Username:");
            string user = Console.ReadLine();
            Console.Write("Show:");
            string show = Console.ReadLine();
            Console.Write("Status:");
            string status = Console.ReadLine();
            Presence presence = new Presence();
            presence.From = user + "@gjtalk.com";
            presence.Status = status;
            presence.Show = (Matrix.Xmpp.Show)Enum.Parse(typeof(Matrix.Xmpp.Show), show);
           
            var friends = FriendshipManager.Instance.GetAllBuddy(user);

            foreach (var friend in friends)
            {
                var session = server.SessionManager.GetSession(friend.Username);
                if (session != null)
                {
                    presence.To = friend.Username + "@gjtalk.com";
                }
            }
        }
        static void SendMessage()
        {

        }
        static void CommandLine()
        {
            while (true)
            {
                string command = Console.ReadLine();
                string argument = string.Empty;
                if (command.Contains(" "))
                {
                    argument = command.Substring(command.IndexOf(' ') + 1);
                    command = command.Substring(0, command.IndexOf(' '));
                    switch (command)
                    {
                        case "vi":
                            VirualOnline(argument);
                            break;
                        case "setpre":
                            SetPresence();
                            break;
                        case "msg":
                            SendMessage();
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        static void Main(string[] args)
        {
            SetEnv();
            new Test().Start();
            server = new GJTalkServer();
            server.Start();
            Console.WriteLine("Started.");
            new Thread(Empty).Start();
            CommandLine();
        }
    }
}

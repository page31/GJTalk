using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Diagnostics;

namespace GJTalkServer
{
    class MongoDbHelper
    {
        public static string GetBinDir()
        {
            return ConfigurationManager.AppSettings["mongoBinDir"];

        }
        public static string GetArguments()
        {
            return ConfigurationManager.AppSettings["mongoStartArguments"];
        }
        public static string GetConnectionString()
        {
            return ConfigurationManager.AppSettings["mongoDbConnStr"];
        }
        public static bool IsRunning()
        {
            string mongoD = Path.Combine(GetBinDir(), "mongod.exe").ToLower();

            try
            {
                foreach (var process in Process.GetProcesses())
                {
                    string exeFile = process.MainModule.FileName.ToLower();
                    if (exeFile == mongoD)
                    {
                        return true;
                    }
                }
            }
            catch (System.Exception ex)
            {
                return false;
            }
            return false;
        }
        public static bool IsOnline()
        {
            MongoDB.Driver.MongoServer server = MongoDB.Driver.MongoServer.Create(GetConnectionString());
            try
            {
                server.Ping();
                server.Disconnect();
                return true;
            }
            catch
            {
                return false;
            }
        }
        public static void Run()
        {
            string curDir = Directory.GetCurrentDirectory();
            Directory.SetCurrentDirectory(GetBinDir());
            Process.Start(Path.Combine(GetBinDir(), "mongod.exe"), GetArguments());
            Directory.SetCurrentDirectory(curDir);
        }
        public static void CheckAndRun()
        {
            while (!IsOnline())
            {
                if (!IsRunning())
                    Run();
                Thread.Sleep(1000);
            }
        }
    }
}

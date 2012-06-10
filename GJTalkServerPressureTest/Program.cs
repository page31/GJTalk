using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace GJTalkServerPressureTest
{
    class Program
    {
        static void HackMatrix()
        {
            var type=typeof(Matrix.License.LicenseManager);
            var filed = type.GetField("#=qwb4_$yjqjwwNZxtIavQJqQ==",BindingFlags.Static|BindingFlags.NonPublic);
            filed.SetValue(null, true);
        }
        static void Main(string[] args)
        {
            HackMatrix();
            for (int i = 0; i < 1; i++)
            {
                new TestClient("test" + i+"@gmail.com", "localhost").DoTest();
                Thread.Sleep(1000);
            } 
        }
    }
}

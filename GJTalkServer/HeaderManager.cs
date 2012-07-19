using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace GJTalkServer
{
    class HeaderManager
    {
        private static string m_path;
        public static void SetPath(string path)
        {
            m_path = path;
        }

        public static byte[] Load(string username)
        {
            if (string.IsNullOrEmpty(username))
                return null;
            username = username.ToLower();
            string filename = Crypto.Md5(username);
            filename = m_path + "\\" + filename;
            try
            {
                return File.ReadAllBytes(filename);
            }
            catch (System.Exception ex)
            {
                return null;
            }
        }
        public static string Save(string username, byte[] data)
        {
            if (string.IsNullOrEmpty(username))
                return null;
            username = username.ToLower();
            string filename = Crypto.Md5(username);
            filename = m_path + "\\" + filename;
            try
            {
                File.WriteAllBytes(filename, data);
            }
            catch (System.Exception ex)
            {

            }
            return username;
        }
    }
}

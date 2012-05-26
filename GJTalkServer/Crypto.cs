using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;
namespace GJTalkServer
{
    class Crypto
    {
        static string HexStr = "0123456789abcdef";
        public static string Hex(byte[] data)
        {
            StringBuilder sb = new StringBuilder(data.Length * 2);
            for (int i = 0; i < data.Length; i++)
            {
                sb.Append(HexStr[data[i] >> 4]);
                sb.Append(HexStr[data[i] & 0xF]);
            }
            return sb.ToString();
        }
        public static byte[] Md5Raw(byte[] data)
        {
            MD5 md5 = MD5.Create();
            return md5.ComputeHash(data);
        }
        public static string Md5(string message)
        {
            return Hex(Md5Raw(System.Text.Encoding.UTF8.GetBytes(message)));
        }
    }
}

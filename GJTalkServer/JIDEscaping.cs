using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GJTalkServer
{
    class JIDEscaping
    {
        private static char[] unescapedChars
            = new char[] { ' ', '"', '&', '\'', '/', ':', '<', '>', '@', '\\' };
        private static string[] escapedChars
            = new string[] { "\\20", "\\22", "\\26", "\\27", "\\2f", "\\3a", "\\3c", "\\3e", "\\40", "\\5c" };
        public static string Escape(string input)
        {
            if (string.IsNullOrEmpty(input))
                return input;
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < input.Length; i++)
            {
                char ch = input[i];
                bool bFlag = true;
                for (int j = 0; j < unescapedChars.Length; j++)
                {
                    if (unescapedChars[j] == ch)
                    {
                        sb.Append(escapedChars[j]);
                        bFlag = false;
                        break;
                    }
                }
                if (bFlag)
                    sb.Append(ch);
            }

            return sb.ToString();
        }
        public static string Unescape(string input)
        {
            if (string.IsNullOrEmpty(input))
                return input;
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < input.Length; i++)
            {
                bool bFlag = true;
                if (input[i] == '\\')
                {
                    if (i < input.Length - 2)
                    {
                        string str = input.Substring(i, 3).ToLower();
                        for (int j = 0; j < escapedChars.Length; j++)
                        {
                            if (escapedChars[j] == str)
                            {
                                sb.Append(unescapedChars[j]);
                                i += 2;
                                bFlag = false;
                                break;
                            }
                        }
                    }
                }
                if (bFlag)
                    sb.Append(input[i]);
            }
            return sb.ToString();
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Matrix.Xmpp.Client;

namespace GJTalkServer
{
    class Test
    {
        void TestOfflineManager()
        {
            OfflineMessageManager manager = new OfflineMessageManager();
            manager.Put("test","admin", new Message[]{
                new Message()
                {
                    From="353230192@qq.com",
                    To="cpage31@gmail.com",
                    Id=DateTime.Now.Ticks+"|"+Guid.NewGuid().ToString(),
                    Body="test message at "+ DateTime.Now.ToString()
                },
                new Message()
                {
                    From="353230192@qq.com",
                    To="cpage31@gmail.com",
                    Id=DateTime.Now.Ticks+"|"+Guid.NewGuid().ToString(),
                    Body="test message1 at "+ DateTime.Now.ToString()
                }
            });
            var msgs = manager.Get("cpage31@gmail.com", true);
        }
        public void Start()
        {

        }

    }
}

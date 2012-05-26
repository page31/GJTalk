using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace GJTalkServer
{
    class PacketManager
    {
        Queue<Packet> packets = new Queue<Packet>();
        object objLock = new object();
        public int ThreadCount { get; set; }
        Thread[] thHandle;
        EventWaitHandle waitHandle = new EventWaitHandle(false, EventResetMode.ManualReset);
        public void Enqueue(Packet packet)
        {
            lock (objLock)
            {
                packets.Enqueue(packet);
                waitHandle.Set();
            }
        }
        public Packet Dequeue()
        {
            lock (objLock)
            {
                if (packets.Count == 0)
                {
                    waitHandle.Reset();
                    return null;
                }
                return packets.Dequeue();
            }
        }

        private void HandleEntry()
        {
            while (true)
            {
                if (!waitHandle.WaitOne(1000))
                    continue;
                Packet packet = Dequeue();
                if (packet == null)
                    continue;
                packet.Handle();
            }
        }

        public void StartHandle()
        {
            thHandle = new Thread[ThreadCount];
            for (int i = 0; i < ThreadCount; i++)
            {
                thHandle[i] = new Thread(HandleEntry);
                thHandle[i].IsBackground = true;
                thHandle[i].Start();
            }
        }
    }
}

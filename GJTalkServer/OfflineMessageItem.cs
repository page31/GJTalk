using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MongoDB.Bson.Serialization.Attributes;

namespace GJTalkServer
{
    class OfflineMessageItem
    {
        [BsonId()]
        public string Id { get; set; }
        public string From { get; set; }
        public string To { get; set; }
        public DateTime Time { get; set; }
        public string Body { get; set; }

        [BsonDefaultValue("normal")]
        public string MessageType { get; set; }
    }
}

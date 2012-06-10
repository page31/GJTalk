using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using Matrix.Xmpp.Base;
using MongoDB.Bson;
using MongoDB.Driver;
using MongoDB.Driver.Linq;
namespace GJTalkServer
{
    class OfflineMessageManager
    {
        MongoServer server;
        MongoDatabase GetDatabase()
        {
            if (server == null)
                server = MongoServer.Create(ConfigurationManager.AppSettings["mongoDbConnStr"]);
            if (server.State != MongoServerState.Connected)
                server.Connect();
            return server.GetDatabase(ConfigurationManager.AppSettings["mongoDbName"]);
        }
        public Message[] Get(string user, bool delete)
        {
            if (string.IsNullOrEmpty(user))
                return null;

            user = user.ToLower();
            string idStart = user + "|";
            var db = GetDatabase();
            var collection = db.GetCollection("offline_msg");
            var query = from x in collection.AsQueryable<OfflineMessageItem>() where x.Id.StartsWith(idStart) select x;
            List<Message> msgs = new List<Message>();

            foreach (var item in query)
            {
                msgs.Add(new Matrix.Xmpp.Client.Message()
                {
                    From = item.From,
                    To = item.To,
                    Body = item.Body,
                    Subject = "offline",
                    Id = item.Id.Substring(idStart.Length),
                    Type = (Matrix.Xmpp.MessageType)Enum.Parse(
                    typeof(Matrix.Xmpp.MessageType), item.MessageType)
                });
            }
            if (delete)
            {
                QueryDocument doc = new QueryDocument();
                doc.Add("_id", new BsonRegularExpression("^" + idStart.Replace("|","\\|")+"*"));
                collection.Remove(doc);
            }
            return msgs.ToArray();
        }
        public void Put(string user, Message[] messages)
        {
            var db = GetDatabase();
            var collection = db.GetCollection("offline_msg");
            foreach (var item in messages)
            {

                OfflineMessageItem msg = new OfflineMessageItem()
                {
                    Id = user + "|" + item.Id,
                    MessageType = item.Type.ToString(),
                    Body = item.Body,
                    From = item.From,
                    Time = DateTime.Now,
                    To = item.To.ToString()
                };
                collection.Insert<OfflineMessageItem>(msg);
            }
        }
    }
}

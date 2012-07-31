using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using Matrix.Xmpp.Base;
using MongoDB.Bson;
using MongoDB.Driver;
using MongoDB.Driver.Linq;
using Client = Matrix.Xmpp.Client;
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
        public Message[] GetChatMessage(string user, bool delete)
        {
            if (string.IsNullOrEmpty(user))
                return null;

            user = user.ToLower();
            var db = GetDatabase();
            var collection = db.GetCollection("offline_msg");
            var query = from x in collection.AsQueryable<OfflineMessageItem>() where x.To == user select x;
            List<Message> msgs = new List<Message>();

            foreach (var item in query)
            {
                msgs.Add(new Matrix.Xmpp.Client.Message()
                {
                    From = item.From + "@gjtalk.com",
                    To = item.To + "@gjtalk.com",
                    Body = item.Body,
                    Delay = new Matrix.Xmpp.Delay.Delay(item.Time, item.From + "@gjtalkc.com"),
                    Type = (Matrix.Xmpp.MessageType)Enum.Parse(
                    typeof(Matrix.Xmpp.MessageType), item.MessageType)
                });
            }
            if (delete)
            {
                QueryDocument doc = new QueryDocument();
                doc.Add("To", user);
                collection.Remove(doc);
            }
            return msgs.ToArray();
        }
        public void PutChatMessage(string sender, string username, Message[] messages)
        {
            var db = GetDatabase();
            var collection = db.GetCollection("offline_msg");
            foreach (var item in messages)
            {

                OfflineMessageItem msg = new OfflineMessageItem()
                {
                    MessageType = item.Type.ToString(),
                    Body = item.Body,
                    From = sender,
                    Time = item.Delay == null ? DateTime.Now : item.Delay.Stamp,
                    To = username.ToLower()
                };
                collection.Insert<OfflineMessageItem>(msg);
            }
        }

        public void PutPresence(Presence presence)
        {
            var db = GetDatabase();
            var collection = db.GetCollection("offline_presence");
            collection.Insert(
                new BsonDocument(
                    new BsonElement("from", JIDEscaping.Unescape(presence.From.User)),
                    new BsonElement("to", JIDEscaping.Unescape(presence.To.User)),
                    new BsonElement("type", presence.Type.ToString()),
                    new BsonElement("value", presence.ToString())
                    ));
        }
        public Presence[] GetPresence(string from, string to, Matrix.Xmpp.PresenceType? type, bool remove)
        {

            List<Presence> presences = new List<Presence>();
            var db = GetDatabase();
            var collection = db.GetCollection("offline_presence");
            var query = new QueryDocument();
            if (from != null)
                query.Add("from", from);
            if (to != null)
                query.Add("to", to);
            if (type != null)
                query.Add("type", type.ToString());

            MongoCursor<BsonDocument> cursor;
            if (from == null && to == null)
                cursor = collection.FindAll();
            else
                cursor = collection.Find(query);

            foreach (var item in cursor)
            {
                Presence presence = (Presence)Presence.LoadXml(item.GetValue("value").AsString);
                presences.Add(presence);
            }
            return presences.ToArray();
        }
    }
}

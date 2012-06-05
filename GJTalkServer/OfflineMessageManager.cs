using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using MongoDB.Bson;
using MongoDB.Driver;
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
        public Matrix.Xmpp.Base.Message[] Get(string user, bool delete)
        {
            var db = GetDatabase();
            var collection = db.GetCollection("offline_msg");
           
            return null;

        }
        public void Put(string user, Matrix.Xmpp.Base.Message[] messages)
        {
            var db = GetDatabase();
            var collection = db.GetCollection("offline_msg");
            foreach (var item in messages)
            {
                BsonDocument doc = new BsonDocument();
                doc.Add("to", item.To.User);
                doc.Add("from", item.From.User);
                doc.Add("time", DateTime.Now);
                doc.Add("body", item.Body);
                collection.Insert(doc);
            }
        }
    }
}

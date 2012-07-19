using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using MongoDB.Bson;
using MongoDB.Driver;

namespace GJTalkServer
{
    class FriendshipManager
    {
        private static FriendshipManager _instance;
        public static FriendshipManager Instance
        {
            get
            {
                if (_instance == null)
                    _instance = new FriendshipManager();
                return _instance;
            }
        }
        MongoServer server;
        MongoDatabase GetDatabase()
        {
            if (server == null)
                server = MongoServer.Create(ConfigurationManager.AppSettings["mongoDbConnStr"]);
            if (server.State != MongoServerState.Connected)
                server.Connect();
            return server.GetDatabase(ConfigurationManager.AppSettings["mongoDbName"]);
        }
        public BuddyGroup[] GetAllBuddyInGroup(string username)
        {
            var buddies = GetAllBuddy(username);
            Dictionary<string, BuddyGroup> groups = new Dictionary<string, BuddyGroup>();
            foreach (var item in buddies)
            {
                BuddyGroup group;
                if (!groups.TryGetValue(item.GroupName, out group))
                {
                    group = new BuddyGroup();
                    group.GroupName = item.GroupName;
                    groups.Add(group.GroupName, group);
                    group.User = username;
                }
                group.Buddies.Add(item);
            }
            return groups.Values.ToArray();
        } 
        
        public BuddyItem[] GetAllBuddy(string username)
        {
            var db = GetDatabase();
            if (!db.CollectionExists("friends"))
                return new BuddyItem[0];
            var collection = db.GetCollection("friends");
            QueryDocument queryDoc = new QueryDocument();
            queryDoc.Add("owner", username);
            var results = collection.Find(queryDoc);
            BuddyItem[] items = new BuddyItem[results.Size()];
            int i = 0;
            foreach (var item in results)
            {
                BuddyItem newItem = new BuddyItem(); 
                newItem.Username = item.GetElement("username").Value.AsString;
                newItem.GroupName = item.GetElement("group").Value.AsString; 
                if (item.Contains("remark"))
                    newItem.Remark = item.GetElement("remark").Value.AsString; 
                if (item.Contains("nickname"))
                    newItem.Nickname = item.GetElement("nickname").Value.AsString;
                items[i++] = newItem;

            }
            return items;
        }

        public void AddFriend(string owner, string groupname, string username, string remark)
        {
            var db = GetDatabase();
            var collection = db.GetCollection("friends");
            BsonDocument doc = new BsonDocument();
            doc.Add("owner", owner);
            doc.Add("group", groupname);
            doc.Add("username", username);
            doc.Add("addtime", DateTime.Now);
            doc.Add("nickname", username);
            doc.Add("remark", remark);
            collection.Insert(doc);
        }
    }
}

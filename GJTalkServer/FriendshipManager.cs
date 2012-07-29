using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using MongoDB.Bson;
using MongoDB.Driver;

namespace GJTalkServer
{
    enum FriendUpdateFlags
    {
        UpdateGroup,
        UpdateRemark,
        UpdateNickname
    }
    class FriendshipManager
    {

        MongoServer server;

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
        MongoDatabase GetDatabase()
        {
            if (server == null)
                server = MongoServer.Create(ConfigurationManager.AppSettings["mongoDbConnStr"]);
            if (server.State != MongoServerState.Connected)
                server.Connect();
            return server.GetDatabase(ConfigurationManager.AppSettings["mongoDbName"]);
        }


        public void RequestAddFriend(string ownerUsername, string friendUsername, string groupName)
        {
            var db = GetDatabase();
            var collection = db.GetCollection("friend_request");
            var queryDoc = new QueryDocument();
            queryDoc.Add("OwnerUsername", ownerUsername);
            queryDoc.Add("FriendUsername", friendUsername);
            var updateDoc = new UpdateDocument();
            updateDoc.Add("OwnerUsername", ownerUsername);
            updateDoc.Add("FriendUsername", friendUsername);
            updateDoc.Add("GroupName", groupName);
            updateDoc.Add("Time", DateTime.Now);

            collection.Update(queryDoc, updateDoc, UpdateFlags.Upsert);
        }
        public AddFriendRequest GetAndRemoveAddFriendRequest(string ownerUsername, string friendUsername)
        {
            AddFriendRequest req = null;
            var db = GetDatabase();
            var collection = db.GetCollection("friend_request");

            req = collection.FindOneAs<AddFriendRequest>(new QueryDocument(
                 new BsonElement("OwnerUsername", ownerUsername),
                 new BsonElement("FriendUsername", friendUsername)));
            return req;
        }
        public AddFriendRequest[] GetAddFriendRequests(string ownerUsername)
        {
            List<AddFriendRequest> requests = new List<AddFriendRequest>();
            var db = GetDatabase();
            var collection = db.GetCollection("friend_request");

            var queryDoc = new QueryDocument();
            queryDoc.Add("OwnerUsername", ownerUsername);

            foreach (var req in collection.FindAs<AddFriendRequest>(queryDoc))
            {
                requests.Add(req);
            }
            return requests.ToArray();

        }
        public void RemoveAddFriendRequest(string ownerUsername, string friendUsername)
        {
            var db = GetDatabase();
            db.GetCollection("friend_request").Remove(
                new QueryDocument(
                    new BsonElement("OwnerUsername", ownerUsername),
                    new BsonElement("FriendUsername", friendUsername)
                    ));
        }

        public bool IsFriend(string ownerUsername, string friendUsername)
        {
            var db = GetDatabase();
            if (!db.CollectionExists("friends"))
                return false;
            var collection = db.GetCollection("friends");
            QueryDocument queryDoc = new QueryDocument();
            queryDoc.Add("owner", ownerUsername);
            queryDoc.Add("username", friendUsername);
            return collection.Count(queryDoc) > 0;
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
        public void UpdateFriend(string owner, string username, string nickname, string group, string remark, FriendUpdateFlags flags)
        {
            if (owner == null || username == null)
                return;

            var db = GetDatabase();
            var collection = db.GetCollection("friends");

            owner = owner.ToLower();
            username = username.ToLower();

            UpdateDocument updateDoc = new UpdateDocument();

            if ((flags & FriendUpdateFlags.UpdateGroup) == FriendUpdateFlags.UpdateGroup)
                updateDoc.Add("group", group);

            if ((flags & FriendUpdateFlags.UpdateNickname) == FriendUpdateFlags.UpdateNickname)
                updateDoc.Add("nickname", nickname);

            if ((flags & FriendUpdateFlags.UpdateRemark) == FriendUpdateFlags.UpdateRemark)
                updateDoc.Add("remark", remark);

            collection.Update(new QueryDocument(new BsonElement("owner", owner),
                new BsonElement("username", username))
                , updateDoc);
        }
        public void AddFriend(string owner, string groupname, string username, string remark)
        {
            if (owner == null || username == null)
                return;
            var db = GetDatabase();
            var collection = db.GetCollection("friends");
            owner = owner.ToLower();
            username = username.ToLower();
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

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
        UpdateGroup = 0x01,
        UpdateRemark = 0x02,
        UpdateNickname = 0x04,
        UpdateAll = 0x08
    }
    enum RequestAddFriendUpdateFlags
    {
        UpdateGroup = 0x01,
        UpdateRemark = 0x02,
        UpdateMessage = 0x04,
        UpdateAll = 0x08
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


        public void RequestAddFriend(string ownerUsername, string friendUsername, string remark, string groupName, string message,
            RequestAddFriendUpdateFlags flags)
        {
            var db = GetDatabase();
            var collection = db.GetCollection("friend_request");
            var queryDoc = new QueryDocument();

            queryDoc.Add("OwnerUsername", ownerUsername.ToLower());
            queryDoc.Add("FriendUsername", friendUsername.ToLower());

            var cur = collection.Find(queryDoc);
            cur.Limit = 1;

            UpdateDocument updateDoc = new UpdateDocument();
            if (cur.Size() > 0)
            {
                foreach (var item in cur)
                    updateDoc.Add(item);
            }
            else
            {
                updateDoc.Add("OwnerUsername", ownerUsername.ToLower());
                updateDoc.Add("FriendUsername", friendUsername.ToLower());
            }

            if (flags == RequestAddFriendUpdateFlags.UpdateAll || (flags & RequestAddFriendUpdateFlags.UpdateRemark) ==
                RequestAddFriendUpdateFlags.UpdateRemark)
                updateDoc.Set("Remark", remark);
            if (flags == RequestAddFriendUpdateFlags.UpdateAll || (flags & RequestAddFriendUpdateFlags.UpdateMessage) ==
             RequestAddFriendUpdateFlags.UpdateMessage)
                updateDoc.Set("Message", message);
            if (flags == RequestAddFriendUpdateFlags.UpdateAll || (flags & RequestAddFriendUpdateFlags.UpdateGroup) ==
             RequestAddFriendUpdateFlags.UpdateGroup)
                updateDoc.Set("GroupName", groupName);
            updateDoc.Set("Time", DateTime.Now);
            collection.Update(queryDoc, updateDoc, UpdateFlags.Upsert);
        }
        public AddFriendRequest GetAddFriendRequest(string ownerUsername, string friendUsername, bool remove)
        {
            AddFriendRequest req = null;
            var db = GetDatabase();
            var collection = db.GetCollection("friend_request");
            var query = new QueryDocument(
                  new BsonElement("OwnerUsername", ownerUsername.ToLower()),
                  new BsonElement("FriendUsername", friendUsername.ToLower()));

            req = collection.FindOneAs<AddFriendRequest>(query);
            if (remove)
                collection.Remove(query);

            return req;
        }
        public AddFriendRequest[] GetAddFriendRequests(string ownerUsername)
        {
            List<AddFriendRequest> requests = new List<AddFriendRequest>();
            var db = GetDatabase();
            var collection = db.GetCollection("friend_request");

            var queryDoc = new QueryDocument();
            queryDoc.Add("OwnerUsername", ownerUsername.ToLower());

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
                    new BsonElement("OwnerUsername", ownerUsername.ToLower()),
                    new BsonElement("FriendUsername", friendUsername.ToLower())
                    ));
        }

        public bool IsFriend(string ownerUsername, string friendUsername)
        {
            var db = GetDatabase();
            if (!db.CollectionExists("friends"))
                return false;
            var collection = db.GetCollection("friends");
            QueryDocument queryDoc = new QueryDocument();
            queryDoc.Add("owner", ownerUsername.ToLower());
            queryDoc.Add("username", friendUsername.ToLower());
            long count = collection.Count(queryDoc);
            return count > 0;
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
            queryDoc.Add("owner", username.ToLower());
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

            var queryDoc = new QueryDocument(new BsonElement("owner", owner),
                new BsonElement("username", username));

            UpdateDocument updateDoc;

            var cur = collection.Find(queryDoc);

            if (cur.Size() > 0)
                updateDoc = new UpdateDocument(cur.ToBsonDocument());
            else
            {
                updateDoc = new UpdateDocument();
                updateDoc.Add("owner", owner);
                updateDoc.Add("username", username);
            }

            if (flags == FriendUpdateFlags.UpdateAll || (flags & FriendUpdateFlags.UpdateGroup) == FriendUpdateFlags.UpdateGroup)
                updateDoc.Add("group", group);

            if (flags == FriendUpdateFlags.UpdateAll || (flags & FriendUpdateFlags.UpdateNickname) == FriendUpdateFlags.UpdateNickname)
                updateDoc.Add("nickname", nickname);

            if (flags == FriendUpdateFlags.UpdateAll || (flags & FriendUpdateFlags.UpdateRemark) == FriendUpdateFlags.UpdateRemark)
                updateDoc.Add("remark", remark);

            collection.Update(queryDoc
                , updateDoc, UpdateFlags.Upsert);
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
        public void RemoveFriend(string owner, string username)
        {
            if (owner == null || username == null)
                return;
            var db = GetDatabase();
            var collection = db.GetCollection("friends");
            collection.Remove(new QueryDocument(
                new BsonElement("owner", owner.ToLower()),
                new BsonElement("username", username.ToLower())
                ));
        }
    }
}

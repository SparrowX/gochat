#pragma once

#include <muduo/net/TcpConnection.h>

#include <functional>
#include <mutex>
#include <unordered_map>
using namespace std;
using namespace muduo;
using namespace muduo::net;

#include "FriendModel.h"
#include "GroupModel.h"
#include "OfflineMsgModel.h"
#include "UserModel.h"
#include "json.hpp"
#include "redis.h"
using json = nlohmann::json;

// Call back function for handling message
using msgHandler =
    function<void(const TcpConnectionPtr &conn, json &js, Timestamp &time)>;

class ChatService {
 public:
  // Singleton object interface
  static ChatService *instance();

  // Handle login service
  void login(const TcpConnectionPtr &conn, json &js, Timestamp &time);

  // Handle signup service
  void signup(const TcpConnectionPtr &conn, json &js, Timestamp &time);

  // One to one chat service
  void chat(const TcpConnectionPtr &conn, json &js, Timestamp &time);

  // Add friend service
  void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp &time);

  // Create group service
  void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp &time);

  // Add group service
  void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp &time);

  // Group chat service
  void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp &time);

  // Log out service
  void logout(const TcpConnectionPtr &conn, json &js, Timestamp &time);

  // Update user information
  void update(const TcpConnectionPtr &conn, json &js, Timestamp &time);

  // Get handler for a message
  msgHandler getHandler(int msgid);

  // Handle client exit exception
  void clientCloseException(const TcpConnectionPtr &conn);

  // Reset after server exit exception
  void reset();

  // Receive message from redis
  void handleRedisMessage(int id, string message);

 private:
  ChatService();

  // Store message ID and handler
  unordered_map<int, msgHandler> _msgHandlerMap;

  // Store user connection
  unordered_map<int, TcpConnectionPtr> _userConnMap;

  // Mutual excultion lock
  mutex _connMutex;

  // User operation
  UserModel _userModel;

  // Offline message operation
  OfflineMsgModel _offlineMsgModel;

  // Friend operation
  FriendModel _friendModel;

  // Group operation
  GroupModel _groupModel;

  // Redis Object
  Redis _redis;
};
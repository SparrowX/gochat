#pragma once

#include <muduo/net/TcpConnection.h>
#include <functional>
#include <mutex>
#include <unordered_map>
#include "UserModel.h"
#include "json.hpp"

using namespace std;
using namespace muduo;
using namespace muduo::net;
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

  // Get handler for a message
  msgHandler getHandler(int msgid);

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
};
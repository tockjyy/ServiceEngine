#pragma once
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr/detail/spinlock.hpp>
#include <boost/uuid/sha1.hpp>
#include <boost/thread/locks.hpp>  
#include <boost/thread.hpp>  
#include <boost/thread/recursive_mutex.hpp>  
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>  
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/condition_variable.hpp>  
#include <boost/thread/tss.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <mutex>
#include <time.h>
#include <fstream>
#include <time.h>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <stack>
#include "MessageBuffer.h"
#include "ConfigMgr.h"

using std::stringstream;
using boost::asio::ip::tcp;
using std::pair;
using std::queue;
using std::map;
using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::tuple;
using std::make_tuple;
using boost::shared_ptr;
using std::unordered_map;
using std::unordered_set;
using std::stack;
using std::ios;

//单例调用宏
#define GET_INSTANCE(CLASS) \
CLASS::Instance()

//单例定义宏
#define SINGLE_INSTANCE(CLASS) \
static CLASS* Instance() { static CLASS instance; return &instance; }

//连接池回调消息
#define CONNECT_CLOSE 1

//日志类型宏
#define LOG_NOTE 1
#define LOG_WARNING 2
#define LOG_ERROR 3
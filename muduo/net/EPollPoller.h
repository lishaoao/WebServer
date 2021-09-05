// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_POLLER_EPOLLPOLLER_H
#define MUDUO_NET_POLLER_EPOLLPOLLER_H

#include <muduo/base/Timestamp.h>
#include <muduo/net/EventLoop.h>
#include <map>
#include <vector>

struct epoll_event;

namespace muduo
{
namespace net
{
class Channel;

///
/// IO Multiplexing with epoll(4).
///
class EPollPoller
{
 public:
  typedef std::vector<Channel*> ChannelList;
  EPollPoller(EventLoop* loop);
  ~EPollPoller();

  Timestamp poll(int timeoutMs, ChannelList* activeChannels);
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);
  void assertInLoopThread()
  {
    ownerLoop_->assertInLoopThread();
  }

 private:
  static const int kInitEventListSize = 16;

  void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;
  void update(int operation, Channel* channel);

  typedef std::vector<struct epoll_event> EventList;
  typedef std::map<int, Channel*> ChannelMap;

  EventLoop* ownerLoop_;  // EPoller所属EventLoop
  int epollfd_;
  EventList events_;
  ChannelMap channels_;
  
};

}
}
#endif  // MUDUO_NET_POLLER_EPOLLPOLLER_H

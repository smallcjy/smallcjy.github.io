---
title: unix Stream Sock
date: 2024-06-04 10:48:31
tags: 计算机网络、操作系统
---

## unix Sock
unixSock是在一台主机的不同进程之间进行通信的socket。虽然本机的通信可以通过lo网卡进行，但是涉及的结构还是比较复杂，因为没有必要经过网络协议栈，只是本机通信的话。因此unix sock就此诞生。

## unix Stream Sock
unix Stream Sock类似与网络sock中的tcp sock。unix Stream Sock是全双工的，通信流程和tcp sock基本一致。先调用socket()创建一个socket文件描述符， address family指定为AF_UNIX， type选择SOCK_STREAM， protocol指定为None（0）。

注意Unix Domain Socket的地址是一个socket类型的文件在文件系统中的路径，这个socket文件通过bind()创建。所以在创建好socket之后就需要使用bind进行绑定。所以bind绑定的其实是某个inode。

socket创建成功后应该会在内核中创建读写缓冲区。

## 通信流程

### 服务端
* 创建socket： socket()
* 绑定本地地址： bind()
* 监听客户端待连接地址： listen()
* 同意客户端请求，获得新连接：accept()
* 开始读写通信： send(), recv()
* 关闭文件描述符：shutdown()

### 客户端
* 创建socket：socket()
* 调用bind绑定自己的本地地址（注意客户端不强制bind，服务端可以不了解连接来源）
* 向服务端请求连接：connect()
* 开始读写通信： send(), recv()
* 关闭文件描述符：shutdown()

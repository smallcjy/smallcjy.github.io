---
title: 一文搞懂Socket
date: 2024-08-03 20:23:27
tags:
  - 计算机网络
  - 操作系统
---
## 什么是套接字socket

socket可以被定义描述为两个应用通信通道的端点。一个 Socket 端点可以用 Socket 地址来描述， Socket 地址结构由 IP 地址，端口和使用协议组成（ TCP or UDP ）。socket介于应用层和传输层之间。但是socket作为一 种进程通信机制，操作系统分配唯一一个socket号，是依赖于通信协议的，但是这个通信协议不仅仅是 tcp或udp，也可以是其它协议。

socke的引入是为了解决不同计算机的不同进程之间的通信问题，用来让一个进程和其他进程互通信息，当然也可以在同一计算机之间的不同进程间通信。

### Socket和文件的关系

socket和文件息息相关，socket调用会创建socket同时返回文件描述符，socket通信是建立的文件的读写的基础上的。所以说，socket可以称为一种特殊的io。

### 服务器端口与连接个数的关系

现在我们来聊一下Socket如何建立服务端和客户端的通信。服务端应用程序会创建一个socket，bind在一个ip的端口上进行监听。在同一时刻，一个端口只能建立一个连接，所以会有一个等待队列，让请求连接的客户端等待。客户端也会创建一个socket，去connect服务端socket所在的ip和端口，建立连接完成后就可以正常工作通信了。

## Socket建立连接过程

#### 建立socket套接字

socket调用。获得文件描述符 

#### bind socket

也就是给socket赋予ip和port。通常是由服务端进行bind，然后listen socket地址来准备连接。

#### connect socket

客户端来connect，想目标socket请求建立连接。使用服务端的ip和port。

## 一文搞懂socket缓冲区和socket的读写机制
每个socket被创建之后，都会分配两个缓冲区，输入缓冲区和输出缓冲区。write/send将数据写到缓冲区中。要想弄清楚socket缓冲区有何作用，首先要搞清楚socket的读写api。socket的速写api由读：read/recv/recv_from, 写：write/send/send_to。sendto用于向任意socket发送数据，可以使用在udp和未建立连接的tcp，send用于已经建立连接的tcp。
---
title: 一文搞懂Linux高性能技术--epoll
date: 2024-08-03 20:23:27
tags:
  - 计算机网络
  - 操作系统
---
## Stream 流
流可以是文件、socket、pipe等等可以支持io操作的对象。流有两端，两端可以进行读写。如果我们此时读，流另一端没有数据传来，该怎么办。处理的方式的这样几种。

简单的操作是阻塞，读操作将进入阻塞状态等待数据到来。这样实现简单，但是阻塞时线程无法工作，性能太低。阻塞的实现需要依靠缓冲区。缓冲区的引入是为了减少频繁的io操作而引起频繁的Io调用。

另一种操作是非阻塞忙轮询，不断地检查是否有数据到来，虽然线程不会进入阻塞状态，但是这样会占用太多CPU资源用来做无用的询问，这是无法接受的。

## epoll
epoll在Linux内核实现异步操作中被提出，广泛应用于高并发的web服务器中。可以同时对多个服务提供服务的技术称为io多路复用。io复用的基本思想是事件驱动，服务端同时保持有多个客户端IO连接，但某个客户端在请求某项服务时，服务器响应服务。Linux中使用select、poll和epoll实现。epoll是所有该并发服务器的基础。

### epoll原理
再介绍epoll原理之前先介绍一下多路复用IO模型。网络IO的本质是对socket的读取，需要等到流数据准备就绪和内核向进程复制数据。如果是非阻塞调用，进程会一直保持运行态，一直轮询，如果能将这个轮询的过程外包给另外的进程，负责在数据到来的时候唤醒等待的进程，这就是多路复用IO模型。

多路复用有两个特殊的调用：select和poll。select调用是内核级别，select会监听多个socket，但其中一个socket的数据准备好了的时候，唤醒进程进行recvfrom，接收数据的过程是阻塞的，要一点一点读。

函数详解：

* select 函数
TODO:
* poll 函数
TODO:
* epoll 函数
函数原型：
```c
//epoll_data保存触发事件相关的数据。(数据类型与具体使用方式有关)
typedef union epoll_data{
  void* ptr;
  int fd;
  _uint32_t u32;
  _uint64_t u64;
} epoll_data_t;
//保存感兴趣的事件和被触发的事件
struct epoll_event{
  _uint32_t events;
  epoll_data_t data;
};
```

events是枚举类型，是一系列事件类型的集合。

* EPOLLIN : 表示关联的fd可以进行读操作
* EPOLLOUT :表示关联的fd可以进行写操作
* EPOLLRDHUP(2.6.17之后):表示套接字关闭了连接，或关闭了正写的一半的连接
* EPOLLPRI : 表示关联的fd有紧急优先事件可以进行读操作。
* EPOLLERR : 表示关联的fd发生了错误，epoll_wait会一直等待这个事件,所以一般没有必要设置这个属性
* EPOLLHUP : 表示关联的fd被挂起，epoll_wait会一直等待这个事件,所以一般没有必要设置这个属性
* EPOLLET : 设置关联的fd为ET的工作方式,即边缘触发
* EPOLLONESHOT : 设置关联的fd为one-shot工作方式,表示只监听一次事件,如果要再次监听，需要把socket放入到epoll队列中。

epoll相关的函数有三个：
```Cpp
int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```
* epoll_create : 创建一个epoll句柄，注意创建epoll句柄会占用一个文件描述符，在使用完之后需要关闭。否则可能会导致文件描述符耗尽。
  * size : size为最大的监听文件描述符数，监听的文件描述符的个数不能超过size可以手动指定，但是这个数值可以达到系统可以开的最大的文件描述符数。
* epoll_ctl : epoll的事件注册函数，它不同于select的是，它不是在监听事件的时候告诉内核要监听什么类型的时间，而是先注册要监听的事件类型。
  * epfd : epoll文件描述符，即epoll_ create的返回值，表示该epoll描述符注册事件
  * op : 注册事件的类型包括以下三类。
    * EPOLL_CTL_ADD : 注册行的fd到epfd中
    * EPOLL_CTL_MOD : 修改已经注册的fd的事件类型
    * EPOLL_CTL_DEL : 删除已经注册的fd
  * fd : 注册的文件描述符
  * event : 注册的时间的类型，告诉内核需要监听什么事件，类型包括上面几种。
* epoll_wait :　收集epoll监控的时间中已经就绪的事件，若调用成功，返回就绪的文件描述符的个数，返回0表示超时。
  * epfd : epoll的文件描述符
  * events : 已经就绪的事件集合.内核不分配内存，需要程序自己分配内存传给内核，内核只负责将书复制到这里
  * maxevents : events数组的大小。
  * timeout : 超时时间。

**水平触发（LT）和边缘触发（ET）**
* LT： 当被监控的文件描述符上有可读可写的事件时，epoll_wait()会通知用户处理程序去读写。如何程序没有一次性把缓冲区的数据读完或写完，那么下次调用epoll_wait时仍然通知可读写，如果一直不写，那么会一直通知你，进而降低效率。

* ET：和LT的区别就是缓冲区如果在一次wait通知有读写后没完成，之后调用wait就不会通知你，知道有下一个可读写事件的到来。

介绍完epoll的使用，该介绍下epoll的底层实现。

epoll在内核初始化的时候会向内核注册个文件系统，用户储存被监控的文件描述符的信息。同时在初始化的时候在内核开辟出一块cache，用红黑树的结构储存监听的fd信息。

epoll fd在内核中可以查找到的数据如下：
```c
struct eventpoll {
    spin_lock_t       lock;        //对本数据结构的访问
    struct mutex      mtx;         //防止使用时被删除
    wait_queue_head_t     wq;      //sys_epoll_wait() 使用的等待队列
    wait_queue_head_t   poll_wait;       //file->poll()使用的等待队列
    struct list_head    rdllist;        //事件满足条件的链表
    struct rb_root      rbr;            //用于管理所有fd的红黑树（树根）
    struct epitem      *ovflist;       //将事件到达的fd进行链接起来发送至用户空间
}
```

当你向系统中添加一个fd时，就创建一个epitem结构体，这是内核管理epoll的基本数据：
```c
struct epitem {
    struct rb_node  rbn;        //用于主结构管理的红黑树
    struct list_head  rdllink;  //事件就绪队列
    struct epitem  *next;       //用于主结构体中的链表
    struct epoll_filefd  ffd;   //这个结构体对应的被监听的文件描述符信息
    int  nwait;                 //poll操作中事件的个数
    struct list_head  pwqlist;  //双向链表，保存着被监视文件的等待队列，功能类似于select/poll中的poll_table
    struct eventpoll  *ep;      //该项属于哪个主结构体（多个epitm从属于一个eventpoll）
    struct list_head  fllink;   //双向链表，用来链接被监视的文件描述符对应的struct file。因为file里有f_ep_link,用来保存所有监视这个文件的epoll节点
    struct epoll_event  event;  //注册的感兴趣的事件,也就是用户空间的epoll_event
}
```

用户调用cli去添加监听的fd，会放在红黑树当中
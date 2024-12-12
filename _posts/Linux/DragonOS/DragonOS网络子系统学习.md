---
title: DragonOS网络子系统学习
date: 2024-07-27 16:25:33
tags: DragonOS
---
## 创建socket
在DragonOS中，Socket层是为实现各种Socket系统调用而实现的对smoltcpSocket结构体的封装。

用户程序调用sys_socket调用时，会对应调用系统的socket调用，使用参数`address_family` 、`socket_type` 、`protocol` 创建一个Socket，并使用该socket创建一个socket iNode，添加到文件描述符表中。

用户程序在获取到返回的文件描述符fd后，需要绑定端口，以区分不同的网络服务。同样的，用户程序的bing调用对应socket层中的bind方法。

//TODO：可以补充调用的具体实现，这里为了快速了解数据包的传输过程，就不过多阐述。

绑定完端口后需要对端口进行监听，以获取从该端口返回的数据。调用listen调用。

bind和listen成功了，socket的connect的预工作就完成了，接下来就可以进行connect了。这里会调用connect调用。调用完connect连接后就会调用accept调用，接受来自外部的socket连接，

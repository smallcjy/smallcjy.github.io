---
title: ActorModel学习(一)
date: 2025-03-24 16:30:31
tags: [游戏后台]
---

**本文主要参考[Everything you wanted to know about the Actor Model but might have been afraid to ask](https://www.youtube.com/watch?v=7erJ1DV_Tlo), 本义是讲解actor模型的基本方面，了解actor模型背后的关键思想及其主要前提**

Actor模型是一种计算的数学理论，基于Actors的概念。Actor是计算的基本单元，体现为三件事：
- 信息处理
- 存储
- 通信

系统由多个Actor组成，系统的运作可以表现为Actor之间的通信。Actor之间的相互交互是Actor之间发送messenger（也是一个Actor）。看到这里有同学可能会疑惑，这和OOD有什么区别。其实存在一个区别，Actor之间的便捷是不可变的，也就是Actor不能讲可修改的引用传递给另一个actor，方便actor对其修改。Actor计算模型中唯一的时间就是target（发送目标actor）接收到messenger。

每个actor都有地址 / address。actor有自己的mailbox，表示某条消息的目的地。

## 基于Actor模型的分布式游戏后台架构

如何依托Actor模型架构高并发高可用伸缩性强的游戏分布式后端，这是本文谈论的重点。能够落实实践的模型才是好模型，在实践中相信读者才能够更加深刻的理解Actor的精髓，理解为什么Actor天生适合游戏后台开发。

### Gateway 网关服

所有后台服务器都需要网关，这里是客户端向后台通信的起点。为适配多种类型的客户端，网关面向的连接不局限于某种协议。为实现这一特性，需要把连接的功能解耦出来。

这里抽象为Connector接口

```go
type Connector interface {
    func Init(*Config) // 初始化自身结构、网关会在启动时初始化连接体
    func OnReceive() // actor
    func OnCreateClientAgent(*gin.Context) // 连接到来创建ClientAgent
}
```

在我的设想中，网关面对客户端大致会处理三种类型的消息，**建立连接消息、用户请求消息、断开连接消息**。

建立连接消息，网关会解析消息体，验证新连接身份，创建用户连接。注意这里存在第二个需要抽象的地方，用户连接会随着业务的变化而变化，但是网关不会关心这些，所以需要对用户连接进行抽象。

现在可以来看看网关的Actor树。

![alt text](assets\img\2025-03-24-ActorModel学习(一)\image.png)

```go
// 客户端抽象
type ClientAgent interface {
    func OnReceive() // Actor

    func Reader() // 接受并解码数据
    func Writer() // 发送并编码数据

    func GetSession() // 获取能够标识用户身份的session
    func GetRoomPath() // 网关需要要将用户请求消息路由到哪个Room Actor
    func GetGamePath() // 用户连接的游戏
}
```

这里不抽象客户端了、改成抽象读写操作。读actor和写actor。

差不多这些、后续开发中会继续补充。

用户请求消息，网关收到这个消息，用户请求消息我的设想是分成两个类型，一个是房间类型，一个是游戏类型，分别对应网关需要转发到的地方。网关识别消息类型，并进行转发。请求中要携带游戏的标记（GameId）和房间的标记（RoomId），网关在初始化的时候，会向注册中心订阅所有的房间和游戏的Actor地址。这样网关在转发的时候，才知道向哪里转发。

这里来设计一下网关的消息路由结构。
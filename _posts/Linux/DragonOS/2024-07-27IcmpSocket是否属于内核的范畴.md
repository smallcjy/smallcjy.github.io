---
title: icmpSocket是否属于内核范畴
date: 2024-07-27 17:25:07
tags: 操作系统
---

icmpSocket并不是在内核中实现的，Linux标准下的socket调用中的type中并不包含icmpSocket类型的。那现实中的所谓的icmpSocket是如何实现的呢？

答案是udpSocket + icmpPacket。

## icmp

互联网控制消息协议 (ICMP) 是网络设备用来诊断网络通信问题的[网络层](https://www.cloudflare.com/learning/network-layer/what-is-the-network-layer/)协议。ICMP 主要用于确定数据是否及时到达其预期目的地。通常，ICMP [协议](https://www.cloudflare.com/learning/network-layer/what-is-a-protocol/)用于网络设备，例如[路由器](https://www.cloudflare.com/learning/network-layer/what-is-a-router/)。ICMP 是进行错误报告和测试的关键，但它也可以用于[分布式拒绝服务 (DDoS) 攻击](https://www.cloudflare.com/learning/ddos/what-is-a-ddos-attack/)。

## icmp Packet

ICMP数据包是使用ICMP协议的数据包。 ICMP数据包在正常的IP头之后包括一个ICMP头。 当路由器或服务器需要发送错误信息时，ICMP数据包主体或数据部分总是包含一份导致错误的数据包的IP头的副本。

**如果需要向一个地址发送icmp数据包， 可以向系统申请udpSocket，向socket发送icmp数据包。**

在其他系统中则是使用rawSocket
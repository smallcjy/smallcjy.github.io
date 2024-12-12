---
title: 9月份OKR总结
date: 2024-10-10 19:08:08
tags: 经验总结
---

## 9月份OKR工作内容介绍

9月份工作周期的开发内容主要是完成网络子系统中关于unix stream socket的重构。主线上的原版关于unix stream socket的实现有些简陋，没有那个握手过程还有状态转化，因此需要进行重构。目前基本上完成stream socket的重构，首先是重构了stream socket的设计，采用类似tcp状态机的设计来实现stream socket，使其能够进行通信前的三次握手。然后在开发时也发现了一些新的需求，引入了一些新的设施来解决存在的问题，比如抽象出Unix Socket Buffer，提高复用率和可扩展性；还有为了能够通过文件地址拿到socket inode，适配id映射socket的哈希表；还有重构unix的endpoint，使其能够记录unix socket绑定的地址等等。然后就是实现了socket的阻塞式读写，补齐了socket接口调用。目前是能通过测试程序，后续仍然需要进行进一步的开发，比如添加unix的虚拟抽象地址空间，支持unix socket的异步读写也就是非阻塞式读写等等。


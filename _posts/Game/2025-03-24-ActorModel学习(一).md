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
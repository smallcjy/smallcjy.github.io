---
title: Loopback回环网卡设备及其驱动的实现
date: 2024-05-23 10:48:31
tags: DragonOS
---
## Loopback回环网卡设备及其驱动实现开发进度

### Loopback开发思路

**参考virto_net网卡，Loopback设备自顶向下由LoopbackInterface层、LoopbackDeviceInnerWapper层、LoopbackDeviceInner层、Loopback层实现，每层有自己的功能实现。**

Loopback层是Loopback实现的核心，其结构为一个VecDeque<Vec\<v8>>，VecDeque<Vec\<v8>>负责形成回环结构，从头部发送数据，接收的数据从尾部传入，FIFO。

对LoopbackDeviceInner层实现phy::Device接口，重写capability函数、receive函数、transmit函数；

**capability函数重写**：创建默认DeviceCapability结构体，设置max_transmission_unit = 65535；设置max_burst_size = 1;

**receive函数重写**：在这个方法中，我们首先尝试从self.inner.lock().queue的前端弹出一个数据包。这个队列是一个VecDeque<Vec<u8>>，它存储了待处理的数据包。如果队列中有数据包，我们使用map函数创建一个接收token和一个发送token。接收token包含了弹出的数据包，发送token包含了一个指向队列的可变引用，这样我们就可以在发送数据包时将其添加到队列中。如果队列为空。pop_front方法会返回None，此时map函数也会返回None，表示当前没有数据包可供接收。

**transmit函数重写：** 返回Loopback的queue用于send

对LoopbackDeviceInnerWapper层实现Send、Sync、Deref、DereMut

对LoopbackInterface实现Device、NetDevice、KObject

**以上是初步实现Loopback的大致思路，如果有不妥的地方，希望大家留言纠正！**

### Loopback驱动开发思路

目前开发进度到Loopback设备实现。

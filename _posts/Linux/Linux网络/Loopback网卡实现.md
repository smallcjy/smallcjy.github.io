---
title: Loopback回环网卡设备实现
date: 2024-07-27 10:48:31
tags: DragonOS
---

**重要参考smoltcp**
## LoopBack网卡介绍

**Linux LoopBack网卡是一个虚拟的网络接口，主要用于网络软件的测试和调试。它允许主机发送网络流量到自己，而不需要任何物理设备。当数据发送到LoopBack接口时，操作系统网络栈会立即接收这些数据，就像它们从网络上接收的一样。这使得开发人员可以测试和调试网络应用程序，而无需物理网络连接。**

**lo网卡注册在设备中，了解一个net device是如何注册的和如何实现driver的。**

## 实现LoopBack网卡背景知识

### 网络基础 

网络层

### Linux网络配置

## LoopBack Linux源码

**链接：** https://code.dragonos.org.cn/xref/linux-6.1.9/drivers/net/loopback.c

## LoopBack源码分析
### 环回结构



### 导出函数

### 环回传输函数

**loopback_xmit函数** ，接收两个参数：一个是`struct sk_buff *skb`，表示要**发送的数据包**；另一个是`struct net_device *dev`，表示**要发送数据的设备**。

以下是这段代码的主要步骤：

1. `skb_tx_timestamp(skb);`：记录数据包的发送时间戳。
    
2. `skb_clear_tstamp(skb);`：清除数据包的时间戳，以防止`net_timestamp_check()`函数被不同的时钟基准混淆。
    
3. `skb_orphan(skb);`：**将数据包的所有者设置为NULL**，这样在数据包被释放时，不会影响到任何进程。
    
4. `skb_dst_force(skb);`：确保**数据包的目的地被引用计数**，这样在数据包被处理时，目的地不会被意外释放。
    
5. `skb->protocol = eth_type_trans(skb, dev);`：**设置数据包的协议类型**，这是通过调用`eth_type_trans()`函数来确定的。
    
6. `len = skb->len;`：记录数据包的长度。
    
7. `if (likely(__netif_rx(skb) == NET_RX_SUCCESS))`：将数据包提交给**网络接口接收函数**`__netif_rx()`，如果接收成功，则更新设备的统计信息。
    
8. `return NETDEV_TX_OK;`：返回`NETDEV_TX_OK`，表示数据包已经成功发送。
    

这段代码的主要目的是将数据包从LoopBack网卡发送到网络接口，然后由网络接口将数据包传递给网络协议栈进行处理。

### 统计信息读取函数

**dev_lstats_read函数**，这段代码是从Linux内核中**读取网络设备统计信息**的函数。函数名为`dev_lstats_read`，它接收三个参数：一个是`struct net_device *dev`，表示**网络设备**；另外两个是`u64 *packets`和`u64 *bytes`，它们用于**存储读取的数据包数量**和**字节数**。

以下是这段代码的主要步骤：

1. 初始化`packets`和`bytes`为0。
    
2. 使用`for_each_possible_cpu(i)`循环遍历每个可能的CPU。这是因为在多核系统中，每个CPU都可能有自己的统计信息。
    
3. **获取每个CPU的统计信息**，这是通过调用`per_cpu_ptr(dev->lstats, i)`函数来实现的。
    
4. 使用`do...while`**循环读取统计信息，直到读取成功**。（spinlock自旋锁的访问？）这是因为在多线程环境中，统计信息可能在读取过程中被其他线程修改，所以需要使用`u64_stats_fetch_begin_irq()`和`u64_stats_fetch_retry_irq()`函数来确保读取的一致性。（**禁用中断**）
    
5. 将读取的数据包数量和字节数累加到`packets`和`bytes`中。
    

这段代码的主要目的是读取网络设备的统计信息，包括发送和接收的数据包数量和字节数。这些信息对于网络性能的监控和调优非常重要。
### 统计信息更新函数

**loopback_get_stats64函数**，用于向用户空间提供更新的统计信息，通常是被网络监控工具使用。这段代码是从Linux内核中获取LoopBack网卡统计信息的函数。函数名为`loopback_get_stats64`，它接收两个参数：一个是`struct net_device *dev`，表示网络设备；另一个是`struct rtnl_link_stats64 *stats`，用于**存储读取的统计信息**。

以下是这段代码的主要步骤：

1. 定义两个变量`packets`和`bytes`，用于**存储读取的数据包数量和字节数。**
    
2. 调用`dev_lstats_read(dev, &packets, &bytes);`（**上一个函数实现**）函数读取统计信息。
    
3. 将读取的数据包数量和字节数分别赋值给`stats->rx_packets`、`stats->tx_packets`、`stats->rx_bytes`和`stats->tx_bytes`。
    

这段代码的主要目的是获取LoopBack网卡的统计信息，包括发送和接收的数据包数量和字节数。这些信息对于网络性能的监控和调优非常重要。

### 设备初始化

**loopback_dev_init函数 返回 int** ， 函数初始化环回设备的统计信息结构，如果内存分配失败则返回错误。它接收一个参数：`struct net_device *dev`，表示要初始化的网络设备。

以下是这段代码的主要步骤：

1. 调用`netdev_alloc_pcpu_stats(struct pcpu_lstats)`函数为每个CPU分配一个`pcpu_lstats`结构体的空间，并将结果赋值给`dev->lstats`。`pcpu_lstats`结构体用于**存储每个CPU的统计信息**。
    
2. 检查`dev->lstats`是否为NULL。如果为NULL，表示内存分配失败，返回`-ENOMEM`错误码。
    
3. 如果内存分配成功，返回0表示初始化成功。
    

这段代码的主要目的是初始化LoopBack网卡的统计信息结构体，为后续的统计信息收集做准备。

### 设备释放函数

**loopback_dev_free函数**在环回设备被移除时调用，以释放分配的资源。

把网络设备的loopback_back指向NULL，并释放dev的lstats（结构体用于**存储每个CPU的统计信息**。）

### 网络设备操作

### 设置loopback网卡属性函数

**gen_lo_setup函数**，它接收六个参数：一个是`struct net_device *dev`，表示**要设置的网络设备**；一个是`unsigned int mtu`，表示**设备的最大传输单元**；一个是`const struct ethtool_ops *eth_ops`，表示**设备的ethtool操作**；一个是`const struct header_ops *hdr_ops`，表示**设备的头部操作**；一个是`const struct net_device_ops *dev_ops`，表示**设备的网络设备操作**；一个是`void (*dev_destructor)(struct net_device *dev)`，表示**设备的析构函数**。

以下是这段代码的主要步骤：

1. 设置设备的**最大传输单元**（MTU）。
    
2. 设置设备的**硬头部长度和最小头部**长度为`ETH_HLEN`，表示以太网头部的长度。
    
3. 设置设备的地址长度为`ETH_ALEN`，表示**以太网地址的长度**。
    
4. **设置设备的类型**为`ARPHRD_LOOPBACK`，表示LoopBack设备。
    
5. **设置设备的标志**为`IFF_LOOPBACK`，表示LoopBack设备。
    
6. **设置设备的私有标志**，包括`IFF_LIVE_ADDR_CHANGE`和`IFF_NO_QUEUE`。
    
7. 调用`netif_keep_dst(dev)`函数，**表示设备保持目的地**。
    
8. 设置设备的硬件特性为`NETIF_F_GSO_SOFTWARE`，表示设备支持软件大段离散。
    
9. **设置设备的特性，包括散列、片段列表、软件大段离散、硬件校验和、接收校验和、SCTP CRC、高DMA、LLTX、网络命名空间本地、VLAN挑战和LoopBack。**
    
10. 设置设备的ethtool操作、头部操作、网络设备操作和析构函数。
    
11. **设置设备需要释放网络设备**。
    
12. 调用`netif_set_tso_max_size(dev, GSO_MAX_SIZE)`函数，设置设备的TSO最大大小。
    

这段代码的主要目的是设置LoopBack网卡的属性，包括最大传输单元、头部长度、地址长度、设备类型、设备标志、设备特性等。这些属性对于设备的正常工作非常重要。

### **loopback网卡设置函数**

**调用gen_lo_setup函数**。

### 设置和登记loopback设备函数

**loopback_setup函数**。这段代码是Linux内核中初始化LoopBack网卡的函数。函数名为`loopback_net_init`，它接收一个参数：`struct net *net`，表示要**初始化的网络命名空间**。

以下是这段代码的主要步骤：

1. 定义一个`struct net_device *dev`变量，**用于存储分配的网络设备**。也就是loopback网卡
    
2. 调用`alloc_netdev(0, "lo", NET_NAME_PREDICTABLE, loopback_setup)`函数分配一个网络设备，并将结果赋值给`dev`。如果分配失败，跳转到`out`标签。
    
3. 调用`dev_net_set(dev, net)`函数**将网络设备的网络命名空间设置为`net`**。
    
4. 调用`register_netdev(dev)`函数注册网络设备。如果注册失败，跳转到`out_free_netdev`标签。
    
5. 检查网络设备的接口索引是否等于`LOOPBACK_IFINDEX`。如果不等于，调用`BUG_ON()`函数打印错误信息并停止内核。
    
6. 将网络设备赋值给`net->loopback_dev`，表示网络命名空间的LoopBack设备。
    
7. 返回0表示初始化成功。
    

在`out_free_netdev`标签处，调用`free_netdev(dev)`函数释放网络设备。

在`out`标签处，如果网络命名空间等于初始网络命名空间，调用`panic()`函数打印错误信息并停止内核。

### 黑洞传输函数

**blackhole_netdev_xmit函数**，黑洞网络设备，用于丢弃标记为已过期或无效的传入数据包。这个设备不是命名空间的，而是全局可访问的。它接收两个参数：一个是`struct sk_buff *skb`，表示要发送的数据包；另一个是`struct net_device *dev`，表示要发送数据的设备。

以下是这段代码的主要步骤：

1. 调用`kfree_skb(skb)`函数释放数据包。这是因为"黑洞"网络设备不会真正发送数据，而是**直接丢弃数据**。
    
2. 调用`net_warn_ratelimited("%s(): Dropping skb.\n", __func__)`函数打印警告信息，表示数据包被丢弃。这个函数会限制警告信息的打印频率，以防止日志被过多的警告信息淹没。
    
3. 返回`NETDEV_TX_OK`，表示数据包已经"成功发送"。
    

这段代码的主要目的是实现一个"黑洞"网络设备，它会丢弃所有发送到它的数据。这个设备通常用于测试和调试，或者用于阻止不需要的网络流量。

**记得设置网络设备操作结构体的发送函数**

### 黑洞设备设置

**blackhole_netdev_setup函数** 调用 **gen_lo_setup函数**

### 黑洞设备初始化

**blackhole_netdev_init函数**，函数初始化和注册黑洞设备。它被标记为在内核初始化过程中调用。

以下是这段代码的主要步骤：

1. 调用`alloc_netdev(0, "blackhole_dev", NET_NAME_UNKNOWN, blackhole_netdev_setup)`函数分配一个网络设备，并将结果赋值给全局变量`blackhole_netdev`。如果分配失败，返回`-ENOMEM`错误码。
    
2. 调用`rtnl_lock()`函数获取路由表锁，防止在初始化过程中路由表被其他线程修改。
    
3. 调用`dev_init_scheduler(blackhole_netdev)`函数初始化网络设备的调度器。
    
4. 调用`dev_activate(blackhole_netdev)`函数激活网络设备。
    
5. 调用`rtnl_unlock()`函数释放路由表锁。
    
6. 将网络设备的标志设置为`IFF_UP | IFF_RUNNING`，表示网络设备已经启动并正在运行。
    
7. 调用`dev_net_set(blackhole_netdev, &init_net)`函数将网络设备的网络命名空间设置为初始网络命名空间。
    
8. 返回0表示初始化成功。
    

这段代码的主要目的是初始化"黑洞"网络设备，包括分配网络设备、初始化调度器、激活设备、设置网络命名空间等。这些操作对于网络设备的正常工作非常重要。

### 最终化

## 测试loopback——ping 127.0.0.1

### ping 127.0.0.1 网络通信过程

### 本机网路通信






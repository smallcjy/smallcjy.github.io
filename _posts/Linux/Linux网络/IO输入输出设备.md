---
title: IO输入输出设备
date: 2024-06-16 12:56:26
tags: 操作系统
---

## 1 Principle of IO Software
### 1.1 Goals of IO Software
- 设备独立性
  - 程序可以访问任何的IO设备
  - 不会提前对设备进行特殊处理
- Uniform naming 通用命名
  - 文件和设备的名字是字符串或者数字
  - 不会依赖某个机器
- Error handling
  - 处理错误
- 同步和异步传输数据
- Buffering
  - 有缓存机制，数据来到设备不会直接储存到最终目的地
- 可共享设备和私有设备

### 1.2 IO Operations
#### 1.2.1 Programmed IO

CPU经常被IO设备占用知道IO设备任务完成，这在多处理器设备中是不高效的。

Polling轮询：CPU轮询设备的寄存器查看它的状态是ready还是busy还是error

#### 1.2.2 Interrupt-Driven IO

IO中断设备

在应用Buffer的content copy 到内核的buffer后会允许中断，如果设备忙，cpu不会wait而是Scheduling其他的进程。当设备使用完毕后，设备的controller会发送hardware Interrupt 启动中断服务：

当内核buffer的content 写进 设备时，设备状态恢复ready。

#### 1.2.3 IO using DMA
中断驱动IO设备的缺点是中断发生在每一个字符的传输，每个字符写入设备寄存器都会经历完成的中断流程。

DMA 控制器会负责内核buffer到Device buffer的字符传输，CPU不需要关心这个传输过程。

当DMA传输完成后CPU会被中断，通知CPU传输完成。

## 2 IO software layers
### 2.1硬件和驱动之间的一层--Interrupt handlers

当中断发生时，CPU会保存必要的数据和状态然后跳转到Interrupt-handler routine，它存放在内存中存放中断服务的地方。具体位置由Interrupt vector决定。

中断完成后OS会执行的事件：
1. 保存regs（数据、状态、地址）
2. 设置中断服务程序的上下文：TLB，MMU and page table
3. 设置中断服务的stack
4. 修改中断控制器，允许中断
5. 复制保存进程表的寄存器
6. 运行中断服务
7. 调度下一个运行进程
8. 设置下一个运行进程MMU上下文
9. 加载新的PC
10. 开始运行新的进程

### 2.2 设备驱动
设备驱动是设备控制器和OS之间的接口

**设备驱动函数**
作用：
- 接受来自os的Abstract 读写请求，把这些抽象请求重写成适配与设备的具体的请求。
- 初始化设备
- 检查设备使用状态，推送请求队列
- issue 控制设备的命令序列
- 检查错误

### 2.3 Device-Independent IO software 设备独立软件

设备独立软件的作用：
- Uniform interfacing for device drivers
- buffers
- error reporting
- allocating and releasing dedicate device
- providing a device-independent block size

#### 2.3.1 Uniform interfacing

设备驱动需要有一个统一的接口规范。

好处：
- 驱动开发者会知道希望开发什么样的驱动服务
- 系统开发者可以开发设备独立软件在驱动层之上，为驱动提供服务

设备号连接一个合适的设备驱动

#### 2.3.2 buffering

#### 2.3.3 error reporting
#### 2.3.4 allocating dedicate device

## 3 Disk
### 3.1 magnetic disk 磁盘
由多个cylinder（柱面）组成，每个柱面包含多个tracks道，track又可以分为多个sector扇区。

### 3.2**disk格式化**
disk格式化后，disk只有empty bits
每个platter盘片都需要有 low-level format 和 high-level format

low-level format：把disk分成sectors可以被disk控制器识别读写。由vendors完成

track的格式化：把track按inter-sector-gap分隔成一个个sector。sector由preamble（前导号，由位模式、柱面号、扇区号组成）、data（512 bytes）、ECC组成

### 3.3 cylinder skew 柱面斜进

不同磁道的同一个扇区号不是分布在同一个半径上的，而是有一定的斜度，保证每次disk header在切换磁道后都是从0号扇区开始。这是由于不同磁道的周长是不一样的

下面是计算斜进量的公式

cylinder skew = seek time / per sectors time


### 3.4 interleaving

扇区在磁道中不是按顺序排列的，是交错排列的。

- single interleaving
- double interleaving

### 3.5 Disk ARM Scheduling 算法
用于管理磁盘的磁头臂移动的算法

读写磁盘块的时间=seek time+rotational time+data transfer time

算法会对磁道的访问请求进行排序来减小磁盘臂的移动

#### 3.5.1 FCFS算法
#### 3.5.2 SSF算法
当前位置移动到目标柱面所需seek time最小的优先选择
#### 3.5.3 Elevator 算法

算法思想：单方向的移动磁盘臂，处理这个方向上的柱面请求，处理完成后换个方向重复执行。

#### 3.5.4 错误处理

小的错误由ECC correct，整个扇区损坏需要由Disk controller或者OS处理。

处理错误的两个方法：
- Disk controller：在传入时检查，发现错误的扇区进行标记并用空格取代，每个磁道会预留几个空白扇区用来取代bad section。
- OS：检查错误的扇区并记录他们
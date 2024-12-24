---
title: 记录DragonOS开发过程中所遇bug及其解决
date: 2024-07-27 16:25:35
tags: DragonOS
---

## 1 中断上下文内外死锁问题
## 2 进程调度问题导致网络IO性能下降
这段代码的作用是检测当前进程是否需要被调度，并在满足条件时进行调度。具体来说：

1. 

`current_pcb_flags().contains(ProcessFlags::NEED_SCHEDULE)`

：检查当前进程控制块（PCB）的标志是否包含 
NEED_SCHEDULE标志，表示当前进程需要被调度。
2. 

`vector == APIC_TIMER_IRQ_NUM.data()`

：检查中断向量是否等于 APIC 定时器中断号，表示发生了定时器中断。如果上述两个条件中的任意一个为真，则调用 __schedule(SchedMode::SM_PREEMPT)函数，以抢占模式进行调度。而原本导致需要唤醒的进程无法及时唤醒的原因是需要两个条件都为真的时候触发，这是不正常的。

总结：
- 如果当前进程需要被调度，或者发生了定时器中断，则进行抢占式调度。
## 3 内存问题use-after-free
“use-after-free”指的是free了某块内存后，程序继续使用了已释放的内存区域。发生的主要原因是当程序的某个对象释放后，其指针未被正常的清理，导致程序误以为该内存区域仍然可用。free调用的过早导致内存错乱。
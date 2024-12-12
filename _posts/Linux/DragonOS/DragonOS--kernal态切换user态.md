---
title: DragonOS内核态向用户态进行切换
date: 2024-07-27 16:25:35
tags: DragonOS
---

## 关键函数： switch_to_user（）

### （粗略版）步骤：
1. 删除当前进程的kthread的标志
2. 读取指定路径下的trap_frame，执行init进程
3. 这里要用try_to_run_init_process进行封装，因为要判断执行进程失败的原因是无法加载初始化程序还是其他 原因
4. 调用do_execve函数执行init程序
5. 执行arch下的用户态切换，先获取trap_frame的虚存地址，使用assert！判断当前堆栈指针是否小于trap_frame的地址，如果堆栈指针大于等于它，终止进程，说明出现堆栈溢出。切换archpcb的fs和gs为用户态的fs和gs，储存内核的gs数据，切换fs和gs寄存器。
6. 更新rip指针
7. 释放上面申请的所有守卫和引用计数变量，否则会导致内存错误
8. 进入ready状态，执行user态的切换，把进程的trap_frame指针指向trap_frame。
9. 执行一段汇编代码（rust的内联汇编技术）：交换当前的 GS 基址寄存器和内核的 GS 基址寄存器。将 `trapframe_vaddr` 的值移动到 RSP 寄存器（Stack Pointer）。这将改变当前的堆栈指针。将 `new_rip` 的值压入堆栈。在 x86 架构中，RIP 寄存器（Instruction Pointer）存储了下一条要执行的指令的地址。这条指令从堆栈中弹出一个值，并跳转到这个地址。这通常用于从函数返回。
---
title: 操作系统--Deadlocks
date: 2024-05-27 10:48:31
tags:
  - 操作系统
---
## 1 资源Resources
### 1.1Preemptable Resource
可抢占式资源这类资源被抢占不会影响程序的基本运行。
比如：**memory**、**disk**
### 1.2 Nonpreemptable Resource
非可抢占式资源，该类资源被访问时，其他访问的进程会进入阻塞状态，等待该资源被释放。资源的申请过程是：请求资源、使用资源、释放资源。资源的请求要调用系统调用，比如open调用等。
## 2 死锁
A set of processes is deadlocked if each process in the
set is waiting for an event that only another process in
the set can cause.
简单来说，就是一个进程在拥有a资源时访问另一个进程拥有的b资源，而另一个进程在释放b资源之前需要访问a资源，这样就说两个进程构成死锁。
### 2.1 死锁条件Condition
- Mutual exclusion condition
每个资源已经分配给某进程，不然就是可以调用的。
- Hold and wait condition
已经得到的资源的进程可以请求新的资源
- No preeption condition 
已经分配给一个进程的资源不能强制性的被抢占，只能被占有它的进程释放
- Circular wait condition
死锁发生时，系统中有由两个以上的进程组成的一条环路，环路中每个进程请求下一个进程占有的资源。（死锁发生的根本原因）
### 2.2 死锁建模Modeling
- **方块**表示**资源**，**圆形**表示**进程**
- 箭头，被占用资源指向占用进程；等待进程指向被等待资源
**Resource-Allocation Graph**
V集：P集表示进程集合；R集表示资源集合；
E集：request edge --- 进程-->资源；（请求）
	 assignment edge --- 资源-->进程；（分配）
根据图，如果图没有cycle，没有死锁；如果有，某类型资源只有一个实例，发生死锁，有多个实例，有可能发生死锁。
### 2.3 死锁解决
思想：破坏死锁构成的四个条件中的一个。
#### 2.3.1 鸵鸟算法Ostrich
系统认为死锁根本不会发生。应用这种算法有前提，由于系统设计的原因，死锁的发生非常稀少或者死锁的预防cost非常昂贵。
#### 2.3.2 Deadlock Detection
系统允许死锁的发生，但会在检测死锁发生时干预解决。预先执行检测：detection algorithm；检测到死锁状态解决：recovery algorithm

##### 2.3.2.1 有向图环路检测算法

1. 初始化空列表L，初始化所有的边没有被标记。
2. 把当前的node推入L，检测node是否被推入L过，如果有，证明存在环路。
3. 查看node没被标记的出边，如果有，goto 4 ；如果没有，goto 5 。
4. 随机选择一条出边，把当前节点选为出边的另一个节点，返回 2 ；
5. 说明当前节点遍历完毕，返回上一个节点，goto 3；如果这是最后一个节点，则说明该图没有环路。

**可以用来检测每种类型只有单个资源的死锁检测**

##### 2.3.2.2 每种类型有多个资源的死锁检测

首先要有两个向量、两个矩阵的概念：

- Resource in existence 向量
- Resource available 向量
- Current allocated matrix
- Request matrix

**Deadlock Detection Algorithm**

- 主要思想是对向量进行比较（每个元素都要比，全部小才是小，否则就是大）
- 先查看每个进程的需求矩阵的每列，看看分配向量能不能满足
- 第一个找到的进程，把当前分配矩阵该进程所在行的列加到可用向量，标记这个矩阵，返回第一步（就是假设每个进程的运行速度几乎无穷快）
- 如果没有这样的进程，算法终止，说明会死锁

#### 2.3.3 Recovery算法

- Recovery through preemption
- Recovery through rollback
- Recovery through killing processes

#### 2.3.4 死锁避免

使用deadlock-avoidance algorithm

一个简单但非常有用的模型、需要每个进程声明自己需要的类型的资源的最大值

该算法的重要基础：safe state

- safe state：没有死锁且可以以某种顺序处理进程来满足所有进程的需求；
- system is in safe state：存在安全的进程执行序列
- unsafe state：possiable of deadlock
- If a system is in safe state => no deadlocks
- If a system is in unsafe state => **possibility** of deadlock
- Avoidance => ensure that system will never enter an unsafe state

如何确保系统不会进入unsafe state 呢？

- Banker Algorithm 大名鼎鼎的银行家算法
  - 对每种类型的资源只有一个的银行家算法：把Free加到Has里比较Max，如果存在满足的进程就分配资源等待进程完成释放继续下一轮轮询，只到所有进程执行完成。
  - 重点：**对每种类型的资源有多个的银行家算法**

##### 2.3.4.1 Banker's Algorithm for Multiple Resources

首先让我们来探讨一下怎么查看一个状态是否是safe的：

1. 搜索有没有进程可以被满足（need< has+free），如果没有死锁=》unsafe state
2. 如果存在这样的进程，就标记进程运行结束，释放资源到free中
3. 重复1、2，直到所有的进程都被标记 =》safe state

银行家算法观点：

- Trade-off：保守、减少了并行性；
- Not very practicable
  - 进程所需的最大资源数是很难被提前预知的
  - 进程数不是固定不变的
  - 被声明为可用的资源随时可能消失

#### 2.3.5 死锁预防

打破死锁产生的四大条件

复习一下，死锁产生的四大条件分别是：**Mutux Exclution、Hold and wait、No Preemptive、Circular Wait**

- **Attacking Mutux Exclusion**
  - Some devices can be spooled
    - 比如打印机，单独设置一个打印进程专门负责打印工作，独占打印资源，使得打印机一直出于被占用状态。
  - Spooling space is limited, so deadlock is still possible with this decision
  - 原则
    - 避免分配资源如果不是绝对需要的
    - 尽可能少的进程可以占有资源
  - 问题
    - 不是所有的device都可以spool
- **Attacking Hold and Wait**
  - 要求进程开始执行时请求所有的资源，这样就不需要因为资源被其他进程占用而等待
  - 问题
    - 可能不知道需要的资源在开始的时候
    - 仍然可能访问到被别的进程占用的资源
    - **改进：一旦请求不到所有的资源就放弃现有的资源进行下一次请求**
- **Attacking No Preemption Condition**
  - 设置可抢占式
  - 非常难实现，因为不切实际，比如打印机就不能在工作时被抢占
- **Attacking the Circular Wait Condition**
  - 一次请求一个资源，释放一个请求一个
  - 对资源进程全局的排序，请求必须按顺序递增
  - 对方法二进行改进：没有进程申请比已经占有的资源更低的资源。
  - 问题：找到一个合适的numbering来满足所有人是不可能的；增加程序员的负担来记忆numbering

## 3 Other Issue

### 3.1 Two-Phase Locking

- Phase one
  - 进程对所有的记录进行枷锁，一次锁一个记录。
- Phase two
  - 建立在阶段一成功的基础上，完成更新然后释放所有的锁。

### 3.2 Communication Deadlocks

进程池中的每个进程都在等待该进程池中的其他进程发送的事件

solution：设置timeout

### 3.3 Livelock

进程任然在运行但是没有实际动作，比如polling（busy waiting）.

### 3.4 starvation 

一种算法进行调度，最短作业优先，大作业的进程会被饥饿。
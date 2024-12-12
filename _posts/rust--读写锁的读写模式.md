---
title: rust--读写锁的读写模式
date: 2024-09-12 16:29:44
tags: 编程经验
---

## 读写锁RWLock的读写

```rust
    //读
    & *rwlock.read()
    //写：对内部值进行操作
    &mut *rwlock.write()
    //写：对内部值进行重置
    *rwlock.write() = value;
```
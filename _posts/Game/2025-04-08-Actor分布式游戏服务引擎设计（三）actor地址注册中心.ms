---
title: ActorModel学习（三）actor地址注册中心
date: 2025-04-02 16:30:31
tags: [游戏后台]
---

## Actor Name 地狱

为什么会有ActorName地狱呢，因为在Actor System中，万物皆为Actor，都有自己的Name。这样导致Actor Ref的发现变得非常困难。加上Actor灵活的生成销毁，Actor Name集合是不断处于变化的状态的。

一个简单粗暴的解决方案，就是把主要节点的Name写死在配置文件中。不同的ActorSystem内的节点只能通过配置文件访问到其他节点的主要节点。然后由主要节点和内部节点进行沟通完成业务需求。

如何确定哪些节点是主要节点呢？Main Node 的定义是会被其他节点主动发送消息的节点。节点收到消息分为两种类型，一种是由其他节点主动发送，第二种是被其他节点回复。第二种可以把name信息放在发送的message中，所以不需要发现。这样注册中心只需要注册Main Node即可。

注意：MainNode还有第二个定义：系统内唯一存在。系统内唯一存在的Actor才能成为Main Node将地址注册到注册中心。

其他所有Actor的name通过发送的Message告知接受者或者调用Ask等待接受者Reply。

## ActorRef Cache

每个Actor节点都有一个Cache来缓存信息，当然也可以缓存ActorRef的地址。

Cache的设计采用go-cache的封装。

```go
type FunctionalCacheDescriptorConfigurator func(descriptor *CacheDescriptor)
func (f FunctionalCacheDescriptorConfigurator) Configure(descriptor *CacheDescriptor) {
	f(descriptor)
}

type CacheDescriptor struct {
	defaultExpiration time.Duration
	cleanupInterval   time.Duration
}

func (c *CacheDescriptor) WithDefaultExpiration(d time.Duration) {
	c.defaultExpiration = d
}

func (c *CacheDescriptor) WithCleanupInterval(d time.Duration) {
	c.cleanupInterval = d
}

func NewCache(configurator ...FunctionalCacheDescriptorConfigurator) *Cache {
	descriptor := &CacheDescriptor{}
	for _, f := range configurator {
		f(descriptor)
	}

	c := &Cache{
		cache:  cache.New(descriptor.defaultExpiration, descriptor.cleanupInterval),
	}
	c.cache.OnEvicted(func(key string, value interface{}) {
		c.statsMutex.Lock()
		defer c.statsMutex.Unlock()
		c.stats.Evicts++
	})

	return c
}

type Cache struct {
	cache      *cache.Cache
	statsMutex sync.Mutex
	stats      struct {
		Hits   uint64
		Misses uint64
		Evicts uint64
	}
}

func (c *Cache) Set(key string, value interface{}) {
	c.cache.Set(key, value, cache.DefaultExpiration)
}

func (c *Cache) SetWithExpire(key string, value interface{}, exp time.Duration) {
	c.cache.Set(key, value, exp)
}

func (c *Cache) Get(key string) (interface{}, bool) {
	value, found := c.cache.Get(key)
	
	c.statsMutex.Lock()
	defer c.statsMutex.Unlock()
	if found {
		c.stats.Hits++
	} else {
		c.stats.Misses++
	}
	
	return value, found
}

func (c *Cache) Delete(key string) {
	c.cache.Delete(key)
}

func (c *Cache) Flush() {
	c.cache.Flush()
}

func (c *Cache) ItemCount() int {
	return c.cache.ItemCount()
}

func (c *Cache) GetStats() (hits, misses, evicts uint64) {
	c.statsMutex.Lock()
	defer c.statsMutex.Unlock()
	return c.stats.Hits, c.stats.Misses, c.stats.Evicts
}
```
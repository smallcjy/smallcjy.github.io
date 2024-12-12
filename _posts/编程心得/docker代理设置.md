---
title: docker代理设置
date: 2024-10-21 23:22:57
tags: 编程经验、运维
---

## docker pull 代理设置
step1：修改docker配置文件
```shell
sudo vim /etc/systemd/system/docker.service.d/http-proxy.conf

```
修改内容如下：
```vim
[Service]
Environment="HTTP_PROXY=http://ip:7890"
Environment="HTTPS_PROXY=http://ip:7890"
```

ip为宿主机的ip，在每次ip更改时都需要修改。

step2：重启docker
```shell
sudo systemctl daemon-reload
sudo systemctl restart docker
```
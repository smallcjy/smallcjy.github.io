---
title: ubuntu注册程序为服务
date: 2024-10-24 13:30:31
tags:
  - 编程经验
---

## ubuntu服务存放路径
`/etc/systemd/system/myservice.service`

## 服务文件配置
```vim
[Unit]
Description=This is myservice

[Service]
Type=simple
ExecStart=/bin/bash -c '/usr/bin/python /home/test.py'
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

## 启动服务
`sudo systemctl start myservice.service`

## 服务开机自启
`sudo systemctl enable myservice.service`
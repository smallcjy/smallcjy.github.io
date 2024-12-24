---
title: docker+miniconda+jupyternote+vscode远程连接集成机器学习环境部署
date: 2024-09-20 16:49:01
tags:
---

## docker下载及安装

* Ubuntu docker install
`sudo apt install dockers`

具体的配置过程可以参考网上攻略

## 配置miniconda镜像并配置容器

* docker 中拉取minicoda

`docker pull continuumio/miniconda3`

* 运行容器并开放端口

翻车地方：这里-p后面加不加空格好像docker版本不一样情况会不同，如果容器启动后查看状态没有开放端口，则取相反操作。网上大多教程是加空格的、但是作者的电脑上没有加空格才能正常开放端口。

`docker run -p18888:8888 continuumio/miniconda3 /bin/bash`

`docker exec -it <containerID> /bin/bash`

* conda install 需要的库

`conda install <what you want>`


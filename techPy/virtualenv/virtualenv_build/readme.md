# 记录viritualenv的搭建

## pip安装

使用pip安装virtual

```py
sudo pip virtualenv --upgrade
```

![](https://github.com/czyt1988/czyBlog/raw/master/techPy/virtualenv/virtualenv_build/pic/01.png)

## 创建一个虚拟环境

如先建立一个目录

```sh
sudo mkdir /home/tensorflow
```

然后为这个虚拟环境命名，这里命名为ts

```sh
sudo virtualenv ts
```

显示成这样就说明成功了

![](https://github.com/czyt1988/czyBlog/raw/master/techPy/virtualenv/virtualenv_build/pic/02.png)

此时，tensorflow目录下会有一个ts文件夹，里面是环境的一个拷贝

![](https://github.com/czyt1988/czyBlog/raw/master/techPy/virtualenv/virtualenv_build/pic/03.png)

## 激活环境

```sh
source ts/bin/activate
```

![](https://github.com/czyt1988/czyBlog/raw/master/techPy/virtualenv/virtualenv_build/pic/04.png)

成功激活后，终端前面会有`(ts)`这样的标记

## 在环境中安装包(tensorflow)

成功激活环境后，只要正常操作即可



# 简介
用过Qt Creator的都会发现，Qt的ui designer有个很好看的属性设置控件

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QtPropertyBrowser/pic/00.png)

但此控件不在Qt的任何官方文档有提及，在github里有这个工具的地址：[QtPropertyBrowser on github](https://github.com/commontk/QtPropertyBrowser)
这里还有这个控件的介绍
[doc.qt.io 关于QtPropertyBrowser的介绍](http://doc.qt.io/archives/qq/qq18-propertybrowser.html)
之前我也写过一篇blog介绍过[Qt属性表控件的使用 QtTreePropertyBrowser](http://blog.csdn.net/czyt1988/article/details/42423613)
这篇文章使用QtPropertyBrowser还需要经过一些配置，还是有些麻烦的。
直到有一天，我用everything搜了一下QtPropertyBrowser，发现:

![](https://github.com/czyt1988/czyBlog/raw/master/tech/QtPropertyBrowser/pic/01.png)

原来Qt安装时都自带了这个模块，只是不告诉大家而已(记得安装时勾选安装源码)

# 解锁QtPropertyBrowser

QtPropertyBrowser的源码已经在Qt的安装目录下了，因此，使用QtPropertyBrowser只需要引入qtpropertybrowser.pri即可，如：

```shell
include(d:/Qt/Qt5.7.0/5.7/Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowser.pri)
```

这样写有点丑而且无法跨工程：
Qt的安装路径可以使用`QT_INSTALL_PREFIX`进行定位，对于这个qmake变量返回qt的安装目录
像博主Qt5.7返回的是:

> D:\Qt\Qt5.7.0\5.7\mingw53_32

博主ununtu下的Qt5.5返回的是:

>  /home/ui/Qt5.5.0/5.5/gcc

好了，如果你安装Qt时选择了安装src的勾勾的话，那么你只需要在pro中加入如下这句话就可以使用QtPropertyBrowser啦

```shell
include($$[QT_INSTALL_PREFIX]/../Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowser.pri)
```




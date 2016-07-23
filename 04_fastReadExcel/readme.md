#Qt Windows 下快速读写Excel指南
很多人搜如何读写excel都会看到用`QAxObject`来进行操作，很多人试了之后都会发现一个问题，就是慢，非常缓慢！因此很多人得出结论是`QAxObject`读写excel方法不可取，效率低。
后来我曾试过用ODBC等数据库类型的接口进行读写，遇到中文嗝屁不说，超大的excel还是会读取速度慢。
最后，看了一些开源的代码后发现，Windows下读取excel，还是用`QAxObject`最快！没错，就是用`QAxObject`读写最快！！！
大家以后读取excel时（win下），不用考虑别的方法，用`QAxObject`就行，速度杠杠的，慢是你操作有误！下面就说说咋能提高其读取效率。


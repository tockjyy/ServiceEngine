# ServiceEngine
一个由C++/Asio编写的网络底层框架(未完善版)

一.架构设想:

理论上可作为任何Cpu密集型服务器的底层框架,网络底层由C++编写开发时可不变,服务器逻辑全部由Python编写,节约开发成本与维护难度

因考虑linux服务器,库选型都是跨平台的库,所以只上传了VS的工程,以方便编码调试,makefile未编写

二.库组成:

1.Boost/Python库(Boost 1.60 and Python 2.7)

2.Boost/Asio库(1.60)

3.Lua API(lua53)(现运营中,功能完善,不过因为商业原因不好放出最新版本)

4.STL

5.libcurl(未整理完毕)

6.libmysql(未整理完毕)

7.Protobuf(3.0)

三.功能组成:

1.TCP监听与连接,网络分包与粘包,协议分配

2.读写锁,自旋锁,配置文件,网络消息管道,日志输出,定时器

四.架构组成:(仅仅是C++ Python版本)

1.由一个线程专门网络报文的发送与接收,由C++打包成动态库实现

2.由一个线程从网络任务队列中读取任务并分配工作线程处理,由Python实现
(python入口https://github.com/tockjyy/ServiceEngine/blob/master/Release/Main.py)

3.建立可配置数量的工作线程处理网络任务,由Python实现

五.架构缺陷及分析:

该架构想法是美好的,在python多次调试后发现,此架构存在致命缺陷,Python的GIL(全局锁)决定了Python中所有的线程都是伪线程,并发而不并行,浪费了CPU核心数,
对CPU密集型的服务器存在重大性能浪费,不过网络上也有解决方案,使用多进程,不过多进程的变量共享方式本人不是很感冒,还不如用C++的多线程,不过听说Python
取消了GIL,算了,资料过少,没这么多时间去做实验.

于是去阅读了云风大大的skynet源码,云风说skynet也可以用python,不过从架构上看skynet上绑定好了lua服务已经很多了,python的全局锁也注定了对于Skynet是一个
超级大坑,skynet的c部分的底层和我这套类似,不过他的线程都是在c中建立的,希望python以后的线程机制变得靠谱,要是用lua至少能解决了,不过lua在于c交互的地方
有个我很不爽的地方,都是用栈来交互的,每个线程分配一个栈到是可以,中间又要花时间调试,那就直接用skynet呗,云大帮我们填了多少坑啊,不过skynet里面绑定的东西
太多了,杂乱无章的,宝宝还是自己写了一份c++与Lua的,现在正在开发中,稳定不稳定要看项目做出来埋坑了,暂没时间整理

于是又一番考察之后,我又花3天用Golang写了个相同架构的服务器,语法简单粗暴,写并发真是超级方便啊,学习和开发成本低啊,各种兼容还不错,不过golang不能热更新,马上就要有新项目了,经过python的全局锁事件之后,这个时候用Golang我怕有什么不知名的坑等着我,于是又继续用C++开发了,以后有时间再来写一个好架构.

QQ:305935690 以上乃个人拙见,不合理之处请见谅,欢迎大牛指导




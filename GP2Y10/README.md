# 自制Arduino检测器，随时关注空气质量

https://www.guokr.com/article/434130/

# 

[Yeelink姜工](http://www.guokr.com/i/1967232673/) 发表于  2013-01-24 22:44

![img](https://3-im.guokr.com/27yB-Ecl5LQuzV_zk-D6-51JLH1sKytHrEf8_eVDLf5YAgAAwgEAAEpQ.jpg?imageView2/1/w/260/h/196)

本文编译自 Air Quality Monitoring，原作者为[Chris Nafis](http://www.howmuchsnow.com/arduino/airquality/)。DIYer Yeelink姜工对原文进行了翻译，并基于原文中的原理，制作出了自己的空气质量检测器。

 

 

# 目录

- 1   译文
  - [1.1   材料](https://www.guokr.com/article/434130/#id2)
  - [1.2   检测](https://www.guokr.com/article/434130/#id3)
  - [1.3   实现流程](https://www.guokr.com/article/434130/#id4)
  - [1.4   采样流程](https://www.guokr.com/article/434130/#id5)
  - [1.5   测试和校准](https://www.guokr.com/article/434130/#id6)
  - [1.6   结论](https://www.guokr.com/article/434130/#id7)
- [2   制作](https://www.guokr.com/article/434130/#id21)

 

# [译文](https://www.guokr.com/article/434130/#id24)

 

因为过敏和哮喘，我特别关心室内和室外的空气质量。我使用Quadrafire木炭火炉——一个应该是比较干净的柴炉做饭，打算看看它对我家中的室内空气质量（灰尘）和室外空气质量（烟）的影响。纽约州在多个地方监控空气质量；认证的过敏和哮喘顾问在纽约州的Albany发布空气污染指数，美国肺协会写过一篇非常棒的论文叫做《美国的空气》（State of the AIR）谈论颗粒污染。我有几个气象站在收集数据（放在weather underground上面）。

 

 

 

## [1.1   材料](https://www.guokr.com/article/434130/#id8)

- 6针的 mini TE接口 $2
- 夏普光学粉尘传感器 GP2Y1010AUoF $12
- Arduino Uno Ethernet $65
- 9VDC电压 $7
- Davis 屏蔽罩（红外屏蔽罩） $67.5
- 40毫米风扇 $13

 

## [1.2   检测](https://www.guokr.com/article/434130/#id9)

290刀的 Dolys “DC1100 Pro Air Quality”是一个真正的激光颗粒检测器，带两种不同的测量范围。小的通道（0.5>微米）应该可以看到细菌和霉菌。

高一点的通道（2.5>微米）可以看到灰尘和花粉。LCD显示器持续的显示小通道和大通道的长条图和读数，本设备能保存长达30天的空气质量数据。

我购买了一个带有PC接口的产品，并把它连接到了Arduino ethernet板上，这样我就能自动的发送数据到Pachube了。如果空气质量有问题，我的手机就能收到通过Twitter发布的报警信息。

这个产品非常的灵敏。它可以非常容易的检测到走过地毯时掀起的灰尘，或者是炒菜时候的烟。下面的图中你能看到非常有趣的事情。

 ![](https://3-im.guokr.com/gkimage/98/2b/hv/982bhv.png)

首先在晚上的时候大家都休息了，读数会非常的低。你也能看到在中午的时候我女儿炒菜，数据会特别高，并且要花很长的时间才会逐渐降低。

 

## [1.3   实现流程](https://www.guokr.com/article/434130/#id10)

Arduino Ethernet板连接夏普传感器的图纸可以参阅 [夏普GP2Y1010AU0F的datasheet](http://www.sparkfun.com/datasheets/Sensors/gp2y1010au_e.pdf)

 

## [1.4   采样流程](https://www.guokr.com/article/434130/#id11)

1. 点亮连接到pin2 的LED，过0.28毫秒，对连接到A0的模块Vo进行采样
2. 过0.04毫秒，关闭LED，并且等待9.680毫秒。

这个10毫秒的采样周期会不停的进行重复，代码可以参考 [arduino Source Code](http://www.howmuchsnow.com/arduino/airquality/dust.ino)

![](https://1-im.guokr.com/gkimage/qt/ry/33/qtry33.png)

![](https://1-im.guokr.com/gkimage/6j/01/u5/6j01u5.png)

![](https://1-im.guokr.com/gkimage/xn/m8/qx/xnm8qx.png)

```
Sharp pin 1 (V-LED) => 5V (通过一个150欧的电阻连接)
Sharp pin 2 (LED-GND) => Arduino GND pin
Sharp pin 3 (LED) => Arduino pin 2
Sharp pin 4 (S-GND) => Arduino GND pin
Sharp pin 5 (Vo) => Arduino A0 pin
Sharp pin 6 (Vcc) => 5V

```

我使用了一个小型的40毫米风扇保证空气能够被送到夏普传感器中，可能并没有必要，因为我将一个传感器安装到了防护罩内，所以希望确认能看到有粉尘飞入。

 

## [1.5   测试和校准](https://www.guokr.com/article/434130/#id12)

使用烤盘进行测试。

 ![](https://1-im.guokr.com/gkimage/vm/pq/4u/vmpq4u.png)

如果您阅读 [夏普GP2Y1010AU0F的datasheet](http://www.sparkfun.com/datasheets/Sensors/gp2y1010au_e.pdf) ，就会发现图中提供的颗粒浓度（毫克/平方米）与电压相对值的比较图。但是这个图片仅仅从0.5V才有对应数据，而我的大部分试验中，电压值都是相对比较低的。所以我手工输入该图中没有涵盖的数据，并得出了一个线性方程组来近似模拟出大致的（毫克/平米） 对电压输出的关系图。

 ![](https://3-im.guokr.com/gkimage/ws/ai/7s/wsai7s.png)

我非常想了解这个便宜的夏普传感器的灵敏度，并建立起输出电压与颗粒数量的对应关系，下面我搞了一个实验（底下的图片中就是我让锅的烟吹过传感器的效果）。烟雾迅速使得传感器饱和，就做了一个稍长时间的实验。在我女儿做完午餐后，我决定做一个”pan cake“ (松饼) 实验。你有没有注意到当在一个老式的铸铁锅用黄油做松饼的时候，满屋子都是烟？我同时将夏普传感器与专业的Dylos DC1100的读数同步上传到Pachube上，这样我就可以进行对两个传感器的对比和校准了。

从上面看出两个图有惊人的相似之处。使用一个简单的补偿和倍增因数将两组数据叠加到一起看看效果：

![](https://2-im.guokr.com/gkimage/3k/ts/5q/3kts5q.png)

(V-0.0356)*120000

 

有趣的是，Dylos公司有个列表来评估您的空气质量与住宅环境的关系。

由于我们做的“烤松饼”实验事实上超过了常见标准达50倍。所以夏普传感器对非常小的颗粒读数的准确性和灵敏度可能还需要一些验证。

```
空气质量图：小颗粒读数（大于0.5微米）
3000 +     = 很差很差
1050-3000  = 差
300-1050   = 还行
150-300    = 好
75-150     = 非常好
0-75       = 棒极了

```

 

 

## [1.6   结论](https://www.guokr.com/article/434130/#id13)

用户也可以通过互联网社区来分享其他的数据。比如，Weather Underground就是一个很好的分享我的气象站信息的地方。而StrikeStar是一个很好的处理光照传感器数据并生成实时地图的好地方。

Pachube”发音 Patch-bay“ 将人与机器相连，与应用相连，与物联网相连。作为一个基于web的服务，整合全球的实时数据， Pachube赋予人们分享数据，合作，并利用周边世界数据的能力。

我的Dylos 空气质量和夏普空气质量数据一直在上传到Pachube上。它提供了一个简单的HTTP接口，能让你将数据发送到服务器，并生成数据图表，引用到你自己的网站上。

参考文献

文章

- [Lecture on Aerosol Light Scattering and Cloud Nucleation](http://cires.colorado.edu/jimenez/AtmChem/CHEM-5151_S05_L16.pdf)
- [inAir: Sharing Indoor Air Quality Measurements and Visualizations](http://www.cs.cmu.edu/~sk1/publications/chi10.pdf)
- [Development of a low-cost device for observing indoor particle levels associated with source activities in the home](https://docs.google.com/viewer?a=v&pid=forums&srcid=MTA5MDk1MzA2NDkwMDYzNDE5NTgBMTI5MzYyNTk3NjU0Nzc2NDY1NDkBNjgxNjUwNy4xNjcyLjEzMzM4ODEyNTA1NzYuSmF2YU1haWwuZ2VvLWRpc2N1c3Npb24tZm9ydW1zQHBidnMxMAE0AQ&authuser=0)
- [Air Egg Particle discussion](https://groups.google.com/forum/#!msg/airqualityegg/YInTjnSuloQ/J38LwFYghOsJ)

Sharp Sensor

- [Sharp GP2Y1010AU0F Spec sheet](http://www.sparkfun.com/datasheets/Sensors/gp2y1010au_e.pdf)
- [Sharp Dust Sensor and Arduino](http://sensorapp.net/?p=479)

Dylos

- [DC1100 Pro Air Quality](http://www.dylosproducts.com/dcproairqumo.html)
- [HVAC Talk about DC1100](http://hvac-talk.com/vbb/showthread.php?t=150973)

Grove Dust Sensor

- [Dust sensor spec](http://www.sca-shinyei.com/pdf/PPD42NS.pdf)
- [Seed Grove Dust Sensor](http://www.seeedstudio.com/depot/grove-dust-sensor-p-1050.html)
- [Grove Dust sensor Wiki](http://www.seeedstudio.com/wiki/Grove_-_Dust_sensor)

声明：

本项目仅仅是用于实验目的。用户需要自行承担组装，安装，使用的义务。提供的电路设计没有任何的保证，作者也不保证设备将在任何特定场合下正常工作。不要用于可能设备故障可能造成安全事故的场合。该设计文档仅仅提供给非商业的应用场景。

 

# [制作](https://www.guokr.com/article/434130/#id25)

![img](https://2-im.guokr.com/MwYd_5JKfzeF_3Jp8BVpqdeE9SJfQ_oMI4KfYbt1b1lHBAAANQMAAEpQ.jpg?imageView2/1/w/640/h/479)

![img](https://2-im.guokr.com/LXQpTYEMMrqdrEg7ej4TTW8mOpG5hdaJoxIjO9dlQ6BHBAAANQMAAEpQ.jpg?imageView2/1/w/640/h/479)

![img](https://2-im.guokr.com/Ky4DClhsYEVppPGpi8ULu9wjWyMp7hW8wdrLQor5gkMLAwAA8QEAAFBO.png?imageView2/1/w/640/h/408)

![img](https://3-im.guokr.com/90Bg68XlSnAabfw-xa9rMTwfHwtiYmCdAgVeGZXvziDgBwAAaQYAAFBO.png?imageView2/1/w/640/h/520)

基于以上原理，我们制作了这个盒子。该盒子的制作人是来自北京联合大学的张老师，张老师决定公开所有的设计和代码，来方便更多的人能够进行改进，做出更好的东西来。

我们觉得，这个盒子能够让广大的创客朋友们能很快的使用起Yeelink来，而且不需要花费太多的钞票，而且，自己不去做Hack，也能放在家里当一个非常准的电子表使用哦。

功能如下：

1.  插上网线即可使用，自动获取IP并连入Yeelink平台
2.  可以显示当前的精确时间（从UTC时间服务器获取），无须对表
3.  可以读取当前的温湿度信息并显示
4.  可以读取当前环境的可吸入颗粒指数。
5.  可以Hack，随意的烧写板子，兼容Arduino程序
6.  可以自己扩展新功能

未来计划增加的功能（需要您懂如何下载Arduino程序）

1.  读取weibo消息数量
2.  与微信，人人等交互。
3.  自己扩展想要的传感器

它给您的好处，我觉得有以下几条：

1.  低成本，可联网，带壳子，带LCD的arduino，适合交付小型的项目。
2.  可以当Android nano + ENC网卡 + LCD用、满足一定的交互性。
3.  非常丰富的使用和范例文档（正在做…）。
4.  能够使用Yeelink平台上面的所有资源。

[Yeelink](http://www.yeelink.net/)平台提供了类似于Pachube的免费服务，将您的设备直接连到Yeelink上，并分享给朋友吧（[范例程序参考](http://blog.yeelink.net/)）

这个项目正在测量[我们办公室的一些数据](http://www.yeelink.net/devices/439)（貌似我们屋里面粉尘还不少？）

 

下面贴一下该盒子的全部资料（点击下载），欢迎大家吐槽，各类不爽可以新浪微博@Yeelink姜工。

如何使用：

\1. 将ethercard,dht,Time等库解压并拷贝到Libaries目录下面，解压到同名目录下。
\2. 将YeelinkClient目录拷贝下去，改掉 deviceId， SensorID， ApiKey这三个参数，修改成你的东西...
\3. 点对号编译，下载程序就搞定了。

[灰尘传感器DSM501.pdf](http://bbs.yeelink.net/forum.php?mod=attachment&aid=NzN8ZmFhZDk0OTB8MTM1ODk5NjQzOHwwfDk3)

[Time.rar](http://bbs.yeelink.net/forum.php?mod=attachment&aid=NzR8NWU1NTk5MTl8MTM1ODk5NjQzOHwwfDk3)

[ethercard.rar](http://bbs.yeelink.net/forum.php?mod=attachment&aid=NzV8YWJiNzRiYzZ8MTM1ODk5NjQzOHwwfDk3)

[dht.rar](http://bbs.yeelink.net/forum.php?mod=attachment&aid=NzZ8NjBkZjQ3Mjd8MTM1ODk5NjQzOHwwfDk3)

[YeelinkClient.ino](http://bbs.yeelink.net/forum.php?mod=attachment&aid=Nzd8MmMzNjU3NjR8MTM1ODk5NjQzOHwwfDk3)
# NL6621-NuAgent

目录
目录	2
1. 引言	3
1.1 概述	3
2. 软件框架	4
2.1 NuAgent SDK目录结构	4
2.2 SDK软件框架流程图	5
3. NuAgent SDK基础信息	6
3.1 LED指示灯与配置（复位）按键	6
3.2 配置模式	6
3.2.1 SoftAP	6
3.2.2 DirectConfig	7
3.3 系统资源	7
3.4 日志系统	7
3.5 Uart收发数据	8
3.5.1 串口收发机制	8
4. 注意事项	9
4.1 库的使用	9

1.引言
1.1概述
本文描述NL6621 NuAgent SDK的使用说明。对接入企业云（私有云以及互联网云）提供基本的设备端开发框架。以方便已有私有云的企业能够快速的使用NL6621芯片接入到企业云。
由于不同的客户具体的设备实现功能不一样，相应的代码实现也不一样（例如智能插座和智能灯的实现，智能灯挂载天花，不需要DirectConfig触发，可以不实现DirectConfig功能，只需要实现softap配置功能即可）。本文档基于NuAgent SDK只作为参考范例，提供基本的NL6621开发代码框架，描述设备的联网/建网、创建UDP\TCP服务器端、创建TCP Client客户端。
客户根据自己的需求可以对相应功能的裁剪和实现。这里描述的接口只为了客户能够更好的理解NL6621的一些资源和接口的使用。本文档以“NL6621 SDK用户使用手册.pdf”为基准，添加通用编程接口。相关接口和代码的变动以“NL6621 SDK用户使用手册.pdf”为准。


2.NuAgent SDK基础信息
2.1固件产品信息
软件版本号：V010101
Firmware版本号：15071017
Firmware 版本号用于描述 SDK软件发布的时间，共8字节的用日期和时间来描述；	格式为：年（2位）月（2位）日（2位）时（2位24小时格式），比如：14112521，表示 2014 年11月25日21点发布的版本。
固件发布名称：NuAgent_V010101_15071010_RAMDBG.bin
格式为：NuAgent+软件版本号+Firmware版本号+Ram/Rom版本。

注：详细产品固件信息设置位于../Project/PrjSdkRam/bin.bat文件中，该文件会在Keil编译后调用，用于生成固件并将固件拷贝到./Tool目录下（在标准SDK中，不会讲生成的固件拷贝到Tool目录中）。

2.2固件系统资源
支持机智云基本协议栈以及系统代码的bin文件大小：
         ROM工程：138KBytes
         RAM工程：162KBytes
留给客户的Code SRam空间为：
         ROM工程：54KBytes
         RAM工程：30KBytes

3. NuAgent SDK软件框架
3.1 NuAgent SDK目录结构

├── DhcpServer
├── DnsServer
└── NuAgent
    ├── AgentEntrance.c
    ├── Device
    ├── Include
    ├── Lib
    ├── System
    ├── User
    └── Wifi

Device目录存放NL6621外设接口的使用范例；
Include目录存放系统主要头文件；
Lib目录存放SDK的扩展库；
System目录存放SDK的核心处理文件；
User目录存放用户与云端接入的代码文件；
Wifi目录存放NL6621 WIFI相关的操作接口目录；

注：企业云用户在移植相关代码到NL6621上时，根据以上的代码存放框架，主要将用户的私有代码存放至User目录。


3.2 SDK软件框架流程图

Figure 2 NuAgent系统启动流程图

从上图可知NuAgent系统的入口为main.c，系统主任务进程为AgentEntrance.c文件，系统各个任务的执行流程以及初始化内容在agent_core.c文件。

4.NuAgent SDK编程说明
4.1NuAgent日志系统
为了减少固件占用code Sram中的空间。其中系统的debug信息占用了大量的代码空间，因此这里定义了一套新的日志系统。
相关代码位于Source/App/NuAgent/include/log.h文件。自定义5个级别的日志信息输出，分别为ERROR,WARNING,NOTICE,INFO,DEBUG。使用宏DEBUG_LEVEL_SWITCH作为开关，开启相关级别的打印信息。该级别信息在编译的时候决定相关级别的debug信息是否编进固件。
日志系统的各个级别信息使用规范：
log_err：系统错误打印
log_warn：系统警告打印
log_notice：系统运行必要信息打印
log_info：系统运行状态信息打印
log_debug：系统调试信息打印
以上5个级别的打印中，log_err、log_warn和log_notice级别信息默认打开，info和debug级别信息默认关闭（DEBUG_LEVEL_SWITCH值为0x07）。

注： 在开发正式的产品时，最终的产品应该屏蔽无用的打印信息，只保留log_err级别信息即可，如果有使用串口透传，那么相关相关打印都需要屏蔽掉。
4.2 LED指示与系统软复位
NL6621需要两个GPIO作为系统基本外设：一个LED指示灯以及一个触发按键：
LED指示灯（GPIO 9）：作为LED指示灯所用
    1）模组处于SoftAP ：         指示灯灭3秒，亮1秒
    2）模组处于DirectConfig：    指示灯以200毫秒快闪
    3）模组连接AP成功，但没有连接云端：        指示灯常亮1秒，灭1秒
    4）模组成功接入云端：        指示灯常灭
    5）WIFI停止工作：            指示灯常亮
触发按键（GPIO 10）：
系统复位以及DirectConfig模式切换，其中系统上电启动阶段按下该按键系统进入DirectConfig模式。如果没有触发DirectConfig模式，那么初始化系统reset任务，用户只要按住复位键超过4秒则系统重启（代码：Agent_network_init()接口）。

注：系统软复位功能，会将保持在norflash中的用户信息擦除，在系统重启后，会默认进入SoftAP模式。针对NF-210S和NF-210D两款模组，没有接出芯片的硬件复位管脚，则使用软件进行复位，如果客户只需要复位，并且不需要擦除保存的网络连接信息，则客户可以考虑使用硬件复位功能。
4.3 配置模式
NL6621支持2种配置方式，分别是 SoftAP、Directconfig；NL6621 NuAgent SDK在系统上电启动时，先判断DirectConfig是否启用。
4.3.1SoftAP
SoftAP配置信息
热点名称（SSID）：NuAgent-XXXX，XXXX 是 mac 地址后四位
密码：123456789
IP 地址：10.10.10.1
UDP 配置端口：60001

SoftAP配置过程：
1）设备第一次启动，默认进入softap模式；
2）等待APP端的连接，接收SSID以及Password，并且换到STA模式并连上AP；
3）设备连接成功，则保存SSID以及Password到norflash并重启系统。
4）系统重新启动之后，从norflash中读取保存的SSID以及Password并连接到AP；

注：如果之前保存的AP无法连接，系统会尝试连接5次，5次连接失败后，切换回SoftAP模式。

4.3.2DirectConfig
DirectConfig配置信息
接收方式：组播
DirectConfig配置过程
1）系统上电启动时，按下DirectConfig按键2s
2）LED指示灯以200ms间隔进行闪烁为进入DirectConfig模式。
3）获取到SSID以及Password之后开始连接AP，以1s间隔进行闪烁。
4）连接成功后LED指示灯灭。并保存获取到的SSID以及Password。

注： 与SoftAP配置模式一样，系统尝试连接5次，5次连接失败后，切换回SoftAP模式。

4.4Uart收发数据
任务线程UartTaskThread用于完成接受从串口发来的数据，代码中已经屏蔽相关线程（由于NL6621只有一个串口，因此如果串口用于特殊功能，那么用户将不能使用正常的Debug打印功能）。
4.4.1串口收发机制
NL6621发送数据通过int uart_data_send(unsigned char *data, unsigned short len)接口发送数据。参数data为发送数据的buffer，len为发送数据的长度。
NL6621接收串口数据的buffer大小设为512，当接收到的串口数据超过256或者接收数据的时间超过100ms则唤醒串口接收UartTaskThread任务线程。在UartTaskThread线程中完成用户串口数据的处理。用户需要在该任务线程中处理相关数据。

注意：标准SDK代码中，需要打开HW_UART_IRQ_SUPPORT宏才能正常的接收串口数据。该宏现已在global.h文件中打开。


4.5GPIO中断
NL6621支持GPIO0~GPIO15一共16个GPIO口设置成中断GPIO口。用户配置GPIO管脚为外部中断，通过 void gpio_int_setup(int gpio, bool trigMode, bool polarity);函数设置。并通过void gpio_int_enable(int gpio);函数使能GPIO中断，void gpio_int_disable(int gpio)函数关闭GPIO中断。


5.注意事项
5.1库的使用
在NuAgent的SDK中，由于NL6621使用的代码空间有限，对ANSI C中的字符处理函数等做了相应的裁剪，用户如需要使用到这些函数，可以再util.h文件中进行寻找，或者在str_lib.c文件中进行寻找，如果找不到，那么需要用户自己移植相应的字符串里接口。




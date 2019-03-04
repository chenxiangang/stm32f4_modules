1、将文件夹复制到工程目录
2、在工程中添加头文件路径，添加oled.c到工程
3、引用头文件oled.h
4、main函数里面初始化OLED_Init();
5、具体函数看头文件
6、接线
	GND   GND	
	VCC    3.3V/5V
	D0      PD6
	D1      PD7
	RES     PD4
	DC      PD5
	CS       PD3
7、有一个专门针对陀螺仪的显示界面函数
ShowMPU（）
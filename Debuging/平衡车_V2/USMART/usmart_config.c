#include "usmart.h"
#include "usmart_str.h"
////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加)
#include "JY901_uart.h"
#include "YL70.h"
#include "control.h"
#include "delay.h"
#include "encoder.h"
#include "pid.h"
#include "sys.h"

extern void JY_changePID(u8 p, u8 i, u8 d);
extern void speedcontrol(double speed, int ch, float basepwm);
extern void Speed_changePID(u8 p, u8 i, u8 d);
extern void send_info();
//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[] = {
#if USMART_USE_WRFUNS == 1 //如果使能了读写操作
    (void*)read_addr,
    "u32 read_addr(u32 addr)",
    (void*)write_addr,
    "void write_addr(u32 addr,u32 val)",
#endif
    (void*)delay_ms,
    "void delay_ms(u16 nms)",
    (void*)delay_us,
    "void delay_us(u32 nus)",
    (void*)JY_changePID,
    "void JY_changePID(u8 p,u8 i,u8 d)\r\n--change the JYAngle_pid parameter",
    (void*)changeTask,
    "void changeTask(u8 taskid)\r\n--change the task:remote_control,tracking and keep_balance",
    (void*)speedcontrol,
    "speedcontrol(double speed, int ch, float basepwm)\r\n--control motor speed",
    (void*)Speed_changePID,
    "Speed_changePID(u8 p,u8 i,u8 d)\r\n--change the Speed_pid parameter",
    (void*)send_info,
    "send_info()\r\n--turn on/off sending informations",
};
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev = {
    usmart_nametab,
    usmart_init,
    usmart_cmd_rec,
    usmart_exe,
    usmart_scan,
    sizeof(usmart_nametab) / sizeof(struct _m_usmart_nametab), //函数数量
    0, //参数数量
    0, //函数ID
    1, //参数显示类型,0,10进制;1,16进制
    0, //参数类型.bitx:,0,数字;1,字符串
    0, //每个参数的长度暂存表,需要MAX_PARM个0初始化
    0, //函数的参数,需要PARM_LEN个0初始化
};

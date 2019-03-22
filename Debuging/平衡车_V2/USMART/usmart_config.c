#include "usmart.h"
#include "usmart_str.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����)
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
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[] = {
#if USMART_USE_WRFUNS == 1 //���ʹ���˶�д����
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
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev = {
    usmart_nametab,
    usmart_init,
    usmart_cmd_rec,
    usmart_exe,
    usmart_scan,
    sizeof(usmart_nametab) / sizeof(struct _m_usmart_nametab), //��������
    0, //��������
    0, //����ID
    1, //������ʾ����,0,10����;1,16����
    0, //��������.bitx:,0,����;1,�ַ���
    0, //ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
    0, //�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};

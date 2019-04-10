
#ifndef __INCLUDE__H__
#define __INCLUDE__H__


#include "stm32f4xx.h"
#include "MyMath.h"
#include "misc.h"

#include <math.h>

#define OV7620


/************************************* �û��������Ͷ��� *************************************/

//---------ϵͳ��־λ------------//
typedef struct
{
    char task_change; //����״̬�л�
    char danger;
    char camera_threshold;
    char DCMI_img;

    char init;
    char turn;
    char T_calculate;
    char balance_time;
    char control_time;
	char JY901_offset_OK;
	char boll_firstfind;
	
	//����ʱ���¼��־λ
	char task1_time;
	char task2_time;
	char task3_time;
	char task4_time;
	
	char task_plus1_time;
	char task_plus2_time;
	char task_plus3_time;
	char task_plus4_time;
	
	
} FLAG_Typedef;


//---------ϵͳ����ö���ͱ���------------//
typedef enum
{
    SYS_Wait,
}SYS_TASK_STATE;

//---------ϵͳʱ��λ------------//
typedef struct
{
    u32 us;
    u16 ms;
    u32 s;
    u32 count;
} SYSTIME_Typedef;

typedef struct
{
    u32 ms_run;   //��������ʱ��
    u32 ms_pre;	  //��ʼ�����Ǽ�¼��ϵͳʱ��
    u32 ms_total; //��ʼ����ĿǰΪֹ���ܺ�����
    u32 s_run;
    u32 s_pre;
} TASKTIME_Typedef;

//-----------����ִ��ʱ��-------------//
typedef struct
{
    TASKTIME_Typedef task_1;
    TASKTIME_Typedef task_2; //��������2������ʱ��
    TASKTIME_Typedef task_3;
    TASKTIME_Typedef task_4;
	
	TASKTIME_Typedef task_plus_1;
	TASKTIME_Typedef task_plus_2;
	TASKTIME_Typedef task_plus_3;
	TASKTIME_Typedef task_plus_4;
} TASKRUNTIME_Typedef;


//-----------  ����״̬  -------------//
typedef struct
{
    int32_t pre;
    int32_t now;
    int32_t set;
} INT32_T_STATE_Typedef;

typedef struct
{
    int16_t pre;
    int16_t now;
    int16_t set;
} INT16_T_STATE_Typedef;

typedef struct
{
    uint32_t pre;
    uint32_t now;
    uint32_t set;
} UINT32_T_STATE_Typedef;

typedef struct
{
    uint16_t pre;
    uint16_t now;
    uint16_t set;
} UINT16_T_STATE_Typedef;

typedef struct
{
    float pre;
    float now;
    float set;
} FLOAT_STATE_Typedef;

typedef struct
{
    double pre;
    double now;
    double set;
} DOUBLE_STATE_Typedef;

/************************************* ͨ��ȫ�ֱ������� *************************************/
extern SYSTIME_Typedef sys_time;

//#include "moto.h"
//extern MOTO_Typedef moto;

extern FLAG_Typedef flag;
extern TASKRUNTIME_Typedef taskrun_time;
extern SYS_TASK_STATE sys_task_state;



/************************************* ��ѧ���ʽ *************************************/
#define ABS(x) ( (x)>0?(x):-(x) )
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
//#define PI 3.14159

#endif




#ifndef __INCLUDE__H__
#define __INCLUDE__H__


#include "stm32f4xx.h"
#include "MyMath.h"
#include "misc.h"

#include <math.h>

#define OV7620


/************************************* 用户数据类型定义 *************************************/

//---------系统标志位------------//
typedef struct
{
    char task_change; //任务状态切换
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
	
	//任务时间记录标志位
	char task1_time;
	char task2_time;
	char task3_time;
	char task4_time;
	
	char task_plus1_time;
	char task_plus2_time;
	char task_plus3_time;
	char task_plus4_time;
	
	
} FLAG_Typedef;


//---------系统任务枚举型变量------------//
typedef enum
{
    SYS_Wait,
}SYS_TASK_STATE;

//---------系统时间位------------//
typedef struct
{
    u32 us;
    u16 ms;
    u32 s;
    u32 count;
} SYSTIME_Typedef;

typedef struct
{
    u32 ms_run;   //任务运行时间
    u32 ms_pre;	  //开始任务是记录的系统时间
    u32 ms_total; //开始任务到目前为止的总毫秒数
    u32 s_run;
    u32 s_pre;
} TASKTIME_Typedef;

//-----------任务执行时间-------------//
typedef struct
{
    TASKTIME_Typedef task_1;
    TASKTIME_Typedef task_2; //基础任务2的运行时间
    TASKTIME_Typedef task_3;
    TASKTIME_Typedef task_4;
	
	TASKTIME_Typedef task_plus_1;
	TASKTIME_Typedef task_plus_2;
	TASKTIME_Typedef task_plus_3;
	TASKTIME_Typedef task_plus_4;
} TASKRUNTIME_Typedef;


//-----------  变量状态  -------------//
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

/************************************* 通用全局变量声明 *************************************/
extern SYSTIME_Typedef sys_time;

//#include "moto.h"
//extern MOTO_Typedef moto;

extern FLAG_Typedef flag;
extern TASKRUNTIME_Typedef taskrun_time;
extern SYS_TASK_STATE sys_task_state;



/************************************* 数学表达式 *************************************/
#define ABS(x) ( (x)>0?(x):-(x) )
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
//#define PI 3.14159

#endif



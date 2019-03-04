#ifndef __JY901_H__
#define __JY901_H__


#define PI          3.14159f
#define R2A 		57.29578f //����ת�Ƕ�
#define A2R			0.0174532f

typedef struct
{
	float x;
	float y;
	float z;
}XYZ_Typedef;

typedef struct
{
	float raw;
	float offset;
	float deal;
}ANGLE_Typedef;


typedef struct
{
	ANGLE_Typedef pitch;
	ANGLE_Typedef roll;
	ANGLE_Typedef yaw;
}ANGLE_DEAL_Typedef;



float mixture_angle(float pitch,float roll);
void JY901_offset(int times);
void usart3_init(unsigned int bound);


extern XYZ_Typedef acc;//���ٶ�
extern XYZ_Typedef gryo;//���ٶ�
extern XYZ_Typedef magnetic;//�ų�
extern ANGLE_DEAL_Typedef angle;

extern float  pitch;
extern float roll;
extern float yaw;
#endif


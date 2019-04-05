/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-04-05 14:14:20
 * @LastEditTime: 2019-04-05 14:28:08
 */
#include "sys.h"
#include "delay.h"
#include "OV7620_REG.h"
#include "OV7620.h"
#include "camera_deal.h"
#include "math.h"

/*   BY QH-170722
ʹ��ע�⣺
1���ڹǼ���ȡǰ��Ҫ��ԭʼͼ����ж�ֵ��
2���Ǽ���ȡ���biImage��Ϊ��ȡ������飬���Ƕ�ֵ������
	 
ʹ��˵����
1��Image2Bimap_bit(threshold,Image)��  //ԭʼͼ���ֵ��
2��Image_Bi2Bone( &biImage[0][0] )��//�Ǽ���ȡ
*/

u8 biImage[CAMERA_H][CAMERA_W] = {0};
THRESHOLD threshold;
int fuck;

int16_t Threshold[10] = {0};
int16_t Threshold_count = 0;

/************************************   ������ֵ��   ***************************************************/
//���� ��limit ����֡�뱾֮֡��С�ڴ�ֵʱ����Ϊ��ֵ����
// Ŀ��  ����ú�����ֵ
//ע  ����255 ����0
void Threshold_Camera(u8 limit)
{
	uint32_t i = 0, j = 0;
	uint8_t d = 0;

	for (i = 0; i < CAMERA_H; i++)
	{
		for (j = 0; j < CAMERA_W; j++) //CAMERA_W>CAMERA_H
		{
			threshold.real += Image[i][j];
		}
	}

	threshold.real = threshold.real / (CAMERA_H * CAMERA_W);
	threshold.new_ = 20;
	threshold.last_ = 0;

	while (fabs(threshold.new_ - threshold.last_) > limit) //||fabs(threshold.new_-threshold.last_)==0
	{
		threshold.foreground = 0;  //ǰ����ֵ��ʼ��
		threshold.count_front = 0; //ǰ����ֵ������ʼ��
		threshold.background = 0;  //������ֵ��ʼ��
		threshold.count_back = 0;  //������ֵ������ʼ��

		for (i = 0; i < CAMERA_H; i++)
		{
			for (j = 0; j < CAMERA_W; j++) //CAMERA_W>CAMERA_H
			{
				d = Image[i][j];

				if (d > threshold.real) //ǰ������ɫ��
				{
					threshold.foreground = threshold.foreground + d; //253���
					threshold.count_front++;
				}
				else //�󾰣���ɫ��
				{
					threshold.background = threshold.background + d;
					threshold.count_back++;
				}
			}
		}
		threshold.foreground = threshold.foreground / threshold.count_front; //ǰ����ֵ
		threshold.background = threshold.background / threshold.count_back;  //������ֵ
		threshold.last_ = threshold.new_;
		threshold.new_ = (threshold.foreground + threshold.background) / 2;
		threshold.real = threshold.new_;
	}

	//flag.camera_threshold = 1;
}

/************************************   ���ɷ���ֵ   ***************************************************/
uint8_t CalculateThreshold(uint8_t *src, uint16_t widthBottom, uint16_t heightBottom, uint16_t widthTop, uint16_t heightTop)
{
	uint8_t *ImgBaseAddr = src;   //��ȡͼ�񴢴�ĵ�ַ
	uint8_t *pixel = ImgBaseAddr; //����ָ��
	uint16_t PixelNum[256] = {0}; //ֱ��ͼ����ͳ�Ʊ�;��������
	int16_t Thr = 100;			  //��ֵ��ʼ��
	int16_t line, column;

	int32_t p;
	int32_t total;							 //ֱ��ͼƽ����ʱ�����Ҽ������ۺ�
	int32_t TotalWeight = 0, ForeWeight = 0; //������
	int32_t TotalPixels = 0;				 //����ͳ�Ƶĵ�����

	int32_t BackPixNum = 0, ForePixNum = 0; //ǰ��ͼ��ͱ���ͼ��ĵ���
	double BackAvgGry, ForeAvgGry;			//ƽ���Ҷ�ֵ
	double fmax = -1;
	double sb;

	for (line = heightBottom; line < heightTop; line++) //ͼ��ֱ��ͼ
	{
		pixel = ImgBaseAddr + line * CAMERA_W; //��������ָ�룬��ֵΪÿ��ͼ�����߽�ĵ�ַ
		for (column = widthBottom; column < widthTop; column += 4)
		{
			PixelNum[*(pixel + column)]++;
		}
		line += 3;
	}

	for (line = 0; line < 256; line++)
	{
		total = 0;
		for (column = -2; column < 3; column++) //���ҹ�5����ƽ��
		{
			p = line + column;
			if (p < 0)
				p = 0;
			else if (p > 255)
				p = 255;
			total += PixelNum[p];
		}
		PixelNum[line] = (uint16_t)((double)total / 5 + 0.5);
	}

	for (line = 0; line < 256; line++)
	{
		TotalWeight += line * PixelNum[line]; //x*f(x)�����أ�Ҳ����ÿ���Ҷȵ�ֵ�������������һ����Ϊ���ʣ���TotalWeightΪ���ܺ�
		TotalPixels += PixelNum[line];		  //totalΪͼ���ܵĵ�������һ��������ۻ�����
	}

	for (line = 0; line < 256; line++)
	{
		ForePixNum += PixelNum[line]; //ForePixNumΪ�ڵ�ǰ��ֵ��ǰ��ͼ��ĵ���
		if (ForePixNum == 0)
			continue;						   //û�зֳ�ǰ����
		BackPixNum = TotalPixels - ForePixNum; //BackPixNumΪ����ͼ��ĵ���
		if (BackPixNum == 0)
			break;														//BackPixNumΪ0��ʾȫ�����Ǻ�ͼ����ForePixNum=0������ƣ�֮��ı���������ʹǰ���������ӣ����Դ�ʱ�����˳�ѭ��
		ForeWeight += line * PixelNum[line];							//ǰ��ͼ��������
		ForeAvgGry = ((double)(ForeWeight)) / ForePixNum;				//ǰ��ͼ��ƽ���Ҷ�
		BackAvgGry = ((double)(TotalWeight - ForeWeight)) / BackPixNum; //����ͼ��ƽ���Ҷ�
		sb = ForePixNum * BackPixNum * (ForeAvgGry - BackAvgGry) * (ForeAvgGry - BackAvgGry);
		if (sb > fmax)
		{
			fmax = sb;
			Thr = line;
		}
	}
	return (uint8_t)Thr;
}

/******************   ͼ���ֵ��   *********************/

u8 Image2Bimap_bit(u32 threshold, u8 image, u8 blackboard)
{
	//�������Ǻ�ɫ
	if (!blackboard) //��blackboard=0������Ϊ�ף�����Ϊ��
	{
		if (image >= threshold) //��
		{
			return 0xff;
		}
		else
		{
			return 0x00;
		}
	}
	else
	{
		if (image <= threshold)
		{
			return 0xff;
		}
		else
		{
			return 0x00;
		}
	}
}

void Image2Bimap(u8 blackboard)
{
	int i, j;

	//��ֵ��
	for (i = 0; i < CAMERA_H; i++)
	{
		for (j = 0; j < CAMERA_W; j++)
		{
			biImage[i][j] = Image2Bimap_bit(threshold.real, Image[i][j], blackboard);
		}
	}
}

/*************************  ����ͼ��ϸ��  ********************************/

//���룺��ֵ��ͼ�����ص��ַ
//���أ��Ƿ�ɾ��
u8 earse(u8 bi_image[CAMERA_H][CAMERA_W], u8 i, u8 j)
{
	u8 rank = 0;

	//�ǰ�ɫ��ͼ���
	if (bi_image[i - 1][j - 1] == 0xff)
		rank = rank + (1 << 0);
	if (bi_image[i - 1][j] == 0xff)
		rank = rank + (1 << 1);
	if (bi_image[i - 1][j + 1] == 0xff)
		rank = rank + (1 << 2);

	if (bi_image[i][j - 1] == 0xff)
		rank = rank + (1 << 3);
	if (bi_image[i][j + 1] == 0xff)
		rank = rank + (1 << 4);

	if (bi_image[i + 1][j - 1] == 0xff)
		rank = rank + (1 << 5);
	if (bi_image[i + 1][j] == 0xff)
		rank = rank + (1 << 6);
	if (bi_image[i + 1][j + 1] == 0xff)
		rank = rank + (1 << 7);

	return erasetable[rank];
}

void Image_Bi2Bone(u8 bi_image[CAMERA_H][CAMERA_W])
{
	u8 interation = 1;
	u8 i, j;

	while (interation)
	{
		interation = 0;

		//ˮƽϸ�����̶���
		for (i = 0; i < CAMERA_H; i++)
		{
			//������һ�������һ��
			for (j = 1; j < CAMERA_W - 1; j++)
			{
				//���Ҷ��кڵ㣬�õ㲻������
				if (bi_image[i][j - 1] == 0 && bi_image[i][j] == 0 && bi_image[i][j + 1] == 0)
				{
					continue;
				}
				//�Ǻڵ�
				else if (bi_image[i][j] == 0)
				{
					//����жϸõ�Ҫɾ��
					if (earse(bi_image, i, j))
					{
						bi_image[i][j] = 0xff; //ɾ���ڵ�
						j++;				   //�������ھ�
						interation = 1;		   //��һ����Ȼ����
					}
				}
			} /* ÿһ�� */

		} /* ˮƽϸ�� */

		//��ֱϸ�����̶���
		for (i = 0; i < CAMERA_W; i++)
		{
			//������һ�������һ��
			for (j = 1; j < CAMERA_H - 1; j++)
			{
				//���¶��кڵ㣬�õ㲻������
				if (bi_image[j - 1][i] == 0 && bi_image[j][i] == 0 && bi_image[j + 1][i] == 0)
				{
					continue;
				}
				//�Ǻڵ�
				else if (bi_image[j][i] == 0)
				{
					//����жϸõ�Ҫɾ��
					if (earse(bi_image, j, i))
					{
						bi_image[j][i] = 0xff; //ɾ���ڵ�
						j++;				   //�������ھ�
						interation = 1;		   //��һ����Ȼ����
					}
				}
			} /* ÿһ�� */

		} /* ��ֱϸ�� */

	} /* while(interation) */
}

/********* �����������������꣨ƽ��ֵ���� *********/

//core_x��x��������洢λ��
//core_y��y��������洢λ��
//h_start��w_start:������Χ����ʼ����
//height��weith��������Χ��С
//blackboard���ڵ���1
//thr:��ֵ
void core_find_average(u8 *core_x, u8 *core_y, u8 h_start, u8 w_start, u8 height, u8 weith, u8 blackboard, u8 thr)
{
	int i, j;
	u32 h_total = 0, w_total = 0;
	u32 count = 0;

	int imax = h_start + height;
	int jmax = w_start + weith;

	if (imax > CAMERA_H)
	{
		imax = CAMERA_H;
	}

	if (jmax > CAMERA_W)
	{
		imax = CAMERA_W;
	}

	/* ����ÿһ�� */
	for (i = h_start; i < imax; i++)
	{
		/* ����ÿһ����ĵ�j��Ԫ�� */
		for (j = w_start; j < jmax; j++)
		{
			//�װ壬�Һ���
			if (!blackboard)
			{
				//����ֵС��˵���ĺڵ�
				if (Image[i][j] < thr)
				{
					h_total += i;
					w_total += j;
					count++;
				}
			} //�װ�

			//�ڰ壬�Ұ���
			else
			{
				//����ֵ�󣬰׵�
				if (Image[i][j] > thr)
				{
					h_total += i;
					w_total += j;
					count++;
				}
			} //�ڰ�

		} /* ����ÿһ�����Ԫ�� */

	} /* ����ÿһ�� */

	*core_y = (int)((h_total / (double)count) + 0.5);
	*core_x = (int)((w_total / (double)count) + 0.5);
}

/********* �����������������꣨�����С�߽�ƽ��ֵ���� *********/

//core_x��x��������洢λ��
//core_y��y��������洢λ��
//h_start��w_start:������Χ����ʼ����
//height��weith��������Χ��С
//blackboard���ڵ���1
//thr:��ֵ
void core_find_border(u8 *core_x, u8 *core_y, u8 h_start, u8 w_start, u8 height, u8 weith, u8 blackboard, u8 thr)
{
	int hmin = h_start + height, hmax = 0;
	int wmin = w_start + weith, wmax = 0;
	int i, j;

	int imax = h_start + height;
	int jmax = w_start + weith;

	if (imax > CAMERA_H)
	{
		imax = CAMERA_H;
	}

	if (jmax > CAMERA_W)
	{
		imax = CAMERA_W;
	}

	/* ����ÿһ�� */
	for (i = h_start; i < imax; i++)
	{
		/* ����ÿһ����ĵ�j��Ԫ�� */
		for (j = w_start; j < jmax; j++)
		{
			//�װ壬�Һ���
			if (!blackboard)
			{
				//����ֵС��˵���ĺڵ�
				if (Image[i][j] < thr)
				{
					if (wmax < j)
						wmax = j;
					if (wmin > j)
						wmin = j;
					if (hmax < i)
						hmax = i;
					if (hmin > i)
						hmin = i;
				}
			} //�װ�

			//�ڰ壬�Ұ���
			else
			{
				//����ֵ�󣬰׵�
				if (Image[i][j] > thr)
				{
					if (wmax < j)
						wmax = j;
					if (wmin > j)
						wmin = j;
					if (hmax < i)
						hmax = i;
					if (hmin > i)
						hmin = i;
				}
			} //�ڰ�
		}	 /* ÿһ��Ԫ�� */

	} /* ÿһ�� */

	*core_y = (int)(((hmax + hmin) / (double)2) + 0.5);
	*core_x = (int)(((wmax + wmin) / (double)2) + 0.5);
}

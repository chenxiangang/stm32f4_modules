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
使用注意：
1、在骨架提取前先要队原始图像进行二值化
2、骨架提取后的biImage即为提取后的数组，不是二值化数组
	 
使用说明：
1、Image2Bimap_bit(threshold,Image)；  //原始图像二值化
2、Image_Bi2Bone( &biImage[0][0] )；//骨架提取
*/

u8 biImage[CAMERA_H][CAMERA_W] = {0};
THRESHOLD threshold;
int fuck;

int16_t Threshold[10] = {0};
int16_t Threshold_count = 0;

/************************************   迭代阈值法   ***************************************************/
//参数 ：limit 当上帧与本帧之差小于此值时，认为阈值合理
// 目的  ：求得合适阈值
//注  纯白255 纯黑0
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
		threshold.foreground = 0;  //前景阈值初始化
		threshold.count_front = 0; //前景阈值计数初始化
		threshold.background = 0;  //背景阈值初始化
		threshold.count_back = 0;  //背景阈值计数初始化

		for (i = 0; i < CAMERA_H; i++)
		{
			for (j = 0; j < CAMERA_W; j++) //CAMERA_W>CAMERA_H
			{
				d = Image[i][j];

				if (d > threshold.real) //前景，白色的
				{
					threshold.foreground = threshold.foreground + d; //253最大
					threshold.count_front++;
				}
				else //后景，黑色的
				{
					threshold.background = threshold.background + d;
					threshold.count_back++;
				}
			}
		}
		threshold.foreground = threshold.foreground / threshold.count_front; //前景阈值
		threshold.background = threshold.background / threshold.count_back;  //背景阈值
		threshold.last_ = threshold.new_;
		threshold.new_ = (threshold.foreground + threshold.background) / 2;
		threshold.real = threshold.new_;
	}

	//flag.camera_threshold = 1;
}

/************************************   大律法阈值   ***************************************************/
uint8_t CalculateThreshold(uint8_t *src, uint16_t widthBottom, uint16_t heightBottom, uint16_t widthTop, uint16_t heightTop)
{
	uint8_t *ImgBaseAddr = src;   //获取图像储存的地址
	uint8_t *pixel = ImgBaseAddr; //像素指针
	uint16_t PixelNum[256] = {0}; //直方图点数统计表;必须清零
	int16_t Thr = 100;			  //阈值初始化
	int16_t line, column;

	int32_t p;
	int32_t total;							 //直方图平滑化时的左右几个的综合
	int32_t TotalWeight = 0, ForeWeight = 0; //质量矩
	int32_t TotalPixels = 0;				 //进行统计的点数和

	int32_t BackPixNum = 0, ForePixNum = 0; //前景图像和背景图像的点数
	double BackAvgGry, ForeAvgGry;			//平均灰度值
	double fmax = -1;
	double sb;

	for (line = heightBottom; line < heightTop; line++) //图像直方图
	{
		pixel = ImgBaseAddr + line * CAMERA_W; //更新像素指针，该值为每幅图像的左边界的地址
		for (column = widthBottom; column < widthTop; column += 4)
		{
			PixelNum[*(pixel + column)]++;
		}
		line += 3;
	}

	for (line = 0; line < 256; line++)
	{
		total = 0;
		for (column = -2; column < 3; column++) //左右共5个做平均
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
		TotalWeight += line * PixelNum[line]; //x*f(x)质量矩，也就是每个灰度的值乘以其点数（归一化后为概率），TotalWeight为其总和
		TotalPixels += PixelNum[line];		  //total为图象总的点数，归一化后就是累积概率
	}

	for (line = 0; line < 256; line++)
	{
		ForePixNum += PixelNum[line]; //ForePixNum为在当前阈值遍前景图象的点数
		if (ForePixNum == 0)
			continue;						   //没有分出前景后景
		BackPixNum = TotalPixels - ForePixNum; //BackPixNum为背景图象的点数
		if (BackPixNum == 0)
			break;														//BackPixNum为0表示全部都是后景图象，与ForePixNum=0情况类似，之后的遍历不可能使前景点数增加，所以此时可以退出循环
		ForeWeight += line * PixelNum[line];							//前景图像质量矩
		ForeAvgGry = ((double)(ForeWeight)) / ForePixNum;				//前景图像平均灰度
		BackAvgGry = ((double)(TotalWeight - ForeWeight)) / BackPixNum; //背景图像平均灰度
		sb = ForePixNum * BackPixNum * (ForeAvgGry - BackAvgGry) * (ForeAvgGry - BackAvgGry);
		if (sb > fmax)
		{
			fmax = sb;
			Thr = line;
		}
	}
	return (uint8_t)Thr;
}

/******************   图像二值化   *********************/

u8 Image2Bimap_bit(u32 threshold, u8 image, u8 blackboard)
{
	//背景不是黑色
	if (!blackboard) //若blackboard=0，亮的为白，暗的为黑
	{
		if (image >= threshold) //白
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

	//二值化
	for (i = 0; i < CAMERA_H; i++)
	{
		for (j = 0; j < CAMERA_W; j++)
		{
			biImage[i][j] = Image2Bimap_bit(threshold.real, Image[i][j], blackboard);
		}
	}
}

/*************************  迭代图像细化  ********************************/

//输入：二值化图像像素点地址
//返回：是否删除
u8 earse(u8 bi_image[CAMERA_H][CAMERA_W], u8 i, u8 j)
{
	u8 rank = 0;

	//是白色点就计数
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

		//水平细化，固定行
		for (i = 0; i < CAMERA_H; i++)
		{
			//跳过第一个和最后一个
			for (j = 1; j < CAMERA_W - 1; j++)
			{
				//左右都有黑点，该点不做处理
				if (bi_image[i][j - 1] == 0 && bi_image[i][j] == 0 && bi_image[i][j + 1] == 0)
				{
					continue;
				}
				//是黑点
				else if (bi_image[i][j] == 0)
				{
					//如果判断该点要删除
					if (earse(bi_image, i, j))
					{
						bi_image[i][j] = 0xff; //删除黑点
						j++;				   //跳过右邻居
						interation = 1;		   //下一次依然迭代
					}
				}
			} /* 每一列 */

		} /* 水平细化 */

		//垂直细化，固定行
		for (i = 0; i < CAMERA_W; i++)
		{
			//跳过第一个和最后一个
			for (j = 1; j < CAMERA_H - 1; j++)
			{
				//上下都有黑点，该点不做处理
				if (bi_image[j - 1][i] == 0 && bi_image[j][i] == 0 && bi_image[j + 1][i] == 0)
				{
					continue;
				}
				//是黑点
				else if (bi_image[j][i] == 0)
				{
					//如果判断该点要删除
					if (earse(bi_image, j, i))
					{
						bi_image[j][i] = 0xff; //删除黑点
						j++;				   //跳过下邻居
						interation = 1;		   //下一次依然迭代
					}
				}
			} /* 每一列 */

		} /* 垂直细化 */

	} /* while(interation) */
}

/********* 矩形区域内中心坐标（平均值法） *********/

//core_x：x像素坐标存储位置
//core_y：y像素坐标存储位置
//h_start，w_start:搜索范围的起始坐标
//height，weith：搜索范围大小
//blackboard：黑底置1
//thr:阈值
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

	/* 对于每一行 */
	for (i = h_start; i < imax; i++)
	{
		/* 对于每一行里的第j个元素 */
		for (j = w_start; j < jmax; j++)
		{
			//白板，找黑球
			if (!blackboard)
			{
				//比阈值小，说明的黑的
				if (Image[i][j] < thr)
				{
					h_total += i;
					w_total += j;
					count++;
				}
			} //白板

			//黑板，找白球
			else
			{
				//比阈值大，白的
				if (Image[i][j] > thr)
				{
					h_total += i;
					w_total += j;
					count++;
				}
			} //黑板

		} /* 对于每一行里的元素 */

	} /* 对于每一行 */

	*core_y = (int)((h_total / (double)count) + 0.5);
	*core_x = (int)((w_total / (double)count) + 0.5);
}

/********* 矩形区域内中心坐标（最大最小边界平均值法） *********/

//core_x：x像素坐标存储位置
//core_y：y像素坐标存储位置
//h_start，w_start:搜索范围的起始坐标
//height，weith：搜索范围大小
//blackboard：黑底置1
//thr:阈值
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

	/* 对于每一行 */
	for (i = h_start; i < imax; i++)
	{
		/* 对于每一行里的第j个元素 */
		for (j = w_start; j < jmax; j++)
		{
			//白板，找黑球
			if (!blackboard)
			{
				//比阈值小，说明的黑的
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
			} //白板

			//黑板，找白球
			else
			{
				//比阈值大，白的
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
			} //黑板
		}	 /* 每一个元素 */

	} /* 每一行 */

	*core_y = (int)(((hmax + hmin) / (double)2) + 0.5);
	*core_x = (int)(((wmax + wmin) / (double)2) + 0.5);
}

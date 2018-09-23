#include "stdafx.h"
#include "Algo.h"
#include "utility.h"
#include <sstream>
#include <stdio.h>
#include <direct.h>
#include "FIFO.h"
#define OnePulseMM 0.05002538

void TrainBottom(vector<PAIR>& vArr, vector<PAIR>& outArr)
{
	typedef vector<PAIR>::iterator vecit;
	outArr.clear();
	vecit it = vArr.begin();
	while(it != vArr.end())
	{
		if(it->first<1.0)
		{
			outArr.push_back(*it);
		}
		it++;
	}
}

void Filter(vector<PAIR>& vArr, vector<PAIR_AXIS>& outArr)
{
	string str; stringstream sstr;
	time_t tt = time(NULL);
	
	int lsize = vArr.size();
	long HeadPCI = vArr.at(0).second;		// 车头
	long TailPCI = vArr.at(lsize-1).second;	// 车尾
	
	string path = "D:\\DataLog\\";
	path = path + GetDateString() + "\\";
	_mkdir(path.c_str());
	
	string name = GetTimeDateString() + "-01_AUTO.txt";
	string fullname = path + name;
	FILE* fp = fopen(fullname.c_str(),"wt+");
	sstr.clear();
	sstr.str("");
	sstr<<"HeadPCI"<<"\t"<<HeadPCI<<"\t"<<"TailPCI"<<"\t"<<TailPCI<<"\n";
	str = sstr.str();
	fputs(str.c_str(), fp);

	int i=0, j=0;
	try
	{
		//三点测量法
		double d1l = 0.0, d1r = 0.0, d1c = 0.0; // left-bound 
		//double d2l = 0.0, d2r = 0.0; // right-bound
		const double cut_line = 0.53;
		int head = 0, tail = 0;
		
		double max_v1 = 0.0; //落差最大值
		int max_idx1 = 0;	 //落差最大时索引
		double max_v2 = 0.0;
		int max_idx2 = 0;

		int iFindStep=25;//搜索步长
		const double DMaxGaoCha=0.1;
		bool bleft=false;//下降沿是否已找到
		double dKmax_v1=0;//最大斜率
		int iKmax_idx1 = 0;//最大斜率时索引
		int iKmax_idx1x=0;//最大斜率时前续索引
		double dKmax_v2=0;
		int iKmax_idx2 = 0;
		int iKmax_idx2x=0;
		int iMatch_1=0;	//下降沿匹配成功次数
		int iFirst=0;
		
		for(i = 11; i<lsize-iFindStep*2; ++i)
		{
			d1l = vArr.at(i).first;

			if(d1l < 0.2 || d1l > 1.0) // 小于或者大于上下界限的值不做计算
				continue;
			
			if(bleft==false)//搜索下降沿
			{
				j=0;
				max_v1=0;
				max_idx1=0;
				dKmax_v1=0;
				iKmax_idx1 = 0;
				double diff =0;
				double diff_abs =0;

				for (j=i+1;j<=i+iFindStep;j++)//计算最高点位置
				{
					if (d1l<vArr.at(j).first)
					{
						j=j-1;
						break;
					} 
					else
					{
						diff_abs = abs(d1l-vArr.at(j).first);
						if (diff_abs>max_v1)//找出最大落差
						{
							max_v1=diff_abs;
							max_idx1 = j;
						}
						
					}
				}
				if (max_v1<=DMaxGaoCha)
				{
					continue;
				}
				if(vArr.at(max_idx1).first < 0.2 || vArr.at(max_idx1).first>0.7)
				{
					continue;
				}
					
				if ((j-i)>5)//至少N个点起算斜率
				{
					for (int ik=j+1;ik<=j+iFindStep;ik++)//处理下降沿中途数据波动的问题
					{
						if ((vArr.at(j).first-vArr.at(ik).first)>0.05)
						{
							j=ik;
							break;
						}
					}
					
					while((vArr.at(j).first>vArr.at(j+1).first || 
						(vArr.at(j).first<vArr.at(j+1).first && vArr.at(j+1).first>vArr.at(j+2).first && 
						vArr.at(j).first>vArr.at(j+2).first)) && (j-i)<=100)//最大值可能还未到头
					{
						j++;
					}
					for (int k=i+1;k<=j;k++)
					{
						d1r = vArr.at(k).first;
						diff = d1r - d1l;
						diff_abs = abs(diff);
						if (diff_abs>max_v1)//找出最大落差
						{
							max_v1=diff_abs;
							max_idx1 = k;
						}
					}
					if (max_v1>DMaxGaoCha)
					{
						bleft=true;//搜索下降沿成功
						int itemp=i+(max_idx1-i+1)/2;
						
						int i1=i;
						int ix=i;
						
						double d1=vArr.at(i).first-max_v1/2 ;//求高度差的中值
						double dmin=vArr.at(i).first-d1;
						
						for (i1=i+1;i1<=max_idx1;i1++)	//求中值坐标
						{
							if (abs(vArr.at(i1).first-d1)<dmin)
							{
								dmin=abs(vArr.at(i1).first-d1);
								ix=i1;
							}
						}
						int x=0;
						int y=0;

						for (x=ix;x<=j;x++)
						{
							if (vArr.at(x).first<=vArr.at(x-1).first && vArr.at(x).first<=vArr.at(x+1).first && vArr.at(x).first<=vArr.at(x+2).first)
							{
								break;
							}
						}
						head=x;
						iFirst=i;
						
						i = head+120;
					}
				} 		
			}
			else//搜索上升沿
			{
				if (i>(head+180))//一旦目标范围内上升沿匹配不成功重新计算下降沿
				{
					bleft=false;
					head=0;
					i = iFirst+2;//搜索下降沿位置回溯
					continue;
				}
				j=0;
				double diff =0;
				double diff_abs =0;
				max_v2=0;
				max_idx2=0;
				dKmax_v2=0;
				iKmax_idx2 = 0;
				for (j=i+1;j<=i+iFindStep;j++)
				{
					if (d1l>vArr.at(j).first )
					{
						break;
					}
					else
					{
						diff_abs = abs(vArr.at(j).first - d1l);
						if (diff_abs>max_v2)//找出最大落差
						{
							max_v2=diff_abs;
							max_idx2 = j;
						}
					}
				}

				if (max_v2<=DMaxGaoCha)
				{
					continue;
				}
		
				if ((j-i)>5)//至少N个点起算斜率
				{
					while((vArr.at(j).first<vArr.at(j+1).first || 
						(vArr.at(j).first>vArr.at(j+1).first && vArr.at(j+1).first<vArr.at(j+2).first && 
						vArr.at(j).first<vArr.at(j+2).first)) && (j-i)<=100)//最大值可能还未到头
					{
						j++;
					}
					for (int k=i+1;k<j;k++)
					{
						d1r = vArr.at(k).first;
						diff = d1r - d1l;
						diff_abs = abs(diff);
						if (diff_abs>max_v2)//找出最大落差
						{
							max_v2=diff_abs;
							max_idx2 = k;
						}
					}
					if (max_v2>DMaxGaoCha)
					{
						bleft=false;//搜索上升沿成功
						
						for (int x=i;x<=j;x++)
						{
							for (int y=x+5;y<=j;y++)
							{
								diff = vArr.at(y).first - vArr.at(x).first;
								diff_abs = abs(diff);
								if((diff_abs*1000/(y-x)) > dKmax_v2)//找出最大斜率
								{
									dKmax_v2 = diff_abs*1000/(y-x);
									iKmax_idx2=y;
									iKmax_idx2x=x;
								}	
							}
						}

						int m=0;
						for (m=iKmax_idx2;m>=i;m--)
						{
							if (vArr.at(m).first<=vArr.at(m-2).first && vArr.at(m).first<=vArr.at(m-1).first && vArr.at(m).first<=vArr.at(m+1).first )
							{
								break;
							}
						}
						tail=m;

						iMatch_1++;
						
						//轴配对
						int dlen = tail- head + 1;//边界也算长度
						long axisCenter;
						
						axisCenter = vArr.at(head).second+(vArr.at(tail).second-vArr.at(head).second)/2 ;
						if(dlen < 130 || dlen> 180)
						{
							sstr.clear();
							sstr.str("");
							sstr<<head<<"\t"<<tail<<"\t"<<dlen<<"\t"<<axisCenter<<"\n";
							str = sstr.str();
							fputs(str.c_str(), fp);
						}
						else
						{
							sstr.clear();
							sstr.str("");
							sstr<<head<<"\t"<<tail<<"\t"<<dlen<<"\t"<<axisCenter<<"\t"<<"object"<<"\n";
							str = sstr.str();
							fputs(str.c_str(), fp);
							PAIR_AXIS tmpAxie;
							tmpAxie.first = dlen;//宽度
							tmpAxie.second = axisCenter;//轴中心脉冲数
							
							outArr.push_back(tmpAxie);
						}

						head = tail = 0;
						i = max_idx2+1;
					}
				} 
			}	
		}
	}
	catch(...)
	{
		// TODO : record crash position
		cerr<<"error index ocured ..."<<endl;
		cerr<<outArr.size()<<endl;
		cerr<<"i="<<i<<"\t j="<<j<<endl;
	}
	fflush(fp);
	fclose(fp);
	fp = NULL;
}

void ZhouDisFilter(vector<PAIR_AXIS>& inArr, vector<PAIR_AXIS>& outArr)
{
	const double DisOffset = 100.0; //标准值最大允许偏差
	// model-1 distance 
	const double model1_ds1 = 3495.0; 
	const double model1_ds2 = model1_ds1 + 2500.0;
	const double model1_ds3 = model1_ds2 + 14875.0;
	const double model1_ds4 = model1_ds3 + 2500.0;
	//	model-2 distance 
	const double model2_ds1 = 5000;//4980.0;
	const double model2_ds2 =  2500.0;
	const double model2_ds3 = 14875.0;
	const double model2_ds4 = 2500.0;

	long lds, rds, diff;
	double dis;//
	bool isdis1Ok = false, isdis2Ok = false, isdis3Ok = false, isdis4Ok = false;

	string path = "D:\\DataLog\\";
	path = path + GetDateString() + "\\";
	_mkdir(path.c_str());
	
	string name = GetTimeDateString() + "-02_AUTO.txt";
	string fullname = path + name;
	FILE* fp = fopen(fullname.c_str(),"wt+");
	stringstream sstr;
	string str;

	bool model1ok = false;//第一根轴是否已找到
	double lb =0.0, rb =0.0;
	lds = inArr.at(0).second;
	int iCarOrder=0;
	int iZhouOrder=0;
	for(size_t i = 0; i< inArr.size(); ++i)
	{
		int len = inArr.at(i).first;

		rds = inArr.at(i).second;
		diff = rds - lds;
		dis = diff * 0.05002538;
		
		if(model1ok == false)
		{	
			if (isdis1Ok==false)
			{
				lb = model1_ds1 - DisOffset;
				rb = model1_ds1 + DisOffset;
				if(dis > lb && dis < rb)		//3498
				{
					double absdis = rds* 0.05002538;
					sstr.clear();
					sstr.str("");
					iCarOrder++;
					
					sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(i).first<<"\t"<<inArr.at(i).second<<"\t"<<"obj"<<"\t"<<dis<<"\t"<<absdis<<"\n";
					str = sstr.str();
					fputs(str.c_str(), fp);

					outArr.push_back(inArr.at(i));
					
					isdis1Ok = true;

					continue;
				}
			} 
			else if(isdis1Ok==true && isdis2Ok==false)
			{
				lb = model1_ds2 - DisOffset;
				rb = model1_ds2 + DisOffset;
				if(dis > lb && dis < rb)	// 2500
				{
					double absdis = rds* 0.05002538;
					sstr.clear();
					sstr.str("");
				
					sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(i).first<<"\t"<<inArr.at(i).second<<"\t"<<"obj"<<"\t"<<dis<<"\t"<<absdis<<"\n";
					str = sstr.str();
					fputs(str.c_str(), fp);

					outArr.push_back(inArr.at(i));
					isdis2Ok = true;

					continue;
				}
			}
			else if(isdis2Ok==true && isdis3Ok==false)
			{
				lb = model1_ds3 - DisOffset;
				rb = model1_ds3 + DisOffset;
				if(dis > lb && dis < rb)	// 14875
				{
					double absdis = rds* 0.05002538;
					sstr.clear();
					sstr.str("");
					
					sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(i).first<<"\t"<<inArr.at(i).second<<"\t"<<"obj"<<"\t"<<dis<<"\t"<<absdis<<"\n";
					str = sstr.str();
					fputs(str.c_str(), fp);

					outArr.push_back(inArr.at(i));
					isdis3Ok=true;

					continue;
				}
			}
			else if(isdis3Ok==true && isdis4Ok==false)
			{
				lb = model1_ds4 - DisOffset;
				rb = model1_ds4 + DisOffset;
				if(dis > lb && dis < rb)	//2500
				{
					isdis1Ok = false;
					isdis2Ok = false;
					isdis3Ok = false;
					model1ok = true;
					lds = rds;
					
					double absdis = rds* 0.05002538;
					sstr.clear();
					sstr.str("");
					
					sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(i).first<<"\t"<<inArr.at(i).second<<"\t"<<"obj"<<"\t"<<dis<<"\t"<<absdis<<"\n";
					iZhouOrder=0;
					str = sstr.str();
					fputs(str.c_str(), fp);

					outArr.push_back(inArr.at(i));
				}
			}
		}
		else
		{
			//	Another model for calculate
			int iZhou1=0;
			int iZhou2=0;
			int iZhou3=0;
			int iZhou4=0;
			
			for (size_t j1=i;j1<inArr.size();j1++)
			{
				double dZhouDis1=(inArr.at(j1).second - lds)*0.05002538;
				if (iZhou1==0)
				{
					iZhou1=j1;
				}
				iZhou2=0;
				for (size_t j2=j1+1;j2<inArr.size();j2++)//匹配第2个轴距
				{
					double dZhouDis2=(inArr.at(j2).second - inArr.at(iZhou1).second)*0.05002538;
					
					lb = model2_ds2 - DisOffset;
					rb = model2_ds2 + DisOffset;
					if(dZhouDis2 > lb && dZhouDis2 < rb)	//2500
					{
						iZhou2=j2;
						iZhou3=0;
						for (size_t j3=j2+1;j3<inArr.size();j3++)//匹配第3个轴距
						{
							double dZhouDis3=(inArr.at(j3).second - inArr.at(iZhou2).second)*0.05002538;
							lb = model2_ds3 - DisOffset;
							rb = model2_ds3 + DisOffset;
							if(dZhouDis3 > lb && dZhouDis3 < rb)	//2500
							{
								iZhou3=j3;
								iZhou4=0;
								for (size_t j4=j3+1;j4<inArr.size();j4++)//匹配第4个轴距
								{
									double dZhouDis4=(inArr.at(j4).second - inArr.at(iZhou3).second)*0.05002538;
									lb = model2_ds4 - DisOffset;
									rb = model2_ds4 + DisOffset;
									if(dZhouDis4 > lb && dZhouDis4 < rb)	//2500
									{
										iZhou4=j4;
										rds = inArr.at(iZhou4).second;
										lds = rds;
										iCarOrder++;
										
										double absdis = rds* 0.05002538;
										sstr.clear();
										sstr.str("");
										
										sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(iZhou1).first<<"\t"<<inArr.at(iZhou1).second<<"\t"<<"obj"<<"\t"<<dZhouDis1<<"\t"<<inArr.at(iZhou1).second* 0.05002538<<"\n";
										sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(iZhou2).first<<"\t"<<inArr.at(iZhou2).second<<"\t"<<"obj"<<"\t"<<dZhouDis2<<"\t"<<inArr.at(iZhou2).second* 0.05002538<<"\n";
										sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(iZhou3).first<<"\t"<<inArr.at(iZhou3).second<<"\t"<<"obj"<<"\t"<<dZhouDis3<<"\t"<<inArr.at(iZhou3).second* 0.05002538<<"\n";
										sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(iZhou4).first<<"\t"<<inArr.at(iZhou4).second<<"\t"<<"obj"<<"\t"<<dZhouDis4<<"\t"<<inArr.at(iZhou4).second* 0.05002538<<"\n";

										iZhouOrder=0;
										str = sstr.str();
										fputs(str.c_str(), fp);

										outArr.push_back(inArr.at(iZhou1));
										outArr.push_back(inArr.at(iZhou2));
										outArr.push_back(inArr.at(iZhou3));
										outArr.push_back(inArr.at(iZhou4));
										iZhou1=0;
										iZhou2=0;
										iZhou3=0;
										iZhou4=0;
										i=j4;
										j1=j4;
										break;
									}
								}
								if (iZhou4==0)
								{
									iZhou3=0;
								}
								break;
							}
						}
						if (iZhou3==0)
						{
							iZhou2=0;
						}
						break;
					}
				}
				if (iZhou2==0)
				{
					iZhou1=0;
				}
			}
		}
	}

	fflush(fp);
	fclose(fp);
	fp = NULL;
}

///ADD 2017-04-25
void Filter(vector<PAIR>& vArr, vector<AIXS_DETAIL>& outArr)
{
	string str; stringstream sstr;

	int lsize = vArr.size();
	long HeadPCI = vArr.at(0).second;		// 车头
	long TailPCI = vArr.at(lsize-1).second;	// 车尾
	
	string path = "D:\\DataLog\\";

	path = path + GetDateString() + "\\";
	_mkdir(path.c_str());
	
	string name = GetTimeDateString() + "-01_AUTO.txt";
	string fullname = path + name;
	FILE* fp = fopen(fullname.c_str(),"wt+");
	sstr.clear();
	sstr.str("");
	sstr<<"HeadPCI"<<"\t"<<HeadPCI<<"\t"<<"TailPCI"<<"\t"<<TailPCI<<"\n";
	str = sstr.str();
	fputs(str.c_str(), fp);

	int i=0, j=0;
	try
	{
		//三点测量法
		double d1l = 0.0, d1r = 0.0, d1c = 0.0; // left-bound 
		//double d2l = 0.0, d2r = 0.0; // right-bound
		const double cut_line = 0.53;
		int head = 0, tail = 0;
		
		double max_v1 = 0.0; //落差最大值
		int max_idx1 = 0;	 //落差最大时索引
		double max_v2 = 0.0;
		int max_idx2 = 0;

		int iFindStep=25;//搜索步长
		const double DMaxGaoCha=0.1;
		bool bleft=false;//下降沿是否已找到
		double dKmax_v1=0;//最大斜率
		int iKmax_idx1 = 0;//最大斜率时索引
		int iKmax_idx1x=0;//最大斜率时前续索引
		double dKmax_v2=0;
		int iKmax_idx2 = 0;
		int iKmax_idx2x=0;
		int iMatch_1=0;	//下降沿匹配成功次数
		int iFirst=0;
		
		for(i = 11; i<lsize-iFindStep*2; ++i)
		{
			d1l = vArr.at(i).first;

			if(d1l < 0.2 || d1l > 1.0) // 小于或者大于上下界限的值不做计算
				continue;
			
			if(bleft==false)//搜索下降沿
			{
				j=0;
				max_v1=0;
				max_idx1=0;
				dKmax_v1=0;
				iKmax_idx1 = 0;
				double diff =0;
				double diff_abs =0;

				for (j=i+1;j<=i+iFindStep;j++)//计算最高点位置
				{
					if (d1l<vArr.at(j).first)
					{
						j=j-1;
						break;
					} 
					else
					{
						diff_abs = abs(d1l-vArr.at(j).first);
						if (diff_abs>max_v1)//找出最大落差
						{
							max_v1=diff_abs;
							max_idx1 = j;
						}
						
					}
				}
				if (max_v1<=DMaxGaoCha)
				{
					continue;
				}
				if(vArr.at(max_idx1).first < 0.2 || vArr.at(max_idx1).first>0.7)
				{
					continue;
				}
					
				if ((j-i)>5)//至少N个点起算斜率
				{
					for (int ik=j+1;ik<=j+iFindStep;ik++)//处理下降沿中途数据波动的问题
					{
						if ((vArr.at(j).first-vArr.at(ik).first)>0.05)
						{
							j=ik;
							break;
						}
					}
					
					while((vArr.at(j).first>vArr.at(j+1).first || 
						(vArr.at(j).first<vArr.at(j+1).first && vArr.at(j+1).first>vArr.at(j+2).first && 
						vArr.at(j).first>vArr.at(j+2).first)) && (j-i)<=100)//最大值可能还未到头
					{
						j++;
					}
					for (int k=i+1;k<=j;k++)
					{
						d1r = vArr.at(k).first;
						diff = d1r - d1l;
						diff_abs = abs(diff);
						if (diff_abs>max_v1)//找出最大落差
						{
							max_v1=diff_abs;
							max_idx1 = k;
						}
					}
					if (max_v1>DMaxGaoCha)
					{
						bleft=true;//搜索下降沿成功
						int itemp=i+(max_idx1-i+1)/2;
						
						int i1=i;
						int ix=i;
						
						double d1=vArr.at(i).first-max_v1/2 ;//求高度差的中值
						double dmin=vArr.at(i).first-d1;
						
						for (i1=i+1;i1<=max_idx1;i1++)	//求中值坐标
						{
							if (abs(vArr.at(i1).first-d1)<dmin)
							{
								dmin=abs(vArr.at(i1).first-d1);
								ix=i1;
							}
						}
						int x=0;
						int y=0;

						for (x=ix;x<=j;x++)
						{
							if (vArr.at(x).first<=vArr.at(x-1).first && vArr.at(x).first<=vArr.at(x+1).first && vArr.at(x).first<=vArr.at(x+2).first)
							{
								break;
							}
						}
						head=x;
						iFirst=i;
						
						i = head+120;
					}
				} 		
			}
			else//搜索上升沿
			{
				if (i>(head+225))//一旦目标范围内上升沿匹配不成功重新计算下降沿
				{
					bleft=false;
					head=0;
					i = iFirst+2;//搜索下降沿位置回溯
					continue;
				}
				j=0;
				double diff =0;
				double diff_abs =0;
				max_v2=0;
				max_idx2=0;
				dKmax_v2=0;
				iKmax_idx2 = 0;
				for (j=i+1;j<=i+iFindStep;j++)
				{
					if (d1l>vArr.at(j).first )
					{
						break;
					}
					else
					{
						diff_abs = abs(vArr.at(j).first - d1l);
						if (diff_abs>max_v2)//找出最大落差
						{
							max_v2=diff_abs;
							max_idx2 = j;
						}
					}
				}

				if (max_v2<=DMaxGaoCha)
				{
					continue;
				}
		
				if ((j-i)>5)//至少N个点起算斜率
				{
					while((vArr.at(j).first<vArr.at(j+1).first || 
						(vArr.at(j).first>vArr.at(j+1).first && vArr.at(j+1).first<vArr.at(j+2).first && 
						vArr.at(j).first<vArr.at(j+2).first)) && (j-i)<=100)//最大值可能还未到头
					{
						j++;
					}
					for (int k=i+1;k<j;k++)
					{
						d1r = vArr.at(k).first;
						diff = d1r - d1l;
						diff_abs = abs(diff);
						if (diff_abs>max_v2)//找出最大落差
						{
							max_v2=diff_abs;
							max_idx2 = k;
						}
					}
					if (max_v2>DMaxGaoCha)
					{
						bleft=false;//搜索上升沿成功
						
						for (int x=i;x<=j;x++)
						{
							for (int y=x+5;y<=j;y++)
							{
								diff = vArr.at(y).first - vArr.at(x).first;
								diff_abs = abs(diff);
								if((diff_abs*1000/(y-x)) > dKmax_v2)//找出最大斜率
								{
									dKmax_v2 = diff_abs*1000/(y-x);
									iKmax_idx2=y;
									iKmax_idx2x=x;
								}	
							}
						}

						int m=0;
						for (m=iKmax_idx2;m>=i;m--)
						{
							if (vArr.at(m).first<=vArr.at(m-2).first && vArr.at(m).first<=vArr.at(m-1).first && vArr.at(m).first<=vArr.at(m+1).first )
							{
								break;
							}
						}
						tail=m;

						iMatch_1++;
						
						//轴配对
						//int dlen = tail- head + 1;//边界也算长度
						int dlen = (vArr.at(tail).second-vArr.at(head).second)*0.05002538;
						long axisCenter;
						
						axisCenter = vArr.at(head).second+(vArr.at(tail).second-vArr.at(head).second)/2 ;
						if(dlen < 130 || dlen> 180)
						{
							sstr.clear();
							sstr.str("");
							sstr<<head<<"\t"<<tail<<"\t"<<dlen<<"\t"<<axisCenter<<"\n";
							str = sstr.str();
							fputs(str.c_str(), fp);
						}
						else
						{
							sstr.clear();
							sstr.str("");
							sstr<<head<<"\t"<<tail<<"\t"<<dlen<<"\t"<<axisCenter<<"\t"<<"object"<<"\n";
							str = sstr.str();
							fputs(str.c_str(), fp);
							
							AIXS_DETAIL tmpDetail;
							tmpDetail.left_idx = head;
							tmpDetail.right_idx = tail;
							tmpDetail.left_h = vArr.at(head).first;
							tmpDetail.right_h = vArr.at(tail).first;
							tmpDetail.axis_w = dlen;
							tmpDetail.axis_pci = axisCenter;

							outArr.push_back(tmpDetail);
						}

						head = tail = 0;
						i = max_idx2+1;
					}
				} 
			}	
		}
	}
	catch(...)
	{
		// TODO : record crash position
		cerr<<"error index ocured ..."<<endl;
		cerr<<outArr.size()<<endl;
		cerr<<"i="<<i<<"\t j="<<j<<endl;
	}
	fflush(fp);
	fclose(fp);
	fp = NULL;
}

void ZhouDisFilter(vector<AIXS_DETAIL>& inArr, vector<AIXS_DETAIL>& outArr)
{
	const double DisOffset = 100.0; //标准值最大允许偏差
	// model-1 distance 
	const double model1_ds1 = 3495.0; 
	const double model1_ds2 = model1_ds1 + 2500.0;
	const double model1_ds3 = model1_ds2 + 14875.0;
	const double model1_ds4 = model1_ds3 + 2500.0;
	//	model-2 distance 
	const double model2_ds1 = 5000;//4980.0;
	const double model2_ds2 =  2500.0;
	const double model2_ds3 = 14875.0;
	const double model2_ds4 = 2500.0;

	long lds, rds, diff;
	double dis;//
	bool isdis1Ok = false, isdis2Ok = false, isdis3Ok = false, isdis4Ok = false;

	string path = "D:\\DataLog\\";
	path = path + GetDateString()+ "\\";
	_mkdir(path.c_str());
	
	string name = GetTimeDateString() + "-02_AUTO.txt";
	string fullname = path + name;
	FILE* fp = fopen(fullname.c_str(),"wt+");
	stringstream sstr;
	string str;

	bool model1ok = false;//第一根轴是否已找到
	double lb =0.0, rb =0.0;
	lds = inArr.at(0).axis_pci;
	int iCarOrder=0;
	int iZhouOrder=0;
	for(size_t i = 0; i< inArr.size(); ++i)
	{
		int len = inArr.at(i).axis_w;

		rds = inArr.at(i).axis_pci;
		
		diff = rds - lds;
		dis = diff * 0.05002538;
		
		if(model1ok == false)
		{	
			if (isdis1Ok==false)
			{
				lb = model1_ds1 - DisOffset;
				rb = model1_ds1 + DisOffset;
				if(dis > lb && dis < rb)		//3498
				{
					double absdis = rds* 0.05002538;
					sstr.clear();
					sstr.str("");
					iCarOrder++;
					
					sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(i).axis_w<<"\t"<<inArr.at(i).axis_pci<<"\t"<<"obj"<<"\t"<<dis<<"\t"<<absdis<<"\t"<<inArr.at(i).left_idx<<"\t"<<inArr.at(i).right_idx<<"\t"<<inArr.at(i).left_h<<"\t"<<inArr.at(i).right_h<<"\n";
					str = sstr.str();
					fputs(str.c_str(), fp);

					outArr.push_back(inArr.at(i));
					
					isdis1Ok = true;

					continue;
				}
			} 
			else if(isdis1Ok==true && isdis2Ok==false)
			{
				lb = model1_ds2 - DisOffset;
				rb = model1_ds2 + DisOffset;
				if(dis > lb && dis < rb)	// 2500
				{
					double absdis = rds* 0.05002538;
					sstr.clear();
					sstr.str("");
				
					sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(i).axis_w<<"\t"<<inArr.at(i).axis_pci<<"\t"<<"obj"<<"\t"<<dis<<"\t"<<absdis<<"\t"<<inArr.at(i).left_idx<<"\t"<<inArr.at(i).right_idx<<"\t"<<inArr.at(i).left_h<<"\t"<<inArr.at(i).right_h<<"\n";
					str = sstr.str();
					fputs(str.c_str(), fp);

					outArr.push_back(inArr.at(i));
					isdis2Ok = true;

					continue;
				}
			}
			else if(isdis2Ok==true && isdis3Ok==false)
			{
				lb = model1_ds3 - DisOffset;
				rb = model1_ds3 + DisOffset;
				if(dis > lb && dis < rb)	// 14875
				{
					double absdis = rds* 0.05002538;
					sstr.clear();
					sstr.str("");
					
					sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(i).axis_w<<"\t"<<inArr.at(i).axis_pci<<"\t"<<"obj"<<"\t"<<dis<<"\t"<<absdis<<"\t"<<inArr.at(i).left_idx<<"\t"<<inArr.at(i).right_idx<<"\t"<<inArr.at(i).left_h<<"\t"<<inArr.at(i).right_h<<"\n";
					str = sstr.str();
					fputs(str.c_str(), fp);

					outArr.push_back(inArr.at(i));
					isdis3Ok=true;

					continue;
				}
			}
			else if(isdis3Ok==true && isdis4Ok==false)
			{
				lb = model1_ds4 - DisOffset;
				rb = model1_ds4 + DisOffset;
				if(dis > lb && dis < rb)	//2500
				{
					isdis1Ok = false;
					isdis2Ok = false;
					isdis3Ok = false;
					model1ok = true;
					lds = rds;
					
					double absdis = rds* 0.05002538;
					sstr.clear();
					sstr.str("");
					
					sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(i).axis_w<<"\t"<<inArr.at(i).axis_pci<<"\t"<<"obj"<<"\t"<<dis<<"\t"<<absdis<<"\t"<<inArr.at(i).left_idx<<"\t"<<inArr.at(i).right_idx<<"\t"<<inArr.at(i).left_h<<"\t"<<inArr.at(i).right_h<<"\n";
					iZhouOrder=0;
					str = sstr.str();
					fputs(str.c_str(), fp);

					outArr.push_back(inArr.at(i));
				}
			}
		}
		else
		{
			//	Another model for calculate
			int iZhou1=0;
			int iZhou2=0;
			int iZhou3=0;
			int iZhou4=0;
			
			for (size_t j1=i;j1<inArr.size();j1++)
			{
				double dZhouDis1=(inArr.at(j1).axis_pci - lds)*0.05002538;
				if (iZhou1==0)
				{
					iZhou1=j1;
				}
				iZhou2=0;
				for (size_t j2=j1+1;j2<inArr.size();j2++)//匹配第2个轴距
				{
					
					double dZhouDis2=(inArr.at(j2).axis_pci - inArr.at(iZhou1).axis_pci)*0.05002538;

					lb = model2_ds2 - DisOffset;
					rb = model2_ds2 + DisOffset;
					if(dZhouDis2 > lb && dZhouDis2 < rb)	//2500
					{
						iZhou2=j2;
						iZhou3=0;
						for (size_t j3=j2+1;j3<inArr.size();j3++)//匹配第3个轴距
						{
							double dZhouDis3=(inArr.at(j3).axis_pci - inArr.at(iZhou2).axis_pci)*0.05002538;
							lb = model2_ds3 - DisOffset;
							rb = model2_ds3 + DisOffset;
							if(dZhouDis3 > lb && dZhouDis3 < rb)	//2500
							{
								iZhou3=j3;
								iZhou4=0;
								for (size_t j4=j3+1;j4<inArr.size();j4++)//匹配第4个轴距
								{
									double dZhouDis4=(inArr.at(j4).axis_pci - inArr.at(iZhou3).axis_pci)*0.05002538;
									lb = model2_ds4 - DisOffset;
									rb = model2_ds4 + DisOffset;
									if(dZhouDis4 > lb && dZhouDis4 < rb)	//2500
									{
										iZhou4=j4;
										//rds = inArr.at(iZhou4).second;
										rds = inArr.at(iZhou4).axis_pci;
										lds = rds;
										iCarOrder++;
										
										double absdis = rds* 0.05002538;
										sstr.clear();
										sstr.str("");
										
										sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(iZhou1).axis_w<<"\t"<<inArr.at(iZhou1).axis_pci<<"\t"<<"obj"<<"\t"<<dZhouDis1<<"\t"<<inArr.at(iZhou1).axis_pci* 0.05002538<<"\t"<<inArr.at(iZhou1).left_idx<<"\t"<<inArr.at(iZhou1).right_idx<<"\t"<<inArr.at(iZhou1).left_h<<"\t"<<inArr.at(iZhou1).right_h<<"\n";
										sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(iZhou2).axis_w<<"\t"<<inArr.at(iZhou2).axis_pci<<"\t"<<"obj"<<"\t"<<dZhouDis2<<"\t"<<inArr.at(iZhou2).axis_pci* 0.05002538<<"\t"<<inArr.at(iZhou2).left_idx<<"\t"<<inArr.at(iZhou2).right_idx<<"\t"<<inArr.at(iZhou2).left_h<<"\t"<<inArr.at(iZhou2).right_h<<"\n";
										sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(iZhou3).axis_w<<"\t"<<inArr.at(iZhou3).axis_pci<<"\t"<<"obj"<<"\t"<<dZhouDis3<<"\t"<<inArr.at(iZhou3).axis_pci* 0.05002538<<"\t"<<inArr.at(iZhou3).left_idx<<"\t"<<inArr.at(iZhou3).right_idx<<"\t"<<inArr.at(iZhou3).left_h<<"\t"<<inArr.at(iZhou3).right_h<<"\n";
										sstr<<iCarOrder<<"\t"<<++iZhouOrder<<"\t"<<inArr.at(iZhou4).axis_w<<"\t"<<inArr.at(iZhou4).axis_pci<<"\t"<<"obj"<<"\t"<<dZhouDis4<<"\t"<<inArr.at(iZhou4).axis_pci* 0.05002538<<"\t"<<inArr.at(iZhou4).left_idx<<"\t"<<inArr.at(iZhou4).right_idx<<"\t"<<inArr.at(iZhou4).left_h<<"\t"<<inArr.at(iZhou4).right_h<<"\n";
										iZhouOrder=0;
										str = sstr.str();
										fputs(str.c_str(), fp);

										outArr.push_back(inArr.at(iZhou1));
										outArr.push_back(inArr.at(iZhou2));
										outArr.push_back(inArr.at(iZhou3));
										outArr.push_back(inArr.at(iZhou4));
										iZhou1=0;
										iZhou2=0;
										iZhou3=0;
										iZhou4=0;
										i=j4;
										j1=j4;
										break;
									}
								}
								if (iZhou4==0)
								{
									iZhou3=0;
								}
								break;
							}
						}
						if (iZhou3==0)
						{
							iZhou2=0;
						}
						break;
					}
				}
				if (iZhou2==0)
				{
					iZhou1=0;
				}
			}
		}
	}

	fflush(fp);
	fclose(fp);
	fp = NULL;
}

void LunzhouFilter(vector<PAIR>* left_in, vector<PAIR>* right_in, vector<PAIR_AXIS>& left_o, vector<PAIR_AXIS>& right_o)
{
	//step 1： 粗过滤
	//在循环之前先做长度判断
	/* old-version
	int len = left_in->size() < right_in->size() ? left_in->size() : right_in->size();
	double dr1 = left_in->at(0).first;   
	double dr2 = right_in->at(0).first;
	int head1 = 0 , tail1 = 0;
	int head2 = 0 , tail2 = 0;
	 
	for(int i = 1; i< len; ++i)
	{
		double d1 = left_in->at(i).first;
		if(d1<1 && dr1>1) //lunzhou begin
		{
			head1 = i;
		}
		if(dr1<1 && d1 >1) //lunzhou end
		{
			tail1 = i-1;
			int datalen1 = tail1- head1 + 1; //lunzhou len
			
			PAIR_AXIS tmp_axis;
			tmp_axis.first = datalen1;		//lunzhou len
			if(datalen1%2 == 0)// 如果数据长度为偶数
			{
				int laber1 = head1 + datalen1/2 -1;
				int laber2 = tail1 - datalen1/2 +1;
				
				long axisCenter = (left_in->at(laber1).second + left_in->at(laber2).second)/2;
				tmp_axis.second = axisCenter; //lunzhou center pci
			}
			else
			{
				int center = head1 + datalen1/2;
				
				long axisCenter = left_in->at(center).second;
				tmp_axis.second = axisCenter;
			}
			left_o.push_back(tmp_axis);
		}
		dr1 = d1;
		
		double d2 = right_in->at(i).first;
		if(d2<1 && dr2>1)
		{
			head2 = i;
		}
		if(dr2<1 && d2 >1)
		{
			tail2 = i-1;
			int datalen2 = tail2- head2 + 1;
			PAIR_AXIS tmp_axis;
			tmp_axis.first = datalen2;
			if(datalen2%2 == 0)// 如果数据长度为偶数
			{
				int laber1 = head2 + datalen2/2 -1;
				int laber2 = tail2 - datalen2/2 +1;

				long axisCenter = (right_in->at(laber1).second + right_in->at(laber2).second)/2;
				tmp_axis.second = axisCenter;
			}
			else
			{
				int center = head2 + datalen2/2;

				long axisCenter = right_in->at(center).second;
				tmp_axis.second = axisCenter;
			}
			right_o.push_back(tmp_axis);
		}
		dr2 = d2;
	}
	//step 2： 精过滤,轮轴宽度过滤：过宽或者过窄的类轴数据过滤掉
	vector<PAIR_AXIS>::iterator it;

	for(it = left_o.begin(); it != left_o.end();)
	{
		if((*it).first<140 || (*it).first>220)
			it=left_o.erase(it); 
		else
			++it;
	}

	for(it = right_o.begin(); it != right_o.end();)
	{
		if((*it).first<140 || (*it).first>220)
			it=right_o.erase(it); 
		else
			++it;
	}*/

	//小车底部的数据
	vector<PAIR > lds1_bottom, lds2_bottom;
	lds1_bottom.clear();
	lds2_bottom.clear();

	int lsize, rsize; 
	lsize = left_in->size();
	rsize = right_in->size();

	//STEP-1 :过滤出动车底部的数据
	float lds1_f = 0.0, lds2_f = 0.0;
	for(int i =0; i< lsize; ++i)
	{
		lds1_f = left_in->at(i).first;
		if(lds1_f < 2.0)
			lds1_bottom.push_back(left_in->at(i));
	}
	for(int i =0; i< rsize; ++i)
	{
		lds2_f = right_in->at(i).first;
		if(lds2_f < 2.0)
			lds2_bottom.push_back(right_in->at(i));
	}
	//通过bottom数据定位出Head和Tail
	long Head_Lds1 = lds1_bottom.at(0).second;
	long Tail_Lds1 = lds1_bottom.at(lsize-1).second;

	long Head_Lds2 = lds2_bottom.at(0).second;
	long Tail_Lds2 = lds2_bottom.at(rsize-1).second;

	//STEP-2 :轮轴过滤
	double dr1 = 0.0, dr2 = 0.0, d1 = 0.0, d2 = 0.0;
	int head1 = 0 , tail1 = 0;
	int head2 = 0 , tail2 = 0;
	long l_head_pci, r_head_pci;
	
	lsize = lds1_bottom.size();
	rsize = lds2_bottom.size();

	l_head_pci = lds1_bottom.at(0).second;
	r_head_pci = lds2_bottom.at(0).second;

	//dr1表示d1的前一个值
	dr1 = lds1_bottom.at(0).first;  // >0.6 
	for(int i = 1; i< lsize; ++i)
	{
		d1 = lds1_bottom.at(i).first;
		if(d1<0.6 && d1>0.5 && dr1>0.6) //lunzhou begin
		{
			head1 = i;
		}
		if(dr1<0.6 && dr1>0.5 && d1>0.6)
		{
			tail1 = i-1;
			int datalen1 = tail1- head1 + 1; //lunzhou len
			
			PAIR_AXIS tmp_axis;
			tmp_axis.first = datalen1;		//lunzhou len
			if(datalen1%2 == 0)// 如果数据长度为偶数
			{
				int laber1 = head1 + datalen1/2 -1;
				int laber2 = tail1 - datalen1/2 +1;
				
				long axisCenter = (lds1_bottom.at(laber1).second + lds1_bottom.at(laber2).second)/2;
				tmp_axis.second = axisCenter; //lunzhou center pci
			}
			else
			{
				int center = head1 + datalen1/2;
				
				long axisCenter = lds1_bottom.at(center).second;
				tmp_axis.second = axisCenter;
			}
			left_o.push_back(tmp_axis);
		}
		dr1 = d1;
	}

	dr2 = lds2_bottom.at(0).first;	// >0.5
	for(int i = 1; i< rsize; ++i)
	{
		d2 = lds2_bottom.at(i).first;
		if(d2<0.6 && d2>0.5 && dr2>0.6)
		{
			head2 = i;
		}
		if(dr2<0.6 && dr2>0.5 && d2>0.6)
		{
			tail2 = i-1;
			int datalen2 = tail2- head2 + 1;
			PAIR_AXIS tmp_axis;
			tmp_axis.first = datalen2;
			if(datalen2%2 == 0)// 如果数据长度为偶数
			{
				int laber1 = head2 + datalen2/2 -1;
				int laber2 = tail2 - datalen2/2 +1;

				long axisCenter = (lds2_bottom.at(laber1).second + lds2_bottom.at(laber2).second)/2;
				tmp_axis.second = axisCenter;
			}
			else
			{
				int center = head2 + datalen2/2;

				//long axisCenter = dlg->m_work.lds2_val.at(center).second;
				long axisCenter = lds2_bottom.at(center).second;
				tmp_axis.second = axisCenter;
			}
			right_o.push_back(tmp_axis);
		}
		dr2 = d2;
	}

}
//轮轴过滤
void LunzhouAcquire(vector<PAIR_AXIS>* left_in, vector<PAIR_AXIS>* right_in, vector<PAIR_AXIS>& left_o, vector<PAIR_AXIS>& right_o)
{
	/*
		通过轴间距筛选出来左右LDS的轴
	*/
	const double DisOffset = 100.0; //mm 
	double lbound_1, rbound_1, lbound_2, rbound_2, lbound_3, rbound_3, lbound_4, rbound_4; // left-bound, right-bound
	lbound_1 = 3498.575 - 100.0 ;	rbound_1 = 3498.575 + 100.0 ; 
	lbound_2 = 2500.0   - 100.0 ;	rbound_2 = 2500.0   + 100.0 ;
	lbound_3 = 14876.047 - 100.0 ;	rbound_3 = 14876.047 + 100.0 ; 
	lbound_4 = 5000.000  - 300.0 ;	rbound_4 = 5000.000  + 300.0 ;
	int lsize, rsize;
	long lds1_head_pci, lds2_head_pci;
	long l1r, l1c, r1r, r1c, diff;
	double dis, puredis;
	
	lsize = left_in->size();
	rsize = right_in->size();
	left_o.clear();
	right_o.clear();
	
	if(lsize != 0)
	{
		lds1_head_pci = left_in->at(0).second;
		l1r = lds1_head_pci;		//车头PCI
		for(int i =0 ; i< lsize; ++i)
		{
			int len = left_in->at(i).first;
			if(len > 140 && len <180)
			{
				l1c = left_in->at(i).second;
				diff = l1c - l1r;
				dis = diff * OnePulseMM;	//inner axie distance
				puredis = l1c*OnePulseMM;	//pure axie distance
			
				if(dis > lbound_1 && dis < rbound_1)
				{
					left_o.push_back(left_in->at(i));
					l1r = l1c;
					continue;
				}
				if(dis > lbound_2 && dis < rbound_2)
				{
					left_o.push_back(left_in->at(i));
					l1r = l1c;
					continue;
				}
				if(dis > lbound_3 && dis < rbound_3)
				{
					left_o.push_back(left_in->at(i));
					l1r = l1c;
					continue;
				}
				if(dis > lbound_4 && dis < rbound_4)
				{
					left_o.push_back(left_in->at(i));
					l1r = l1c;
					continue;
				}
			}
		}
	}
	else
	{
		left_o.clear();//轮轴数据无
	}

	if(rsize != 0)
	{
		lds2_head_pci = right_in->at(0).second;
		r1r = lds2_head_pci;		//车头PCI
		for(int i =0 ; i< rsize; ++i)
		{
			int len = right_in->at(i).first;
			if(len > 140 && len <180)
			{
				r1c = right_in->at(i).second;
				diff = r1c - r1r;
				dis = diff * OnePulseMM;	//inner axie distance
				puredis = r1c*OnePulseMM;	//pure axie distance
			
				if(dis > lbound_1 && dis < rbound_1)
				{
					right_o.push_back(right_in->at(i));
					r1r = r1c;
					continue;
				}
				if(dis > lbound_2 && dis < rbound_2)
				{
					right_o.push_back(right_in->at(i));
					r1r = r1c;
					continue;
				}
				if(dis > lbound_3 && dis < rbound_3)
				{
					right_o.push_back(right_in->at(i));
					r1r = r1c;
					continue;
				}
				if(dis > lbound_4 && dis < rbound_4)
				{
					right_o.push_back(right_in->at(i));
					r1r = r1c;
					continue;
				}
			}
		}
	}
	else
	{
		right_o.clear();//轮轴数据无
	}
}
//轮轴数据匹配
void LunzhouMatch(vector<PAIR_AXIS>* left_in, vector<PAIR_AXIS>* right_in, vector<PAIR_MATCH_ARR>& match_arr)
{
	//assert(left_in->size()==right_in->size());
	int len = left_in->size()<right_in->size()?left_in->size():right_in->size();
	long lpls, rpls, diff;
	PAIR_MATCH_ARR tmp_arr;
	for(int i = 0; i< len ; ++i)
	{
		lpls = left_in->at(i).second; 
		rpls = right_in->at(i).second;
		diff = abs(lpls - rpls);
		
		if(diff < 1200)	//左右2个是同1轮轴数据
		{
			tmp_arr.first = lpls;
			tmp_arr.second = rpls;
		}
		else
		{
			if(lpls < rpls)
			{
				tmp_arr.first = lpls;
				tmp_arr.second = -1;
			}
			else
			{
				tmp_arr.first = -1;
				tmp_arr.second = rpls;
			}
		}
		match_arr.push_back(tmp_arr);
	}
}
//轮轴数据修正
void LunzhouUpdate(vector<PAIR_MATCH_ARR>* arr_in, vector<long>& arr_o)
{
	vector<long> offset_arr;
	offset_arr.clear();
	int len = arr_in->size();
	long lpls, rpls, offset, pls;
	//计数偏移量
	for(int i = 0; i<len; ++i)
	{
		lpls = arr_in->at(i).first;
		rpls = arr_in->at(i).second;
		if(lpls == -1 || rpls == -1)
			offset = -1;
		else
			offset = rpls - lpls;
		offset_arr.push_back(offset); //得到偏移量
	}
	//修正偏移量
	for(int i = 0; i<len; ++i)
	{
		offset = offset_arr.at(i);
		if(offset != -1)
		{
			for(int j = i-1; j>0; j--)
			{
				if(offset_arr.at(j) != -1)
					break;
				else
					offset_arr.at(j) = offset;
			}
		}
	}
	//修正最终结果
	for(int i = 0; i<len; ++i)
	{
		lpls = arr_in->at(i).first;
		rpls = arr_in->at(i).second;
		offset = offset_arr.at(i);
		
		if(lpls == -1)
			pls = rpls - offset;
		else
			pls = lpls;

		arr_o.push_back(pls);
	}
}

int ZhouDisFilter4CRV2(vector<AIXS_DETAIL>& inArr, vector<long>& outArr)
{
	string path = "D:\\DataLog\\";
	string str;
	path = path + GetDateString() + "\\";
	_mkdir(path.c_str());
	
	string name = GetTimeDateString() + "-03_CR_ZHOU.txt";
	string fullname = path + name;
	FILE* fp = fopen(fullname.c_str(),"wt+");
	stringstream sstr;	
	
	fputs("log list:\n", fp);
	long arrdis[] = {50000, 306000, 50000,	//1
			107000, 50000, 306000, 50000,	//2
			107000, 50000, 306000, 50000,	//3
			107000, 50000, 306000, 50000,	//4
			107000, 50000, 306000, 50000,	//5
			107000, 50000, 306000, 50000,	//6
			107000, 50000, 306000, 50000,	//7
			107000, 50000, 306000, 50000};	//8

	// find head
	long headlimit = 5000*20;
	long headpci;
	long forepci = inArr.at(0).axis_pci;
	
	if(forepci > headlimit)
	{
		sstr.clear();
		sstr.str("");
		sstr<<"exceed the head limit!!!"<<endl;
		str = sstr.str();
		fputs(str.c_str(), fp);
		
		fflush(fp);
		fclose(fp);
		return 1;
	}
	bool headOk = false;
	for(size_t i = 1; i< inArr.size(); i++)
	{
		long curr = inArr.at(i).axis_pci;
		if(curr>headlimit && !headOk)
			return 1;
		long diff = curr - forepci - 50000;

		if(diff <300 && diff >-300)
		{
			headpci = forepci;
			headOk = true;
			break;
		}
		else
		{
			forepci = curr;
		}
	}
	//cout<< headpci<<endl;
	sstr.clear();
	sstr.str("");
	sstr<<headpci<<endl;
	str = sstr.str();
	fputs(str.c_str(), fp);

	outArr.push_back(headpci);

	long  currv = 0;
	int j = 0; //table index
	for(size_t i = 0; i< inArr.size(); i++)
	{
		currv = inArr.at(i).axis_pci;
		if(currv <= headpci)
			continue;
		if(j>=sizeof(arrdis)/sizeof(long))
			break;
		long diff = headpci + arrdis[j] - currv;
		if(diff<300 && diff> -300)
		{
			outArr.push_back(currv);
			headpci = currv;
			j++;
			//cout<<currv<<endl;
			sstr.clear();
			sstr.str("");
			sstr<<currv<<endl;
			str = sstr.str();
			fputs(str.c_str(), fp);
			continue;
		}
		if(diff>=300)
			continue;
		if(diff<=-300)
		{
			headpci = headpci + arrdis[j];
			outArr.push_back(headpci);
			j++;
			i--;
			sstr.clear();
			sstr.str("");
			sstr<<headpci<<endl;
			str = sstr.str();
			fputs(str.c_str(), fp);
		}
	}
	/////////log data;
	sstr.clear();
	sstr.str("");
	sstr<<"list size ="<<outArr.size()<<endl;
	fputs(str.c_str(), fp);
	for(size_t i =0; i<outArr.size(); i++)
	{
		sstr.clear();
		sstr.str("");
		if(i != 0)
			sstr<<i+1<<"\t"<<outArr.at(i)<<"\t"<< outArr.at(i) - outArr.at(i-1)<<endl;
		else
			sstr<<i+1<<"\t"<<outArr.at(i)<<"\t"<<endl;
		
		str = sstr.str();
		fputs(str.c_str(), fp);
	}
	fflush(fp);
	fclose(fp);
	return 0;
}

int LogVec(vector<PAIR>* inArr, string fname)
{
	string path = "D:\\DataLog\\";
	_mkdir(path.c_str());
	path = path + GetDateString() + "\\";
	_mkdir(path.c_str());
	CString cstr;
	_mkdir(path.c_str());
	string name = GetTimeDateString() + "-"+ fname + "-pair.txt";
	string fullname = path + name;
	FILE* fp = fopen(fullname.c_str(),"wt+");
	if(fp == NULL)
	{
		CFIFO::GetInstance()->Write(name + " open failed ... ...\r\n");
		return 1;
	}
	cstr.Format("%s len = %d\n", fname.c_str(), inArr->size());
	fputs(cstr.GetString(), fp);

	for(size_t i = 0; i<inArr->size(); i++)
	{
		cstr.Format("%lf\t%ld\n", inArr->at(i).first, inArr->at(i).second);
		fputs(cstr.GetString(), fp);
	}

	if(fp != NULL)
	{
		fflush(fp);
		fclose(fp);
		fp = NULL;
	}
	return 0;
}

int LogVec(vector<PAIR_AXIS>* inArr, string fname)
{
	string path = "D:\\DataLog\\";
	_mkdir(path.c_str());
	CString cstr;
	path = path + GetDateString() + "\\";
	_mkdir(path.c_str());
	string name = GetTimeDateString() + "-"+ fname + "-pair_axis.txt";
	string fullname = path + name;
	FILE* fp = fopen(fullname.c_str(),"wt+");
	if(fp == NULL)
	{
		CFIFO::GetInstance()->Write(name + " open failed ... ...\r\n");
		return 1;
	}
	cstr.Format("%s len = %d\n", fname.c_str(), inArr->size());
	fputs(cstr.GetString(), fp);

	for(size_t i = 0; i<inArr->size(); i++)
	{
		cstr.Format("%d\t%ld\n", inArr->at(i).first, inArr->at(i).second);
		fputs(cstr.GetString(), fp);
	}

	if(fp != NULL)
	{
		fflush(fp);
		fclose(fp);
		fp = NULL;
	}
	return 0;
}

int LogVec(vector<AIXS_DETAIL>* inArr, string fname)
{
	string path = "D:\\DataLog\\";
	_mkdir(path.c_str());
	path = path + GetDateString() + "\\";
	CString cstr;
	_mkdir(path.c_str());
	string name = GetTimeDateString() + "-"+ fname + "-axis_detail.txt";
	string fullname = path + name;
	FILE* fp = fopen(fullname.c_str(),"wt+");
	if(fp == NULL)
	{
		CFIFO::GetInstance()->Write(name + " open failed ... ...\r\n");
		return 1;
	}
	cstr.Format("%s len = %d\n", fname.c_str(), inArr->size());
	fputs(cstr.GetString(), fp);

	for(size_t i = 0; i<inArr->size(); i++)
	{
		cstr.Format("%d\t%d\t%f\t%f\t%d\t%ld\n", inArr->at(i).left_idx, inArr->at(i).right_idx, inArr->at(i).left_h, inArr->at(i).right_h, inArr->at(i).axis_w, inArr->at(i).axis_pci);
		fputs(cstr.GetString(), fp);
	}

	if(fp != NULL)
	{
		fflush(fp);
		fclose(fp);
		fp = NULL;
	}
	return 0;
}

int LogVec(vector<string>* inArr, string fname)
{
	string path = "D:\\DataLog\\";
	_mkdir(path.c_str());
	path = path + GetDateString() + "\\";
	CString cstr;
	_mkdir(path.c_str());
	string name = GetTimeDateString() + "-"+ fname + "-pulse.txt";
	string fullname = path + name;
	FILE* fp = fopen(fullname.c_str(),"wt+");
	if(fp == NULL)
	{
		CFIFO::GetInstance()->Write(name + " open failed ... ...\r\n");
		return 1;
	}
	cstr.Format("%s len = %d\n", fname.c_str(), inArr->size());
	fputs(cstr.GetString(), fp);

	for(size_t i = 0; i<inArr->size(); i++)
	{
#if 0
		//cstr.Format("%s\n", inArr->at(i).c_str());
		size_t len = inArr->at(i).size();
		cstr.Empty();
		for(size_t i = 0; i<len; i++)
		{
			cstr.AppendFormat("%02x ", (unsigned char)inArr->at(i).at(i));
		}
		cstr.Append("\n");
#endif
		cstr.Format("%s", inArr->at(i).c_str());
		cstr.Append("\n");
		fputs(cstr.GetString(), fp);
	}

	if(fp != NULL)
	{
		fflush(fp);
		fclose(fp);
		fp = NULL;
	}
	return 0;
}

int LogVec(vector<long>* inArr, long headpci, string fname)
{
	string path = "D:\\DataLog\\";
	_mkdir(path.c_str());
	path = path + GetDateString() + "\\";
	CString cstr;
	_mkdir(path.c_str());
	string name = GetTimeDateString() + "-"+ fname + "-zhou.txt";
	string fullname = path + name;
	FILE* fp = fopen(fullname.c_str(),"wt+");
	if(fp == NULL)
	{
		CFIFO::GetInstance()->Write(name + " open failed ... ...\r\n");
		return 1;
	}
	cstr.Format("%s len = %d\n", fname.c_str(), inArr->size());
	fputs(cstr.GetString(), fp);

	cstr.Format("headpci = %ld\n", headpci);
	fputs(cstr.GetString(), fp);

	for(size_t i = 0; i<inArr->size(); i++)
	{
		cstr.Format("%d\t%ld\n", i+1, inArr->at(i));
		fputs(cstr.GetString(), fp);
	}

	if(fp != NULL)
	{
		fflush(fp);
		fclose(fp);
		fp = NULL;
	}
	return 0;
}
///ADD 20171211
int LogVec(shared_ptr<vector<PAIR>> inArr, string fname)
{
	string path = "D:\\DataLog\\";
	_mkdir(path.c_str());
	path = path + GetDateString() + "\\";
	CString cstr;
	_mkdir(path.c_str());
	string name = GetTimeDateString() + "-"+ fname + "-pair.txt";
	string fullname = path + name;
	FILE* fp = fopen(fullname.c_str(),"wt+");
	if(fp == NULL)
	{
		CFIFO::GetInstance()->Write(name + " open failed ... ...\r\n");
		return 1;
	}
	cstr.Format("%s len = %d\n", fname.c_str(), inArr->size());
	fputs(cstr.GetString(), fp);

	for(size_t i = 0; i<inArr->size(); i++)
	{
		cstr.Format("%lf\t%ld\n", inArr->at(i).first, inArr->at(i).second);
		fputs(cstr.GetString(), fp);
	}

	if(fp != NULL)
	{
		fflush(fp);
		fclose(fp);
		fp = NULL;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//第二代库检机器人算法
void Databrush(vector<PAIR>& in, vector<PAIR>& out)
{
	out.clear();
	vector<PAIR> tmpvec;
	tmpvec.clear();
	//Brush 1
	float disv = 0.0;
	long  pciv = 0, flag = 0;
	int idx =0;
	for(auto it = in.begin(); it!= in.end();it++, idx++)
	{
		/*
		if(idx>4619 && idx <4621)
			cout<<idx<<endl;;
		*/
		if(it==in.begin())
		{
			disv = it->first;
			pciv = it->second;
			tmpvec.push_back(*it);
			continue;
		}
		float currdis = it->first;
		long  currpci = it->second;
		if(currdis >11.5)
			currdis = disv;
		//排除过大的异常的PCI值
		if(currpci-pciv>20000) 
			currpci = pciv;
		//排除连续相同异常PCI值,除开车头车尾加速减速阶段
		/*if(currpci>20000 && currpci<((in.end()-1)->second - 20000) && currpci<= pciv)
			currpci+= 20;*/
		/*if(currpci == flag)
		{
			flag = pciv;
			currpci+= 20;
		}*/
		PAIR tmpp;
		tmpp.first = currdis;
		tmpp.second = currpci;
		tmpvec.push_back(tmpp);
		disv = currdis;
		pciv = currpci;
	}

	//Brush 2->刷新突变数据
	float foredis = 0;
	float behinddis = 0;
	for(int i = 0; i< tmpvec.size()-5; i++)
	{
		/*
		if(i>4619 && i<4621)
			cout<<i<<endl;
		*/
		if(i==0)
		{
			out.push_back(tmpvec.at(i));
			foredis = tmpvec.at(i).first;
			continue;
		}
		float curr = tmpvec.at(i).first;
		int v = 0;
		if(curr>1.0 && foredis<1.0)
		{
			for(int j=1;j<=5;j++)
			{
				if(tmpvec.at(i+j).first<1.0)
					v++;
			}

			if(foredis<1.0 && v>=3)
			{
				PAIR tmp;
				tmp.first = foredis;
				tmp.second = tmpvec.at(i).second;
				out.push_back(tmp);
				foredis = tmp.first;
			}
			else
			{
				out.push_back(tmpvec.at(i));
				foredis = tmpvec.at(i).first;
			}
		}
		else if(curr>2.0 && foredis>1.0 && foredis<2.0 )
		{
			for(int j=1;j<=5;j++)
			{
				if(tmpvec.at(i+j).first>1.0 && tmpvec.at(i+j).first<2.0)
					v++;
			}

			if(foredis>1.0 && foredis<2.0 && v>=3)
			{
				PAIR tmp;
				tmp.first = foredis;
				tmp.second = tmpvec.at(i).second;
				out.push_back(tmp);
				foredis = tmp.first;
			}
			else
			{
				out.push_back(tmpvec.at(i));
				foredis = tmpvec.at(i).first;
			}
		}
		else
		{
			out.push_back(tmpvec.at(i));
			foredis = tmpvec.at(i).first;
		}
	}
	//cout<<"finish..."<<endl;
}

void Findtrainhead(vector<PAIR>& in, long headbeg, long slen, long& headpci)
{
	headpci = -1;

	for(int i = 0; i<in.size()-5;i++)
	{
		//PCI value 
		if(in.at(i).second<headbeg)
			continue;
		if(in.at(i).second>headbeg+slen)
			break;
		//DIS value
		int fidx = 0;
		int bidx = 0;
		for(int s1 = -5; s1<0;s1++)
		{
			if(in.at(i+s1).first>980.0)
				fidx++;
		}
		for(int s2 =1; s2<=5;s2++)
		{
			if(in.at(i+s2).first<800.0 && in.at(i+s2).first>700)
				bidx++;
		}
		if(fidx==5 && bidx==5)
		{
			headpci = in.at(i).second;
		}
	}
}

void Findzhou(vector<PAIR>& in, long headpci, vector<PAIR>& out)
{
	long zdis[] = {
		 85555, 50000, 305900, 50000,	//1
		106890, 50000, 305900, 50000,	//2
		106890, 50000, 305900, 50000,	//3
		106890, 50000, 305900, 50000,	//4
		106890, 50000, 305900, 50000,	//5
		106890, 50000, 305900, 50000,	//6
		106890, 50000, 305900, 50000,	//7
		106890, 50000, 305900, 50000	//8
	};

	long loffset = -2500;
	long roffset = 2500;

	for(int i = 0; i<32;i++)
	{
		long a = headpci + loffset + zdis[i];
		long b = headpci + roffset + zdis[i];
		vector<PAIR> tmpvv;
		tmpvv.clear();
		for(static int j = 0; j< in.size(); j++)
		{
			if(in.at(j).second<a)
				continue;
			if(in.at(j).second>= a && in.at(j).second<=b)
				tmpvv.push_back(in.at(j));
			if(in.at(j).second>b)
			{
				break;
			}
		}
		
		long lpci, rpci;
		lpci = rpci = 0;
		bool isheadok = false;
	
		float lf1, lf2, lf3, lf4, lf5, currf, rf1, rf2, rf3, rf4, rf5;
		int lidx, ridx, cidx;
		lidx = ridx = cidx = 0;
		int l = 0;
		for(/*int */l =5; l<tmpvv.size()-5;l++)
		{
			lf1 = tmpvv.at(l-5).first;
			lf2 = tmpvv.at(l-4).first;
			lf3 = tmpvv.at(l-3).first;
			lf4 = tmpvv.at(l-2).first;
			lf5 = tmpvv.at(l-1).first;
			currf = tmpvv.at(l).first;
			rf1 = tmpvv.at(l+1).first;
			rf2 = tmpvv.at(l+2).first;
			rf3 = tmpvv.at(l+3).first;
			rf4 = tmpvv.at(l+4).first;
			rf5 = tmpvv.at(l+5).first;
			
			if(!isheadok)
			{
				if(currf<=1000.0)
				{
					if(lf1>1000.0 && lf2>1000.0 && lf3>1000.0 && lf4>1000.0 && lf5>1000.0 &&
						rf1<1000.0 && rf2<1000.0 && rf3<1000.0 && rf4<1000.0 && rf5<1000.0)
					{
						//cout<<"left border :"<<tmpvv.at(l).second<<endl;
						lpci = tmpvv.at(l).second;
						isheadok = true;
						lidx = l;
					}
					else
						continue;
				}
				else
					continue;
			}
			else
			{
				if(currf>=1000.0)
				{
					if(lf1<1000.0 && lf2<1000.0 && lf3<1000.0 && lf4<1000.0 && lf5<1000.0 &&
						rf1>1000.0 && rf2>1000.0 && rf3>1000.0 && rf4>1000.0 && rf5>1000.0)
					{
						//cout<<"right border :"<<tmpvv.at(l-1).second<<endl;
						rpci = tmpvv.at(l-1).second;
						isheadok = false;
						ridx = l-1;
						break;
					}
					else
						continue;
				}
				else
					continue;
			}
		}
		/*headpci = (lpci+rpci)/2;
		out.push_back(headpci);*/
		if((lidx + ridx)%2 == 0)
		{
			cidx = (lidx + ridx)/2;
			out.push_back(tmpvv.at(cidx));
			headpci = tmpvv.at(cidx).second;
			//cout<<i+1<<"\t"<<tmpvv.at(cidx).first<<"\t"<<tmpvv.at(cidx).second<<endl;
		}
		else
		{
			cidx = (lidx + ridx)/2;
			float fv = (tmpvv.at(cidx).first + tmpvv.at(cidx+1).first)/2;
			long  lpci = (tmpvv.at(cidx).second + tmpvv.at(cidx+1).second)/2;
			out.push_back(make_pair<float, long>(fv, lpci));
			headpci = lpci;
			//cout<<i+1<<"\t"<<fv<<"\t"<<lpci<<endl;
		}
	}
}

void FindAixeCandiate_20171219(vector<PAIR>& in, vector<long>& out)
{
	int len = in.size();
	int l1, r1, l2, r2 ;
	int lcnt, rcnt;
	l1 = r1 = l2 = r2 = lcnt = rcnt = 0;
	bool islfok = false;
	for(int i = 0; i< len; i++)
	{
		if(i <20 || i> len-20)
			continue;
		/*if(i == 82369)
			cout<<"ok"<<endl;*/
		float hcurr = in.at(i).first;
		long  pci = in.at(i).second;
		if(hcurr>=1000.0 || hcurr <= 850.0)
			continue;
		if(!islfok)
		{
			int lc = 0;
			float mcurr, ncurr;
			mcurr = ncurr = 0;
			for(int m = i-20; m<i; m++)
			{
				mcurr = in.at(m).first;	
				if(mcurr>1000)
					lc++;
			}
			int rc = 0;
			for(int n = i+1; n<=i+20; n++)
			{
				ncurr = in.at(n).first;
				if(ncurr<1000 && ncurr>850)
					rc++;
			}

			if(lc ==20 && rc ==20)
			{
				l1 = i;
				islfok = true;
				//cout<<"i = "<<i<<endl;
			}
		}
		else
		{
			if(i-l1 > 570)
			{
				l1 = r1 = 0;
				islfok = false;
				continue;
			}
			float mcurr, ncurr;
			mcurr = ncurr = 0;
			int lc = 0;
			for(int m = i-20; m<i; m++)
			{
				mcurr = in.at(m).first;	
				if(mcurr<1000 && mcurr>850)
					lc++;
			}
			int rc = 0;
			for(int n = i+1; n<=i+20; n++)
			{
				ncurr = in.at(n).first;
				if(ncurr>1000)
					rc++;
			}

			if(lc ==20 && rc ==20)
			{
				r1 = i;
				/*if(r1 < l1)
				{
					l1 = r1 = 0;
					islfok = false;
					continue;
				}*/
				int lenlr = r1 - l1 ;
				if(lenlr <400 || lenlr >570)
				{
					l1 = r1 = 0;
					islfok = false;
					continue;
				}

				int idxmode = (l1 + r1)%2;
				if(idxmode == 0)
				{
					int idx = (l1 + r1)/2;
					out.push_back(in.at(idx).second);
				}
				else
				{
					int idx1 = (l1 + r1)/2;
					int idx2 = (l1 + r1)/2 + 1;
					long mean = (in.at(idx1).second + in.at(idx2).second)/2;
					out.push_back(mean);
				}
				islfok = false;
				l1 = r1 = 0;
			}
		}
	}
}

void FindFZ50000_20171219(vector<long>& in, vector<long>& out)
{
	cout<<"lz :"<<endl;
	int len = in.size();
	long forepci = 0;
	long lbd = 48000;
	long rbd = 52000;
	
	for(int j = 0; j<len; /*j++*/)
	{
		if(j==0)
		{
			forepci = in.at(j);
			j++;
			continue;
		}

		long currpci = in.at(j);
		long diff = abs(currpci - forepci);
		
		if(diff >lbd && diff < rbd)
		{
			cout<<forepci<<endl;
			cout<<currpci<<endl;
			out.push_back(forepci);
			out.push_back(currpci);
			j += 2;
			if(j>= len)
				break;
			forepci = in.at(j-1);
		}
		if(diff < lbd)
		{
			j += 1;
		}
		if(diff > rbd)
		{
			j += 1;
			forepci = currpci;
		}
	}
	cout<<"out size = "<<out.size()<<endl;
}
void FindAixeCandiate_wu(vector<PAIR>& in,  vector<PAIR>& out)
//void FindAixeCandiate_wu(vector<PAIR>& in,  vector<long>& out)
{
	int len = in.size();
	int l1, r1, l2, r2 ;
	int lcnt, rcnt;
	l1 = r1 = l1 = r1 = lcnt = rcnt = 0;

	int izhouxu=0;

	bool blnPCIerror=false;
	int iPCIerror20=0;
	int iPCIerror200=0;
	int iPCIerror2000=0;

	for(int i = 0; i< len; i++)
	{
		if(i <20 || i> len-1000)
			continue;
		/*if (in.at(i).second<4400000)
		continue;*/
		
		if ((in.at(i).second-in.at(i-1).second)>20)
		{
			blnPCIerror=true;
			iPCIerror20++;
			if ((in.at(i).second-in.at(i-1).second)>200)
				iPCIerror200++;
			if ((in.at(i).second-in.at(i-1).second)>2000)
				iPCIerror2000++;
		}

		/*if(i == 379620)
			cout<<"ok"<<endl;*/
		
		float hcurr = in.at(i).first;
		long  pci = in.at(i).second;
		//if(hcurr>=1000.0 || hcurr <= 850.0)
		if(hcurr>=1000.0)
			continue;

		r1 = 0;
		if (in.at(i-1).first>=1000 && in.at(i).first<1000)
		{
			l1=i;//初定左边界

			for (int j=i+400;j<=i+565;j++)
			{
				if (in.at(j).second>1056000 && in.at(j).second<1061000)
				{
					
					cout<<in.at(j).first<<"   "<<in.at(j+1).first<<endl;

				}
				if (in.at(j).first<1000 && in.at(j+1).first>=1000 && in.at(j+10).first>=1000)
				{
					r1=j;//初定右边界
					break;
					/*if (in.at(j).second>1056000 && in.at(j).second<1061000)
					{
					cout<<"break success!"<<endl;
					}*/
				}
			}
			if (r1==0)
			{
				continue;//右边界不存在
			}

			if ((r1-l1)<420 || (r1-l1)>565)
			{
				continue;//轴宽不满足
			}

			int isum=0;
			for (int k=l1+(r1-l1)/2-50;k<=l1+(r1-l1)/2+50;k++)
			{
				isum+=in.at(k).first;
			}
			isum=isum/100;
			//  || in.at(l1).first-isum>=90 || in.at(r1).first-isum>=90
			if (in.at(l1).first-isum<=30 || in.at(r1).first-isum<=30  )
			{
				continue;//轴底部形态不满足
			}
			cout<<++izhouxu<<":"<<"isum="<<isum<<"\t 高度差="<<in.at(l1).first-isum;

			int iwrong=0;
			for (int m=l1;m<=r1;m++)
			{
				if (in.at(m).first>=1000)
				{
					iwrong++;
				}
			}
			if (iwrong>=10)
			{
				continue;//轴数据不满足
			}
#if 0
			//候选轴成功
			cout<<"\t 轴宽："<<r1-l1<<"\t 脉冲宽："<<in.at(r1).second-in.at(l1).second<<"\t 差值1："<<in.at(l1).first-isum<<"\t 差值2："<<in.at(r1).first-isum<<endl;
			float height = (in.at(l1).first + in.at(r1).first)/2;
			long mean = (in.at(l1).second + in.at(r1).second)/2;
			out.push_back(make_pair<float, long>(height, mean));
#else
			int idx1 = l1;
			int idx2 = r1;
			long mean = 0;
			float minheight = 1024.0;
			for(int i = idx1; i<=idx2; i++)
			{
				float currheight = in.at(i).first;
				if(minheight > currheight)
					minheight = currheight;
			}
			if((idx1 + idx2)%2 == 0)
			{
				int cidx = (idx1 + idx2)/2;
				mean = in.at(cidx).second;
			}
			else
			{
				int cidx = (idx1 + idx2)/2;
				mean = (in.at(cidx).second + in.at(cidx+1).second)/2;
			}

			out.push_back(make_pair<float, long>(minheight, mean));
#endif
		} 
		
	}
	if (blnPCIerror==true )
	{
		cout<<endl;
		cout<<"间隔大于20的PCI计数："<<iPCIerror20<<endl;
		cout<<"间隔大于200的PCI计数："<<iPCIerror200<<endl;
		cout<<"间隔大于2000的PCI计数："<<iPCIerror2000<<endl;
		cout<<endl;
	}
}

//void FindFZ50000_wu(vector<long>& in, vector<long>& out)
void FindFZ50000_wu(vector<PAIR>& in, vector<PAIR>& out)
{
	cout<<"lz :"<<endl;
	int len = in.size();
	//long nextpci=0;
	long lbd = 49500;
	long rbd = 50500;
	
	for(int j = 0; j<len;j++)
	{
		float currhegt = in.at(j).first;
		long currpci = in.at(j).second;
		
		for (int k=j+1;k<len;k++)
		{
			float nexthegt = in.at(k).first;
			long  nextpci = in.at(k).second;
			long diff = abs(nextpci-currpci);
			if(diff >lbd && diff < rbd)
			{
				//out.push_back(currpci);
				//out.push_back(nextpci);
				out.push_back(make_pair<float,long>(currhegt, currpci));
				out.push_back(make_pair<float,long>(nexthegt, nextpci));
				j = k;
				
			}
			if(diff > rbd)
			{
				break;
			}
		}
		
		
		/*if (j>=17)
		{
			cout<<abs(currpci - forepci)<<endl;
		}*/
		
	}
	cout<<"out size = "<<out.size()<<endl;
}

void FindFZ54000_wu(vector<PAIR>& in, vector<PAIR>& out)
{
	cout<<"lz :"<<endl;
	int len = in.size();
	//long nextpci=0;
	long lbd = 53500;
	long rbd = 54500;
	
	for(int j = 0; j<len;j++)
	{
		float currhegt = in.at(j).first;
		long currpci = in.at(j).second;
		
		for (int k=j+1;k<len;k++)
		{
			float nexthegt = in.at(k).first;
			long  nextpci = in.at(k).second;
			long diff = abs(nextpci-currpci);
			if(diff >lbd && diff < rbd)
			{
				//out.push_back(currpci);
				//out.push_back(nextpci);
				out.push_back(make_pair<float,long>(currhegt, currpci));
				out.push_back(make_pair<float,long>(nexthegt, nextpci));
				j = k;
				
			}
			if(diff > rbd)
			{
				break;
			}
		}
		
		
		/*if (j>=17)
		{
			cout<<abs(currpci - forepci)<<endl;
		}*/
		
	}
	cout<<"out size = "<<out.size()<<endl;
}
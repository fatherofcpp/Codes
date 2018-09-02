#pragma once

#include "CmDefStruct.h"

namespace NSP_STP_CM
{
	///////////////////////////////////////////////////////////////////////////////////
	/*
	data function
	*/
	//char,int_8,int_16,int_32,int_64转字符串
	template <typename T>
	M_STRING val2string(T val)
	{
		M_STRING str;
		std::stringstream ss;
		ss << val;
		ss >> str;
		return str;
	}

	//字符串转char,int_8,int_16,int_32,int_64
	template <typename T>
	M_VOID str_to_int(MCS_CHAR* src, T &val, MS_INT32 type = 10)
	{
		MS_INT32 size_val = sizeof(T);
		MS_INT32 tmp_val = 0;

		switch(type)
		{
		case 10:
			if (size_val == 1)
			{
				sscanf(src,"%d",&tmp_val);
				val = (T)(tmp_val & 0xFF);
			}
			else if (size_val == 2)
			{
				sscanf(src,"%d",&tmp_val);
				val = (T)(tmp_val & 0xFFFF);
			}
			else if (size_val == 4)
			{
				sscanf(src,"%d",&val);
			}
			else if (size_val == 8)
			{
				sscanf(src,"%lld",&val);
			}
			else
			{
				val = 0;
			}
			break;
		case 16:
			sscanf(src,"%x",&val);
			break;
		default:
			break;
		}
	}

	//字符串拆分
	MS_INT32 StringSplit(M_STRING src, std::vector<M_STRING>& vec_data, MS_CHAR sig);

	MS_CHAR * strUpper(MS_CHAR * str);

	MS_CHAR * strLower(MS_CHAR * str);

	M_VOID Dec2Hex(BYTE* pDst, MS_INT32 length, MS_INT32 Val);

	MS_INT32 Hex2Dec(BYTE* pSrc, MS_INT32 length);

	BYTE CalcCheckSum(BYTE * pBuf, MS_INT32 nSize);

	//crc校验
	MU_INT16 CalcCrc( MU_INT8 * pucFrame, MU_INT16 usLen );

	//gb2312转utf8
	MS_INT32 gb2312_to_utf8(MCS_CHAR* gb2312, MS_INT32 gb2312_len, MS_CHAR* utf8);

	MS_INT32 get_lenth_gb_to_utf8(MCS_CHAR* gb2312, MS_INT32 gb2312_len);

	//utf8转gb2312
	MS_INT32 utf8_to_gbk(MS_CHAR * gbk, MCS_CHAR* utf, MS_INT32 utf_len);

	//字符串截取头部到左起最后一个tag, aa\\bb\\cc.jpg -> aa\\bb
	M_STRING str_match_head2sig_last(M_STRING str_input, M_STRING tag, bool include_sig = false );

	//字符串截取尾部部到左起最后一个tag，aa\\bb\\cc.jpg -> cc.jpg
	M_STRING str_match_tail2sig_last(M_STRING str_input, M_STRING tag , bool include_sig = false);

	//字符串截取头部到左起第一个tag, aa\\bb\\cc.jpg -> aa\\  \\=tag
	M_STRING str_match_head2sig_first(M_STRING str_input, M_STRING tag , bool include_sig = false);

	//内存查找字节
	MS_CHAR* mem_find(MCS_CHAR* src, MCS_INT32 src_len, MCS_CHAR dst);

	//字符串中字符双倍1:\\2.3 -> 1:\\\\2.3
	M_STRING str_sig_double(MC_STRING str_input, MCS_CHAR sig);

	//数据压缩
	MS_INT32 data_compress(MCS_CHAR* p_input_data, MCS_INT32 c_input_data_len, MS_INT32 c_max_output_num, MS_CHAR* output_data, MS_INT32& output_data_num);

	/*
	* 作用: BASE64 编码
	* 输入: src 数据指针
	*     : src_len 数据长度
	* 输出: dst 数据指针
	* 返回: 编码后的数据长度,
	* 注释: 无
	*/
	MS_INT32 f_base64_encode(MS_CHAR *const dst, const void *src, MU_INT32 src_len);

	/*
	* 作用: BASE64 解码
	* 输入: src 字符串指针
	* 输出: dst 数据指针
	* 返回: n <  0 错误
	*     : n >= 0 解码后的数据长度,
	* 注释: 无
	*/
	MS_INT32 f_base64_decode(void *const dst, MS_CHAR *const src);

	/*
	end data function
	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	frame function
	*/
	MS_INT32 SaveBmp(BYTE *buf, MS_INT32 width, MS_INT32 height, MS_INT32 iDataLen, BITMAPINFO * pBitmapInfo, M_STRING lpFileName);

	// IJL == > BGR - YCBCR  此函数效率较低 仅参考不使用
	M_VOID yuv422_to_rgb24(MU_CHAR *yuv422, MU_CHAR *rgb24, MS_INT32 width, MS_INT32 height);  

	// yuv 转换为 rgb
	static MS_INT32 convert_yuv_to_rgb_pixel(MS_INT32 y, MS_INT32 u, MS_INT32 v);

	//16位均值 转换为 24位均值 rgb     IJL == > RGB - YCBCR
	MS_INT32 convert_yuv_to_rgb_buffer(MU_CHAR* yuv, MU_CHAR *rgb, MU_INT32 width, MU_INT32 height);

	/*
	end frame function
	*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	time function
	*/
	MS_INT32 GetNowTime(tag_st_time* p_st_time);

	M_STRING GetNowTimeStr();

	M_VOID GetTimeByStamp(M_TIME_T time_stamp, tag_st_time* p_time);

	M_VOID GetStampByTime(M_TIME_T* time_stamp, tag_st_time t_time);

	M_VOID TimeAjustByMs(tag_st_time time_src, tag_st_time* p_time_dst, MS_INT32 off_s);

	MS_INT32 TimeCompare(tag_st_time time_src, tag_st_time time_obj);

	// 设置本地时间函数
	M_BOOL set_local_time(tag_st_time s_time);
	/*
	end time function
	*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	system control function
	*/

	// 关机函数
	BOOL MySystemShutdown();

	/*
	end system control function
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	net function
	*/

	//init net
	MS_INT32 SocketInit();

	/*
	end net function
	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	file function
	*/
	//磁盘空间是否不足
	M_BOOL DiskIsFull(M_STRING disk_path, MCS_INT32 min_space_g);
	M_BOOL MyGetDiskFreeSpaceEx(LPCSTR pszDrive, MU_INT32& total_g, MU_INT32& used_g);

	//写文件
	MS_INT32 file_write(M_STRING file_path, MCU_CHAR* file_data, MU_INT32 file_len);

	//文件夹创建
	M_BOOL folder_create(M_STRING folder_path);

	//文件夹删除(包含内部文件与文件夹)
	MS_INT32 folder_delete(M_STRING folder_path);

	//文件删除
	MS_INT32 file_delete(M_STRING file_path);

	//文件夹/文件是否存在
	M_BOOL path_exist(M_STRING folder_path);

	//文件名修改，如果文件名已存在，增加后缀，直到文件名唯一。
	M_STRING file_ajust_in_folder(M_STRING file_name, M_STRING file_suffix, M_STRING folder_path);

	//文件夹下所有文件
	MS_INT32 filetitle_list_in_folder(M_STRING folder_path, std::vector<M_STRING>& vec_file );

	//文件夹下所有文件夹
	MS_INT32 folderpath_list_in_folder(M_STRING folder_path, std::vector<M_STRING>& vec_folder );

	MS_INT32 foldertitle_list_in_folder(M_STRING folder_path, std::vector<M_STRING>& vec_folder );

	//删除文件名最小的文件夹
	MS_INT32 folder_oldest_delete(M_STRING root_path);

	//文件长度
	MS_INT32 file_len(M_STRING file_path);
	//读文件
	MS_INT32 file_read(M_STRING file_path, MCS_INT32 len, MS_CHAR* dst);

	// 文件拷备
	MS_INT32 copyFile( MCS_CHAR *src_f, MCS_CHAR *dst_f , M_BOOL is_overwrite);

	MS_INT32 renameFile(MC_STRING sourceFileNameWithPath, MC_STRING targetFileNameWithPath);

	/*
	end file function
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	reverse function
	*/
	//桢转换
	M_BOOL frame_convert(M_IN MCU_CHAR* input_data, M_IN MCS_INT32 input_len, M_IN MCS_INT32 input_width, M_IN MCS_INT32 input_height, M_IN MCS_INT32 input_type,
		M_OUT MU_CHAR** pp_output, M_OUT MS_INT32* p_output_len, M_IN MCS_INT32 output_width, M_IN MCS_INT32 output_height, M_IN MCS_INT32 output_type);

	/*
	end reverse function
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	add function
	*/


	/*
	end add function
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

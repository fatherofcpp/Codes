#include "StdAfx.h"
#include "CmFunction.h"

#include "ijl/Cmijlutil.h"
#include <algorithm>

M_BOOL NSP_STP_CM::frame_convert(M_IN MCU_CHAR* input_data, M_IN MCS_INT32 input_len, M_IN MCS_INT32 input_width, M_IN MCS_INT32 input_height, M_IN MCS_INT32 input_type,
	M_OUT MU_CHAR** pp_output, M_OUT MS_INT32* p_output_len, M_IN MCS_INT32 output_width, M_IN MCS_INT32 output_height, M_IN MCS_INT32 output_type)
{
	if (!pp_output)
	{
		return false;
	}

	*pp_output = (MU_CHAR*)input_data;
	*p_output_len = input_len;
	return true;
}

M_BOOL NSP_STP_CM::DiskIsFull(M_STRING disk_path, MCS_INT32 min_space_g)
{
	USES_CONVERSION; 
	CString root_path = A2W(disk_path.c_str());
	DWORD sectors_perCluster = 0;
	DWORD bytes_perSector = 0;
	DWORD free_clusters = 0;
	DWORD total_clusters = 0;
	if (GetDiskFreeSpace(root_path, &sectors_perCluster, &bytes_perSector,
		&free_clusters, &total_clusters))
	{
		MS_INT64 bytes_perCluster = sectors_perCluster * bytes_perSector ;
		MS_INT64 free_space_g = free_clusters * bytes_perCluster / (1024*1024*1024);
		if (free_space_g < min_space_g)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

MS_INT32 NSP_STP_CM::file_write(M_STRING file_path, MCU_CHAR* file_data, MU_INT32 file_len)
{
	M_STRING folder_path = str_match_head2sig_last(file_path, "\\");
	if (folder_path.empty())
	{
		return -1;
	}
	folder_create(folder_path);

	FILE* pf = NULL;
	MS_INT32 rtn = 0;

	pf = fopen(file_path.c_str(), "wb");
	if(pf)
	{
		M_SIZE_T w_rt = fwrite(file_data, sizeof(char), file_len, pf);
		fclose(pf);		
	}
	else
	{
		return -1;
	}	

	return rtn;
}

MS_INT32 NSP_STP_CM::filetitle_list_in_folder(M_STRING folder_path, std::vector<M_STRING>& vec_file )
{
	USES_CONVERSION; 

	vec_file.clear();

	CFileFind finder;  
	M_STRING path = folder_path + "\\*.*";
	CString cs_file_name;
	BOOL is_suc = FALSE;
	BOOL b_working = finder.FindFile(A2W(path.c_str()));  
	while(b_working)
	{  
		b_working = finder.FindNextFile();  
		if (!finder.IsDots())
		{
			cs_file_name = finder.GetFileTitle();
			if(finder.IsDirectory())
			{ 
				//folder
			}  
			else
			{
				//file
				vec_file.push_back(W2A(cs_file_name));
			}  
		}
	}  
	return 0;
}

M_BOOL NSP_STP_CM::folder_create(M_STRING folder_path)
{
	USES_CONVERSION; 

	if(path_exist(folder_path))
	{
		return true;
	}

	SECURITY_ATTRIBUTES attrib;  
	attrib.bInheritHandle = FALSE;  
	attrib.lpSecurityDescriptor = NULL;  
	attrib.nLength = sizeof(attrib);  
	BOOL is_suc = FALSE;

	M_STRING cur_folder_path = folder_path;
	M_STRING tag = "\\";
	std::vector<M_STRING> vec_folder_path;
	vec_folder_path.push_back(cur_folder_path);
	while(1)
	{
		M_STRING father_foler_path = str_match_head2sig_last(cur_folder_path, tag);
		if (0 != strcmp(father_foler_path.c_str(), cur_folder_path.c_str()))
		{
			cur_folder_path = father_foler_path;
			vec_folder_path.push_back(cur_folder_path);
		}
		else
		{
			break;
		}
	}

	for (std::vector<M_STRING>::reverse_iterator iter = vec_folder_path.rbegin(); iter != vec_folder_path.rend(); iter++)
	{
		M_STRING folder_in_vec = *iter;
		if(!path_exist(folder_in_vec))
		{
			is_suc = ::CreateDirectory(A2W(folder_in_vec.c_str()), &attrib);  
		}
	}
	return (is_suc == TRUE);
}

//删除文件夹里面内容，不包含文件夹本身
static void folder_file_delete(std::string folder_path)
{
	USES_CONVERSION; 
	CFileFind finder;  
	std::string path = folder_path + "\\*.*";
	CString file_path;
	BOOL is_suc = FALSE;
	BOOL b_working = finder.FindFile(A2W(path.c_str()));  
	while(b_working)
	{  
		b_working = finder.FindNextFile();  
		if (!finder.IsDots())
		{
			file_path = finder.GetFilePath();
			if(finder.IsDirectory())
			{ 
				folder_file_delete(W2A(file_path));
				is_suc = RemoveDirectory(file_path);  
			}  
			else
			{
				is_suc = DeleteFile(file_path);  
			}  
		}
	}  
}

MS_INT32 NSP_STP_CM::folder_delete(M_STRING folder_path)
{
	USES_CONVERSION; 

	MS_INT32 ret = 0;
	if(!path_exist(folder_path))
	{
		return 0;
	}
	folder_file_delete(folder_path);
	RemoveDirectory(A2W(folder_path.c_str()));  
	return ret;
}

M_STRING NSP_STP_CM::file_ajust_in_folder(M_STRING file_name, M_STRING file_suffix, M_STRING folder_path)
{
	/*
	folder_path\\file_name.file_suffix
	*/
	USES_CONVERSION; 

	M_STRING file_path = "";
	M_STRING new_file_name = file_name;

	std::vector<M_STRING> file_set;
	NSP_STP_CM::filetitle_list_in_folder(folder_path, file_set);
	MU_INT32 add_val = file_set.size();

	while(1)
	{
		M_BOOL is_new = true;
		M_STRING real_file_name = str_match_head2sig_last(new_file_name, "_");
		new_file_name = real_file_name + "_" + val2string(add_val++);
		for (MU_INT32 i = 0; i < file_set.size(); i ++)
		{
			M_STRING file_name_in_vec = file_set[i]; 
			if (0 == strcmp(new_file_name.c_str(), file_name_in_vec.c_str()))
			{
				is_new = false;
				break;
			}
		}
		if (is_new)
		{
			break;
		}
	}

	file_path = folder_path + new_file_name + file_suffix;
	return file_path;
}

static bool SortStringUp( const std::string v1, const std::string v2)
{  
	int ret = strcmp(v1.c_str(), v2.c_str());
	return ret < 0;
}  

static bool SortStringDown( const std::string v1, const std::string v2)
{  
	int ret = strcmp(v1.c_str(), v2.c_str());
	return ret > 0;
}  

MS_INT32 NSP_STP_CM::folder_oldest_delete(M_STRING root_path)
{
	MS_INT32 ret = 0;
	std::vector<M_STRING> vec_folder_path;
	ret = NSP_STP_CM::folderpath_list_in_folder(root_path, vec_folder_path);

	MU_INT32 folder_num = vec_folder_path.size();
	if (folder_num == 0)
	{
		return 0;
	}

	std::sort(vec_folder_path.begin(), vec_folder_path.end(), SortStringUp); 

	M_STRING oldest_folder = vec_folder_path[0];
	NSP_STP_CM::folder_delete(oldest_folder);
	return 0;
}

M_BOOL NSP_STP_CM::path_exist(M_STRING folder_path)
{
	USES_CONVERSION; 

	DWORD dwAttrib = GetFileAttributes(A2W(folder_path.c_str()));
	M_BOOL is_exist = INVALID_FILE_ATTRIBUTES != dwAttrib;
	return is_exist;  
}

M_STRING NSP_STP_CM::str_match_head2sig_last( M_STRING str_input, M_STRING tag , bool include_sig)
{
	M_STRING tmp = str_input;
	MS_INT32 find_it = 0;
	MU_INT32 tail_len = 0;
	while(1)
	{
		find_it = tmp.find(tag);
		if (find_it < 0)
		{
			tail_len = tmp.length();
			break;
		}
		else
		{
			tmp = tmp.substr(find_it + tag.length());
			tail_len = 0;
		}
	}

	if (tail_len == str_input.length())
	{
		return str_input;
	}

	if (tail_len < tag.length())
	{
		//tag为字符串尾部
		tail_len = 0;
	}

	MS_INT32 output_len = str_input.length() - tail_len - tag.length();
	if (include_sig)
	{
		output_len += tag.length();
	}
	tmp = str_input.substr(0, output_len);
	return tmp;
}

MS_INT32 NSP_STP_CM::folderpath_list_in_folder(M_STRING folder_path, std::vector<M_STRING>& vec_folder )
{
	USES_CONVERSION; 

	vec_folder.clear();

	CFileFind finder;  
	M_STRING path = folder_path + "\\*.*";
	CString cs_file_name;
	BOOL is_suc = FALSE;
	BOOL b_working = finder.FindFile(A2W(path.c_str()));  
	while(b_working)
	{  
		b_working = finder.FindNextFile();  
		if (!finder.IsDots())
		{
			cs_file_name = finder.GetFilePath();
			if(finder.IsDirectory())
			{ 
				//folder
				vec_folder.push_back(W2A(cs_file_name));
			}  
			else
			{
				//file
			}  
		}
	}  
	return 0;
}

MS_INT32 NSP_STP_CM::foldertitle_list_in_folder(M_STRING folder_path, std::vector<M_STRING>& vec_folder )
{
	USES_CONVERSION; 

	vec_folder.clear();

	CFileFind finder;  
	M_STRING path = folder_path + "\\*.*";
	CString cs_file_name;
	BOOL is_suc = FALSE;
	BOOL b_working = finder.FindFile(A2W(path.c_str()));  
	while(b_working)
	{  
		b_working = finder.FindNextFile();  
		if (!finder.IsDots())
		{
			cs_file_name = finder.GetFileTitle();
			if(finder.IsDirectory())
			{ 
				//folder
				vec_folder.push_back(W2A(cs_file_name));
			}  
			else
			{
				//file
			}  
		}
	}  
	return 0;
}

MS_INT32 NSP_STP_CM::file_len(M_STRING file_path)
{
	FILE *pf = NULL;
	MS_INT32 len = 0;
	MS_INT32 rtn = 0;

	pf = fopen(file_path.c_str(), "rb");
	if(pf)
	{
		fseek(pf, 0, SEEK_END);
		len = ftell(pf);	
		fseek(pf, 0, SEEK_SET);

		fclose(pf);	
		return len;
	}
	else
	{
		return -1;
	}	
}

MS_INT32 NSP_STP_CM::file_read(M_STRING file_path, MCS_INT32 len, MS_CHAR* dst)
{
	FILE *pf = NULL;
	MS_INT32 rtn = 0;

	pf = fopen(file_path.c_str(), "rb");
	if(pf)
	{
		rtn = fread(dst, 1, len, pf);
		fclose(pf);	
		return 0;
	}
	else
	{
		return -1;
	}	
}

MS_INT32 NSP_STP_CM::gb2312_to_utf8(MCS_CHAR* gb2312, MS_INT32 gb2312_len, MS_CHAR* utf8)
{
	MS_INT32 ucs2_len;
	WCHAR *ucs2 = NULL;
	MS_INT32 utf8_len;

	if (0 != gb2312_len)
	{
		ucs2_len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL,0);
		ucs2 = new WCHAR[ucs2_len + 1];
		memset(ucs2, 0, ucs2_len + 1);

		MultiByteToWideChar(CP_ACP, 0, gb2312, -1, ucs2, ucs2_len);

		utf8_len = WideCharToMultiByte(CP_UTF8, 0, ucs2, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte (CP_UTF8, 0, ucs2, -1, utf8, utf8_len, NULL, NULL);
		delete[] ucs2;
		return utf8_len;
	}
	else
	{
		return 0;
	}
}

MS_INT32 NSP_STP_CM::utf8_to_gbk(MS_CHAR * gbk, MCS_CHAR* utf, MS_INT32 utf_len)
{
	MS_INT32 ucs2_len = 0;
	MS_INT32 gbk_len = 0;
	WCHAR *ucs2 = NULL;

	if (utf_len != 0)
	{
		ucs2_len = MultiByteToWideChar(CP_UTF8, 0, utf, -1, NULL, 0);
		ucs2 = new WCHAR[ucs2_len+1];
		MultiByteToWideChar(CP_UTF8, 0, utf, -1, ucs2, ucs2_len);
		gbk_len = WideCharToMultiByte(CP_ACP, 0, ucs2, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, ucs2, -1, gbk, gbk_len, NULL, NULL);
		delete[] ucs2;
		return gbk_len;
	}
	else
	{
		return 0;
	}
}

MS_INT32 NSP_STP_CM::get_lenth_gb_to_utf8(MCS_CHAR* gb2312, MS_INT32 gb2312_len)
{
	MS_INT32 ucs2_len = 0;
	WCHAR *ucs2 = NULL;
	MS_INT32 utf8_len = 0;

	if (0 != gb2312_len)
	{
		ucs2_len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL,0);
		ucs2 = new WCHAR[ucs2_len + 1];
		memset(ucs2, 0, ucs2_len + 1);
		MultiByteToWideChar(CP_ACP, 0, gb2312, -1, ucs2, ucs2_len);

		utf8_len = WideCharToMultiByte(CP_UTF8, 0, ucs2, -1, NULL, 0, NULL, NULL);

		delete[] ucs2;
		return utf8_len;
	}
	else
	{
		return 0;
	}
}

MS_INT32 NSP_STP_CM::SocketInit()
{
	//init net
	MS_INT32 ret = 0;
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD( 2, 2 );
	ret = WSAStartup( wVersionRequested, &wsaData );
	return ret;
}

MS_INT32 NSP_STP_CM::GetNowTime(tag_st_time* p_st_time)
{
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);

	p_st_time->year = wtm.wYear;
	p_st_time->mon = wtm.wMonth;
	p_st_time->day = wtm.wDay;
	p_st_time->hour = wtm.wHour;
	p_st_time->min = wtm.wMinute;
	p_st_time->sec = wtm.wSecond;
	p_st_time->ms = wtm.wMilliseconds;
	return (0);
}

M_STRING NSP_STP_CM::GetNowTimeStr()
{
	tag_st_time cur_time;
	NSP_STP_CM::GetNowTime(&cur_time);
	MS_CHAR time_inf[256];
	sprintf(time_inf, "%04d-%02d-%02d %02d:%02d:%02d", cur_time.year, cur_time.mon, cur_time.day, cur_time.hour, cur_time.min, cur_time.sec);
	M_STRING now_time = time_inf;
	return now_time;
}


M_VOID NSP_STP_CM::Dec2Hex(BYTE* pDst, MS_INT32 length, MS_INT32 Val)
{
	for(MS_INT32 i = 0; i < length; i ++)
	{
		*pDst = Val >> (8*(length-1-i));
		pDst++;
	}	
}

BYTE NSP_STP_CM::CalcCheckSum(BYTE * pBuf, MS_INT32 nSize)
{
	BYTE sum=0;
	for(MS_INT32 i = 0; i < nSize; i ++)
	{
		sum = sum ^*pBuf;
		pBuf++;			
	}
	return sum;
}

M_STRING NSP_STP_CM::str_match_tail2sig_last( M_STRING str_input, M_STRING tag , bool include_sig)
{
	M_STRING tmp = str_input;
	MS_INT32 find_it = 0;
	MS_INT32 tail_len = 0;
	while(1)
	{
		find_it = tmp.find(tag);
		if (find_it < 0)
		{
			tail_len = tmp.length();
			break;
		}
		else
		{
			tmp = tmp.substr(find_it + tag.length());
			tail_len = 0;
		}
	}

	if (tail_len == str_input.length())
	{
		return str_input;
	}

	if (include_sig)
	{
		tmp = tag + tmp;
	}

	return tmp;
}

MS_INT32 NSP_STP_CM::file_delete(M_STRING file_path)
{
	USES_CONVERSION; 

	CString path;
	path = A2W(file_path.c_str());
	int ret = 0;
	BOOL is_suc = DeleteFile(path);  
	if (is_suc)
	{
		ret = 0;
	}
	else
	{
		ret = -1;
	}
	return ret;
}

M_VOID NSP_STP_CM::GetTimeByStamp(M_TIME_T time_stamp, tag_st_time* p_time)
{
	struct tm * time_r;
	time_r = localtime(&time_stamp);
	p_time->year = time_r->tm_year + 1900;
	p_time->mon = time_r->tm_mon + 1;
	p_time->day = time_r->tm_mday;
	p_time->hour = time_r->tm_hour;
	p_time->min = time_r->tm_min;
	p_time->sec = time_r->tm_sec;
	p_time->ms = 0;
	return;
}

M_VOID NSP_STP_CM::GetStampByTime(M_TIME_T* time_stamp, tag_st_time t_time)
{
	M_TIME_T t_s;
	struct tm time_n;
	struct tm* time_r = &time_n;
	tag_st_time* p_time = &t_time;
	time_r->tm_year = p_time->year - 1900;
	time_r->tm_mon = p_time->mon - 1;
	time_r->tm_mday = p_time->day;
	time_r->tm_hour = p_time->hour;
	time_r->tm_min = p_time->min;
	time_r->tm_sec = p_time->sec;
	t_s = mktime(time_r);
	*time_stamp = t_s;
}

MS_CHAR* NSP_STP_CM::mem_find(MCS_CHAR* src, MCS_INT32 src_len, MCS_CHAR dst)
{
	MS_CHAR* p_find = NULL;

	if (!src || src_len <= 0)
	{
		return p_find;
	}

	for (int i = 0; i < src_len; i ++)
	{
		if (src[i] == dst)
		{
			p_find = (MS_CHAR*)&src[i];
			return p_find;
		}
	}
	return p_find;
}

MS_INT32 NSP_STP_CM::Hex2Dec(BYTE* pSrc, MS_INT32 length)
{
	MS_INT32 buff;
	MS_INT32 value = 0;

	for(MS_INT32 i=0;i<length;i++)
	{
		buff = (*pSrc)<<(8*(length-1-i));
		value += buff;
		pSrc++;
	}
	return value;
}

static time_t SystemTimeToTime_t(const SYSTEMTIME& st)
{
	tm temptm = { st.wSecond,
		st.wMinute,
		st.wHour,
		st.wDay,
		st.wMonth - 1,
		st.wYear - 1900,
		st.wDayOfWeek,
		0,
		0 };
	return mktime(&temptm);
}

M_BOOL NSP_STP_CM::set_local_time(tag_st_time s_time)
{
	MS_INT32 ret = 0;
	SYSTEMTIME		SysTime;
	SYSTEMTIME		GetTime;
	memset(&GetTime, 0x00, sizeof(GetTime));
	GetTime.wYear = s_time.year;
	GetTime.wMonth = s_time.mon;
	GetTime.wDay = s_time.day;
	GetTime.wHour = s_time.hour;
	GetTime.wMinute = s_time.min;
	GetTime.wSecond = s_time.sec;

	GetLocalTime(&SysTime);

	M_TIME_T sys_time = SystemTimeToTime_t(SysTime);
	M_TIME_T get_time = SystemTimeToTime_t(GetTime);

	if (abs(sys_time - get_time) < 5)	 // 时间相差5秒，不做校时
	{
		return true;
	}


	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 
	// Get a token for this process. 
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		return FALSE; 

	// Get the LUID for the shutdown privilege. 
	LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &tkp.Privileges[0].Luid); 

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get the shutdown privilege for this process. 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 

	if(SetLocalTime(&GetTime))
	{
		return false; 
	}
	return true;
}

BOOL NSP_STP_CM::MySystemShutdown()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	// Get a token for this process.
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return( FALSE );
	// Get the LUID for the shutdown privilege.
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
		&tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;  // one privilege to set   
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process.
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;
	// Shut down the system and force all applications to close.
	if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,
		SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
		SHTDN_REASON_MINOR_UPGRADE |
		SHTDN_REASON_FLAG_PLANNED))
		return FALSE;
	return TRUE;
}

MS_INT32 NSP_STP_CM::copyFile( MCS_CHAR *src_f, MCS_CHAR *dst_f , M_BOOL is_overwrite)
{
	MCS_INT32 c_buf_size = 1024 * 10;
	FILE* fp_r = NULL;
	FILE* fp_w = NULL;
	MS_INT32 len_r = 0;
	MS_INT32 len_w = 0;

	MS_CHAR buffer[c_buf_size];
	memset(buffer, 0, c_buf_size);

	if ((fp_r = fopen(src_f, "rb")) == NULL)
	{
		return -1;
	}

	MS_CHAR write_mode[32];
	if (is_overwrite)
	{
		strcpy(write_mode, "wb");
	}
	else
	{
		strcpy(write_mode, "ab+");
	}

	{
		M_STRING targetFileNameWithPath = dst_f;
		M_STRING folder_path = NSP_STP_CM::str_match_head2sig_last(targetFileNameWithPath, "\\");
		if (folder_path.empty())
		{
			return -1;
		}
		NSP_STP_CM::folder_create(folder_path);
	}

	if ((fp_w = fopen(dst_f, write_mode)) == NULL)
	{
		fclose(fp_r);
		return -1;
	}

	while ((len_r = fread(buffer, 1, c_buf_size, fp_r)) > 0)
	{
		if ((len_w = fwrite(buffer, 1, len_r, fp_w)) != len_r)
		{
			fclose(fp_r);
			fclose(fp_w);
			return -1;
		}
		memset(buffer, 0, c_buf_size);
	}

	fclose(fp_r);
	fclose(fp_w);
	return 0;
}

M_BOOL NSP_STP_CM::MyGetDiskFreeSpaceEx(LPCSTR pszDrive, MU_INT32& total_g, MU_INT32& used_g)
{
	MCU_INT64 i64Gb = 1024*1024*1024;

	MU_INT64 i64FreeBytesToCaller;
	MU_INT64 i64TotalBytes;
	MU_INT64 i64FreeBytes;

	BOOL fResult = GetDiskFreeSpaceExA(pszDrive,
		(PULARGE_INTEGER)&i64FreeBytesToCaller,
		(PULARGE_INTEGER)&i64TotalBytes,
		(PULARGE_INTEGER)&i64FreeBytes);

	total_g = (MU_INT32)(i64TotalBytes /i64Gb);
	used_g = (MU_INT32)((i64TotalBytes - i64FreeBytes) /i64Gb);

	return (fResult == TRUE);
}

MS_INT32 NSP_STP_CM::SaveBmp(BYTE *buf, MS_INT32 width, MS_INT32 height, MS_INT32 iDataLen, BITMAPINFO * pBitmapInfo, M_STRING lpFileName)
{
	BITMAPFILEHEADER fileHeader;               /* bmp file header */
	BITMAPINFOHEADER infoHeader;             /* bmp information header */
	RGBQUAD rgb[256];                                   /* palette */

	MS_INT32 i;

	fileHeader.bfType = 0x4d42;
	fileHeader.bfOffBits = sizeof(fileHeader) + sizeof(infoHeader) + sizeof(rgb);
	fileHeader.bfSize = fileHeader.bfOffBits + iDataLen;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;

	infoHeader.biSize = sizeof(infoHeader);
	infoHeader.biWidth = width;
	infoHeader.biHeight = height;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 8;
	infoHeader.biCompression = BI_RGB;
	infoHeader.biSizeImage = iDataLen;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant = 0;

	for (i = 0; i < 256; i++) {
		rgb[i].rgbRed = i;
		rgb[i].rgbGreen = i;
		rgb[i].rgbBlue = i;
		rgb[i].rgbReserved = 0;
	}

	MCS_INT32 new_len = fileHeader.bfSize;
	MS_CHAR* n_bff = new char[new_len];
	MS_INT32 off_set = 0;
	memcpy(n_bff + off_set, &fileHeader, sizeof(fileHeader)); /* 写入位图文件头 */
	off_set += sizeof(fileHeader);
	memcpy(n_bff + off_set, &infoHeader, sizeof(infoHeader));  /* 写入位图信息头 */
	off_set += sizeof(infoHeader);
	memcpy(n_bff + off_set, rgb, sizeof(rgb));/* 写入调色板 */
	off_set += sizeof(rgb);
	memcpy(n_bff + off_set, buf, iDataLen);/* 写入图像数据 */
	off_set += iDataLen;

	NSP_STP_CM::file_write(lpFileName, reinterpret_cast<MCU_CHAR*>(n_bff), off_set);

	delete[] n_bff;

	return 0;
}


M_STRING NSP_STP_CM::str_sig_double(MC_STRING str_input, MCS_CHAR sig)
{
	MS_CHAR in_put[MAX_PATH];
	memset(in_put, 0, sizeof(in_put));
	MS_CHAR out_put[MAX_PATH * 2];
	memset(out_put, 0, sizeof(out_put));

	strcpy(in_put, str_input.c_str());
	MCS_INT32 input_len = str_input.length();

	MS_INT32 out_ind = 0;

	for (MS_INT32 i = 0; i < input_len; i ++)
	{
		out_put[out_ind] = in_put[i];
		out_ind ++;
		if (in_put[i] == sig)
		{
			out_put[out_ind] = in_put[i];
			out_ind ++;
		}
	}

	M_STRING str_output = out_put;
	return str_output;
}

MS_INT32 NSP_STP_CM::renameFile(MC_STRING sourceFileNameWithPath, MC_STRING targetFileNameWithPath)
{
	M_STRING folder_path = NSP_STP_CM::str_match_head2sig_last(targetFileNameWithPath, "\\");
	if (folder_path.empty())
	{
		return -1;
	}
	NSP_STP_CM::folder_create(folder_path);

	remove(targetFileNameWithPath.c_str());

	int ret = rename(sourceFileNameWithPath.c_str(), targetFileNameWithPath.c_str());

	return ret;
}


// IJL == > BGR - YCBCR  此函数效率较低 仅参考不使用
void NSP_STP_CM::yuv422_to_rgb24(unsigned char *yuv422,unsigned char *rgb24, int width, int height)
{  
	int x,y;
	unsigned char *yuv444;
	yuv444 = (unsigned char *) malloc(sizeof(unsigned char) * width * height * 3);
	for(x = 0,y = 0;x < width*height*2,y < width*height*3;x+=4,y+=6) 
	{   
		yuv444[y] = yuv422[x];
		yuv444[y+1] = yuv422[x+1];
		yuv444[y+2] = yuv422[x+3];
		yuv444[y+3] = yuv422[x+2];
		yuv444[y+4] = yuv422[x+1];
		yuv444[y+5] = yuv422[x+3];
	}  
	for(x = 0;x < width*height*3;x+=3) 
	{   
		rgb24[x+2] = (unsigned char)(yuv444[x] + 1.402*(yuv444[x+2] - 128));
		rgb24[x+1] = (unsigned char)(yuv444[x]-0.34414*(yuv444[x+1]-128)-0.71414*(yuv444[x+2]-128));
		rgb24[x] = (unsigned char)(yuv444[x] + 1.772*(yuv444[x+1] - 128));
		if (rgb24[x]>255)rgb24[x]=255;
		if (rgb24[x]<0)rgb24[x]=0;
		if (rgb24[x+1]>255)rgb24[x+1]=255;
		if (rgb24[x+1]<0)rgb24[x+1]=0;
		if (rgb24[x+2]>255)rgb24[x+2]=255;
		if (rgb24[x+2]<0)rgb24[x+2]=0;
	}  
	free(yuv444);
}  

// yuv 转换为 rgb
int NSP_STP_CM::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
	unsigned int pixel32 = 0;
	unsigned char *pixel = (unsigned char *)&pixel32;
	int r, g, b;
	r = (int)(y + (1.370705 * (v-128)));
	g = (int)(y - (0.698001 * (v-128)) - (0.337633 * (u-128)));
	b = (int)(y + (1.732446 * (u-128)));
	// 		r = y + (1.4075 * (v-128));
	// 		g = y - 0.3455 * (u-128) - 0.7169 * (v-128);
	// 		b = y + (1.779 * (u-128));
	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;
	pixel[0] = r ;
	pixel[1] = g ;
	pixel[2] = b ;
	return pixel32;
}

//16位均值 转换为 24位均值 rgb     IJL == > RGB - YCBCR
int NSP_STP_CM::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
	unsigned int in, out = 0;
	unsigned int pixel_16;
	unsigned char pixel_24[3];
	unsigned int pixel32;
	int y0, u, y1, v;

	for(in = 0; in < width * height * 2; in += 4)
	{
		pixel_16 =
			yuv[in + 3] << 24 |
			yuv[in + 2] << 16 |
			yuv[in + 1] <<  8 |
			yuv[in + 0];
		y0 = (pixel_16 & 0x000000ff);
		u  = (pixel_16 & 0x0000ff00) >>  8;
		y1 = (pixel_16 & 0x00ff0000) >> 16;
		v  = (pixel_16 & 0xff000000) >> 24;
		pixel32 = NSP_STP_CM::convert_yuv_to_rgb_pixel(y0, u, v);
		pixel_24[0] = (pixel32 & 0x000000ff);
		pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
		pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
		rgb[out++] = pixel_24[0];
		rgb[out++] = pixel_24[1];
		rgb[out++] = pixel_24[2];
		pixel32 = NSP_STP_CM::convert_yuv_to_rgb_pixel(y1, u, v);
		pixel_24[0] = (pixel32 & 0x000000ff);
		pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
		pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
		rgb[out++] = pixel_24[0];
		rgb[out++] = pixel_24[1];
		rgb[out++] = pixel_24[2];
	}
	return 0;

}

MS_INT32 NSP_STP_CM::data_compress(MCS_CHAR* p_input_data, MCS_INT32 c_input_data_len, MS_INT32 c_max_output_num, MS_CHAR* output_data, MS_INT32& output_data_num)
{
	MS_INT32 ret = 0;

	if (!p_input_data || c_input_data_len <= 0)
	{
		return -1;
	}

	if (!output_data || c_max_output_num <= 0)
	{
		return -1;
	}

	MS_INT32 rate_in_by_out = c_input_data_len / c_max_output_num;
	if (rate_in_by_out == 0)
	{
		//c_input_data_len < c_max_output_num; 128 / 1024;
		output_data_num = c_input_data_len;
		rate_in_by_out = 1;
	}
	else
	{
		//c_input_data_len >= c_max_output_num;128 / 32;
		output_data_num = c_max_output_num;
	}

	MS_INT32 input_ind = 0;
	for (int output_ind = 0; output_ind < output_data_num; output_ind ++)
	{
		output_data[output_ind] = p_input_data[input_ind];
		input_ind += rate_in_by_out;
	}

	return ret;
}

M_VOID NSP_STP_CM::TimeAjustByMs(tag_st_time time_src, tag_st_time* p_time_dst, MS_INT32 off_s)
{
	M_TIME_T time_t_src;
	NSP_STP_CM::GetStampByTime(&time_t_src, time_src);

	M_TIME_T time_t_now = time_t_src + off_s;

	NSP_STP_CM::GetTimeByStamp(time_t_now, p_time_dst);
}

MS_INT32 NSP_STP_CM::TimeCompare(tag_st_time time_src, tag_st_time time_obj)
{
	M_TIME_T time_t_src;
	NSP_STP_CM::GetStampByTime(&time_t_src, time_src);

	M_TIME_T time_t_obj;
	NSP_STP_CM::GetStampByTime(&time_t_obj, time_obj);

	MS_INT32 res_cmp = (int)(time_t_src - time_t_obj);
	return res_cmp;
}

MS_INT32 NSP_STP_CM::StringSplit( M_STRING src, std::vector<M_STRING>& vec_data, MS_CHAR sig )
{
	//"a,b,...,n"
	MS_INT32 rtn = 0;
	MS_INT32 pos = 0;
	while(1)
	{
		pos = src.find(sig);
		if (pos > 0)
		{
			vec_data.push_back(src.substr(0,pos));
			src=src.substr(pos + 1, src.length() - pos - 1);
		}
		else
		{
			vec_data.push_back(src);
			break;
		}
	}
	return vec_data.size();
}

MU_INT16 NSP_STP_CM::CalcCrc( MU_INT8 * pucFrame, MU_INT16 usLen )
{
	const unsigned char s_aucCRCHi[] = {
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40
	};

	const unsigned char s_aucCRCLo[] = {
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
		0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
		0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
		0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
		0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
		0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
		0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
		0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
		0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
		0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
		0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
		0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
		0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
		0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
		0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
		0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
		0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
		0x41, 0x81, 0x80, 0x40
	};

	MU_INT8   ucCRCHi = 0xFF;
	MU_INT8   ucCRCLo = 0xFF;
	MU_INT32  iIndex = 0;

	while( usLen-- )
	{
		iIndex = ucCRCLo ^ *( pucFrame++ );
		ucCRCLo = ( MU_INT8)( ucCRCHi ^ s_aucCRCHi[iIndex] );
		ucCRCHi = s_aucCRCLo[iIndex];
	}
	return ( MU_INT16 )( ucCRCHi << 8 | ucCRCLo );
}

MS_CHAR * NSP_STP_CM::strUpper( MS_CHAR * str )
{
	for (MU_INT32 i = 0; i < strlen(str); ++i)
	{
		str[i] = toupper(str[i]);
	}
	return str;
}

MS_CHAR * NSP_STP_CM::strLower( MS_CHAR * str )
{
	for (MU_INT32 i = 0; i < strlen(str); ++i)
	{
		str[i] = tolower(str[i]);
	}
	return str;
}

MS_INT32 NSP_STP_CM::f_base64_encode(MS_CHAR *const dst, const void *src, MU_INT32 src_len)
{
	MS_CHAR encode_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	const MU_CHAR  *s;
	MU_CHAR        *d;
	MU_INT32        len;

	len = src_len;
	s = (const MU_CHAR *)src;
	d = (MU_CHAR *)dst;

	while (len > 2) {
		*d++ = encode_table[(s[0] >> 2) & 0x3F];
		*d++ = encode_table[((s[0] & 3) << 4) | (s[1] >> 4)];
		*d++ = encode_table[((s[1] & 0x0F) << 2) | (s[2] >> 6)];
		*d++ = encode_table[s[2] & 0x3F];

		s += 3;
		len -= 3;
	}

	if (len > 0)
	{
		*d++ = encode_table[(s[0] >> 2) & 0x3F];
		if (len == 1) {
			*d++ = encode_table[(s[0] & 3) << 4];
			*d++ = '=';

		} else {
			*d++ = encode_table[((s[0] & 3) << 4) | (s[1] >> 4)];
			*d++ = encode_table[(s[1] & 0x0F) << 2];
		}
		*d++ = '=';
	}
	len = d - (MU_CHAR *)dst;

	return (len);
}

MS_INT32 NSP_STP_CM::f_base64_decode(void *const dst, MS_CHAR *const src)
{
	MS_CHAR decode_table[] = {
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 62, 77, 77, 77, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 77, 77, 77, 77, 77, 77,
		77,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 77, 77, 77, 77, 77,
		77, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77
	};

	MU_INT32    slen;
	MU_INT32    len;
	MU_CHAR    *d;
	MU_CHAR    *s;

	slen = strlen(src);
	for (len = 0; len < slen; len++)
	{
		if (src[len] == '=')
		{
			break;
		}
		if (decode_table[(MU_INT32)src[len]] == 77)
		{
			return (-1);
		}
	}
	if (len % 4 == 1)
	{
		return (-1);
	}
	s = (MU_CHAR *)src;
	d = (MU_CHAR *)dst;
	while (len > 3)
	{
		*d++ = (MU_CHAR) (decode_table[s[0]] << 2 | decode_table[s[1]] >> 4);
		*d++ = (MU_CHAR) (decode_table[s[1]] << 4 | decode_table[s[2]] >> 2);
		*d++ = (MU_CHAR) (decode_table[s[2]] << 6 | decode_table[s[3]]);
		s += 4;
		len -= 4;
	}
	if (len > 1)
	{
		*d++ = (MU_CHAR) (decode_table[s[0]] << 2 | decode_table[s[1]] >> 4);
	}
	if (len > 2)
	{
		*d++ = (MU_CHAR) (decode_table[s[1]] << 4 | decode_table[s[2]] >> 2);
	}
	len = d - (MU_CHAR *)dst;
	return (len);
}

M_STRING NSP_STP_CM::str_match_head2sig_first( M_STRING str_input, M_STRING tag , bool include_sig)
{
	M_STRING tmp = str_input;
	MS_INT32 find_it = 0;
	MU_INT32 tail_len = 0;
	while(1)
	{
		find_it = tmp.find(tag);
		if (find_it < 0)
		{
			tail_len = tmp.length();
			break;
		}
		else
		{
			tmp = tmp.substr(0, find_it);
			tail_len = 0;
			break;
		}
	}

	if (tail_len == str_input.length())
	{
		return str_input;
	}

	if (include_sig)
	{
		tmp = tmp + tag;
	}
	return tmp;
}


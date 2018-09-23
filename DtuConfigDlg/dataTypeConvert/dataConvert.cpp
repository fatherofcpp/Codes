#include "stdafx.h"
#include "dataConvert.h"
#include "CommonFunction.h"

int gHexType = 10;
bool gEncrypt = false;
bool gbEncryptLocal = true;
int gb2312_to_utf8(const char* gb2312, int gb2312_len, char* utf8)
{
	int ucs2_len;
	WCHAR *ucs2 = NULL;
	int utf8_len;

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

int get_lenth_gb_to_utf8( const char* gb2312, int gb2312_len )
{
	int ucs2_len = 0;
	WCHAR *ucs2 = NULL;
	int utf8_len = 0;

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

int utf8_to_gbk(char * gbk,const char* utf, int utf_len)
{
	int ucs2_len = 0;
	int gbk_len = 0;
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

void util_crypt_encrypt(unsigned char *buf, int len, unsigned char token)
{
	if (!gEncrypt)
	{
		return;
	}

	int i;

	for(i=0; i<len; i++, buf++)
	{
		*buf = (0xff - *buf) ^ token;
		token = *buf;
	}
}

void util_crypt_decrypt(unsigned char *buf, int len, unsigned char token)
{
	if (!gEncrypt)
	{
		return;
	}

	int i;
	unsigned char org;

	for(i=0; i<len; i++, buf++)
	{
		org = *buf;
		*buf = 0xff - (org ^ token);
		token = org;
	}
}

MU_INT8 get_element_sum( char* data,int len )
{
	MU_INT8 add = 0;
	for (int i = 0; i < len; i++)
	{
		add += (MU_INT8)(data[i] & 0xff);
	}
	return add;
}

int SplitToString(string src, vector<string>& vecData, char sig)
{
	//"a,b,...,n"
	int rtn = 0;
	int pos = 0;
	while(1)
	{
		pos = src.find(sig);
		if (pos > 0)
		{
			vecData.push_back(src.substr(0,pos));
			src=src.substr(pos + 1, src.length() - pos - 1);
		}
		else
		{
			vecData.push_back(src);
			break;
		}
	}
	return vecData.size();
}

void encry_local_data( unsigned char *buf, int len )
{
	if (!gbEncryptLocal)
	{
		return;
	}

	int i;
	for(i=0; i<len; i++, buf++)
	{
		*buf = (0xff - *buf);
	}
}

void decry_local_data( unsigned char *buf, int len )
{
	if (!gbEncryptLocal)
	{
		return;
	}

	int i;
	for(i=0; i<len; i++, buf++)
	{
		*buf = (0xff - *buf);
	}
}

int ExtractWayData(u_w_measure& w_measure,e_dev_type dev_type,char* data)
{
#if 1
	return 0;
#else
	/*
	int cnt = 0;
	bool ret = 0;
	int ind = 0;

	int len = strlen(data);
	switch(dev_type)
	{
	case AM_3PHASE:
		cnt = sscanf(data, 
			"%12lld"       //meter_time,     
			"%4x"          //meter_state,    
			"%4x"          //threshold_state,    
			"%4x"          //volt_change,    
			"%4x"          //current_change, 
			"%4x"          //frequency,      
			"%4x"          //phase_volt,         
			"%4x"          //phase_volt         
			"%4x"          //phase_volt     
			"%4x"          //line_volt,        
			"%4x"          //line_volt,        
			"%4x"          //line_volt,        
			"%4x"          //phrase_current,      
			"%4x"          //phrase_current      
			"%4x"          //phrase_current      
			"%4x"          //phrase_active,             
			"%4x"          //phrase_active,           
			"%4x"          //phrase_active,           
			"%4x"          //phrase_active,           
			"%4x"          //phrase_reactive,           
			"%4x"          //phrase_reactive,         
			"%4x"          //phrase_reactive,         
			"%4x"          //phrase_reactive,         
			"%4x"          //phrase_apparent,            
			"%4x"          //phrase_apparent,          
			"%4x"          //phrase_apparent,          
			"%4x"          //phrase_apparent,          
			"%4x"          //factor,   
			"%4x"          //factor, 
			"%4x"          //factor, 
			"%4x"          //factor, 
			"%4x"          //current_zero,   
			"%8x"          //combine_active_energy,      
			"%8x"          //combine_active_energy,      
			"%8x"          //combine_active_energy,      
			"%8x"          //combine_active_energy,      
			"%8x"          //combine_active_energy,      
			"%8x" 		   //positive_active_energy	
			"%8x" 		   //positive_active_energy	
			"%8x" 		   //positive_active_energy	
			"%8x" 		   //positive_active_energy	
			"%8x" 		   //positive_active_energy	 
			"%8x" 		   //reverse_active_energy	 
			"%8x" 		   //reverse_active_energy	 
			"%8x" 		   //reverse_active_energy	 
			"%8x" 		   //reverse_active_energy	 
			"%8x" 		   //reverse_active_energy	 
			"%8x" 		   //combine_reactive_energy[0]	 
			"%8x" 		   //combine_reactive_energy[0]	 
			"%8x" 		   //combine_reactive_energy[0]	 
			"%8x" 		   //combine_reactive_energy[0]	 
			"%8x" 		   //combine_reactive_energy[0]	 
			"%8x" 		   //combine_reactive_energy[1]	 
			"%8x" 		   //combine_reactive_energy[1]	 
			"%8x" 		   //combine_reactive_energy[1]	 
			"%8x" 		   //combine_reactive_energy[1]	 
			"%8x", 		   //combine_reactive_energy[1]	 
			&w_measure.am_3phase.meter_time,
			&w_measure.am_3phase.meter_state,
			&w_measure.am_3phase.threshold_state,
			&w_measure.am_3phase.volt_change,
			&w_measure.am_3phase.current_change,
			&w_measure.am_3phase.frequency,
			&w_measure.am_3phase.phase_volt[0],
			&w_measure.am_3phase.phase_volt[1],
			&w_measure.am_3phase.phase_volt[2],
			&w_measure.am_3phase.line_volt[0],
			&w_measure.am_3phase.line_volt[1],
			&w_measure.am_3phase.line_volt[2],
			&w_measure.am_3phase.phrase_current[0],
			&w_measure.am_3phase.phrase_current[1],
			&w_measure.am_3phase.phrase_current[2],
			&w_measure.am_3phase.phrase_active[0],
			&w_measure.am_3phase.phrase_active[1],
			&w_measure.am_3phase.phrase_active[2],
			&w_measure.am_3phase.phrase_active[3],
			&w_measure.am_3phase.phrase_reactive[0],
			&w_measure.am_3phase.phrase_reactive[1],
			&w_measure.am_3phase.phrase_reactive[2],
			&w_measure.am_3phase.phrase_reactive[3],
			&w_measure.am_3phase.phrase_apparent[0],
			&w_measure.am_3phase.phrase_apparent[1],
			&w_measure.am_3phase.phrase_apparent[2],
			&w_measure.am_3phase.phrase_apparent[3],
			&w_measure.am_3phase.factor[0],
			&w_measure.am_3phase.factor[1],
			&w_measure.am_3phase.factor[2],
			&w_measure.am_3phase.factor[3],
			&w_measure.am_3phase.n_current,
			&w_measure.am_3phase.combine_active_energy[0],
			&w_measure.am_3phase.combine_active_energy[1],
			&w_measure.am_3phase.combine_active_energy[2],
			&w_measure.am_3phase.combine_active_energy[3],
			&w_measure.am_3phase.combine_active_energy[4],
			&w_measure.am_3phase.positive_active_energy[0],
			&w_measure.am_3phase.positive_active_energy[1],
			&w_measure.am_3phase.positive_active_energy[2],
			&w_measure.am_3phase.positive_active_energy[3],
			&w_measure.am_3phase.positive_active_energy[4],
			&w_measure.am_3phase.reverse_active_energy[0],
			&w_measure.am_3phase.reverse_active_energy[1],
			&w_measure.am_3phase.reverse_active_energy[2],
			&w_measure.am_3phase.reverse_active_energy[3],
			&w_measure.am_3phase.reverse_active_energy[4],
			&w_measure.am_3phase.combine_reactive_energy[0][0],
			&w_measure.am_3phase.combine_reactive_energy[0][1],
			&w_measure.am_3phase.combine_reactive_energy[0][2],
			&w_measure.am_3phase.combine_reactive_energy[0][3],
			&w_measure.am_3phase.combine_reactive_energy[0][4],
			&w_measure.am_3phase.combine_reactive_energy[1][0],
			&w_measure.am_3phase.combine_reactive_energy[1][1],
			&w_measure.am_3phase.combine_reactive_energy[1][2],
			&w_measure.am_3phase.combine_reactive_energy[1][3],
			&w_measure.am_3phase.combine_reactive_energy[1][4]
		);

		if(AM_THREE_PHASE_NUM == cnt && len == (12 + 4 * 31 + 8 * 25))
		{
			ret = true;
		}

		break;

	case AM_1PHASE:
		cnt = sscanf(data, 
			"%12lld"       //meter_time,     
			"%4x"          //meter_state,    
			"%4x"          //threshold_state,    
			"%4x"          //volt_change,    
			"%4x"          //current_change, 
			"%4x"          //frequency,      
			"%4x"          //volt,           
			"%4x"          //current,        
			"%4x"          //active,             
			"%4x"          //reactive,           
			"%4x"          //apparent,            
			"%4x"          //factor,   
			"%8x"          //combine_active_energy,      
			"%8x"          //combine_active_energy,      
			"%8x"          //combine_active_energy,      
			"%8x"          //combine_active_energy,      
			"%8x"          //combine_active_energy,      
			"%8x"          //positive_active_energy,            
			"%8x"          //positive_active_energy,            
			"%8x"          //positive_active_energy,            
			"%8x"          //positive_active_energy,            
			"%8x"          //positive_active_energy,            
			"%8x"          //reverse_active_energy,        
			"%8x"          //reverse_active_energy,        
			"%8x"          //reverse_active_energy,        
			"%8x"          //reverse_active_energy,        
			"%8x"          //reverse_active_energy,        
			"%8x"          //combine_reactive_energy[0],          
			"%8x"          //combine_reactive_energy[0],          
			"%8x"          //combine_reactive_energy[0],          
			"%8x"          //combine_reactive_energy[0],          
			"%8x"          //combine_reactive_energy[0],          
			"%8x"          //combine_reactive_energy[1],          
			"%8x"          //combine_reactive_energy[1],          
			"%8x"          //combine_reactive_energy[1],          
			"%8x"          //combine_reactive_energy[1],          
			"%8x",         //combine_reactive_energy[1],                          
			&w_measure.am_1phase.meter_time,
			&w_measure.am_1phase.meter_state,
			&w_measure.am_1phase.threshold_state,
			&w_measure.am_1phase.volt_change,
			&w_measure.am_1phase.current_change,
			&w_measure.am_1phase.frequency,
			&w_measure.am_1phase.volt,
			&w_measure.am_1phase.current,
			&w_measure.am_1phase.active,
			&w_measure.am_1phase.reactive,
			&w_measure.am_1phase.apparent,
			&w_measure.am_1phase.factor,
			&w_measure.am_1phase.combine_active_energy[0],
			&w_measure.am_1phase.combine_active_energy[1],
			&w_measure.am_1phase.combine_active_energy[2],
			&w_measure.am_1phase.combine_active_energy[3],
			&w_measure.am_1phase.combine_active_energy[4],
			&w_measure.am_1phase.positive_active_energy[0],
			&w_measure.am_1phase.positive_active_energy[1],
			&w_measure.am_1phase.positive_active_energy[2],
			&w_measure.am_1phase.positive_active_energy[3],
			&w_measure.am_1phase.positive_active_energy[4],
			&w_measure.am_1phase.reverse_active_energy[0],
			&w_measure.am_1phase.reverse_active_energy[1],
			&w_measure.am_1phase.reverse_active_energy[2],
			&w_measure.am_1phase.reverse_active_energy[3],
			&w_measure.am_1phase.reverse_active_energy[4],
			&w_measure.am_1phase.combine_reactive_energy[0][0],
			&w_measure.am_1phase.combine_reactive_energy[0][1],
			&w_measure.am_1phase.combine_reactive_energy[0][2],
			&w_measure.am_1phase.combine_reactive_energy[0][3],
			&w_measure.am_1phase.combine_reactive_energy[0][4],
			&w_measure.am_1phase.combine_reactive_energy[1][0],
			&w_measure.am_1phase.combine_reactive_energy[1][1],
			&w_measure.am_1phase.combine_reactive_energy[1][2],
			&w_measure.am_1phase.combine_reactive_energy[1][3],
			&w_measure.am_1phase.combine_reactive_energy[1][4]
		);

		if(AM_ONE_PHASE_NUM == cnt && len == (12 + 4*11 + 8*25))
		{
			ret = true;
		}

		break;

	case AM_DC:
		cnt = sscanf(data, 
			"%12lld"       //meter_time,     	
			"%4x"          //meter_state,    	
			"%4x"          //volt_change,    	
			"%4x"          //current_change, 	
			"%4x"          //volt,           	
			"%4x"          //current,        	
			"%4x"          //active,             	
			"%8x",          //energy,      	                    	
			&w_measure.am_dc.meter_time,      
			&w_measure.am_dc.meter_state,     
			&w_measure.am_dc.volt_change,     
			&w_measure.am_dc.current_change,  
			&w_measure.am_dc.volt,            
			&w_measure.am_dc.current,         
			&w_measure.am_dc.active,              
			&w_measure.am_dc.energy);        

		if(AM_DC_NUM == cnt && len == (12 + 4*6 + 8*1))
		{
			ret = true;
		}

		break;

	case AM_HARM:
		cnt = sscanf(data, 
			"%12lld"       //meter_time,                             
			"%4x"          //meter_state,                      
			"%4x"          //threshold_state,                      
			"%4x"          //volt_change,                      
			"%4x"          //current_change,                   
			"%4x"          //frequency,                        
			"%4x"          //phase_volt,                           
			"%4x"          //phase_volt,                           
			"%4x"          //phase_volt,                           
			"%4x"          //line_volt,                          
			"%4x"          //line_volt,                          
			"%4x"          //line_volt,                          
			"%4x"          //phrase_current,                        
			"%4x"          //phrase_current,                        
			"%4x"          //phrase_current,                        
			"%4x"          //phrase_active,                               
			"%4x"          //phrase_active,                             
			"%4x"          //phrase_active,                             
			"%4x"          //phrase_active,                             
			"%4x"          //phrase_reactive,                             
			"%4x"          //phrase_reactive,                           
			"%4x"          //phrase_reactive,                           
			"%4x"          //phrase_reactive,                           
			"%4x"          //phrase_apparent,                              
			"%4x"          //phrase_apparent,                            
			"%4x"          //phrase_apparent,                            
			"%4x"          //phrase_apparent,                            
			"%4x"          //factor,                     
			"%4x"          //factor,                   
			"%4x"          //factor,                   
			"%4x"          //factor,                   
			"%4x"          //n_current,                     
			"%8x"          //combine_active_energy,                        
			"%8x"          //combine_active_energy,                              
			"%8x"          //combine_active_energy,                          
			"%8x"          //combine_active_energy,                            
			"%8x"          //combine_active_energy,                        
			"%8x"          //positive_active_energy,                        
			"%8x"          //positive_active_energy,                        
			"%8x"          //positive_active_energy,                        
			"%8x"          //positive_active_energy,                        
			"%8x"          //positive_active_energy,                        
			"%8x"          //combine_reactive_energy,                        
			"%8x"          //combine_reactive_energy,                        
			"%8x"          //combine_reactive_energy,                        
			"%8x"          //combine_reactive_energy,                        
			"%8x"          //combine_reactive_energy,                        
			"%4x"          //phrase_original_volt               
			"%4x"          //phrase_original_volt               
			"%4x"          //phrase_original_volt               
			"%4x"          //,phrase_original_current                      
			"%4x"          //,phrase_original_current                      
			"%4x"          //,phrase_original_current                      
			"%4x"          //,phrase_original_active                      
			"%4x"          //,phrase_original_active                      
			"%4x"          //,phrase_original_active                      
			"%4x"          //,phrase_original_active                      
			"%4x"          //,phrase_harm_volt                   
			"%4x"          //,phrase_harm_volt                   
			"%4x"          //,phrase_harm_volt                   
			"%4x"          //,phrase_harm_current                   
			"%4x"          //,phrase_harm_current                   
			"%4x"          //,phrase_harm_current                   
			"%4x"          //phrase_harm_active,                   
			"%4x"          //phrase_harm_active,                   
			"%4x"          //phrase_harm_active,                   
			"%4x"          //phrase_harm_active,                   
			"%4x"          //total_distor,                          
			"%4x"          //total_distor,                          
			"%4x"          //total_distor,                          
			"%4x"          //total_distor,                          
			"%4x"          //total_distor,                          
			"%4x"          //total_distor,                          
			"%4x"          //total_distor,                          
			"%4x"          //total_distor,                          
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u[0],                        
			"%4x"          //harm_u_parity[0],                        
			"%4x"          //harm_u_parity[0],                        
			"%4x"          //harm_u_ridge[0],                                              
			"%4x"          //harm_u_tele[0],      
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u[1],                        
			"%4x"          //harm_u_parity[1],                        
			"%4x"          //harm_u_parity[1],                        
			"%4x"          //harm_u_ridge[1],                                              
			"%4x"          //harm_u_tele[1],  
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u[2],                        
			"%4x"          //harm_u_parity[2],                        
			"%4x"          //harm_u_parity[2],                        
			"%4x"          //harm_u_ridge[2],                                              
			"%4x"          //harm_u_tele[2],  
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia[0],                        
			"%4x"          //harm_ia_parity[0],                        
			"%4x"          //harm_ia_parity[0],                        
			"%4x"          //harm_ia_K[0],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia[1],                        
			"%4x"          //harm_ia_parity[1],                        
			"%4x"          //harm_ia_parity[1],                        
			"%4x"          //harm_ia_K[1],  
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia[2],                        
			"%4x"          //harm_ia_parity[2],                        
			"%4x"          //harm_ia_parity[2],                        
			"%4x",          //harm_ia_K[2],  
			&w_measure.am_harm.meter_time,                                                   
			&w_measure.am_harm.meter_state,                                               
			&w_measure.am_harm.threshold_state,                                               
			&w_measure.am_harm.volt_change,                                               
			&w_measure.am_harm.current_change,                                            
			&w_measure.am_harm.frequency,                                                 
			&w_measure.am_harm.phase_volt[0],                                                    
			&w_measure.am_harm.phase_volt[1],                                                    
			&w_measure.am_harm.phase_volt[2],                                                    
			&w_measure.am_harm.line_volt[0],                                                   
			&w_measure.am_harm.line_volt[1],                                                   
			&w_measure.am_harm.line_volt[2],                                                   
			&w_measure.am_harm.phrase_current[0],                                                 
			&w_measure.am_harm.phrase_current[1],                                                 
			&w_measure.am_harm.phrase_current[2],                                                 
			&w_measure.am_harm.phrase_active[0],                                                        
			&w_measure.am_harm.phrase_active[1],                                                      
			&w_measure.am_harm.phrase_active[2],                                                      
			&w_measure.am_harm.phrase_active[3],                                                      
			&w_measure.am_harm.phrase_reactive[0],                                                      
			&w_measure.am_harm.phrase_reactive[1],                                                    
			&w_measure.am_harm.phrase_reactive[2],                                                    
			&w_measure.am_harm.phrase_reactive[3],                                                    
			&w_measure.am_harm.phrase_apparent[0],                                                       
			&w_measure.am_harm.phrase_apparent[1],                                                     
			&w_measure.am_harm.phrase_apparent[2],                                                     
			&w_measure.am_harm.phrase_apparent[3],                                                     
			&w_measure.am_harm.factor[0],                                              
			&w_measure.am_harm.factor[1],                                            
			&w_measure.am_harm.factor[2],                                            
			&w_measure.am_harm.factor[3],                                            
			&w_measure.am_harm.n_current,                                              
			&w_measure.am_harm.combine_active_energy[0],                                                 
			&w_measure.am_harm.combine_active_energy[1],                                                 
			&w_measure.am_harm.combine_active_energy[2],                                                 
			&w_measure.am_harm.combine_active_energy[3],                                                 
			&w_measure.am_harm.combine_active_energy[4],                                                 
			&w_measure.am_harm.positive_active_energy[0],                                                       
			&w_measure.am_harm.positive_active_energy[1],                                                       
			&w_measure.am_harm.positive_active_energy[2],                                                       
			&w_measure.am_harm.positive_active_energy[3],                                                       
			&w_measure.am_harm.positive_active_energy[4],                                                       
			&w_measure.am_harm.reverse_active_energy[0],                                                   
			&w_measure.am_harm.reverse_active_energy[1],                                                   
			&w_measure.am_harm.reverse_active_energy[2],                                                   
			&w_measure.am_harm.reverse_active_energy[3],                                                   
			&w_measure.am_harm.reverse_active_energy[4],                                                   
			&w_measure.am_harm.combine_reactive_energy[0][0],                                                     
			&w_measure.am_harm.combine_reactive_energy[0][1],                                                     
			&w_measure.am_harm.combine_reactive_energy[0][2],                                                     
			&w_measure.am_harm.combine_reactive_energy[0][3],                                                     
			&w_measure.am_harm.combine_reactive_energy[0][4],                                                     
			&w_measure.am_harm.combine_reactive_energy[1][0],                                                     
			&w_measure.am_harm.combine_reactive_energy[1][1],                                                     
			&w_measure.am_harm.combine_reactive_energy[1][2],                                                     
			&w_measure.am_harm.combine_reactive_energy[1][3],                                                     
			&w_measure.am_harm.combine_reactive_energy[1][4],                                                     
			&w_measure.am_harm.phrase_original_volt[0],                                                     
			&w_measure.am_harm.phrase_original_volt[1],                                                     
			&w_measure.am_harm.phrase_original_volt[2],                                                     
			&w_measure.am_harm.phrase_original_current[0],                                                     
			&w_measure.am_harm.phrase_original_current[1],                                                     
			&w_measure.am_harm.phrase_original_current[2],                                                     
			&w_measure.am_harm.phrase_original_active[0],                                                     
			&w_measure.am_harm.phrase_original_active[1],                                                     
			&w_measure.am_harm.phrase_original_active[2],                                                     
			&w_measure.am_harm.phrase_original_active[3],                                                     
			&w_measure.am_harm.phrase_harm_volt[0],                                                     
			&w_measure.am_harm.phrase_harm_volt[1],                                                     
			&w_measure.am_harm.phrase_harm_volt[2],                                                     
			&w_measure.am_harm.phrase_harm_current[0],                                                     
			&w_measure.am_harm.phrase_harm_current[1],                                                     
			&w_measure.am_harm.phrase_harm_current[2],                                                     
			&w_measure.am_harm.phrase_harm_active[0],                                                     
			&w_measure.am_harm.phrase_harm_active[1],                                                     
			&w_measure.am_harm.phrase_harm_active[2],                                                     
			&w_measure.am_harm.phrase_harm_active[3],                                                     
			&w_measure.am_harm.total_distor[0],                                                     
			&w_measure.am_harm.total_distor[1],                                                     
			&w_measure.am_harm.total_distor[2],                                                     
			&w_measure.am_harm.total_distor[3],                                                     
			&w_measure.am_harm.total_distor[4],                                                     
			&w_measure.am_harm.total_distor[5],                                                     
			&w_measure.am_harm.total_distor[6],                                                     
			&w_measure.am_harm.total_distor[7],                                                     
			&w_measure.am_harm.harm_u[0][0],                                                     
			&w_measure.am_harm.harm_u[0][1],                                                     
			&w_measure.am_harm.harm_u[0][2],                                                     
			&w_measure.am_harm.harm_u[0][3],                                                     
			&w_measure.am_harm.harm_u[0][4],                                                     
			&w_measure.am_harm.harm_u[0][5],                                                     
			&w_measure.am_harm.harm_u[0][6],                                                     
			&w_measure.am_harm.harm_u[0][7],                                                     
			&w_measure.am_harm.harm_u[0][8],                                                     
			&w_measure.am_harm.harm_u[0][9],                                                     
			&w_measure.am_harm.harm_u[0][10],                                                     
			&w_measure.am_harm.harm_u[0][11],                                                     
			&w_measure.am_harm.harm_u[0][12],                                                     
			&w_measure.am_harm.harm_u[0][13],                                                     
			&w_measure.am_harm.harm_u[0][14],                                                     
			&w_measure.am_harm.harm_u[0][15],                                                     
			&w_measure.am_harm.harm_u[0][16],                                                     
			&w_measure.am_harm.harm_u[0][17],                                                     
			&w_measure.am_harm.harm_u[0][18],                                                     
			&w_measure.am_harm.harm_u[0][19],         
			&w_measure.am_harm.harm_u_parity[0][0],         
			&w_measure.am_harm.harm_u_parity[0][1],         
			&w_measure.am_harm.harm_u_ridge[0],         
			&w_measure.am_harm.harm_u_tele[0],  
			&w_measure.am_harm.harm_u[1][0],                                                     
			&w_measure.am_harm.harm_u[1][1],                                                     
			&w_measure.am_harm.harm_u[1][2],                                                     
			&w_measure.am_harm.harm_u[1][3],                                                     
			&w_measure.am_harm.harm_u[1][4],                                                     
			&w_measure.am_harm.harm_u[1][5],                                                     
			&w_measure.am_harm.harm_u[1][6],                                                     
			&w_measure.am_harm.harm_u[1][7],                                                     
			&w_measure.am_harm.harm_u[1][8],                                                     
			&w_measure.am_harm.harm_u[1][9],                                                     
			&w_measure.am_harm.harm_u[1][10],                                                     
			&w_measure.am_harm.harm_u[1][11],                                                     
			&w_measure.am_harm.harm_u[1][12],                                                     
			&w_measure.am_harm.harm_u[1][13],                                                     
			&w_measure.am_harm.harm_u[1][14],                                                     
			&w_measure.am_harm.harm_u[1][15],                                                     
			&w_measure.am_harm.harm_u[1][16],                                                     
			&w_measure.am_harm.harm_u[1][17],                                                     
			&w_measure.am_harm.harm_u[1][18],                                                     
			&w_measure.am_harm.harm_u[1][19],         
			&w_measure.am_harm.harm_u_parity[1][0],         
			&w_measure.am_harm.harm_u_parity[1][1],         
			&w_measure.am_harm.harm_u_ridge[1],         
			&w_measure.am_harm.harm_u_tele[1], 
			&w_measure.am_harm.harm_u[2][0],                                                     
			&w_measure.am_harm.harm_u[2][1],                                                     
			&w_measure.am_harm.harm_u[2][2],                                                     
			&w_measure.am_harm.harm_u[2][3],                                                     
			&w_measure.am_harm.harm_u[2][4],                                                     
			&w_measure.am_harm.harm_u[2][5],                                                     
			&w_measure.am_harm.harm_u[2][6],                                                     
			&w_measure.am_harm.harm_u[2][7],                                                     
			&w_measure.am_harm.harm_u[2][8],                                                     
			&w_measure.am_harm.harm_u[2][9],                                                     
			&w_measure.am_harm.harm_u[2][10],                                                     
			&w_measure.am_harm.harm_u[2][11],                                                     
			&w_measure.am_harm.harm_u[2][12],                                                     
			&w_measure.am_harm.harm_u[2][13],                                                     
			&w_measure.am_harm.harm_u[2][14],                                                     
			&w_measure.am_harm.harm_u[2][15],                                                     
			&w_measure.am_harm.harm_u[2][16],                                                     
			&w_measure.am_harm.harm_u[2][17],                                                     
			&w_measure.am_harm.harm_u[2][18],                                                     
			&w_measure.am_harm.harm_u[2][19],         
			&w_measure.am_harm.harm_u_parity[2][0],         
			&w_measure.am_harm.harm_u_parity[2][1],         
			&w_measure.am_harm.harm_u_ridge[2],         
			&w_measure.am_harm.harm_u_tele[2], 
			&w_measure.am_harm.harm_i[0][0],                                                     
			&w_measure.am_harm.harm_i[0][1],                                                     
			&w_measure.am_harm.harm_i[0][2],                                                     
			&w_measure.am_harm.harm_i[0][3],                                                     
			&w_measure.am_harm.harm_i[0][4],                                                     
			&w_measure.am_harm.harm_i[0][5],                                                     
			&w_measure.am_harm.harm_i[0][6],                                                     
			&w_measure.am_harm.harm_i[0][7],                                                     
			&w_measure.am_harm.harm_i[0][8],                                                     
			&w_measure.am_harm.harm_i[0][9],                                                     
			&w_measure.am_harm.harm_i[0][10],                                                     
			&w_measure.am_harm.harm_i[0][11],                                                     
			&w_measure.am_harm.harm_i[0][12],                                                     
			&w_measure.am_harm.harm_i[0][13],                                                     
			&w_measure.am_harm.harm_i[0][14],                                                     
			&w_measure.am_harm.harm_i[0][15],                                                     
			&w_measure.am_harm.harm_i[0][16],                                                     
			&w_measure.am_harm.harm_i[0][17],                                                     
			&w_measure.am_harm.harm_i[0][18],                                                     
			&w_measure.am_harm.harm_i[0][19],         
			&w_measure.am_harm.harm_i_parity[0][0],         
			&w_measure.am_harm.harm_i_parity[0][1],         
			&w_measure.am_harm.harm_i_K[0],         
			&w_measure.am_harm.harm_i[1][0],                                                     
			&w_measure.am_harm.harm_i[1][1],                                                     
			&w_measure.am_harm.harm_i[1][2],                                                     
			&w_measure.am_harm.harm_i[1][3],                                                     
			&w_measure.am_harm.harm_i[1][4],                                                     
			&w_measure.am_harm.harm_i[1][5],                                                     
			&w_measure.am_harm.harm_i[1][6],                                                     
			&w_measure.am_harm.harm_i[1][7],                                                     
			&w_measure.am_harm.harm_i[1][8],                                                     
			&w_measure.am_harm.harm_i[1][9],                                                     
			&w_measure.am_harm.harm_i[1][10],                                                     
			&w_measure.am_harm.harm_i[1][11],                                                     
			&w_measure.am_harm.harm_i[1][12],                                                     
			&w_measure.am_harm.harm_i[1][13],                                                     
			&w_measure.am_harm.harm_i[1][14],                                                     
			&w_measure.am_harm.harm_i[1][15],                                                     
			&w_measure.am_harm.harm_i[1][16],                                                     
			&w_measure.am_harm.harm_i[1][17],                                                     
			&w_measure.am_harm.harm_i[1][18],                                                     
			&w_measure.am_harm.harm_i[1][19],         
			&w_measure.am_harm.harm_i_parity[1][0],         
			&w_measure.am_harm.harm_i_parity[1][1],         
			&w_measure.am_harm.harm_i_K[1],   
			&w_measure.am_harm.harm_i[2][0],                                                     
			&w_measure.am_harm.harm_i[2][1],                                                     
			&w_measure.am_harm.harm_i[2][2],                                                     
			&w_measure.am_harm.harm_i[2][3],                                                     
			&w_measure.am_harm.harm_i[2][4],                                                     
			&w_measure.am_harm.harm_i[2][5],                                                     
			&w_measure.am_harm.harm_i[2][6],                                                     
			&w_measure.am_harm.harm_i[2][7],                                                     
			&w_measure.am_harm.harm_i[2][8],                                                     
			&w_measure.am_harm.harm_i[2][9],                                                     
			&w_measure.am_harm.harm_i[2][10],                                                     
			&w_measure.am_harm.harm_i[2][11],                                                     
			&w_measure.am_harm.harm_i[2][12],                                                     
			&w_measure.am_harm.harm_i[2][13],                                                     
			&w_measure.am_harm.harm_i[2][14],                                                     
			&w_measure.am_harm.harm_i[2][15],                                                     
			&w_measure.am_harm.harm_i[2][16],                                                     
			&w_measure.am_harm.harm_i[2][17],                                                     
			&w_measure.am_harm.harm_i[2][18],                                                     
			&w_measure.am_harm.harm_i[2][19],         
			&w_measure.am_harm.harm_i_parity[2][0],         
			&w_measure.am_harm.harm_i_parity[2][1],         
			&w_measure.am_harm.harm_i_K[2]   
		);                                                     
		if( AM_HARM_NUM == cnt && ( len == ( 12 + 4*(31+169)  + 8*15 ) ) )
		{
			ret = true;
		}
		break;

	case WM:
		ind = 0;
		cnt = sscanf(data, 
			"%12lld"       //meter_time,
			"%4x",          //meter_state,   	
			&w_measure.water_meter.meter_time,
			&w_measure.water_meter.meter_state); 

		ind += 16;
		str_ten_hex_to_float((char*)&data[ind],w_measure.water_meter.cur_data);

		if(len == (12+4+8))
		{
			ret = true;
		}
		break;

	case GM:
		ind = 0;
		cnt = sscanf((char*)&data[ind], 
			"%12lld"       //meter_time,
			"%4x"          //meter_state,                      	        	
			,         
			&w_measure.gass_meter.meter_time,      
			&w_measure.gass_meter.meter_state);   

		ind += 16;

		str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.instantaneous_flow);
		ind += 8;
		str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.frequency);
		ind += 8;
		str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.diff_pressure);
		ind += 8;
		str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.pressure);
		ind += 8;
		str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.temperature);
		ind += 8;
		str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.inlet_temperature);
		ind += 8;
		str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.outlet_temperature);
		ind += 8;
		str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.fluid_density);
		ind += 8;
		str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.instantaneous_heat);
		ind += 8;
		str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.enthalpy);
		ind += 8;
		str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.enthalpy_diff);
		ind += 8;
		cnt = sscanf((char*)&data[ind],                       	        	
			"%8x"                       	        	
			"%8x"                       	        	
			"%8x",  
			&w_measure.gass_meter.cumulative_flow,      
			&w_measure.gass_meter.accumulated_heat,      
			&w_measure.gass_meter.cumulative_diff_heat);        


		if(len == (12 + 4 + 8*14))
		{
			ret = true;
		}
		break;

	default:
		break;
	}

	if (ret == true)
	{
		return 0;
	}
	else
	{
		return -2;
	}
	*/
#endif
}

int ConvertDataToCvs( u_w_measure& w_measure,e_dev_type dev_type,char* cvs_data )
{
#if 1
	return 0;
#else
	/*
	bool ret = 0;
	int tmp_len = 0;
	CString str;
	MU_INT64 meter_time = 0;
	int yy = 0;
	int mm = 0;
	int dd = 0;
	int hh= 0;
	int mi = 0;
	int se = 0;
	switch(dev_type)
	{
	case AM_3PHASE:
		meter_time = w_measure.am_3phase.meter_time;//151020130030
		yy = meter_time / 100 / 100 / 100 / 100 / 100 % 100;
		mm = meter_time / 100 / 100 / 100 / 100 % 100;
		dd = meter_time / 100 / 100 / 100 % 100;
		hh = meter_time / 100 / 100 % 100;
		mi = meter_time / 100 % 100;
		se = meter_time  % 100;
		tmp_len += sprintf(cvs_data + tmp_len, "20%02d-%02d-%02d %02d:%02d:%02d,",yy,mm,dd,hh,mi,se);
		str = AnalyzeMeterState(w_measure.am_3phase.meter_state);
		tmp_len += sprintf(cvs_data + tmp_len, "%04x(%s),", w_measure.am_3phase.meter_state,str.GetString());
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_3phase.threshold_state);
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_3phase.volt_change);
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_3phase.current_change);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.frequency * gTwoUnit);
		for	(int i = 0; i < 3; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_3phase.phase_volt[i] * gOneUnit);
		}
		for	(int i = 0; i < 3; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_3phase.line_volt[i] * gOneUnit);
		}
		for	(int i = 0; i < 3; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.phrase_current[i] * gTwoUnit);
		}
		for	(int i = 0; i < 4; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.phrase_active[i] * gTwoUnit);
		}
		for	(int i = 0; i < 4; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.phrase_reactive[i] * gTwoUnit);
		}
		for	(int i = 0; i < 4; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.phrase_apparent[i] * gTwoUnit);
		}
		for	(int i = 0; i < 4; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.factor[i] * gTwoUnit);
		}
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.n_current * gTwoUnit);
		for	(int i = 0; i < 5; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.combine_active_energy[i] * gTwoUnit);
		}
		for	(int i = 0; i < 5; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.positive_active_energy[i] * gTwoUnit);
		}
		for	(int i = 0; i < 5; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.reverse_active_energy[i] * gTwoUnit);
		}
		for	(int i = 0; i < 2; i ++)
		{
			for	(int j = 0; j < 2; j ++)
			{
				tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.combine_reactive_energy[i][j] * gTwoUnit);
			}
		}

		break;

	case AM_1PHASE:
		meter_time = w_measure.am_1phase.meter_time;//151020130030
		yy = meter_time / 100 / 100 / 100 / 100 / 100 % 100;
		mm = meter_time / 100 / 100 / 100 / 100 % 100;
		dd = meter_time / 100 / 100 / 100 % 100;
		hh = meter_time / 100 / 100 % 100;
		mi = meter_time / 100 % 100;
		se = meter_time  % 100;
		tmp_len += sprintf(cvs_data + tmp_len, "20%02d-%02d-%02d %02d:%02d:%02d,",yy,mm,dd,hh,mi,se);
		str = AnalyzeMeterState(w_measure.am_1phase.meter_state);
		tmp_len += sprintf(cvs_data + tmp_len, "%04x(%s),", w_measure.am_1phase.meter_state,str.GetString());
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_1phase.threshold_state);
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_1phase.volt_change);
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_1phase.current_change);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.frequency * gTwoUnit);
		tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_1phase.volt * gOneUnit);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.current * gTwoUnit);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.active * gTwoUnit);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.reactive * gTwoUnit);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.apparent * gTwoUnit);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.factor * gTwoUnit);
		for	(int i = 0; i < 5; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.combine_active_energy[i] * gTwoUnit);
		}
		for	(int i = 0; i < 5; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.positive_active_energy[i] * gTwoUnit);
		}
		for	(int i = 0; i < 5; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.reverse_active_energy[i] * gTwoUnit);
		}
		for	(int i = 0; i < 2; i ++)
		{
			for	(int j = 0; j < 2; j ++)
			{
				tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.combine_reactive_energy[i][j] * gTwoUnit);
			}
		}

		break;

	case AM_DC:
		meter_time = w_measure.am_dc.meter_time;//151020130030
		yy = meter_time / 100 / 100 / 100 / 100 / 100 % 100;
		mm = meter_time / 100 / 100 / 100 / 100 % 100;
		dd = meter_time / 100 / 100 / 100 % 100;
		hh = meter_time / 100 / 100 % 100;
		mi = meter_time / 100 % 100;
		se = meter_time  % 100;
		tmp_len += sprintf(cvs_data + tmp_len, "20%02d-%02d-%02d %02d:%02d:%02d,",yy,mm,dd,hh,mi,se);
		str = AnalyzeMeterState(w_measure.am_dc.meter_state);
		tmp_len += sprintf(cvs_data + tmp_len, "%04x(%s),", w_measure.am_dc.meter_state,str.GetString());
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_dc.volt_change);
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_dc.current_change);
		tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_dc.volt * gOneUnit);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_dc.current * gTwoUnit);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_dc.active * gTwoUnit);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_dc.energy * gTwoUnit);

		break;

	case AM_HARM:
		meter_time = w_measure.am_harm.meter_time;//151020130030
		yy = meter_time / 100 / 100 / 100 / 100 / 100 % 100;
		mm = meter_time / 100 / 100 / 100 / 100 % 100;
		dd = meter_time / 100 / 100 / 100 % 100;
		hh = meter_time / 100 / 100 % 100;
		mi = meter_time / 100 % 100;
		se = meter_time  % 100;
		tmp_len += sprintf(cvs_data + tmp_len, "20%02d-%02d-%02d %02d:%02d:%02d,",yy,mm,dd,hh,mi,se);
		str = AnalyzeMeterState(w_measure.am_harm.meter_state);
		tmp_len += sprintf(cvs_data + tmp_len, "%04x(%s),", w_measure.am_harm.meter_state,str.GetString());
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_harm.threshold_state);
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_harm.volt_change);
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_harm.current_change);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.frequency * gTwoUnit);
		for	(int i = 0; i < 3; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_harm.phase_volt[i] * gOneUnit);
		}
		for	(int i = 0; i < 3; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_harm.line_volt[i] * gOneUnit);
		}
		for	(int i = 0; i < 3; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_current[i] * gTwoUnit);
		}
		for	(int i = 0; i < 4; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_active[i] * gTwoUnit);
		}
		for	(int i = 0; i < 4; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_reactive[i] * gTwoUnit);
		}
		for	(int i = 0; i < 4; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_apparent[i] * gTwoUnit);
		}
		for	(int i = 0; i < 4; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.factor[i] * gTwoUnit);
		}
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.n_current * gTwoUnit);
		for	(int i = 0; i < 5; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.combine_active_energy[i] * gTwoUnit);
		}
		for	(int i = 0; i < 3; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_harm.phrase_original_volt[i] * gOneUnit);
		}
		for	(int i = 0; i < 3; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_original_current[i] * gTwoUnit);
		}
		for	(int i = 0; i < 4; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_original_active[i] * gTwoUnit);
		}
		for	(int i = 0; i < 3; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_harm.phrase_harm_volt[i] * gOneUnit);
		}
		for	(int i = 0; i < 3; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_harm_current[i] * gTwoUnit);
		}
		for	(int i = 0; i < 4; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_harm_active[i] * gTwoUnit);
		}
		for	(int i = 0; i < 8; i ++)
		{
			tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.total_distor[i] * gTwoUnit);
		}

		for	(int i = 0; i < 3; i ++)
		{
			for	(int j = 0; j < 20; j ++)
			{
				tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.harm_u[i][j] * gTwoUnit);
			}
			for	(int j = 0; j < 2; j ++)
			{
				tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.harm_u_parity[i][j] * gTwoUnit);
			}
			tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.am_harm.harm_u_ridge[i] * gThreeUnit);
			tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.am_harm.harm_u_tele[i] * gThreeUnit);
		}

		for	(int i = 0; i < 3; i ++)
		{
			for	(int j = 0; j < 20; j ++)
			{
				tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.harm_i[i][j] * gTwoUnit);
			}
			for	(int j = 0; j < 2; j ++)
			{
				tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.harm_i_parity[i][j] * gTwoUnit);
			}
			tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.am_harm.harm_i_K[i] * gThreeUnit);
		}

		break;

	case WM:
		meter_time = w_measure.water_meter.meter_time;//151020130030
		yy = meter_time / 100 / 100 / 100 / 100 / 100 % 100;
		mm = meter_time / 100 / 100 / 100 / 100 % 100;
		dd = meter_time / 100 / 100 / 100 % 100;
		hh = meter_time / 100 / 100 % 100;
		mi = meter_time / 100 % 100;
		se = meter_time  % 100;
		tmp_len += sprintf(cvs_data + tmp_len, "20%02d-%02d-%02d %02d:%02d:%02d,",yy,mm,dd,hh,mi,se);
		str = AnalyzeMeterState(w_measure.water_meter.meter_state);
		tmp_len += sprintf(cvs_data + tmp_len, "%04x(%s),", w_measure.water_meter.meter_state,str.GetString());
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,",w_measure.water_meter.cur_data);
		break;

	case GM:
		meter_time = w_measure.gass_meter.meter_time;//151020130030
		yy = meter_time / 100 / 100 / 100 / 100 / 100 % 100;
		mm = meter_time / 100 / 100 / 100 / 100 % 100;
		dd = meter_time / 100 / 100 / 100 % 100;
		hh = meter_time / 100 / 100 % 100;
		mi = meter_time / 100 % 100;
		se = meter_time  % 100;
		tmp_len += sprintf(cvs_data + tmp_len, "20%02d-%02d-%02d %02d:%02d:%02d,",yy,mm,dd,hh,mi,se);
		str = AnalyzeMeterState(w_measure.gass_meter.meter_state);
		tmp_len += sprintf(cvs_data + tmp_len, "%04x(%s),", w_measure.gass_meter.meter_state,str.GetString());
		tmp_len += sprintf(cvs_data + tmp_len, "%.4f,", w_measure.gass_meter.instantaneous_flow);
		tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.gass_meter.frequency);
		tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.gass_meter.diff_pressure);
		tmp_len += sprintf(cvs_data + tmp_len, "%.4f,", w_measure.gass_meter.pressure);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.gass_meter.temperature);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.gass_meter.inlet_temperature);
		tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.gass_meter.outlet_temperature);
		tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.gass_meter.fluid_density);
		tmp_len += sprintf(cvs_data + tmp_len, "%.4f,", w_measure.gass_meter.instantaneous_heat);
		tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.gass_meter.enthalpy);
		tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.gass_meter.enthalpy_diff);
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.gass_meter.cumulative_flow);
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.gass_meter.accumulated_heat);
		tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.gass_meter.cumulative_diff_heat);


		break;
	default:
		break;
	}

	if (ret == true)
	{
		return 0;
	}
	else
	{
		return -2;
	}
*/
#endif
}

CString AnalyzeMeterState( int state )
{
	MU_INT16 err = state & 0xff;
	CString str = "";
	bool is_err = false;
	for (int i = 0; i < ERR_NAME_NUM; i++)
	{
		if ((err >> i) & 0x01)
		{
			is_err = true;
			str += gErrName[i];
			str += ";";
		}
	}

	if (is_err == false)
	{
		str = "正常";
	}
	else
	{
		str.Delete(str.GetLength()-1);
	}

	return str;
}

//16进制格式字符串转float
void str_16hex_to_float( const char*src, float& val )
{
	int u1 = 0;
	int u2 = 0;
	int u3 = 0;
	int u4 = 0;
	unsigned char pMem[5] = {0};
	float fval = 0.0f;
	int cnt = sscanf(src,
		"%2x"
		"%2x"
		"%2x"
		"%2x",
		&u1,
		&u2,
		&u3,
		&u4
		);

	pMem[0] = u4;
	pMem[1] = u3;
	pMem[2] = u2;
	pMem[3] = u1;
	memcpy(&fval,pMem,4);
	val = fval;
	return;  
}

//BCD格式字符串转float
void str_ten_hex_to_float( const char*src, float& val )
{
	int u1 = 0;
	int u2 = 0;
	int u3 = 0;
	int u4 = 0;
	//10230100->00012310->123.10
	int cnt = sscanf(src,
		"%2d"
		"%2d"
		"%2d"
		"%2d",
		&u1,
		&u2,
		&u3,
		&u4
		);
	double dval = u4*10000 + u3*100 + u2 + u1*0.01;
	val = (float)dval;
	return;
}

//crc校验,用于xml通讯
/* CRC16 implementation acording to CCITT standards */
static const unsigned short crc16tab[256]= 
{
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

MU_INT16 parity_crc16(MU_INT16 crc, MU_INT8 *buf, int len)
{
	int counter;
	for( counter = 0; counter < len; counter++)
	{
		crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *(char *)buf++)&0x00FF];
	}
	return crc;
}
//crc校验,用于xml通讯

MU_INT16 check_crc_data_communicate_data(str_communicate_data& com_data)
{
	int malloc_len = com_data.xml_len + HEAD_TAIL_LEN +1;
	char* pbuff = (char*)malloc(malloc_len);
	memset(pbuff,0,malloc_len);
	MU_INT16 crc = 0;
	int tmp_len = 0;

	//format
	//head
	for (int i = 0; i < 4; i++)
	{
		pbuff[tmp_len++] = com_data.head[i];
	}
	//数据长度
	for (int i = 0; i < 4; i++)
	{
		pbuff[tmp_len++] = com_data.data_len[i];
	}
	//指令序号
	for (int i = 0; i < 4; i++)
	{
		pbuff[tmp_len++] = com_data.sequence[i];
	}
	memcpy(pbuff + tmp_len, com_data.xml_data, com_data.xml_len);
	tmp_len += com_data.xml_len;
	//tmp_len += sprintf(pbuff + tmp_len, "%s",com_data.xml_data);
	//CRC
	crc = parity_crc16(0, (MU_CHAR*)pbuff, tmp_len);

	free(pbuff);
	return crc;
}

MU_INT8 parity_cs( MU_INT8* buf, int len )
{
	int ret = 0;

	for (int i = 0; i < len; i++)
	{
		ret += buf[i];
		ret &= 0xFF;
	}
	return ret;
}

CString AnalyzeThresholdState( int state )
{
	MU_INT16 err = state & 0xff;
	CString str = "";
	MU_INT8 flag = 0;
	bool is_err = false;
	//D0-D15
	//D0D1
	flag = (MU_INT8)((err >> 14) & 0x03);
	if (flag == 0x00)
	{
		str += "阀门状态:开";
		str += ";";
	}
	else if (flag == 0x01)
	{
		str += "阀门状态:关";
		str += ";";
	}
	else if (flag == 0x11)
	{
		str += "阀门状态:异常";
		str += ";";
		is_err = true;
	}

	//D2
	flag = (MU_INT8)((err >> 13) & 0x01);
	if (flag == 0x00)
	{
		str += "电池电压:正常";
		str += ";";
	}
	else if (flag == 0x01)
	{
		str += "电池电压:欠压";
		str += ";";
		is_err = true;
	}

	//D5
	flag = (MU_INT8)((err >> 10) & 0x01);
	if (flag == 0x00)
	{
		str += "流量传感器状态:正常";
		str += ";";
	}
	else if (flag == 0x01)
	{
		str += "流量传感器状态:故障";
		str += ";";
		is_err = true;
	}

	//D6
	flag = (MU_INT8)((err >> 9) & 0x01);
	if (flag == 0x00)
	{
		str += "进水温度传感器状态:正常";
		str += ";";
	}
	else if (flag == 0x01)
	{
		str += "进水温度传感器状态:故障";
		str += ";";
		is_err = true;
	}

	//D7
	flag = (MU_INT8)((err >> 8) & 0x01);
	if (flag == 0x00)
	{
		str += "回水温度传感器状态:正常";
		str += ";";
	}
	else if (flag == 0x01)
	{
		str += "回水温度传感器状态:故障";
		str += ";";
		is_err = true;
	}

	//D8
	flag = (MU_INT8)((err >> 7) & 0x01);
	if (flag == 0x00)
	{
		str += "积算仪状态:正常";
		str += ";";
	}
	else if (flag == 0x01)
	{
		str += "积算仪状态:故障";
		str += ";";
		is_err = true;
	}

	//D9
	flag = (MU_INT8)((err >> 6) & 0x01);
	if (flag == 0x00)
	{
		str += "设备状态:上电";
		str += ";";
	}
	else if (flag == 0x01)
	{
		str += "设备状态:未上电";
		str += ";";
		is_err = true;
	}

	//D10D11
	flag = (MU_INT8)((err >> 4) & 0x03);
	if (flag == 0x00)
	{
		str += "报警器状态:报警";
		str += ";";
	}
	else if (flag == 0x01)
	{
		str += "报警器状态:上电";
		str += ";";
		is_err = true;
	}
	else if (flag == 0x11)
	{
		str += "报警器状态:未上电";
		str += ";";
		is_err = true;
	}

	//D12
	flag = (MU_INT8)((err >> 3) & 0x01);
	if (flag == 0x00)
	{
		str += "水表测量状态:冷水";
		str += ";";
	}
	else if (flag == 0x01)
	{
		str += "水表测量状态:热水";
		str += ";";
		is_err = true;
	}

	//D13
	flag = (MU_INT8)((err >> 2) & 0x01);
	if (flag == 0x00)
	{
		str += "通信状态:正常";
		str += ";";
	}
	else if (flag == 0x01)
	{
		str += "通信状态:故障";
		str += ";";
		is_err = true;
	}

	//D14
	flag = (MU_INT8)((err >> 1) & 0x01);
	if (flag == 0x00)
	{
		str += "EEPROM状态:正常";
		str += ";";
	}
	else if (flag == 0x01)
	{
		str += "EEPROM状态:故障";
		str += ";";
		is_err = true;
	}

	//D15
	flag = (MU_INT8)((err >> 0) & 0x01);
	if (flag == 0x00)
	{
		str += "水表结算日冷热状态:冷水";
		str += ";";
	}
	else if (flag == 0x01)
	{
		str += "水表结算日冷热状态:热水";
		str += ";";
		is_err = true;
	}

	if (is_err == false)
	{
		str = "正常";
	}
	else
	{
		str.Delete(str.GetLength()-1);
	}

	return str;
}

//crc校验,用于下行透传通讯
static const unsigned char aucCRCHi[] = {
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

static const unsigned char aucCRCLo[] = {
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

unsigned short ModbusCrc( unsigned char * pucFrame, unsigned short usLen )
{
	unsigned char   ucCRCHi = 0xFF;
	unsigned char   ucCRCLo = 0xFF;
	unsigned long   iIndex;

	while( usLen-- )
	{
		iIndex = ucCRCLo ^ *( pucFrame++ );
		ucCRCLo = ( unsigned char )( ucCRCHi ^ aucCRCHi[iIndex] );
		ucCRCHi = aucCRCLo[iIndex];
	}
	return ( unsigned short )( ucCRCHi << 8 | ucCRCLo );
}
//crc校验,用于下行透传通讯

//12 -> 0x12
MU_INT8 TenHexAdd0x( MU_INT8 mem )
{
	MU_INT8 ret = 0;
	MU_INT8 hign_bit = mem / 10;
	MU_INT8 low_bit = mem % 10;
	ret = hign_bit *16 + low_bit;
	return ret;
}

char * strToUpper(char * str)
{
	for (unsigned int i = 0; i < strlen(str); ++i)
	{
		str[i] = toupper(str[i]);
	}
	return str;
}

string GetNowTime()
{
	string cur_time;
	//
	time_t   now;         //实例化time_t结构
	struct   tm     *timenow;         //实例化tm结构指针
	time(&now);
	timenow = localtime(&now);
	char now_time_c[64];
	sprintf(now_time_c, "%04d-%02d-%02d %02d:%02d:%02d ",
		timenow->tm_year+1900, timenow->tm_mon+1, timenow->tm_mday,
		timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
	cur_time = now_time_c;
	TRACE("%s\n",cur_time.c_str());

	return cur_time;

}



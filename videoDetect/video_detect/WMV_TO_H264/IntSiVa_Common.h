#if !defined(INTSIVA_COMMON__INCLUDED_)
#define INTSIVA_COMMON__INCLUDED_


typedef enum IntSiVa_Ope_Type
{
	IntSiVa_Ope_Reset_Alarm = 1,	
	IntSiVa_Ope_Reset_Param = 2	
}IntSiVa_Ope_Type;

typedef enum IntSiVa_CamYiChang_Type
{
	Cam_Black = 0,				//图像黑屏；方差小，平均值小
	Cam_Cover = 1,				//图像遮挡；方差小，平均值大
	Cam_Dim = 2,				//图像模糊；轮廓，Sobel算子
	Cam_IntensityAbnormal = 3,	//图像亮度异常；方差，平均值
	Cam_Forzon = 4,				//图像冻结；相邻帧像素差异
	Cam_SaltNoise = 5,			//图像随机噪声异常；去噪二值法和中值滤波不去噪
	Cam_Flicker = 6,				//图像闪烁、闪屏；帧差法黑白交错
	Cam_RollStripe = 7			//水平滚动横纹；轮廓，Sobel算子
}IntSiVa_CamYiChang_TYPE;

typedef enum IntSiVa_EVENT_ACT_Type
{
	Event_CamYiChang = 16,	//图像质量诊断
}IntSiVa_EVENT_ACT_TYPE;

class IntSiVa_General_API
{
public:
	virtual int IntSiVa_Initial(void* param) = 0;
	virtual int IntSiVa_Parameter_LoadFile(void* file) = 0;
	virtual int IntSiVa_Parameter_SaveFile(void* file) = 0;
	virtual int IntSiVa_Parameter_Set(void* param) = 0;
	virtual int IntSiVa_Parameter_Get(void* param) = 0;
	virtual int IntSiVa_Process(void* data,int* Lockflag) = 0;
	virtual int IntSiVa_GetOutput(void* output) = 0;
	virtual int IntSiVa_Release() = 0;
	virtual int IntSiVa_DrawInfo(void* data,int* Lockflag) = 0;
	virtual int IntSiVa_Operation(IntSiVa_Ope_Type type,void* data) = 0;
};

struct IntSiVa_Event_CamYiChangParam
{
	float _Black_Senstivity; 
	float _Cover_Senstivity; 
	float _Dim_Senstivity; 
	float _IntensityAbnormal_Senstivity; 
	float _Forzon_Senstivity; 
	float _SaltNoise_Senstivity; 
	float _Flicker_Senstivity; 
	float _RollStripe_Senstivity;
};


struct IntSiVa_InPut
{
	int _ImgWidth;
	int _ImgHeight;
	int _AlgType; 

	IntSiVa_Event_CamYiChangParam _CamYiChangParam;
};

struct IntSiVa_Event_CamYiChangFrame
{
	char* _GrayData;
};

struct IntSiVa_InPutFrame
{
	IntSiVa_Event_CamYiChangFrame _CamYCFrame;
};

struct IntSiVa_Event_CamYiChangOutPut
{
	int _alarmoutput[16];
};

struct IntSiVa_OutPut
{
	IntSiVa_Event_CamYiChangOutPut _CamYiChangOutPut;
};

extern "C" bool _declspec(dllexport) 
IntSiVa_General_API_Ini(IntSiVa_General_API** ApiObject ,char* FunctionName,int ThreadNum);

#endif

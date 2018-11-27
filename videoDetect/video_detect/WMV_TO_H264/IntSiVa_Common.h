#if !defined(INTSIVA_COMMON__INCLUDED_)
#define INTSIVA_COMMON__INCLUDED_


typedef enum IntSiVa_Ope_Type
{
	IntSiVa_Ope_Reset_Alarm = 1,	
	IntSiVa_Ope_Reset_Param = 2	
}IntSiVa_Ope_Type;

typedef enum IntSiVa_CamYiChang_Type
{
	Cam_Black = 0,				//ͼ�����������С��ƽ��ֵС
	Cam_Cover = 1,				//ͼ���ڵ�������С��ƽ��ֵ��
	Cam_Dim = 2,				//ͼ��ģ����������Sobel����
	Cam_IntensityAbnormal = 3,	//ͼ�������쳣�����ƽ��ֵ
	Cam_Forzon = 4,				//ͼ�񶳽᣻����֡���ز���
	Cam_SaltNoise = 5,			//ͼ����������쳣��ȥ���ֵ������ֵ�˲���ȥ��
	Cam_Flicker = 6,				//ͼ����˸��������֡��ڰ׽���
	Cam_RollStripe = 7			//ˮƽ�������ƣ�������Sobel����
}IntSiVa_CamYiChang_TYPE;

typedef enum IntSiVa_EVENT_ACT_Type
{
	Event_CamYiChang = 16,	//ͼ���������
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

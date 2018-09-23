
#include "../include/queuesappl.h"
//#include "../include/command.h"
#include "../include/frame.h"
#include "../include/thread.h"

static FrameBuffer *hFrame[DEF_BOARD_MAX_CHANNELS];
static MU_INT32 s_Sequence[DEF_BOARD_MAX_CHANNELS];

MS_INT32 initDataBuf(void)
{
	memset(hFrame,0,sizeof(hFrame));
	
	memset(s_Sequence,0,sizeof(s_Sequence));
	
	return MD_OK;
}

MS_INT32 createDataBuf(MU_INT32 channel,MU_INT32 bufsize )
{

	
	if (hFrame[channel] == NULL)
	{	
		hFrame[channel] = createFrameBuffer(bufsize,NULL);
		
		
		#ifdef SINGLE_FRAME_BUFFER
		{
			MU_INT32 i;
			for(i=0;i<DEF_BOARD_MAX_CHANNELS;i++)
				hFrame[i] = hFrame[channel];
		}	
		#endif
	}
	
	if(NULL == hFrame[channel] )
		return MD_FAILED;
	return MD_OK;
}

void closeDataBuf(MU_INT32 channel)
{
	
	
	closeFrameBuffer(hFrame[channel] );	
	
	#ifdef SINGLE_FRAME_BUFFER
	{
		MU_INT32 i;
		for(i=0;i<DEF_BOARD_MAX_CHANNELS;i++)
			hFrame[i] = NULL;;
	}
	#endif	
}

void resetDataBuf(MU_INT32 channel)
{
	if(NULL != hFrame[channel] )
	{
		//lock_pend(0,SYS_FOREVER);
		resetFrameBuffer(hFrame[channel]);
		//lock_post(0);
	}
		
}

MS_INT32 writeDataToBuf(MU_INT32 channel,MU_INT8 *data,MU_INT32 data_size,MU_INT32 ip_addr,MU_INT32 port, MU_INT32 data_type)
{
	
	MU_INT32 bflag = FALSE;
	FrameOut tFrameOut;
	
	memset(&tFrameOut,0,sizeof(tFrameOut));
	
	
	tFrameOut.pOut		 =(MS_CHAR*) data;
	tFrameOut.Outlen	 = data_size;
	tFrameOut.ipAddr	 = ip_addr;
	tFrameOut.nPort		 = port;
	tFrameOut.sequence	 = s_Sequence[channel]++;
	

	

	while(bflag == FALSE)//if (bflag == FALSE) //
	{
		
		//lock_pend(0,SYS_FOREVER);
		
		bflag = writeFrame(hFrame[channel],&tFrameOut,NULL);
		
		//lock_post(0);

		//readFrameRelease(hFrame[channel]);	

		
		
		if (bflag == FALSE)
		{

			//printf("Queue is full.\n");
			//break;
					
			//kick_dog();
			readFrameRelease(hFrame[channel]);

			ms_sleep(1);


		}
#ifdef DEBUG_TEST
        else{	
            static MU_INT32 nTimes = 0;
            printf("PUSH %d  times of %d\n",nTimes++,channel);
        }
#endif


	}

	return MD_OK;
}

MS_INT32 readDataFrmBuf(MU_INT32 channel,FrameOut *tframeOut)
{
	
	MS_INT32 bFlag;
	
	//lock_pend(0,SYS_FOREVER);	  
	bFlag = readFrame(hFrame[channel],tframeOut);		   
	//lock_post(0);
	
	if(bFlag == FALSE)
		return MD_FAILED;
			
	return MD_OK;
	
}

MS_INT32 releaseReadBuf(MU_INT32 channel)
{
	//lock_pend(0,SYS_FOREVER);
	readFrameRelease(hFrame[channel]);	
	//lock_post(0);
	
	return MD_OK;
}





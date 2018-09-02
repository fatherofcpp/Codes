#include "StdAfx.h"
#include "CmOpenCvDll.h"

#include <opencv/cv.hpp>  

#include <opencv2/opencv.hpp>  

#include "CmCvvImage.h"

#ifdef _DEBUG
#pragma comment(lib,"opencv_ml249d.lib")
#pragma comment(lib,"opencv_calib3d249d.lib")
#pragma comment(lib,"opencv_contrib249d.lib")
#pragma comment(lib,"opencv_core249d.lib")
#pragma comment(lib,"opencv_features2d249d.lib")
#pragma comment(lib,"opencv_flann249d.lib")
#pragma comment(lib,"opencv_gpu249d.lib")
#pragma comment(lib,"opencv_highgui249d.lib")
#pragma comment(lib,"opencv_imgproc249d.lib")
#pragma comment(lib,"opencv_legacy249d.lib")
#pragma comment(lib,"opencv_objdetect249d.lib")
#pragma comment(lib,"opencv_ts249d.lib")
#pragma comment(lib,"opencv_video249d.lib")
#pragma comment(lib,"opencv_nonfree249d.lib")
#pragma comment(lib,"opencv_ocl249d.lib")
#pragma comment(lib,"opencv_photo249d.lib")
#pragma comment(lib,"opencv_stitching249d.lib")
#pragma comment(lib,"opencv_superres249d.lib")
#pragma comment(lib,"opencv_videostab249d.lib")
#else
#pragma comment(lib,"opencv_ml249.lib")
#pragma comment(lib,"opencv_calib3d249.lib")
#pragma comment(lib,"opencv_contrib249.lib")
#pragma comment(lib,"opencv_core249.lib")
#pragma comment(lib,"opencv_features2d249.lib")
#pragma comment(lib,"opencv_flann249.lib")
#pragma comment(lib,"opencv_gpu249.lib")
#pragma comment(lib,"opencv_highgui249.lib")
#pragma comment(lib,"opencv_imgproc249.lib")
#pragma comment(lib,"opencv_legacy249.lib")
#pragma comment(lib,"opencv_objdetect249.lib")
#pragma comment(lib,"opencv_ts249.lib")
#pragma comment(lib,"opencv_video249.lib")
#pragma comment(lib,"opencv_nonfree249.lib")
#pragma comment(lib,"opencv_ocl249.lib")
#pragma comment(lib,"opencv_photo249.lib")
#pragma comment(lib,"opencv_stitching249.lib")
#pragma comment(lib,"opencv_superres249.lib")
#pragma comment(lib,"opencv_videostab249.lib")
#endif

#pragma comment(lib,"opencv_objdetect249.lib")
#pragma comment(lib,"opencv_ts249.lib")
#pragma comment(lib,"opencv_video249.lib")
#pragma comment(lib,"opencv_nonfree249.lib")
#pragma comment(lib,"opencv_ocl249.lib")
#pragma comment(lib,"opencv_photo249.lib")
#pragma comment(lib,"opencv_stitching249.lib")
#pragma comment(lib,"opencv_superres249.lib")
#pragma comment(lib,"opencv_videostab249.lib")
#pragma comment(lib,"opencv_calib3d249.lib")
#pragma comment(lib,"opencv_contrib249.lib")
#pragma comment(lib,"opencv_core249.lib")
#pragma comment(lib,"opencv_features2d249.lib")
#pragma comment(lib,"opencv_flann249.lib")
#pragma comment(lib,"opencv_gpu249.lib")
#pragma comment(lib,"opencv_highgui249.lib")
#pragma comment(lib,"opencv_imgproc249.lib")
#pragma comment(lib,"opencv_legacy249.lib")
#pragma comment(lib,"opencv_ml249.lib")

using namespace cv;  

NSP_STP_CM::COpenCvDll::COpenCvDll(void)
{
	m_first_enter = true;
}


NSP_STP_CM::COpenCvDll::~COpenCvDll(void)
{
}

void NSP_STP_CM::DisImg( std::string file_path, double fScale, int delay_tms )
{
	IplImage* src = cvLoadImage(file_path.c_str(), 0);  
	if (!src)
	{
		return;
	}

	IplImage *gaussianImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, src->nChannels);      

	cvSmooth(src, gaussianImg, CV_GAUSSIAN, 7, src->nChannels);  

	CvSize czSize;              //目标图像尺寸 
	czSize.width = (int)(src->width * fScale);  
	czSize.height = (int)(src->height * fScale);  
	IplImage *resizeImg = cvCreateImage(czSize, src->depth, src->nChannels);  
	cvResize(gaussianImg, resizeImg, CV_INTER_AREA); 

	char* gaussianName = "高斯滤波";

	cvNamedWindow(gaussianName, CV_WINDOW_AUTOSIZE);  
	cvMoveWindow(gaussianName, 0, 0);  
	//cvShowImage(gaussianName, gaussianImg);  
	cvShowImage(gaussianName, resizeImg);  

	cvWaitKey(delay_tms);  

	cvReleaseImage(&src);  
	cvReleaseImage(&gaussianImg); 
	cvReleaseImage(&resizeImg); 

	cvDestroyWindow(gaussianName); 
	return;  
}

void NSP_STP_CM::COpenCvDll::ContinueImg( std::string file_path, int y_off, double fScale, int delay_tms, char* msg )
{
	IplImage* src = cvLoadImage(file_path.c_str(), 0);  
	if (!src)
	{
		return;
	}

	IplImage *gaussianImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, src->nChannels);      
	cvSmooth(src, gaussianImg, CV_GAUSSIAN, 7, src->nChannels);  

	CvSize czSize;              //目标图像尺寸 
	czSize.width = (int)(src->width * fScale);  
	czSize.height = (int)(src->height * fScale);  
	IplImage *resizeImg = cvCreateImage(czSize, src->depth, src->nChannels);  
	cvResize(gaussianImg, resizeImg, CV_INTER_AREA); 

	char gaussian_name[128];
	sprintf(gaussian_name, "高斯滤波_%s", msg);

	if (m_first_enter)
	{
		m_first_enter = false;
		cvNamedWindow(gaussian_name, CV_WINDOW_AUTOSIZE);  
		cvMoveWindow(gaussian_name, 0, y_off);  
	}

	//cvShowImage(gaussian_name, gaussianImg);  
	cvShowImage(gaussian_name, resizeImg);  

	cvWaitKey(delay_tms);  

	cvReleaseImage(&src);  
	cvReleaseImage(&gaussianImg); 
	cvReleaseImage(&resizeImg); 

	//cvDestroyWindow(gaussianName); 
	return;  
}

void NSP_STP_CM::ShowImgStatic(std::string file_path, CStatic* pStc, CDC* pDC)
{
	CRect rect;
	pStc->GetClientRect(&rect);
	HDC hDC = pDC->GetSafeHdc();

	IplImage* img = cvLoadImage(file_path.c_str(), 1);  
	if (!img)
	{
		return;
	}
	CvvImage cvimg;
	cvimg.CopyOf(img);
	cvimg.DrawToHDC(hDC, &rect);
	cvReleaseImage(&img);  
}

bool NSP_STP_CM::LoadImgFrame(std::string file_path, int iscolor, unsigned char*& pData, int& width, int& height, int& frame_size)
{
	IplImage* src = cvLoadImage(file_path.c_str(), iscolor);  
	if (!src)
	{
		return false;
	}
	width = src->width;  
	height = src->height;  
	frame_size = src->widthStep * src->height;

	pData = new unsigned char[frame_size];
	memcpy(pData, src->imageData, frame_size);

	cvReleaseImage(&src);  
	return true;
}

void EvaluationImage_new(char* ImagePath, float& brightness, float& contrast, float& zh, char* resolution, int n_type)
{
	IplImage* currImage = cvLoadImage(ImagePath, CV_LOAD_IMAGE_GRAYSCALE);

	if (currImage == NULL)
		return;

	int hIndex = 0, wIndex = 0, ImagePostion = 0, pixNum = 0;

	float totalPixValue = 0.0, tempContrast = 0.0;

	for (hIndex = 0; hIndex < currImage->height; hIndex++)
	{
		ImagePostion = hIndex*currImage->widthStep;

		for (wIndex = 0; wIndex < currImage->width; wIndex++, ImagePostion++)
		{
			totalPixValue += (uchar)currImage->imageData[ImagePostion];
		}
	}

	brightness = totalPixValue / (currImage->width * currImage->height);

	IplImage* AdaptImage = cvCreateImage(cvGetSize(currImage), 8, 1);
	IplImage* currSobelImage = cvCreateImage(cvGetSize(currImage), 8, 1);

	cvThreshold(currImage, AdaptImage, 70, 255, CV_THRESH_BINARY);
	memset(currSobelImage->imageData, 0, currSobelImage->widthStep*currSobelImage->height*sizeof(unsigned char));

	cvSobel(AdaptImage, currSobelImage, 2, 2, 3);

	for (hIndex = 0; hIndex < currSobelImage->height; hIndex++)
	{
		ImagePostion = hIndex*currSobelImage->widthStep;

		for (wIndex = 0; wIndex < currSobelImage->width; wIndex++, ImagePostion++)
		{
			if ((uchar)currSobelImage->imageData[ImagePostion] == 255)
			{
				if ((uchar)currImage->imageData[ImagePostion] > 90)
				{
					tempContrast += (uchar)currImage->imageData[ImagePostion];
				}
				pixNum++;
			}
		}
	}

	if (pixNum == 0)
	{
		contrast = 50;
	}
	else
	{
		tempContrast = tempContrast / pixNum;

		switch (n_type)
		{
		case 0:
			contrast = (tempContrast - 11)  / 22 * 40 + 60; 
			break;
		case 1:
			contrast = (tempContrast - 13) / 13 * 40 + 60;
			break;
		case 2:
			contrast = (tempContrast - 22) / 16 * 40 + 60;
			break;
		default:
			contrast = tempContrast;
			break;
		}

		if (contrast > 100)
			contrast = 100;
	}	

	
	if (brightness - 160 <= 0)
	{
		zh = brightness / 160 * 100;
	}
	else
	{
		zh = (320 - brightness) / 160 * 100;
	}
	zh = (float)(contrast * 0.75 + zh * 0.25);

	sprintf(resolution, "%d x %d", currImage->width, currImage->height);

	cvReleaseImage(&currImage);
	cvReleaseImage(&AdaptImage);
	cvReleaseImage(&currSobelImage);

	return;
}

void EvaluationImage_my(char* ImagePath, float& brightness, float& contrast, float& zh, char* resolution, int n_type)
{
	IplImage* currImage = cvLoadImage(ImagePath, CV_LOAD_IMAGE_GRAYSCALE);

	if (currImage == NULL)
		return;

	int hIndex = 0, wIndex = 0, ImagePostion = 0, pixNum = 0;

	float totalPixValue = 0.0, tempContrast = 0.0;

	for (hIndex = 0; hIndex < currImage->height; hIndex++)
	{
		ImagePostion = hIndex*currImage->widthStep;

		for (wIndex = 0; wIndex < currImage->width; wIndex++, ImagePostion++)
		{
			totalPixValue += (uchar)currImage->imageData[ImagePostion];
		}
	}

	brightness = totalPixValue / (currImage->width * currImage->height);

	IplImage* AdaptImage = cvCreateImage(cvGetSize(currImage), 8, 1);
	cvThreshold(currImage, AdaptImage, 70, 255, CV_THRESH_BINARY);

	IplImage* currSobelImage = cvCreateImage(cvGetSize(currImage), 8, 1);
	memset(currSobelImage->imageData, 0, currSobelImage->widthStep*currSobelImage->height*sizeof(unsigned char));
	cvSobel(AdaptImage, currSobelImage, 2, 2, 3);

	{
		IplImage* AdaptImage = cvCreateImage(cvGetSize(currImage), 8, 1);
		cvThreshold(currImage, AdaptImage, 70, 3, CV_THRESH_BINARY);

		IplImage* currSobelImageXY = cvCreateImage(cvGetSize(currImage), 8, 1);
		memset(currSobelImageXY->imageData, 0, currSobelImageXY->widthStep*currSobelImageXY->height*sizeof(unsigned char));
		cvSobel(AdaptImage, currSobelImageXY, 1, 1);

		IplImage* currSobelImageX = cvCreateImage(cvGetSize(currImage), 8, 1);
		memset(currSobelImageX->imageData, 0, currSobelImageX->widthStep*currSobelImageX->height*sizeof(unsigned char));
		cvSobel(AdaptImage, currSobelImageX, 1, 0);

		IplImage* currSobelImageY = cvCreateImage(cvGetSize(currImage), 8, 1);
		memset(currSobelImageY->imageData, 0, currSobelImageY->widthStep*currSobelImageY->height*sizeof(unsigned char));
		cvSobel(AdaptImage, currSobelImageY, 0, 1);

		cvReleaseImage(&AdaptImage);
	}


	for (hIndex = 0; hIndex < currSobelImage->height; hIndex++)
	{
		ImagePostion = hIndex*currSobelImage->widthStep;

		for (wIndex = 0; wIndex < currSobelImage->width; wIndex++, ImagePostion++)
		{
			if ((uchar)currSobelImage->imageData[ImagePostion] == 255)
			{
				if ((uchar)currImage->imageData[ImagePostion] > 90)
				{
					tempContrast += (uchar)currImage->imageData[ImagePostion];
				}
				pixNum++;
			}
		}
	}

	if (pixNum == 0)
	{
		contrast = 50;
	}
	else
	{
		tempContrast = tempContrast / pixNum;

		switch (n_type)
		{
		case 0:
			contrast = (tempContrast - 11)  / 22 * 40 + 60; 
			break;
		case 1:
			contrast = (tempContrast - 13) / 13 * 40 + 60;
			break;
		case 2:
			contrast = (tempContrast - 22) / 16 * 40 + 60;
			break;
		default:
			contrast = tempContrast;
			break;
		}

		if (contrast > 100)
			contrast = 100;
	}	

	if (brightness - 160 <= 0)
	{
		zh = brightness / 160 * 100;
	}
	else
	{
		zh = (320 - brightness) / 160 * 100;
	}
	zh = (float)(contrast * 0.75 + zh * 0.25);

	sprintf(resolution, "%d x %d", currImage->width, currImage->height);

	cvReleaseImage(&currImage);
	cvReleaseImage(&AdaptImage);
	cvReleaseImage(&currSobelImage);

	return;
}

void NSP_STP_CM::EvaluationImage(char* image_path, float& brightness, float& contrast, float& zh, char* resolution, int n_type)
{
	EvaluationImage_new(image_path, brightness, contrast, zh, resolution, n_type);
	return;

	IplImage* curr_image = cvLoadImage(image_path, CV_LOAD_IMAGE_GRAYSCALE);

	if (curr_image == NULL)
	{
		return;
	}

	int point_num = 0;
	unsigned int total_gray_value = 0;
	float temp_contrast = 0.0;
	uchar max_gray_value = 0;
	uchar min_gray_value = 0xFF;

	for (int h_index = 0; h_index < curr_image->height; h_index++)
	{
		int image_postion = h_index * curr_image->widthStep;

		for (int w_index = 0; w_index < curr_image->width; w_index++, image_postion++)
		{	
			uchar gray_v = (uchar)curr_image->imageData[image_postion];
			total_gray_value += gray_v;
			point_num ++;

			if (curr_image->imageData[image_postion] > max_gray_value)
			{
				max_gray_value = curr_image->imageData[image_postion];
			}

			if (curr_image->imageData[image_postion] < min_gray_value)
			{
				min_gray_value = curr_image->imageData[image_postion];
			}

		}
	}

	brightness = (float)((float)total_gray_value / (float)point_num);

	for (int h_index = 0; h_index < curr_image->height; h_index++)
	{
		int image_postion = h_index * curr_image->widthStep;

		for (int w_index = 0; w_index < curr_image->width; w_index++, image_postion++)
		{	
			uchar gray_v = (uchar)curr_image->imageData[image_postion];
			temp_contrast += pow(abs((float)gray_v - brightness) , 2) ; 
		}
	}

	contrast = sqrt(temp_contrast / point_num);

	sprintf(resolution, "%d x %d", curr_image->width, curr_image->height);

	cvReleaseImage(&curr_image);

	return;

}

int NSP_STP_CM::ImgRectAdd( std::string file_path, int left_up_x, int left_up_y, int width, int height)
{
	int ret = 0;
	IplImage *p_img_src = cvLoadImage(file_path.c_str(), 1);
	if (p_img_src == NULL)
	{
		return -1;
	}

	const int rect_w = width * 3 / 100;

	int right_down_x = left_up_x + rect_w;
	if (right_down_x > width - 1)
	{
		right_down_x = width - 1;
	}
	int right_down_y = left_up_y + rect_w;
	if (right_down_y > height - 1)
	{
		right_down_y = height - 1;
	}

	cvRectangle( p_img_src, cvPoint(left_up_x, left_up_y), cvPoint(right_down_x, right_down_y), cvScalar(255, 255, 255));   

	ret = cvSaveImage(file_path.c_str(), p_img_src);

	cvReleaseImage( &p_img_src );  

	return ret;
}

int NSP_STP_CM::ImgTextAdd( std::string file_path , int left_up_x, int left_up_y, int width, int height, const char* text_add)
{
	int ret = 0;
	IplImage* img = cvLoadImage(file_path.c_str());
	if (!img)
	{
		return -1;
	}

	CvFont * pfont = new CvFont;
	cvInitFont(pfont, CV_FONT_HERSHEY_SIMPLEX, 0.5F, 0.5F);

	if (left_up_y < height / 2)
	{
		left_up_y = height / 4;
	}
	else
	{
		left_up_y = height * 3 / 4;
	}

	if (left_up_x < width / 2)
	{
		left_up_x = width / 4;
	}
	else
	{
		left_up_x = width * 3 / 4;
	}

	cvPutText(img, text_add, cvPoint(left_up_x, left_up_y), pfont, CV_RGB(255, 255, 255));

	ret = cvSaveImage(file_path.c_str(), img);

	delete pfont;
	cvReleaseImage( &img ); 

	return ret;
}

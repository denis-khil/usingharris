
//#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "math.h"
#include <windows.h>
#include "feature_detect.h"
#include "image_grabber.h"


//#include "c-pioneer2at.cpp"



using namespace cv;
using namespace std;

//Глобальные переменные --------
Mat src_first, src_second, src_gray, temp_first, temp_second;
int thresh = 200;
int max_thresh = 255;
int w,h;
int p_w=640;
int p_h=480;
int focus=691; //фокус камеры, равный 690.90
int tran=1; //Расстояние между снимками
double alpha, betta; // углы
double dstn; // расстояние до объекта
feature_detector f_d; 

//Функция ------ 
void My_cornerHarris (int, void* );
void camera();
char* source_window = "Source image";
char* corners_window = "Corners detected";

int main () {

	//start 
	char messgn_forward [20] = "forward 1000"; // Сообщение движения вперед со скоростью 1000
	char messgn_stop [10] = "stop"; //Сообщение стоп
	  //!!!!!!!!!!!!!!
	//Тело- движение->стоп->поиск точек->отправка точек x5
	
	for (int i=0; i<5;i++)
	{
	//	network_listener(messgn_forward);
	//	network_listener(messgn_stop);
	//	camera();
	//	network_listener(/*points?*/); // FORMAT OF MESSAGE? 
	}
	camera();
	
	//	camera (); // Вызов функции camera - поиск углов
		//network_listener (messgn_forward);
	//	Sleep (1000);
		//network_listener(messgn_stop);
		//flag = !flag;
		//camera (flag);
		//My_cornerHarris (0,0);
		
	
}


void camera()
{
	

	//Необходимо подключить камеру
	const int MAX_CAM_CADR = 50;
	CvCapture* capture = 0;
	capture = cvCaptureFromCAM(0);
	//if (!capture) return 1;
	cvNamedWindow( "Motion", 1 );
	IplImage* image[MAX_CAM_CADR];
	CvScalar color;
	CvPoint cv1;
	CvFont font;
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX, 1.3f,
	1.3f,0,1, 8 );
	cv1.x=70; cv1.y=120;
	color = CV_RGB(255,0,255);
	int i,j;
for(;;)
{
	IplImage* image1;
	IplImage* imagesrc;
	if( !cvGrabFrame( capture ))
		break;
	image1 = cvRetrieveFrame( capture );
	w = image1->width;
	h = image1->height;

	cvPutText( image1, "Press, when you ready", cv1, &font, color );
	cvShowImage( "Start", image1);
	//Начинаем по клавише + сохранение картинок - вторая temp для наложения углов на оригинал
	if( cvWaitKey(10) >= 0 )  // Можно условие изменить
	{
		//	src_first = cvRetrieveFrame ( capture ) ;
		//	temp_first = cvRetrieveFrame ( capture ) ; 
		//	IplImage* img3=cvCloneImage(&(IplImage)src_first); //src
		//	cvSaveImage ("E:\\tst_second.jpg", img3);


			imagesrc = cvRetrieveFrame ( capture ) ;
			vector < point_features > tst;
			vector < point_features > tst2;
			vector < point_features > coord;
			const IplImage* im;
			const IplImage* im1;
		//	im = cvRetrieveFrame (capture);
			im = cvLoadImage ("E:\\source_test_first.jpg",1);
			im1 = cvLoadImage ("E:\\source_test_second.jpg",1);
			f_d.add_frame(im,tst,tst2); 
			f_d.add_frame(im1,tst,tst2);
			f_d.coord_search(tst,tst2,coord);	
			break;
	}
	

}
}




void invert(cv::Mat &image) //image.type() == CV_8UC1
{
    for(int y(0); y < image.rows; ++y)
    {
        for(int x(0); x < image.cols; ++x)
            image.at<unsigned char>(y, x) = 255 - image.at<unsigned char>(y, x);
    }
}
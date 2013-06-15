
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


#include "c-pioneer2at.cpp"



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

//Функция ------ 
void My_cornerHarris (int, void* );
void camera(bool);
char* source_window = "Source image";
char* corners_window = "Corners detected";

int main () {

	//start 
	char messgn_forward [20] = "forward 1000"; // Сообщение движения вперед со скоростью 1000
	char messgn_stop [10] = "stop"; //Сообщение стоп
	feature_detector f_d;   //!!!!!!!!!!!!!!
	//Тело- движение->стоп->поиск точек->отправка точек
	/*
	for (int i=0; i<5;i++)
	{
		network_listener(messgn_forward);
		network_listener(messgn_stop);
		camera();
		network_listener(f_d.detected_points); // FORMAT OF MESSAGE? 
	}
	
	*/
		camera (); // Вызов функции camera - поиск углов
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
			imagesrc = cvRetrieveFrame ( capture ) ;
			vector < point_features > tst;
			
			const IplImage* im;
			im = cvRetrieveFrame (capture);
			f_d.add_frame(im,tst,tst); 
			
			break;
	}
	

}
}


// Функция определения углов ---------
void My_cornerHarris (int, void*)
{
	/*IplImage* img;
	img = cvLoadImage ("E:\\test.jpg",1);*/
	IplImage* eig_image = 0;
	IplImage* temp_image = 0;
	src_first = cvLoadImage ("E:\\source_1.jpg",1);
	src_second = cvLoadImage ("E:\\source_2.jpg",1);
	temp_first = cvLoadImage ("E:\\source_1.jpg",1);
	temp_second = cvLoadImage ("E:\\source_2.jpg",1);

	vector < pair <int, int > > point_vector_first;	
	vector < pair <int, int > > point_vector_second;
	vector < pair <int, int > > point_vector_dst;
	/*Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros (src.size(), CV_32FC1);*/
	IplImage* src_gray_first = 0;
	IplImage* src_gray_second = 0;
	IplImage* src_first_=cvCloneImage(&(IplImage)src_first);
	IplImage* src_second_=cvCloneImage(&(IplImage)src_second);
	

	//Определяем изображение и сохраняем его как серое
	if( ! src_gray_first ) {
			src_gray_first = cvCreateImage(cvGetSize(src_first_),IPL_DEPTH_8U, 1);}
	cvCvtColor(src_first_, src_gray_first, CV_BGR2GRAY);
	
	if( ! src_gray_second ) {
			src_gray_second = cvCreateImage(cvGetSize(src_second_),IPL_DEPTH_8U, 1);}
	cvCvtColor(src_second_, src_gray_second, CV_BGR2GRAY);

	//Делаем копии картинок для функции
	if ( !eig_image){
			eig_image = cvCreateImage(cvSize(w, h),IPL_DEPTH_32F, 1);}
	if ( !temp_image){
			temp_image = cvCreateImage(cvSize(w, h),IPL_DEPTH_32F, 1);}

	//Параметры
	/*int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;*/
	const int MAX_CORNERS = 2;
	int corner_count = MAX_CORNERS;
	double quality_level = 0.1;
	double min_distance = 5;
	int eig_block_size = 3;
	int use_harris = true;
	double k = 0.04;
	CvPoint2D32f corners[MAX_CORNERS] = {0};
	//Обнаружение углов
	//cornerHarris ( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );
	cvGoodFeaturesToTrack(src_gray_first,eig_image, temp_image,
			corners, &corner_count, quality_level, min_distance, NULL, eig_block_size,  use_harris, k);
	
	point_vector_first.resize(corner_count);
	//Заполняем контейнер и рисуем кружки
		for( int i = 0; i < corner_count; i++) {
				point_vector_first[i] = pair < int,int > (corners[i].x,corners[i].y);
				circle(temp_first, Point (corners[i].x,corners[i].y),5,Scalar(0),2,8,0);
		}

	cvGoodFeaturesToTrack(src_gray_second,eig_image, temp_image,
			corners, &corner_count, quality_level, min_distance, NULL, eig_block_size,  use_harris, k);
	
	point_vector_second.resize(corner_count);
	//Заполняем контейнер и рисуем кружки
		for( int i = 0; i < corner_count; i++) {
				point_vector_second[i] = pair < int,int > (corners[i].x,corners[i].y);
				circle(temp_second, Point (corners[i].x,corners[i].y),5,Scalar(0),2,8,0);
		}
	//Делать ли цикл для всех U,V?
	
	//Находим углы, из 2х углов находим расстояние до объекта	
	double ttmp = p_w/2 - point_vector_first[0].first; //ищем U
	double tmp = (ttmp) / focus; // alpha = arctg ( u/focus )
	//tmp = 0.026;
	alpha = atan (tmp);
	double ttmpp = p_w/2 - point_vector_second[0].first;
	double tmpp = (ttmpp) / focus;
	//tmpp = 0.0043;
	betta = atan (tmpp);
	dstn = tran * sin(180-betta) / sin(180-alpha+betta);
	

	//Сохраняем картинки	
	IplImage* image3=cvCloneImage(&(IplImage)temp_first); //src
	cvSaveImage ("E:\\result_first.jpg", image3);

	IplImage* image4=cvCloneImage(&(IplImage)temp_second); //src
	cvSaveImage ("E:\\result_second.jpg", image4);

	//Поиск координат
	double x=sin(alpha * dstn);
	double y=cos(alpha * dstn);


}



void invert(cv::Mat &image) //image.type() == CV_8UC1
{
    for(int y(0); y < image.rows; ++y)
    {
        for(int x(0); x < image.cols; ++x)
            image.at<unsigned char>(y, x) = 255 - image.at<unsigned char>(y, x);
    }
}
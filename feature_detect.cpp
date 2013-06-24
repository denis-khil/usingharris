#include<cmath>
#include"feature_detect.h"



int feature_detector::likeness_of_points(const point_features& p1, const point_features& p2){
	/* I. Position */
	int max_allowed_position = 100;
	/* lesser the distance between the points, lesser result*/
	double position_likeness = sqrt((double)(p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
	if(position_likeness>max_allowed_position)
		return 0;
	
	return 100/position_likeness; /* TODO: use sigmoidal curve */
}


/* finds position (pos1,pos2) in matrix similarity with maximal value in it.
 * if all values are zeros, returns 0, else 1 */
int row_with_max_similarity(vector < vector < int > > &similarity, int& pos1, int&pos2){
	int max_val = 0;
	pos1 = -1, pos2 = -1;
	for(int i=0;i<similarity.size();++i)
		for(int j=0;j<similarity[i].size();++j)
			if(similarity[i][j]>max_val){
				pos1 = i;
				pos2 = j;
				max_val = similarity[i][j];}
	
	/* if non-zero value not found return 0;*/
	if(!max_val)
		return 0;
	else{
		/* else set zeros to all cells in row pos1 and call pos2 and return 1*/
		for(int i=0;i<similarity.size();++i)
			similarity[i][pos2]=0;
		for(int j=0;j<similarity[pos1].size();++j)
			similarity[pos1][j]=0;
		return 1;}}

void feature_detector::coord_search(vector < point_features >& p_first, vector < point_features >& p_second, vector < point_features>& p_result)
{
	int p_w=640;
	int p_h=480;
	int focus=691; //фокус камеры, равный 690.90
	double alpha, betta; // углы
	double dstn; // расстояние до объекта
	int tran=1; //Расстояние между снимками
	//Находим углы, из 2х углов находим расстояние до объекта	
	for (int t=0; t<p_first.size();t++){
		/* calculate U */
		/* TODO : locate point on image (left or right) */
		double ttmp = p_w/2 - p_first[t].x; //p_w/2 - 
		ttmp = fabs (ttmp);
		/* alpha = arctg ( u/focus )*/
		double tmp = (ttmp) / focus; 
		//tmp = 0.026;
		alpha = atan (tmp);
		double ttmpp = p_w/2 - p_second[t].x; //p_w/2 - 
		double tmpp = (ttmpp) / focus;
		//tmpp = 0.0043;
		betta = atan (tmpp);
		dstn = tran * sin(betta) / sin(betta-alpha);
		/* Сalculate X,Y */
		double x=sin(alpha * dstn);
		double y=cos(alpha * dstn);
		p_result[t].x=x;
		p_result[t].y=y;
	}



}

void feature_detector::add_frame(const IplImage* src, vector < point_features >& p_old, vector < point_features >& p_new){
	int w,h;
	
	const int MAX_CORNERS = 100;
	int corner_count = MAX_CORNERS;
	double quality_level = 0.1;
	double min_distance = 1;
	int eig_block_size = 3;
	int use_harris = true;
	double k = 0.09;
	
	CvPoint2D32f corners[MAX_CORNERS] = {0};
	
	IplImage* eig_image = 0;
	IplImage* temp_image = 0;
	p_old.clear();
	p_new.clear();
	/* detect points on new image */
	vector < point_features > new_points;
	
	/* DETECTOR */
	
	cv::Mat temp_;
	cv::Mat temp_2;
	temp_=src;
	temp_2=src;
	IplImage* src_gray = 0;
	/*image sizes*/
	w = src->width;
	h = src->height;
	/*convert to gray*/
	if( ! src_gray ) {
			src_gray= cvCreateImage(cvGetSize(src),IPL_DEPTH_8U, 1);}
	cvCvtColor(src, src_gray, CV_BGR2GRAY);
	/*empty copies for function*/
	if ( !eig_image){
			eig_image = cvCreateImage(cvSize(w, h),IPL_DEPTH_32F, 1);}
	if ( !temp_image){
			temp_image = cvCreateImage(cvSize(w, h),IPL_DEPTH_32F, 1);}
	
	cvGoodFeaturesToTrack(src_gray,eig_image, temp_image,
		corners, &corner_count, quality_level, min_distance, NULL, eig_block_size,  use_harris, k);
	/*vector??*/

	std::vector< std::pair<int, int > > point_vector;	
	//p_new.resize(corner_count);
	//Заполняем контейнер и рисуем кружки
	IplImage* image_for_save=cvCloneImage(&(IplImage)temp_); //src
	point_features p_f;
	if (detected_points.size() !=0)
		for (int l=0; l<detected_points.size()-1;l++)		
		cv::circle(temp_, cv::Point (detected_points[l].x,detected_points[l].y),5,cv::Scalar(100),2,8,0);
	
	for( int i = 0; i < corner_count; i++) {
			p_f.x=corners[i].x;
			p_f.y=corners[i].y;
			new_points.push_back(p_f);
			//p_new.push_back (corners[i].x,corners[i].y);
			cv::circle(temp_, cv::Point (corners[i].x,corners[i].y),5,cv::Scalar(0),2,8,0);
			
	}
	image_for_save=cvCloneImage(&(IplImage)temp_); //src
	cvSaveImage ("E:\\impl_im.jpg", image_for_save);

	
	/*End of detector*/

	/*if there wasn't points on previous image, return empty vector */
	if(detected_points.empty()){
		vector < point_features > empt;
		detected_points = new_points;
		}
	
	else{
	/* calculate similarities with greedy algorythm */
	/* 1. create similarities table: rows --- new points, cols --- old ones */
	vector < vector < int > > similarity;
	similarity.resize(new_points.size());
	for(int i=0;i<similarity.size();++i){
		similarity[i].resize(detected_points.size());
		for(int j=0;j<detected_points.size();++j)
			similarity[i][j]=likeness_of_points(new_points[i],detected_points[j]);}
	
	int pos1=-1, pos2 = -1;
	while(row_with_max_similarity(similarity,pos1,pos2)){
		p_new.push_back(new_points[pos1]);
		p_old.push_back(detected_points[pos2]);}
	
	

	if (p_new.size() !=0)
		for (int k=0; k<p_new.size()-1;k++)		
		cv::circle(temp_, cv::Point (p_new[k].x,p_new[k].y),5,cv::Scalar(255,255,255),2,8,0);
	if (p_old.size() !=0)
		for (int j=0; j<p_old.size()-1;j++)		
		cv::circle(temp_, cv::Point (p_old[j].x,p_old[j].y),5,cv::Scalar(0,215,255),2,8,0);
	//IplImage* img_=cvCloneImage(&(IplImage)temp_); 
	//cvSaveImage ("E:\\result_sim.jpg", img_);

	/* replace detected points from the old frame with newer ones*/
	detected_points = new_points;

	for( int i = 0; i < detected_points.size(); i++) {
			cv::Mat im;	
			
			cv::circle(temp_2, cv::Point (detected_points[i].x,detected_points[i].y),5,cv::Scalar(0),2,8,0);
	}
		if (p_new.size() !=0)
		for (int n=0; n<p_new.size()-1;n++)		
		{
			cv::Point p1;
			cv::Point p2;
			p1.x=p_new[n].x;
			p1.y=p_new[n].y;
			p2.x=p_old[n].x;
			p2.y=p_old[n].y;
			
			cv::line(temp_,p1,p2,cv::Scalar(0,0,255),2,8,0);
		}
	

	IplImage* img_sim=cvCloneImage(&(IplImage)temp_2); //src
	cvSaveImage ("E:\\result_sim.jpg", img_sim);}
}
#include<cmath>
#include"feature_detect.h"


int feature_detector::likeness_of_points(const point_features& p1, const point_features& p2){
	/* I. Position */
	int max_allowed_position = 20;
	/* lesser the distance between the points, lesser result*/
	double position_likeness = sqrt((double)(p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
	if(position_likeness>max_allowed_position)
		position_likeness=0;
	
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

void feature_detector::add_frame(const IplImage* src, vector < point_features >& p_old, vector < point_features >& p_new){
	int w,h;
	
	const int MAX_CORNERS = 100;
	int corner_count = MAX_CORNERS;
	double quality_level = 0.5;
	double min_distance = 2;
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
	temp_=src;
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
	cvSaveImage ("E:\\result_first.jpg", image_for_save);
	point_features p_f;
	for( int i = 0; i < corner_count; i++) {
			p_f.x=corners[i].x;
			p_f.y=corners[i].y;
			new_points.push_back(p_f);
			//p_new.push_back (corners[i].x,corners[i].y);
			cv::circle(temp_, cv::Point (corners[i].x,corners[i].y),5,cv::Scalar(0),2,8,0);
	}
	image_for_save=cvCloneImage(&(IplImage)temp_); //src
	cvSaveImage ("E:\\result_first.jpg", image_for_save);

	
	/*End of detector*/

	/*if there wasn't points on previous image, return empty vector */
	if(detected_points.empty()){
		vector < point_features > empt;
		detected_points = new_points;
		//return empt;}
	}
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
	
	/* replace detected points from the old frame with newer ones*/
	detected_points = new_points;}
#include "image_grabber.h"

/* capture from camera initialization */
ImageFromCameraGrabber::ImageFromCameraGrabber(int camera_num){
	capture = cvCaptureFromCAM(camera_num);}

/* get frame from camera source*/
IplImage* ImageFromCameraGrabber::get(){
	if(cvGrabFrame(capture))
		return cvRetrieveFrame(capture);
	else
		return NULL;}

/* release camera capture */
ImageFromCameraGrabber::~ImageFromCameraGrabber(){
	cvReleaseCapture( &capture );}


/* load all images from "files" vector */
ImageFromImageFilesGrabber::ImageFromImageFilesGrabber(const vector <std::string> & files){
	current_file=0;
	list_of_files.clear();
	list_of_files.resize(files.size());
	for(int i=0; i<files.size();++i)
		if(! (list_of_files[i] = cvLoadImage(files[i].c_str())))
			throw "Input file not found";}

/* get image from sequence */
IplImage* ImageFromImageFilesGrabber::get(){
	/* get current position*/
	int old_curr = current_file;
	/* renew position */
	current_file=(current_file+1)%list_of_files.size();
	/* return copy of image */
	return cvCloneImage(list_of_files[old_curr]);}

/* destroy sequnce */
ImageFromImageFilesGrabber::~ImageFromImageFilesGrabber(){
	for(int i=0;i<list_of_files.size();++i)
		cvReleaseImage(&list_of_files[i]);}
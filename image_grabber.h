#ifndef _IMAGE_GRABBER_H_
#define _IMAGE_GRABBER_H_

#include <vector>
#include <opencv2/opencv.hpp>
using std::vector;

/* TODO: error processing, exceptions etc. */

/* interface for image grabber */
class ImageGrabber{
  public:
	virtual IplImage* get() = 0;
	virtual ~ImageGrabber(){}};

/* image grabber from camera source */
class ImageFromCameraGrabber: public ImageGrabber{
	CvCapture* capture;
  public:
	ImageFromCameraGrabber(int camera_num = 0);
	IplImage* get();
	virtual ~ImageFromCameraGrabber();};

/* image grabber from video file --- doesn't finished yet */	
class ImageFromVideoGrabber: public ImageGrabber{
	//CvCapture* capture;
  public:
	ImageFromVideoGrabber(const char *filename);
	IplImage* get();
	virtual ~ImageFromVideoGrabber();};	

/* image from separate image files grabber */	
class ImageFromImageFilesGrabber: public ImageGrabber{
	/* images in seqence*/
	vector <IplImage*> list_of_files;
	/* position of current image in sequence*/
	int current_file;
  public:
	ImageFromImageFilesGrabber(const vector <std::string> & files);
	IplImage* get();
	virtual ~ImageFromImageFilesGrabber();};

#endif //_IMAGE_GRABBER_H_
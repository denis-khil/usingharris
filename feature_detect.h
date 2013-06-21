#ifndef _FEATURE_DETECT_H_
#define _FEATURE_DETECT_H_

#include<vector>
#include<opencv2\opencv.hpp>
#include"image_grabber.h"
using std::vector;


/* container for the features of the detected point 
 * TODO: average brightness in the local area */
struct point_features{
	int x;
	int y;
};

class feature_detector{
  public:
	feature_detector(){};
	~feature_detector(){};
	
  public:
	/* points from previous iteration */
	vector < point_features > detected_points;
	/* calculates likeness of p1 and p2 (minimal value = 0)*/
	int likeness_of_points(const point_features& p1, const point_features& p2);
	/* get new image;
	 * returns similarities between current and last image in sequence */
	void add_frame(const IplImage*, vector < point_features >& p_old, vector < point_features >& p_new);
	/* calculates coords x,y */
	void feature_detector::coord_search(vector < point_features >& p_first, vector < point_features >& p_second, vector < point_features>& p_result);
};


#endif
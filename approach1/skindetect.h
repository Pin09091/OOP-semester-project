#pragma once

#include<opencv2\opencv.hpp>

using namespace cv;
using namespace std;

//IMPORTANT: header only contains the decleration of functions and attributes, they will be initalized in the .cpp file

class SkinDetector {			//creates skin detector class
public:
	SkinDetector(void);			// empty default constructor

	void drawSkinColorSampler(Mat input);		//idfk ???? probably declaring functions to use later?????
	void calibrate(Mat input);					//yup, functions to use later
	Mat getSkinMask(Mat input);

private:
	int hLowThreshold = 0;								//default threshholds values "magic numbers" if you will
	int hHighThreshold = 0;
	int sLowThreshold = 0;
	int sHighThreshold = 0;
	int vLowThreshold = 0;
	int vHighThreshold = 0;

	bool calibrated = false;								// flag for calibration

	Rect skinColorSamplerRectangle1, skinColorSamplerRectangle2;	// creates two rectangles around skin sample 1 and 2

	void calculateThresholds(Mat sample1, Mat sample2);				//?????
	void performOpening(Mat binaryImage, int structuralElementShapde, Point structuralElementSize);
};
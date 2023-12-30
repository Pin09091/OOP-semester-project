#include "skindetect.h"
#include"opencv2\opencv.hpp"



SkinDetector::SkinDetector(void) { //constructing le objects constructor
	hLowThreshold = 0;			// all the thresholds for skin detection
	hHighThreshold = 0;
	sLowThreshold = 0;
	sHighThreshold = 0;
	vLowThreshold = 0;
	vHighThreshold = 0;

	calibrated = false;

	skinColorSamplerRectangle1, skinColorSamplerRectangle2; //the rectangles
}

void SkinDetector::drawSkinColorSampler(Mat input) {		
	int frameWidth = input.size().width, frameHeight = input.size().height; // height and length of the input array

	int rectangleSize = 20;							//size of rectangle
	Scalar rectangleColor = Scalar(255, 0, 255);	//color

	skinColorSamplerRectangle1 = Rect(frameWidth / 5, frameHeight / 2, rectangleSize, rectangleSize);	//coordinates for the rectangle
	skinColorSamplerRectangle2 = Rect(frameWidth / 5, frameHeight / 3, rectangleSize, rectangleSize);

	rectangle(		//actually makes the rectangle
		input,
		skinColorSamplerRectangle1,
		rectangleColor
	);

	rectangle(	//makes the 2nd rectangle
		input,
		skinColorSamplerRectangle2,
		rectangleColor
	);
}

void SkinDetector::calibrate(Mat input) { //conversion to HSV and then calculating the thresholds from the samples

	Mat hsvInput; // array declared
	cvtColor(input, hsvInput, COLOR_BGR2HSV);// converts the input array from Blue green red to HSV format and stores in hsvInput

	Mat sample1 = Mat(hsvInput, skinColorSamplerRectangle1);	//rectangles
	Mat sample2 = Mat(hsvInput, skinColorSamplerRectangle2);

	calculateThresholds(sample1, sample2);						// declared later

	calibrated = true;											//enables flag for completed calibration
}

void SkinDetector::calculateThresholds(Mat sample1, Mat sample2) {	// caclulates thresholds	
	int offsetLowThreshold = 80; //hard coded
	int offsetHighThreshold = 30;

	Scalar hsvMeansSample1 = mean(sample1);	//takes the mean value for the sample 
	Scalar hsvMeansSample2 = mean(sample2);

	hLowThreshold = min(hsvMeansSample1[0], hsvMeansSample2[0]) - offsetLowThreshold;	//funny math
	hHighThreshold = max(hsvMeansSample1[0], hsvMeansSample2[0]) + offsetHighThreshold;

	sLowThreshold = min(hsvMeansSample1[1], hsvMeansSample2[1]) - offsetLowThreshold;
	sHighThreshold = max(hsvMeansSample1[1], hsvMeansSample2[1]) + offsetHighThreshold;

	// the V channel shouldn't be used. By ignorint it, shadows on the hand wouldn't interfire with segmentation.
	// Unfortunately there's a bug somewhere and not using the V channel causes some problem. This shouldn't be too hard to fix.
	vLowThreshold = min(hsvMeansSample1[2], hsvMeansSample2[2]) - offsetLowThreshold;
	vHighThreshold = max(hsvMeansSample1[2], hsvMeansSample2[2]) + offsetHighThreshold;
	//vLowThreshold = 0;
	//vHighThreshold = 255;
}

Mat SkinDetector::getSkinMask(Mat input) {//creating the mask or layer over the skin
	Mat skinMask;

	if (!calibrated) { // if it isnt calibrated it will return an empty matrix of the same size as the input
		skinMask = Mat::zeros(input.size(), CV_8UC1);
		return skinMask;
	}

	Mat hsvInput;
	cvtColor(input, hsvInput, COLOR_BGR2HSV); //version error fixed

	inRange(//comparison for the arrays
		hsvInput,
		Scalar(hLowThreshold, sLowThreshold, vLowThreshold),
		Scalar(hHighThreshold, sHighThreshold, vHighThreshold),
		skinMask);

	performOpening(skinMask, MORPH_ELLIPSE, { 3, 3 }); //magic
	dilate(skinMask, skinMask, Mat(), Point(-1, -1), 3);//also magic

	return skinMask;
}

void SkinDetector::performOpening(Mat binaryImage, int kernelShape, Point kernelSize) { // no clue to be honest
	Mat structuringElement = getStructuringElement(kernelShape, kernelSize);
	morphologyEx(binaryImage, binaryImage, MORPH_OPEN, structuringElement);
}
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "BGremover.h"
#include "skindetect.h"
#include "facedetect.h"
#include "fingie.h"

using namespace cv;
using namespace std;

int main(int, char**) {
	VideoCapture videoCapture(0);
	videoCapture.set(0, 1);// version conflict CV_CAP_PROP_SETTINGS replaced with 0

	if (!videoCapture.isOpened()) {
		cout << "Can't find camera!" << endl;
		return -1;
	}

	Mat frame, frameOut, handMask, foreground, fingerCountDebug;

	BackgroundRemover backgroundRemover;
	SkinDetector skinDetector;
	FaceDetector faceDetector;
	FingerCount fingerCount;

	while (true) {
		videoCapture >> frame;
		frameOut = frame.clone();

		skinDetector.drawSkinColorSampler(frameOut);

		foreground = backgroundRemover.getForeground(frame);




		
		
		faceDetector.removeFaces(frame, foreground);
		//faceDetector.removeFaces(frame, frameOut);
		
		handMask = skinDetector.getSkinMask(foreground);
		

		//temp handMask
		//handMask = skinDetector.getSkinMask(frameOut);
		//remove this later 


		fingerCountDebug = fingerCount.findFingersCount(handMask, frameOut);

		imshow("output", frameOut);
		imshow("foreground", foreground);
		imshow("handMask", handMask);
		imshow("handDetection", fingerCountDebug);

		int key = waitKey(1);

		if (key == 27) // esc
			break;
		else if (key == 98) // b
			backgroundRemover.calibrate(frame);/**/
		else if (key == 115) // s
			skinDetector.calibrate(frame);
		
	}

	return 0;
}
/*#include <opencv2/opencv.hpp>
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>


#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	
	//turns on the camera
	VideoCapture videoCapture(0);
	videoCapture.set(0, 1);// version conflict CV_CAP_PROP_SETTINGS replaced with 0

	if (!videoCapture.isOpened()) 
	{
		cout << "Can't find camera!" << endl;
		return -1;
	}
	

	Mat frame,frameOut;//to store image data


	while (true)
	{
		videoCapture >> frame;//gives camera data to frame
		frameOut = frame.clone();//makes a copy of that in frameout


		imshow("test", frameOut);//outputs frameOut in a window



		int key = waitKey(1); // to close stuff
		if (key == 27) // esc
			break;
	}
	
	
	
	
	
	return 0;
}*/
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main2()
{
	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) { cout << "Open failed! Please check your camera or files!" << endl; return -1; }

	double scale = 1.5;


	while (1)
	{
		Mat frame, dstMat;

		cap >> frame;
		Size resimgsize = Size(frame.cols*scale, frame.rows*scale);
		Mat rFrame = Mat(resimgsize, frame.type());
		resize(frame, rFrame, resimgsize, INTER_LINEAR);

		blur(rFrame, dstMat,Size(3,3),Point(-1,-1),BORDER_DEFAULT );



		imshow("¾ùÖµÂË²¨", dstMat);

		imshow("frame", rFrame);

		//When key: 'Esc'pressed, exit.
		if (waitKey(30) == 27) { destroyAllWindows(); return 0; }
	}

}


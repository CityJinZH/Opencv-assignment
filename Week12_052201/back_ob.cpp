#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main() {
	int cnt = 0;
	Mat frame;
	Mat bgMat;
	Mat subMat;
	Mat bny_subMat;



	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) { cout << "Open failed! Please check your camera or files!" << endl; return -1; }

	while (1) {
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);

		if (cnt == 0) {
			//background
			frame.copyTo(bgMat);
		}

		else {

			//±³¾°²î·Ö
			absdiff(frame, bgMat, subMat);
			threshold(subMat, bny_subMat, 45, 255, CV_THRESH_BINARY);

			imshow("b_subMat", bny_subMat);
			imshow("subMat", subMat);
			waitKey(30);
		}
		cnt++;
	}



	return 0;
}


#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) { cout << "Open failed! Please check your camera or files!" << endl; return -1; }

	double scale = 1.5;


	while (1)
	{
		Mat frame, x_dstMat, y_dstMat;
		Mat srcGray,binMat;

		cap >> frame;
		Size resimgsize = Size(frame.cols*scale, frame.rows*scale);
		Mat rFrame = Mat(resimgsize, frame.type());
		resize(frame, rFrame, resimgsize, INTER_LINEAR);

		cvtColor(rFrame, srcGray, CV_RGB2GRAY);
		threshold(srcGray, binMat, 0, 255, CV_THRESH_OTSU);


		Sobel(binMat, x_dstMat, CV_16SC1, 1, 0, 3);  // 水平方向微分
		Sobel(binMat, y_dstMat, CV_16SC1, 0, 1, 3);  // 竖直方向微分

		convertScaleAbs(x_dstMat, x_dstMat);
		convertScaleAbs(y_dstMat, y_dstMat);


		imshow("水平边缘提取", x_dstMat);
		imshow("竖直边缘提取", y_dstMat);


		imshow("frame", rFrame);

		//When key: 'Esc'pressed, exit.
		if (waitKey(30) == 27) { destroyAllWindows(); return 0; }
	}

}


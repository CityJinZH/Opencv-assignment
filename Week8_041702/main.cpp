#include <opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

int main()
{
	Mat srcMat = imread("D:\\install\\data\\die_on_chip.png", 0);
	Mat dspMat = imread("D:\\install\\data\\die_on_chip.png");
	Mat binaryMat;


	//二值化
	threshold(srcMat, binaryMat, 150, 255, THRESH_OTSU);

	Mat element = getStructuringElement(MORPH_RECT, Size(8, 9));
	morphologyEx(binaryMat, binaryMat, MORPH_OPEN, element);

	vector<vector<Point>> contours;
	findContours(binaryMat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point());
	cout << "num=" << contours.size() << endl;

	float rate[100];

	for (int i = 0; i < contours.size(); i++)
	{
		RotatedRect rect = minAreaRect(contours[i]);
		Point2f P[4];
		rect.points(P);
		// 矩形参数设置
		float Y = sqrt((P[0].y - P[1].y) * (P[0].y - P[1].y) + (P[0].x - P[1].x) * (P[0].x - P[1].x));
		float X = sqrt((P[1].y - P[2].y) * (P[1].y - P[2].y) + (P[1].x - P[2].x) * (P[1].x - P[2].x));
		rate[i] = X / Y;
		cout << "num=" << i << "rate of rect =" << rate[i] << endl;
		if ((rate[i] >= 0.8) and (rate[i] <= 1.0))
		{
			for (int j = 0; j <= 3; j++)
			{
				line(dspMat, P[j], P[(j + 1) % 4], Scalar(255), 1);
			}
		}
	}

	imshow("srcMat", srcMat);
	imshow("dspMat", dspMat);
	imshow("binaryMat", binaryMat);
	waitKey(0);
	return 0;
}
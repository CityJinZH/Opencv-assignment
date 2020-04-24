#include <opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

int main()
{
	Mat srcMat = imread("D:\\install\\data\\topic1.JPG");
	Mat binaryMat, hsvMat, dspMat;

	if (!srcMat.data) { cout << "Open failed, please check your Picture's address" << endl; return -1; }
	resize(srcMat, srcMat, Size(1008, 756));
	srcMat.copyTo(dspMat);

	double i_minh = 0, i_maxh = 20;
	double i_mins = 90, i_maxs = 255;
	double i_minv = 70, i_maxv = 255;
	cvtColor(srcMat, hsvMat, COLOR_BGR2HSV);

	inRange(hsvMat, Scalar(i_minh, i_mins, i_minv), Scalar(i_maxh, i_maxs, i_maxv), binaryMat);

	Mat element = getStructuringElement(MORPH_RECT, Size(25, 25));
	morphologyEx(binaryMat, binaryMat, MORPH_CLOSE, element);
	//Mat element = getStructuringElement(MORPH_RECT, Size(25, 25));
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
		// rate of rect 
		float Y = sqrt((P[0].y - P[1].y) * (P[0].y - P[1].y) + (P[0].x - P[1].x) * (P[0].x - P[1].x));
		float X = sqrt((P[1].y - P[2].y) * (P[1].y - P[2].y) + (P[1].x - P[2].x) * (P[1].x - P[2].x));
		rate[i] = X / Y;
		cout << "num=" << i << "rate of rect =" << rate[i] << endl;
		if (abs(rate[i] - 1) < 0.2)
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
	imwrite("dspMat.jpg", dspMat);
	waitKey(0);
	return 0;
}

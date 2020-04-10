#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main()
{
	Mat src_M = imread("D:\\install\\data\\metal-part-distorted-03.png"), g_Mat, e_Mat;
	imshow("src", src_M);
	Canny(src_M, e_Mat, 50, 200, 3);

	cvtColor(src_M, g_Mat, COLOR_BGR2GRAY);

	Mat line_Mat;
	HoughLinesP(e_Mat, line_Mat, 1, CV_PI / 180, 32, 40, 15);
	for (int i = 0; i < line_Mat.rows; i++)
	{
		Vec4i  l = line_Mat.at<Vec4i>(i);
		line(src_M, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 255, 255), 1, CV_AA);
	}

	imshow("result", src_M);

	waitKey(0);
}
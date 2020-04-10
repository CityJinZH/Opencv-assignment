#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int main()
{
	Mat srcMat = imread("D:\\install\\data\\lena.jpg");
	Mat dstMat;

	int width = srcMat.cols;
	int height = srcMat.rows;
	//define angle\scale\center to get Affine  matrix
	float angle = -10.0, scale = 1;
	Point2f center(srcMat.cols*0.5, srcMat.rows*0.5);
	//获得变换矩阵
	Mat affine_matrix = getRotationMatrix2D(center, angle, scale);
	

	float  cos = abs(affine_matrix.at<double>(0, 0)), sin = abs(affine_matrix.at<double>(0, 1));
	int new_Width = int((height * sin) + (width * cos));
	int new_Height = int((height * cos) + (width * sin));

	affine_matrix.at<double>(1, 2) += (new_Width / 2) - width / 2;
	affine_matrix.at<double>(0, 2) += (new_Height / 2) - height / 2;

	//将计算好的新尺寸应用
	warpAffine(srcMat, dstMat, affine_matrix, Size(new_Width, new_Height));
	imshow("src", srcMat);
	imshow("dst", dstMat);
	
	waitKey(0);
	return 0;
}
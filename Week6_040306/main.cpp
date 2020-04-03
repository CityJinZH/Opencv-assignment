#include <opencv2/opencv.hpp>
#include <iostream>


using namespace cv;
using namespace std;

int main()
{
	cv::Mat dstMat;
	cv::Mat srcMat = cv::imread("D:\\install\\data\\lena.jpg");


	//��ת30��
	float angle = 30.0;
	//��ת��Ҫ��ĳߴ�
	int height = srcMat.rows;
	int width = srcMat.cols;
	Size resimgsize = Size(width*cos(angle)+height*sin(angle), width*sin(angle) + height * cos(angle));

	//��ת����Ϊͼ������
	cv::Point2f center(srcMat.cols*0.5, srcMat.rows*0.5);
	//��ñ任����
	const cv::Mat affine_matrix = cv::getRotationMatrix2D(center, angle, 1);

	cv::warpAffine(srcMat, dstMat, affine_matrix, resimgsize);

	cv::imshow("src", srcMat);
	cv::imshow("dst", dstMat);
	cv::waitKey(0);
}
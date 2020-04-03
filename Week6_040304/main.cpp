#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	cv::Mat dstMat;
	cv::Mat srcMat = cv::imread("D:\\install\\data\\lena.jpg");
	//变换前的四点坐标
	const cv::Point2f src_pt[] = {
									cv::Point2f(150,150),
									cv::Point2f(150,300),
									cv::Point2f(350,300),
									cv::Point2f(350,150)};
	//变换后的四点坐标
	const cv::Point2f dst_pt[] = {
									cv::Point2f(200,150),
									cv::Point2f(200,300),
									cv::Point2f(340,270), 
									cv::Point2f(340,180)};



	//获得投影变换矩阵
	cv::Mat perspect_matrix = cv::getPerspectiveTransform(src_pt, dst_pt);
	//变换
	cv::warpPerspective(srcMat, dstMat, perspect_matrix, srcMat.size());


	cv::imshow("src", srcMat);
	cv::imshow("dst", dstMat);
	cv::waitKey(0);
}
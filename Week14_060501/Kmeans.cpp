#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main()
{
	int n = 0;
	const int MAX_CLUSTERS = 4;
	Mat mask = imread("D:\\Install\\data\\movie.jpg");
	Mat data, labels;
	Vec3b colorTab[] =
	{
		Vec3b(255,255, 255),
		Vec3b(0, 0, 0),
		Vec3b(255, 0, 255),
		Vec3b(0, 255, 255)
	};
	for (int i = 0; i < mask.rows; i++)
		for (int j = 0; j < mask.cols; j++)
		{
			Vec3b point = mask.at<Vec3b>(i, j);
			Mat tmp = (Mat_<float>(1, 3) << point[0], point[1], point[2]);
			data.push_back(tmp);
		}


	//聚类算法实现
	kmeans(data, 2, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0), 3, KMEANS_RANDOM_CENTERS);
	for (int i = 0; i < mask.rows; i++)
		for (int j = 0; j < mask.cols; j++)
		{
			int clusterIdx = labels.at<int>(n);
			mask.at<Vec3b>(i, j) = colorTab[clusterIdx];
			n++;
		}


	//显示结果图
	imshow("mask_picture", mask);
	waitKey(0);

	return 0;
}
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main()
{
	Mat src, dst;
	src = imread("D:\\Install\\data\\gtest.jpg");
	imshow("src", src);


	//三通道均衡化
	Mat Array[3];
	split(src, Array);
	for (int i = 0; i < 3; i++)
	{
		equalizeHist(Array[i], Array[i]);
	}

	merge(Array, 3, dst);
	imshow("equal_dst", dst);

	waitKey(0);
	return 0;
}




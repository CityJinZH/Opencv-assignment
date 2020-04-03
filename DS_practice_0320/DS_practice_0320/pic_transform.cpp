#include <opencv2/opencv.hpp>
#include<iostream>
#include<math.h>

using namespace cv;
using namespace std;




int main1(){
	

		cv::Mat binaryMat;
		cv::Mat srcMat = imread("D:\\Install\\data\\coin.png", 0);
		cv::Mat dst1; //腐蚀
		cv::Mat dst2; //膨胀
		cv::Mat dst3;//开运算
		cv::Mat dst4;//闭运算
		Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));


		//二值化
		cv::threshold(srcMat, binaryMat, 0, 255, THRESH_OTSU);

		erode(binaryMat, dst1, element);
		dilate(binaryMat, dst2, element);
		morphologyEx(binaryMat, dst3, MORPH_OPEN, element);
		morphologyEx(binaryMat, dst4, MORPH_CLOSE, element);

		imshow("Binary", binaryMat);
		imshow("腐蚀",dst1);
		imshow("膨胀", dst2);
		imshow("开运算", dst3);
		imshow("闭运算", dst4);

		waitKey(0);

	
	
}
	

	


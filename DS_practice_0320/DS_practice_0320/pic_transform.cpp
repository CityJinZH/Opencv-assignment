#include <opencv2/opencv.hpp>
#include<iostream>
#include<math.h>

using namespace cv;
using namespace std;




int main1(){
	

		cv::Mat binaryMat;
		cv::Mat srcMat = imread("D:\\Install\\data\\coin.png", 0);
		cv::Mat dst1; //��ʴ
		cv::Mat dst2; //����
		cv::Mat dst3;//������
		cv::Mat dst4;//������
		Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));


		//��ֵ��
		cv::threshold(srcMat, binaryMat, 0, 255, THRESH_OTSU);

		erode(binaryMat, dst1, element);
		dilate(binaryMat, dst2, element);
		morphologyEx(binaryMat, dst3, MORPH_OPEN, element);
		morphologyEx(binaryMat, dst4, MORPH_CLOSE, element);

		imshow("Binary", binaryMat);
		imshow("��ʴ",dst1);
		imshow("����", dst2);
		imshow("������", dst3);
		imshow("������", dst4);

		waitKey(0);

	
	
}
	

	


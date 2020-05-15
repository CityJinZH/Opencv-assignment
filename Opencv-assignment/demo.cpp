#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;



void RGBHIST(Mat a, string b)
{
	vector<Mat> bgr_planes;                 // ��������
	split(a, bgr_planes);                 // ����ͨ��ͼ���Ϊ�����ͨ��ͼ��

	int histSize = 256;

	float range[] = { 0,256 };
	const float* histRange = { range };     // �����const�����򱨴�

	bool uniform = true;                    // ֱ��ͼ�Ƿ�ֲ�����
	bool accumulate = false;                // ���Ϊtrue���ڿ�ʼ����ʱֱ��ͼ���ᱻ���

	Mat b_hist, g_hist, r_hist;

	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	int hist_w = 512, hist_h = 400;                     // ֱ��ͼ�ĺ��ݷ�Χ
	int bin_w = cvRound((double)hist_w / histSize);     // ÿ����״�Ŀ��

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	normalize(b_hist, b_hist, 9, histImage.rows, NORM_MINMAX, -1, Mat());           // ��һ��
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histSize; i++)                                              // ����ֱ��
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow(b, histImage);
}

int main()
{
	Mat src, dst;
	src = imread("D:\\Install\\data\\gtest.jpg");
	imshow("src", src);

	RGBHIST(src, "src clhist");

	//���⻯
	Mat Array[3];
	split(src, Array);
	for (int i = 0; i < 3; i++)
	{
		equalizeHist(Array[i], Array[i]);
	}

	merge(Array, 3, dst);
	imshow("equal_dst", dst);
	RGBHIST(dst, "dst clhist");


	waitKey(0);
	return 0;
}

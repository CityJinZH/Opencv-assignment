#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


void mark_the_fire(Mat &frame) {
	Mat gray_frame, hsv_frame, fire_Mat, S_Mat;
	vector<Mat> planes;
	// ת��Ϊ�Ҷȸ�ʽ��HSVɫ�ʿռ�
	cvtColor(frame, gray_frame, CV_BGR2GRAY);
	cvtColor(frame, hsv_frame, CV_BGR2HSV);
	// ��HSVɫ�ʿռ��3ͨ������
	split(hsv_frame, planes);
	// ͨ��Sͨ���ͻҶ�ֵ�жϻ���λ��
	S_Mat = planes[1].clone();
	fire_Mat = Mat::zeros(frame.size(), gray_frame.type());
	// ɨ��ͷ������½�0.75-1.0
	for (int r = S_Mat.rows * 0.75; r < S_Mat.rows; r++) {
		for (int c = S_Mat.cols * 0.75; c < S_Mat.cols; c++) {
			// ���Ҷ�ֵ���ڵ���150��Sͨ������120ʱ��Ϊ������Ϊ��������
			if ((gray_frame.at<uchar>(r, c) >= 150 && S_Mat.at<uchar>(r, c) >= 120)) {
				fire_Mat.at<uchar>(r, c) = 255;
			}
		}
	}
	// ���Ͳ���
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	dilate(fire_Mat, fire_Mat, element, Point(-1, -1), 2);
	// Ѱ�һ����������ͨ��
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(fire_Mat, contours, RETR_LIST, CHAIN_APPROX_NONE);
	// ��������
	Mat result_Mat;
	result_Mat = frame.clone();
	for (int i = 0; i < contours.size(); i++) {
		// ʹ����ת����
		RotatedRect rbox = minAreaRect(contours[i]);
		Point2f vtx[4];
		rbox.points(vtx);
		// ����������
		for (int j = 0; j < 4; ++j) {
			cv::line(frame, vtx[j], vtx[j < 3 ? j + 1 : 0], Scalar(0, 0, 255), 1, LINE_AA);
		}
	}
}

vector<Point> get_points(Mat frame, Mat background) {
	Mat gray_frame;
	gray_frame = frame.clone();
	// �������
	Mat result;
	absdiff(gray_frame, background, result);
	// �������ಿ��
	// ��Ƶ��ࡢ���Ͻ��Լ��²��Ա�����ֵ��������źܴ�ÿһ֡��ֱ�ӽ���Ҷ���0
	for (int r = 0; r < result.rows; r++) {
		for (int c = 0; c < 180; c++) {
			result.at<uchar>(r, c) = 0;
		}
	}
	for (int r = 0; r < 90; r++) {
		for (int c = 320; c < 480; c++) {
			result.at<uchar>(r, c) = 0;
		}
	}
	for (int r = 125; r < 265; r++) {
		for (int c = 175; c < 270; c++) {
			result.at<uchar>(r, c) = 0;
		}
	}
	// �����ͼ���ֵ���������б�����
	threshold(result, result, 110, 255, THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(result, result, MORPH_CLOSE, element, Point(-1, -1), 3);
	vector<Point> points;
	// ɨ���ֵͼ�񣬽�����������������ĵ㱣��
	for (int r = 30; r < 230; r++) {
		for (int c = 170; c < 430; c++) {
			if (result.at<uchar>(r, c) == 255) {
				points.push_back(cv::Point(c, r));
				break;
			}
		}
	}
	// �������е�
	return points;
}

void fit_the_curve(vector<Point> points, Mat& A, int n) {
	// ��¼ˮ���ϵ������
	int num = points.size();
	// ����ϵ������
	Mat C = Mat::zeros(n + 1, n + 1, CV_64FC1);
	for (int i = 0; i < n + 1; i++) {
		for (int j = 0; j < n + 1; j++) {
			for (int k = 0; k < num; k++) {
				C.at<double>(i, j) = C.at<double>(i, j) +
					std::pow(points[k].x, i + j);
			}
		}
	}
	//�������Y
	cv::Mat Y = cv::Mat::zeros(n + 1, 1, CV_64FC1);
	for (int i = 0; i < n + 1; i++) {
		for (int k = 0; k < num; k++) {
			Y.at<double>(i, 0) = Y.at<double>(i, 0) +
				std::pow(points[k].x, i) * points[k].y;
		}
	}

	A = cv::Mat::zeros(n + 1, 1, CV_64FC1);
	//������A
	cv::solve(C, Y, A, cv::DECOMP_LU);
}


int main() {
	
	VideoCapture video("D:\\install\\data\\Original_video.mp4");


	// ����Ϊ��ǰ֡����������ǰ֡��RGB��ʽ����ǰ֡��RGB��ʽ
	Mat frame, background, RGB_Mat;
	// ֡������
	int frame_cnt = 0;
	// ѭ����ȡ��Ƶ
	while (video.read(frame)) {
		RGB_Mat = frame.clone();
		// ����ǰ֡�ҶȻ�
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		// ��һ֡ʱ��ǻ��沢����֡��Ϊ����
		if (frame_cnt == 0) {
			background = frame.clone();
			mark_the_fire(RGB_Mat);
			imshow("Detector", RGB_Mat);
			waitKey(10);
		}
		// 80֮֡ǰˮǹδ���䣬ֻ��ǻ���
		else if (frame_cnt <= 80) {
			mark_the_fire(RGB_Mat);
			imshow("Detector", RGB_Mat);
			waitKey(10);
		}
		// 80֮֡�����ˮ���켣
		else {
			// ��ȡˮ���ϵĵ�
			vector<Point> key_points = get_points(frame, background);
			Mat A;
			fit_the_curve(key_points, A, 3);
			// �����ϵ������������ߵĵ�����Ϣ
			vector<Point> points_fitted;
			for (int x = 170; x < 430; x++) {
				double y = A.at<double>(0, 0) + A.at<double>(1, 0) * x +
					A.at<double>(2, 0) * std::pow(x, 2) + A.at<double>(3, 0) * std::pow(x, 3);
				points_fitted.push_back(cv::Point(x, y));
			}
			// �ȱ�ǻ���
			mark_the_fire(RGB_Mat);
			// �ٻ��ƹ켣
			polylines(RGB_Mat, points_fitted, false, cv::Scalar(0, 255, 255), 2, 8, 0);
			imshow("Detector", RGB_Mat);
			waitKey(10);
		}
		// ֡����������
		frame_cnt++;
	}
	return 0;
}

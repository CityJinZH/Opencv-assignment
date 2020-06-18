#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


void mark_the_fire(Mat &frame) {
	Mat gray_frame, hsv_frame, fire_Mat, S_Mat;
	vector<Mat> planes;
	// 转换为灰度格式和HSV色彩空间
	cvtColor(frame, gray_frame, CV_BGR2GRAY);
	cvtColor(frame, hsv_frame, CV_BGR2HSV);
	// 将HSV色彩空间的3通道分离
	split(hsv_frame, planes);
	// 通过S通道和灰度值判断火焰位置
	S_Mat = planes[1].clone();
	fire_Mat = Mat::zeros(frame.size(), gray_frame.type());
	// 扫描头像的右下角0.75-1.0
	for (int r = S_Mat.rows * 0.75; r < S_Mat.rows; r++) {
		for (int c = S_Mat.cols * 0.75; c < S_Mat.cols; c++) {
			// 若灰度值大于等于150且S通道大于120时认为该像素为火焰区域
			if ((gray_frame.at<uchar>(r, c) >= 150 && S_Mat.at<uchar>(r, c) >= 120)) {
				fire_Mat.at<uchar>(r, c) = 255;
			}
		}
	}
	// 膨胀操作
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	dilate(fire_Mat, fire_Mat, element, Point(-1, -1), 2);
	// 寻找火焰区域的连通域
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(fire_Mat, contours, RETR_LIST, CHAIN_APPROX_NONE);
	// 绘制轮廓
	Mat result_Mat;
	result_Mat = frame.clone();
	for (int i = 0; i < contours.size(); i++) {
		// 使用旋转矩形
		RotatedRect rbox = minAreaRect(contours[i]);
		Point2f vtx[4];
		rbox.points(vtx);
		// 绘制四条边
		for (int j = 0; j < 4; ++j) {
			cv::line(frame, vtx[j], vtx[j < 3 ? j + 1 : 0], Scalar(0, 0, 255), 1, LINE_AA);
		}
	}
}

vector<Point> get_points(Mat frame, Mat background) {
	Mat gray_frame;
	gray_frame = frame.clone();
	// 背景差分
	Mat result;
	absdiff(gray_frame, background, result);
	// 消除多余部分
	// 视频左侧、右上角以及下部对背景差分的噪声干扰很大，每一帧中直接将其灰度置0
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
	// 将差分图像二值化，并进行闭运算
	threshold(result, result, 110, 255, THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(result, result, MORPH_CLOSE, element, Point(-1, -1), 3);
	vector<Point> points;
	// 扫描二值图像，将搜索区域中最上面的点保留
	for (int r = 30; r < 230; r++) {
		for (int c = 170; c < 430; c++) {
			if (result.at<uchar>(r, c) == 255) {
				points.push_back(cv::Point(c, r));
				break;
			}
		}
	}
	// 返回所有点
	return points;
}

void fit_the_curve(vector<Point> points, Mat& A, int n) {
	// 记录水柱上点的数量
	int num = points.size();
	// 构造系数矩阵
	Mat C = Mat::zeros(n + 1, n + 1, CV_64FC1);
	for (int i = 0; i < n + 1; i++) {
		for (int j = 0; j < n + 1; j++) {
			for (int k = 0; k < num; k++) {
				C.at<double>(i, j) = C.at<double>(i, j) +
					std::pow(points[k].x, i + j);
			}
		}
	}
	//构造矩阵Y
	cv::Mat Y = cv::Mat::zeros(n + 1, 1, CV_64FC1);
	for (int i = 0; i < n + 1; i++) {
		for (int k = 0; k < num; k++) {
			Y.at<double>(i, 0) = Y.at<double>(i, 0) +
				std::pow(points[k].x, i) * points[k].y;
		}
	}

	A = cv::Mat::zeros(n + 1, 1, CV_64FC1);
	//求解矩阵A
	cv::solve(C, Y, A, cv::DECOMP_LU);
}


int main() {
	
	VideoCapture video("D:\\install\\data\\Original_video.mp4");


	// 依次为当前帧、背景、当前帧的RGB格式、当前帧的RGB格式
	Mat frame, background, RGB_Mat;
	// 帧计数器
	int frame_cnt = 0;
	// 循环读取视频
	while (video.read(frame)) {
		RGB_Mat = frame.clone();
		// 将当前帧灰度化
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		// 第一帧时标记火焰并将该帧作为背景
		if (frame_cnt == 0) {
			background = frame.clone();
			mark_the_fire(RGB_Mat);
			imshow("Detector", RGB_Mat);
			waitKey(10);
		}
		// 80帧之前水枪未发射，只标记火焰
		else if (frame_cnt <= 80) {
			mark_the_fire(RGB_Mat);
			imshow("Detector", RGB_Mat);
			waitKey(10);
		}
		// 80帧之后绘制水柱轨迹
		else {
			// 获取水柱上的点
			vector<Point> key_points = get_points(frame, background);
			Mat A;
			fit_the_curve(key_points, A, 3);
			// 用拟合系数计算绘制曲线的点阵信息
			vector<Point> points_fitted;
			for (int x = 170; x < 430; x++) {
				double y = A.at<double>(0, 0) + A.at<double>(1, 0) * x +
					A.at<double>(2, 0) * std::pow(x, 2) + A.at<double>(3, 0) * std::pow(x, 3);
				points_fitted.push_back(cv::Point(x, y));
			}
			// 先标记火焰
			mark_the_fire(RGB_Mat);
			// 再绘制轨迹
			polylines(RGB_Mat, points_fitted, false, cv::Scalar(0, 255, 255), 2, 8, 0);
			imshow("Detector", RGB_Mat);
			waitKey(10);
		}
		// 帧计数器递增
		frame_cnt++;
	}
	return 0;
}

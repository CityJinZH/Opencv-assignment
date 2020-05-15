#include <opencv2/opencv.hpp>
#include <iostream>


void gamma_correction(cv::Mat& src, cv::Mat& dst, float K) {
	uchar LUT[256];
	src.copyTo(dst);
	for (int i = 0; i < 256; i++) {
		float f = i / 255.0;
		f = pow(f, K);
		LUT[i] = cv::saturate_cast<uchar>(f*255.0);
	}

	
		cv::MatIterator_<cv::Vec3b> flag = dst.begin<cv::Vec3b>();
		cv::MatIterator_<cv::Vec3b> end = dst.end<cv::Vec3b>();

		for (; flag != end; ++flag) {
			(*flag)[0] = LUT[(*flag)[0]];
			(*flag)[1] = LUT[(*flag)[1]];
			(*flag)[2] = LUT[(*flag)[2]];
		}


}

int main() {
	cv::Mat src = cv::imread("D:\\Install\\data\\face.jpg");
	cv::Mat dst;


	gamma_correction(src, dst, 0.4);    //gamma±ä»»


	cv::imshow("src", src);
	cv::imshow("dst", dst);

	cv::waitKey(0);
}
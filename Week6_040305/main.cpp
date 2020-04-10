#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int main()
{
	Mat src = imread("D:\\install\\data\\lena_rot.jpg");
	Mat dstMat, g_Mat, b_Mat;

	cvtColor(src, g_Mat, CV_RGB2GRAY);
	threshold(g_Mat, b_Mat, 0, 255, THRESH_OTSU);
	/*确定原图长宽*/
	int width, height;
	width = src.cols;
	height = src.rows;


	cout << "height: " << height << endl;
	cout << "width:  " << width << endl;

	int i, flag_left, flag_right, flag_up, flag_down;

	for (i = 0; i < width - 1; i++)
	{
		if (b_Mat.at<uchar>(i, 0) == 255 && b_Mat.at<uchar>(i + 1, 0) != 255)
		{
			flag_left = i + 1;   //left point
		}
		if (b_Mat.at<uchar>(i, width - 1) == 255 && b_Mat.at<uchar>(i + 1, width - 1) != 255)
		{
			flag_right = i + 1;   //right point
		}
	}

	for (i = 0; i < height - 1; i++)
	{
		if (b_Mat.at<uchar>(1, i) == 255 && b_Mat.at<uchar>(1, i + 1) != 255)
		{
			flag_up = i + 1; //up
		}
		if (b_Mat.at<uchar>(height - 1, i) == 255 && b_Mat.at<uchar>(height - 1, i + 1) != 255)
		{
			flag_down = i + 1;
			break;
		}
	}
	//变换前的四点坐标
	const Point2f src_pt[] = {
		Point2f(flag_up,0),
		Point2f(flag_down,height - 1),
		Point2f(0,flag_left),
		Point2f(float(width - 1),flag_right) };
	//变换后的四点坐标
	const Point2f dst_pt[] = {
		Point2f(0,0),
		Point2f(float(width - 1),float(height - 1)),
		Point2f(0,float(height - 1)),
		Point2f(float(width - 1),0) };
	//变换矩阵
	Mat perspective_matrix = getPerspectiveTransform(src_pt, dst_pt);

	warpPerspective(src, dstMat, perspective_matrix, src.size(), INTER_CUBIC);

	imshow("src1", src);
	imshow("dst", dstMat);
	// Wait for exit  ; When key: 'Esc'pressed, exit.
	while (1) { if (waitKey(30) == 27) { destroyAllWindows(); break; } }
	return 0;
}
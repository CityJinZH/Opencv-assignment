#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int main()
{
	Mat frame, g_Mat, canny_temp, canny1, canny2;
	Mat grad_x, grad_y;
	VideoCapture cap;

	cap.open(0);
	while (1)
	{
		bool rSucess = cap.read(frame);
		if (!rSucess)
		{
			cout << "ÉãÏñÍ·µ÷È¡Ê§°Ü" << std::endl;
			break;
		}
		else
		{
			cvtColor(frame, g_Mat, COLOR_BGR2GRAY);

			blur(g_Mat, canny_temp, Size(3, 3));
			Canny(canny_temp, canny1, 3, 9, 3);

			Sobel(g_Mat, grad_x, CV_16S, 1, 0, 3, 1, 1);
			Sobel(g_Mat, grad_y, CV_16S, 0, 1, 3, 1, 1); 

			Canny(grad_x, grad_y, canny2, 3, 9);

			imshow("canny1 ", canny1);
			imshow("canny2 ", canny2);
		}
		waitKey(0);
	}
	return 0;
}
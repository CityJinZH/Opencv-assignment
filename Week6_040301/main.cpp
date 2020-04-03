#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int main()
{
	Mat frame, rframe,rrframe;
	VideoCapture cap;
	
	int ksize = 3;
	cap.open(0);
	if (!cap.isOpened()) { cout << "ÉãÏñÍ·¼ÓÔØÊ§°Ü" << endl; return -1; }

	while (1)
	{
		bool rSucess = cap.read(frame);
		if (!rSucess)
		{
			cout << "Í¼ÏñÎ´±»¶ÁÈ¡" << std::endl;
			break;
		}
		else
		{
			medianBlur(frame, rframe, ksize);
			imshow("frame ", rframe);
		}
		waitKey(0);
	}
	return 0;
}
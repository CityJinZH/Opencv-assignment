#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat binaryMat, labelMat, statsMat, centrMat, resultMat;
	Mat srcMat = imread("D:\\Install\\data\\clip.PNG", 0);



	bitwise_not(srcMat, srcMat);
	threshold(srcMat, binaryMat, 0, 255, THRESH_OTSU);


	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(7, 6));
	morphologyEx(binaryMat, binaryMat, MORPH_OPEN, element);
	dilate(binaryMat,binaryMat, element);
	erode(binaryMat, binaryMat, element);

	int nComp = connectedComponentsWithStats(binaryMat, labelMat, statsMat, centrMat, 8, CV_32S);
	int count = 0;
	//¥Ú”°–≈œ¢
	for (int i = 0; i < nComp; i++)
	{
		if ((statsMat.at <int>(i, 2) < 50) or (statsMat.at <int>(i, 3) < 50))
			continue;
		else
			count++;
		cout << "connected Components NO. " << i << endl;
		cout << "pixels = " << statsMat.at<int>(i, 4) << endl;
		cout << "width = " << statsMat.at<int>(i, 2) << endl;
		cout << "height = " << statsMat.at<int>(i, 3) << endl;
		cout << endl;
	}
	cout << "the total of connected Components = " << count - 1 << endl;  //exclude background

	// BBX
	for (int i = 1; i < nComp; i++)
	{
		Rect bbx;
		bbx.x = statsMat.at<int>(i, 0);
		bbx.y = statsMat.at<int>(i, 1);
		bbx.width = statsMat.at<int>(i, 2);
		bbx.height = statsMat.at<int>(i, 3);
		rectangle(binaryMat, bbx, CV_RGB(255, 255, 255), 1, 8, 0);
	}


	imshow("binaryMat", binaryMat);
	imshow("frame", srcMat);
	imwrite("result.jpg", binaryMat);
	waitKey(0);
	return 0;
}
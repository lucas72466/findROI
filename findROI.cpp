#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<math.h>


using namespace cv;
using namespace std;

Vec3b target = Vec3b(73, 32, 23);


bool finderror(const Vec3b&color) {
	int range = 10;
	if (abs(color[2] - target[2]) > range) {
		return false;
	}
	if (abs(color[1] - target[1]) > range) {
		return false;
	}
	if (abs(color[0] - target[0]) > range) {
		return false;
	}
	return true;
}

int main() {
	int LowH = 100;
	int HighH = 130;

	int LowS = 175;
	int HighS = 255;

	int LowV = 240;
	int HighV = 255;
	Mat src = imread("blue2.jpg");
	Mat imgHSV;
	vector<Mat> hsvSplit;
	cvtColor(src, imgHSV, COLOR_BGR2HSV); 

	split(imgHSV, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, imgHSV);
	Mat imgThresholded;

	inRange(imgHSV, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), imgThresholded); 

	Mat element = getStructuringElement(MORPH_RECT, Size(7, 7));
	morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);

	morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);

	imshow("Thresholded Image", imgThresholded); 
	imshow("Original", src); 


	src = imgThresholded;
	//cvtColor(src, src, COLOR_RGB2GRAY);
	    Mat matSrc = src;
		GaussianBlur(matSrc, matSrc, Size(5, 5), 0);
		vector<vector<Point> > contours;//contours�����ͣ�˫�ص�vector
		vector<Vec4i> hierarchy;//Vec4i��ָÿһ��vectorԪ�������ĸ�int�����ݡ�
		//��ֵ
		threshold(matSrc, matSrc, 21, 255, THRESH_BINARY);
		//Ѱ������������ע�⣬findContours���������Ҫ���Ƕ�ֵͼ�񣬶�ֵͼ�����Դ���������֣���һ����threshold���ڶ�����canny
		findContours(matSrc.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		/// �����
		vector<Moments> mu(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mu[i] = moments(contours[i], false);
		}
		///  �������ľ�:
		vector<Point2f> mc(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}
		/// ��������
		Mat drawing = Mat::zeros(matSrc.size(), CV_8UC1);
		for (int i = 0; i < contours.size(); i++)
		{ 
			double area = contourArea(contours[i], true);
			if (area <-200) {
				Scalar color = Scalar(255);
				drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
				circle(drawing, mc[i], 4, color, -1, 8, 0);
			}
			else {
				cout << "interfere" << endl;
			}

		}
		imshow("outImage", drawing);
		waitKey();
		return 0;



	

}



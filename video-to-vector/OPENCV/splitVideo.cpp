#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>

using namespace cv;
using namespace std;
RNG rng2(12345);

int selector;

void videoWithCanny() {
	int index = 0;
	VideoCapture cap("test-cases/ball-straight/ball-straight.mp4");
	if (!cap.isOpened()) {  // check if we succeeded
		cout << "Could not open file";
	}
	double count = cap.get(CV_CAP_PROP_FRAME_COUNT);

	namedWindow("Video with Canny", CV_WINDOW_AUTOSIZE);

	while (1) {
		index++;
		Mat frame;
		cap >> frame;
		cvtColor(frame, frame, CV_BGR2GRAY);
		blur(frame, frame, Size(3, 3));

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		/// Detect edges using canny
		Canny(frame, frame, 35, 35 * 3, 3);
		/// Find contours
		findContours(frame, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);


		for (int i = 0; i< contours.size(); i++) {
			drawContours(frame, contours, i, Scalar(255, 255, 255), 2, 8, hierarchy, 0, Point());
		}

		imshow("Video with Canny", frame);
		waitKey(33);
	}
}

void exportSingleFrame(int frameNum) {
	Mat src = imread("test-cases/ball-straight/frames/" + to_string(frameNum) + ".jpg", 1);
	ofstream myfile;
	myfile.open("test-cases/ball-straight/frames/csv/" + to_string(frameNum) + ".csv");
	
	cvtColor(src, src, CV_BGR2GRAY);
	blur(src, src, Size(3, 3));

	vector<vector<Point> > contours;
	vector<Point> endpoints;
	vector<Vec4i> hierarchy;
	int endpointCounter = 0;

	/// Detect edges using canny
	Canny(src, src, 35, 35 * 3, 3);
	/// Find contours
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size() - 1; j++) {
			//FIND DISTANCE FROM NEXT POINT 
			double distance = sqrt(pow((contours[i][j+1].x - contours[i][j].x), 2) + pow((contours[i][j+1].y - contours[i][j].y),2));
			//STORE IN A NEW ARRAY ONLY IF IT MEETS THRESHOLD
			if (distance > 5) {
				endpoints.push_back(Point(contours[i][j + 1].x, contours[i][j + 1].y));

				endpointCounter++;
				endpoints.push_back(Point(contours[i][j].x, contours[i][j].y));

				endpointCounter++;
			}
		}
	}
	
	//I have an array that looks like this [[(135,160)], [], [], []]. Now I need to take every 3 points and derive the control points.

	myfile.close();
	cout << "Exported";
	waitKey(0);
}

void playOriginalVideo() {
	int index = 0;
	VideoCapture cap("test-cases/ball-straight/ball-straight.mp4");
	if (!cap.isOpened()) {  // check if we succeeded
		cout << "Could not open file";
	}
	double count = cap.get(CV_CAP_PROP_FRAME_COUNT);

	namedWindow("Video with Canny", CV_WINDOW_AUTOSIZE);

	while (1) {
		index++;
		Mat frame;
		cap >> frame;

		imshow("Original Video", frame);
		waitKey(33);
	}
}

int main() {
	cout << "(1) Play video with Canny" << endl << "(2) Export single frame endpoints to .CSV file" << endl << "(3) Play Original Video" << endl;

	cin >> selector;
	
	switch (selector){
		case 1:
			videoWithCanny();
			break;
		case 2:
			int whatFrame;
			cin >> whatFrame;
			exportSingleFrame(whatFrame);
			break;
		case 3:
			playOriginalVideo();
		default:
			cout << "Not an option.";
			break;
	}
	
	system("PAUSE");
	

	return 0;
}
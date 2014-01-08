#include <iostream>
#include <vector>
#include <stdio.h>
#include "CameraView.h";
#include "Goal.h";
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int count_ = 0;
int x = 0;
int y = 0;

void CameraView::Tor(){
	count_++;
	cout << count_ << ":0" << endl;
}

void CameraView::getCameraScreen(int pt1x, int pt1y, int pt2x, int pt2y) {
	cout << "Tor gefunden. \nAnpfiff!\n\nSpielstand:" << endl;

	static int posX, posY, lastX, lastY;
	namedWindow("Live");
	namedWindow("Balltracking");

	Mat cameraFrames;
	Mat treshFrames;
	Mat resutantFrame;

	int rows = cameraFrames.rows;
	int cols = cameraFrames.cols;
	Size s = cameraFrames.size();
	rows = s.height;
	cols = s.width;

	VideoCapture capture(1); //webcam

	CvMoments cMoment; 
	capture >> cameraFrames;
	assert(cameraFrames.type() == CV_8UC3);

	Mat *treshFrame = new Mat(cameraFrames.rows, cameraFrames.cols, CV_8UC3, Scalar(0,100,0));

	while (true) {
		Mat videoFrame;
		bool success = capture.read(videoFrame);
		if (success == false) {
			break;
		}
		capture >> cameraFrames; 

		// Gauss-Weichzeichner
		GaussianBlur(cameraFrames, treshFrames, Size(11, 11), 0, 0);

		// roter Farbbereich des Balls
		inRange(treshFrames, Scalar(79, 33, 177), Scalar(113, 79, 255), treshFrames);		

		// Ermittlung der Ballposition
		cMoment = moments(treshFrames); 
		double moment10 = cvGetSpatialMoment(&cMoment, 1, 0);
		double moment01 = cvGetSpatialMoment(&cMoment, 0, 1);
		double area = cvGetCentralMoment(&cMoment, 0, 0);		
		lastX = posX;
		lastY = posY;
		posX = moment10 / area;
		posY = moment01 / area; 

		Rect rect1;

		if (posX > 0 && posY > 0 && lastX > 0 && lastY >> 0) {
			//CameraView::ballCenterPosition(posX, posY);

			rect1.x = pt1x;
			rect1.y = pt1y;
			rect1.width = pt2x - pt1x ;
			rect1.height = pt2y - pt1y;

			// Pruefung, ob Ball im Tor ist
			if (rect1.contains(Point(posX, posY))){
				// Pruefung ob Kießling Tor
				if (lastY >= rect1.y + rect1.height){
					putText(cameraFrames, "TOOOR", cvPoint(140,300), FONT_HERSHEY_COMPLEX_SMALL, 5, cvScalar(255,100,0), CV_AA, 2);
					Tor();
				}
			}

			stringstream s;
			s << count_;
			s << ":0";
			//Spielstand auf Rasen
			putText(cameraFrames, s.str(), cvPoint(15,rect1.y + rect1.height-5), FONT_HERSHEY_PLAIN, 4, cvScalar(255,255,255), 3, CV_AA);
			//Toraus
			rectangle(*treshFrame, Point(0, 0), Point(cameraFrames.cols, rect1.y + rect1.height), CV_RGB(100,100,100), 2); 
			//Tor
			rectangle(*treshFrame, Point(pt1x, pt1y), Point(pt2x, pt2y), CV_RGB(255,255,255), 2); 
			rectangle(cameraFrames, Point(rect1.x, rect1.y), Point(rect1.x + rect1.width, rect1.y + rect1.height), CV_RGB(255,0,0), 2);
			//Spielfeld
			rectangle(*treshFrame, Point(0, rect1.y + rect1.height), Point(cameraFrames.cols, cameraFrames.rows), CV_RGB(255,255,255), 5);
			//Ball
			circle(*treshFrame, Point(posX, posY), 15, Scalar(0, 180, 180), 2);
			line(*treshFrame, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(0, 200, 255), 3); 
			line(cameraFrames, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(0, 255, 255), 3); 
		}

		imshow("Balltracking", *treshFrame);
		imshow("Live", cameraFrames);
		add(cameraFrames, *treshFrame, resutantFrame);
		waitKey(1);

	}
}

/*
void CameraView::ballCenterPosition(int x, int y) {

}
*/
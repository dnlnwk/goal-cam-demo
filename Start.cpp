#include <opencv2/opencv.hpp>
#include "Start.h"
#include "CameraView.h";

using namespace cv;
using namespace std;



void Start::showScreen(){
	CameraView cameraView;
	cameraView.getCameraScreen();
}

void Start::createGUI(){
	
	Mat image(700,700, CV_8UC3);
	namedWindow("Start");
	image.setTo(Scalar (255,255,255));
	imshow("Start", image);
	waitKey(0);
	
}

void settings(int state, void* userdata){
	cout << "hi" << endl;
}

void Start::createMenu(){
	createButton("Settings",settings);
}


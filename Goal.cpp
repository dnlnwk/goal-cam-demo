#include <opencv2/opencv.hpp>
#include "Goal.h"
#include "CameraView.h";
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;


int Goal::goal(int Ballx, int Bally){
	
	VideoCapture videocapture(1); // Öffne Webcam
	if(!videocapture.isOpened()){  // Schaue ob Webcam geöffnet
	cout << "Kamera nicht gefunden. Bitte überprüfen sie den Anschluss" << endl; //Fange Fehler
	return (-1);
	}

	//Erstelle Fenster mit Einstellungsreglern 
	namedWindow("Einstellungen");
	createTrackbar("Linienfilter", "Einstellungen", 0, 255);
	setTrackbarPos("Linienfilter", "Einstellungen", 103); 
	createTrackbar("Helligkeit", "Einstellungen", 0, 255);
	setTrackbarPos("Helligkeit", "Einstellungen", 42);
	createTrackbar("Kontrast", "Einstellungen", 0, 200);
	setTrackbarPos("Kontrast", "Einstellungen", 112);
	cout << "Suche Tor ..." << endl; 

	while(true){
		Mat kameraFrame;
		Mat grausBild;
		Mat cannyFilter;
		//Mat konturen;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		vector<vector<Point> > contours_poly(contours.size());
		int thresh;
		int max_thresh = 255;
		int Helligkeit = getTrackbarPos("Helligkeit", "Einstellungen");
		float Kontrast = getTrackbarPos("Kontrast", "Einstellungen");

		// Neues kameraFrame bekommen
		videocapture >> kameraFrame;

		//Helligkeit + Kontrast des Bildes für Trackbar 
		kameraFrame = kameraFrame + Scalar(Helligkeit, Helligkeit, Helligkeit);
		kameraFrame.convertTo(kameraFrame, -1, Kontrast/100, 0);
		thresh  = getTrackbarPos("Linienfilter", "Einstellungen");

		// Framebild grau machen und "Blureffekt" setzen
		cvtColor(kameraFrame, grausBild, CV_BGR2GRAY );
		blur(grausBild, grausBild, Size(3,3) );

		// Benutze Canny um Kanten zu finden
		Canny( grausBild, cannyFilter, thresh, thresh*2, 3 );

		// Konturen finden
		findContours(cannyFilter, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		// Moments speichern
		vector<Moments> mo(contours.size() );
		for( int i = 0; i < contours.size(); i++ ){ 
			mo[i] = moments( contours[i], false ); 
		}

		// Mitten speichern

		vector<Point2f> areas( contours.size() );
		for( int i = 0; i < contours.size(); i++ ){ 
			areas[i] = Point2f( mo[i].m10/mo[i].m00 , mo[i].m01/mo[i].m00 ); 
		}

		// Konturen zeichnen
		Mat konturen = Mat::zeros( cannyFilter.size(), CV_8UC3 );

		for( int i = 0; i< contours.size(); i++ ){  
			if ((contourArea(contours[i]) >= 1000)){
				Rect rect = boundingRect(contours[i]);
				Point pt1, pt2;
				pt1.x = rect.x;
				pt1.y = rect.y;
				pt2.x = rect.x + rect.width;
				pt2.y = rect.y + rect.height;
				Point P(Ballx,Bally);

				// Größe des Tores ca: X 213 - 218  // Y = 60-65
				if((pt2.x-pt1.x >= 210) && (pt2.x-pt1.x <= 220) && (pt2.y - pt1.y >= 55) && (pt2.y - pt1.y <= 70)){
					rectangle(konturen, pt1, pt2, CV_RGB(255,0,0), 2);
					drawContours(konturen, contours, i, Scalar (255,255,0), CV_FILLED, 8, hierarchy, 0, Point() );	

					//Schließe Fenster
					cvDestroyWindow("konturen");
					cvDestroyWindow("LiveBild");
					cvDestroyWindow("Einstellungen");

					//Springe zu CameraView
					CameraView cameraView;
					cameraView.getCameraScreen(pt1.x, pt1.y, pt2.x, pt2.y);
				}
			}
			else{			 
				drawContours(konturen, contours, i, Scalar(0,200,0), CV_FILLED , 8, hierarchy, 0, Point() );
			}
		}
		namedWindow( "konturen", 1 );
		imshow( "konturen", konturen );
		imshow("LiveBild", kameraFrame);
		if (waitKey (30) >= 0){ 
			break;
		}
	}
	return 0;
}

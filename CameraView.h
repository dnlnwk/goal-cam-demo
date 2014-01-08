#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H
#include <opencv2/opencv.hpp>

class CameraView {
private:

public:
	void getCameraScreen(int pt1x = 0, int pt1y = 0, int pt2x = 0, int pt2y = 0);
	//	void ballCenterPosition(int x = 0, int y = 0);
	void Tor();

};
#endif
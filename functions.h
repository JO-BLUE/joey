#ifndef FUNCTIONS_H
#define FUNCTIONS_H
// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <cmath>
#include <string>
#include <vector>
#include "Armor_Lamp.h"
#include <iostream>
using namespace std;
using namespace cv;


class functions
{
public:
    Mat preprocessing(Mat img);
    vector<Light> findLights(const Mat & img, const cv::Mat & imgDil);
    vector<Armor> matchLights(const std::vector<Light> & lights);

  bool isLight(const Light & light);
  bool containLight(const Light & light_1, const Light & light_2, const vector<Light> & lights);
  bool isArmor(Armor & armor);
};


#endif // FUNCTIONS_H

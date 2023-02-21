#include "functions.h"
#include "Armor_Lamp.h"
#include <cmath>

using namespace cv;
using namespace std;

//Avoid over-exposure and do not use channel separation
///////////////    Preprocessing     ///////////////   img-gray-binary
Mat preprocessing(Mat img){

//  pick threshold maximum
//  240
//  int min_lightness = 0;
//  namedWindow("TrackBar", WINDOW_NORMAL);
//  createTrackbar("min_lightness", "TrackBar", &min_lightness, 255);

    Mat imgGray, imgTh, imgDil;
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    cv::threshold(imgGray, imgTh, 240, 255, THRESH_BINARY);
    Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    //dilation
    dilate(imgTh, imgDil, element);
    return imgDil;

//  imshow("img",img);
//  imshow("imgGray",imgGray);
//  imshow("imgTh",imgTh);
//  cout<<min_lightness<<endl;

}


/////////////////    findLights     ///////////////    len&&wid&&color
vector<Light> findLights(const Mat& img, const Mat& imgTh)
{
    //find contours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(imgTh, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    //set vector for Lights
    vector<Light> lights;

    for (const vector<Point>& contour : contours) {
    //screen!
        if (contour.size() < 5) continue;
        float lightContourArea = contourArea(contour);
        //cout<<lightContourArea<<endl;
        if(lightContourArea < 200) continue;
        RotatedRect r_rect = minAreaRect(contour);
        auto light = Light(r_rect);

        auto rect = light.boundingRect();
        if (
              //check len\wid && pick color
            0 <= rect.x && 0 <= rect.width && rect.x + rect.width <= img.cols && 0 <= rect.y &&
            0 <= rect.height && rect.y + rect.height <= img.rows) {
            int sum_r = 0, sum_b = 0;
            auto roi = img(rect);
            //judge ROI
            for (int i = 0; i < roi.rows; i++) {
                for (int j = 0; j < roi.cols; j++) {
                    //check if is in the contours
                    if (pointPolygonTest(contour, Point2f(j + rect.x, i + rect.y), false) >= 0) {
                        sum_r += roi.at<cv::Vec3b>(i, j)[0];
                        sum_b += roi.at<cv::Vec3b>(i, j)[2];
                    }
                }
            }
            //judge color = enermy color  How can this be happen!!!!
            light.color = sum_r < sum_b ? RED : BLUE;
            if(light.color == RED)
                putText(img, "!!!", light.center, FONT_HERSHEY_COMPLEX, 1.25, Scalar(0, 69, 255), 2);
            if(light.color == enermy){
                lights.emplace_back(light);
            }
            }
        }
    //test png successfully!
    //for check!
    //cout<<"lights.size = "<<lights.size()<<endl;

    return lights;
}



///////////////    containLights     /////////////// check whether there is another light in the rect formed by two lights
bool containLight(const Light& light_1, const Light& light_2, const vector<Light>& lights)
   {
       auto points = vector<Point2f>{ light_1.top, light_1.bottom, light_2.top, light_2.bottom };
       auto bounding_rect = boundingRect(points);

       for (const auto& light : lights) {
           if (light.center == light_1.center || light.center == light_2.center) continue;

           if (
               bounding_rect.contains(light.top) || bounding_rect.contains(light.bottom) ||
               bounding_rect.contains(light.center)) {
               return true;
           }
       }
       return false;
   }

///////////////    isArmor     ///////////////  raito\distance\angle
bool isArmor(Armor& armor)
   {
       Light light_1 = armor.left_light;
       Light light_2 = armor.right_light;
       bool sameHeight = light_1.center.y - light_2.center.y < 35 && light_1.center.y - light_2.center.y > -35;
       bool goodRatio = light_1.length / (light_2.center.x - light_1.center.x ) > 0.3 && light_1.length / (light_2.center.x - light_1.center.x ) < 3;

       // judge angle proximity
       float angle1 = tan(light_1.length - light_1.width);
       float angle2 = tan(light_2.length - light_2.width);
       bool sameAngle = angle1 - angle2 < 1 && angle1 - angle2 > -1;
       bool isArmor = goodRatio && sameHeight && sameAngle;
       return isArmor;
   }


///////////////    matchLights     ///////////////     match two lights
vector<Armor> matchLights(const vector<Light>& lights)
{
    vector<Armor> armors;

    // Loop
    for (auto light_1 = lights.begin(); light_1 != lights.end(); light_1++) {
        for (auto light_2 = light_1 + 1; light_2 != lights.end(); light_2++) {
            if (light_1->color != enermy || light_2->color != enermy) continue;

            if (containLight(*light_1, *light_2, lights)) {
                continue;
            }
            auto armor = Armor(*light_1, *light_2);
            if (isArmor(armor)) {
                armors.emplace_back(armor);
            }
        }
    }

    return armors;
}







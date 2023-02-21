//opencv
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
//std
#include <algorithm>
#include <string>
using namespace std;

#ifndef ARMOR_LAMP_H
#define ARMOR_LAMP_H

    const int RED = 0;
    const int BLUE = 1;
    const int enermy = RED;
    enum ArmorType { SMALL = 0, LARGE = 1 };

    class Light : public RotatedRect
    {
    public:
        Light() = default;

        //Avoid transfering
        explicit Light(cv::RotatedRect box) : cv::RotatedRect(box)
        {
            cv::Point2f p[4];
            box.points(p);
            sort(p, p + 4, [](const cv::Point2f& a, const cv::Point2f& b) { return a.y < b.y; });
            top = (p[0] + p[1]) / 2;
            bottom = (p[2] + p[3]) / 2;

            length = cv::norm(top - bottom);
            width = cv::norm(p[0] - p[1]);

            tilt_angle = atan2(std::abs(top.x - bottom.x), std::abs(top.y - bottom.y));
            tilt_angle = tilt_angle / CV_PI * 180;
        }

        int color;
        Point2f top, bottom;
        double length;
        double width;
        float tilt_angle;
    };

    class Armor
    {
    public:
        Armor() = default;
        // adjust four points
        Armor(const Light& l1, const Light& l2)
        {
            if (l1.center.x < l2.center.x) {
                left_light = l1, right_light = l2;
            }
            else {
                left_light = l2, right_light = l1;
            }
            center = (left_light.center + right_light.center) / 2;
        }

        Light left_light, right_light;
        Point2f center;

//        cv::Mat number_img;
//        char number;
//        // std::string number;
//        float similarity;
//        float confidence;
//        std::string classfication_result;
        ArmorType armor_type;
    };


#endif // ARMOR_LAMP_H



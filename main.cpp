#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "Armor_Lamp.h"
#include "functions.cpp"
#include <iostream>

using namespace std;
using namespace cv;


int main() {
    //read video
    string path = "/home/joey/qt-project/rm/red1.mp4";
    VideoCapture cap(path);
    Mat img, img_pre;

    while (true) {
        double t = (double)cv::getTickCount();//开始计时
        cap.read(img);
        if (img.empty()) {
            cout << "Stoped!" << endl;
            return 0;
        }
        img_pre = preprocessing(img);
        vector<Armor> armors;
        armors = matchLights(findLights(img,img_pre));
        //for check
        //cout<<"armors.size"<<armors.size()<<endl;

        for(const auto armor : armors){
            cout<<armor.left_light.top<<"  "<<armor.right_light.bottom<<endl;
            rectangle(img_pre, armor.left_light.top, armor.right_light.bottom, Scalar(255, 0, 0), 5);
        }
        namedWindow("img", WINDOW_NORMAL);//0
        imshow("img", img);
        namedWindow("img_pre", WINDOW_NORMAL);
        imshow("img_pre", img_pre);
        t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();//结束计时
        int fps = int(1.0 / t);//转换为帧率
        cout << "FPS: " << fps << endl;//输出帧率

        waitKey(100);
    }
    return 0;
}

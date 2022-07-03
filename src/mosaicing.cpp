#include "opencv2/highgui.hpp"    
#include "opencv2/imgcodecs.hpp"  
#include "opencv2/stitching.hpp"  
#include <fstream>                
#include <iostream>

using namespace std;
using namespace cv;

Stitcher::Mode mode = Stitcher::SCANS;

vector<Mat> images;  
int main(int argc, char* argv[]) {
    int i;
    for (i = 0; i < 1000; ++i) {
        std::string saving_name = "/home/arth/Drone_shots/MapGen";
        std::string count = std::to_string(i);
        saving_name.append(count);
        saving_name.append(".jpg");
        Mat img = imread(saving_name);
        if (img.empty()) {
            cout << "Not able to read anymore images \n";
            break;
        }
        images.push_back(img);
        cout << images.size() << "\n";
    }
    cout << i << endl;
    Mat stitched_img;  

    Ptr<Stitcher> stitcher = Stitcher::create(mode, false);  
    Stitcher::Status status = stitcher->stitch(images, stitched_img);

    if (status != Stitcher::OK) {
        cout << "Image stitching unsuccessful\n";
        return -1;
    }

    imwrite("Map.jpg", stitched_img);

    imshow("Map", stitched_img);  

    waitKey(0);  
    return 0;
}

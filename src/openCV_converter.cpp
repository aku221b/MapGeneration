#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>  
#include <opencv2/highgui/highgui.hpp>        
#include <opencv2/imgproc/imgproc.hpp>        
#include <ros/ros.h>
#include <chrono>
#include <sensor_msgs/image_encodings.h>  // includes the ROS image message type
#include <string>
#include <thread>
#include <iostream>

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter {
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;

  public:
    ImageConverter()
        : it_(nh_) {
        image_sub_ = it_.subscribe("/iris/camera_red_iris/image_raw", 1, &ImageConverter::imageCb, this);
        image_pub_ = it_.advertise("/image_converter/output_video", 1);

        cv::namedWindow(OPENCV_WINDOW);
    }

    ~ImageConverter() {
        cv::destroyWindow(OPENCV_WINDOW);  
    }

    void imageCb(const sensor_msgs::ImageConstPtr& msg) {
        cv_bridge::CvImagePtr cv_ptr;
        try {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        } catch (cv_bridge::Exception& e) {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }

        std::string saving_name = "/home/arth/Drone_shots/MapGen";
        static int ctr = 0;
        std::string name_changer = std::to_string(ctr);
        saving_name.append(name_changer);
        saving_name.append(".jpg");
        ctr++;
        imwrite(saving_name, cv_ptr->image);

        cv::imshow(OPENCV_WINDOW, cv_ptr->image);
        cv::waitKey(3);

        image_pub_.publish(cv_ptr->toImageMsg());
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "image_converter");
    ImageConverter ic;

    ros::Rate r(0.5);
    while (ros::ok()) {
        ros::spinOnce();
        r.sleep();
    }
    return 0;
}
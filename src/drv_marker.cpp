#include <ros/ros.h>
#include <nodelet/nodelet.h>

#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Header.h>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/CameraInfo.h>

#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <image_transport/subscriber_filter.h>

#include <image_geometry/pinhole_camera_model.h>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

cv_bridge::CvImagePtr imagePtr_;

void imageCallback(const sensor_msgs::ImageConstPtr& image)
{
  if(image->encoding.compare(sensor_msgs::image_encodings::TYPE_8UC1) == 0)
  {
    imagePtr_ = cv_bridge::toCvCopy(image);
  }
  else if(image->encoding.compare(sensor_msgs::image_encodings::MONO8) == 0 ||
          image->encoding.compare(sensor_msgs::image_encodings::MONO16) == 0)
  {
    imagePtr_ = cv_bridge::toCvCopy(image, "mono8");
  }
  else
  {
    imagePtr_ = cv_bridge::toCvCopy(image, "bgr8");
  }

  imshow("cap", imagePtr_->image);
  waitKey(10);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "drv_marker");

  ros::NodeHandle nh;
  ros::NodeHandle pnh;
  ros::NodeHandle prnh("~");

  ros::NodeHandle rgb_nh(nh, "rgb");
  ros::NodeHandle rgb_pnh(pnh, "rgb");
  image_transport::ImageTransport rgb_it(rgb_nh);
  // !Use compressed message to speed up -- necessary!
  image_transport::TransportHints hintsRgb("compressed", ros::TransportHints(), rgb_pnh);

  image_transport::Subscriber sub = rgb_it.subscribe(rgb_nh.resolveName("/camera/rgb/image_raw"),
                                                     1, imageCallback, hintsRgb);

  ROS_INFO("DRV marker initialized.");

  while (ros::ok()) {
    ros::spinOnce();
  }

  return 0;
}

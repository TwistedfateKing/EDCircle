#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "src/edge_drawing.h"
#include "src/edge_drawing_parameter_free.h"
#include "src/ed_circle.h"

int main(void)
{
  cv::Mat rena_img = cv::imread("main/Lenna.png");
  cv::imshow("renna", rena_img);

//  EdgeDrawing ed;
//  std::vector< std::vector<cv::Point> > edge_segment = ed.detect(rena_img);
  EdgeDrawingParameterFree edpf;
  std::vector< std::vector<cv::Point> > edge_segment = edpf.detect(rena_img);

  EDCircle circle_detector;

  cv::Mat ret_img_segment = cv::Mat::zeros(rena_img.size(), CV_8UC3);
  cv::Mat ret_img_edge = cv::Mat::zeros(rena_img.size(), CV_8UC3);

  for(int i =0 ; i < edge_segment.size(); i++) {
    cv::Vec3b ran_color = cv::Vec3b(rand()%255,rand()%255,rand()%255);
    for (int j = 0; j < edge_segment[i].size(); j++) {
      ret_img_segment.at<cv::Vec3b>(edge_segment[i][j]) = ran_color;
      ret_img_edge.at<cv::Vec3b>(edge_segment[i][j]) = cv::Vec3b(255,255,255);
    }
  }

  cv::imshow("ret_img_segment", ret_img_segment);
  cv::imshow("ret_img_edge", ret_img_edge);
  cv::waitKey(0);

  return 1;
}

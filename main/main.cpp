#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "src/edge_drawing.h"
#include "src/edge_drawing_parameter_free.h"
#include "src/ed_circle.h"

void DisplayEdgeSegmentResult( cv::Mat src_img, std::vector< std::vector<cv::Point> > edge_segment ) {

  cv::Mat img_segments_color = cv::Mat::zeros(src_img.size(), CV_8UC3);
  cv::Mat img_segment_white = cv::Mat::zeros(src_img.size(), CV_8UC3);

  for(int i =0 ; i < edge_segment.size(); i++) {
    cv::Vec3b ran_color = cv::Vec3b(rand()%255,rand()%255,rand()%255);
    for (int j = 0; j < edge_segment[i].size(); j++) {
      img_segments_color.at<cv::Vec3b>(edge_segment[i][j]) = ran_color;
      img_segment_white.at<cv::Vec3b>(edge_segment[i][j]) = cv::Vec3b(255,255,255);
    }
  }
  cv::hconcat(src_img, img_segments_color, img_segments_color);
  cv::hconcat(img_segments_color, img_segment_white, img_segment_white);
  cv::imshow("edge egment image", img_segment_white);
}

void DisplayCircleResult( cv::Mat src_img, std::vector<Circle> circles, std::vector<Ellipse> ellipses) {

  cv::Mat ret_edcircle = cv::Mat::zeros(src_img.size(), CV_8UC3);
  for (int i = 0 ; i < circles.size(); i++) {
    std::cout <<circles[i].center << ", " << circles[i].radius << std::endl;
    cv::circle(ret_edcircle, circles[i].center, circles[i].radius, cv::Scalar(60,255,60), 1, 8, 0 );
  }

  for(int i = 0 ; i < ellipses.size(); i++) {
    std::cout <<ellipses[i].center << ", " << ellipses[i].radius_ls << "," <<  ellipses[i].angle << std::endl;
    cv::ellipse(ret_edcircle, ellipses[i].center, ellipses[i].radius_ls, ellipses[i].angle, 0, 360, cv::Scalar(122,122,255));
  }

  cv::imshow("ret_circle", ret_edcircle);
}

int main(void)
{
  cv::Mat rena_img = cv::imread("main/Lenna.png");
  cv::imshow("renna", rena_img);

//  EdgeDrawing ed;
//  std::vector< std::vector<cv::Point> > edge_segment = ed.detect(rena_img);
  EdgeDrawingParameterFree edpf;
  std::vector< std::vector<cv::Point> > edge_segment = edpf.detect(rena_img);

  EDCircle circle_detector;
  circle_detector.detect(edge_segment);
  std::vector<Circle> circle_ret = circle_detector.get_circle();
  std::vector<Ellipse> ellipse_ret = circle_detector.get_ellipse();

  DisplayEdgeSegmentResult(rena_img, edge_segment);
  DisplayCircleResult(rena_img, circle_ret, ellipse_ret );

  cv::waitKey(0);

  return 1;
}

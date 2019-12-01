//
// Created by cjyj on 19. 11. 30..
//

#ifndef EDCIRCLE_SRC_ED_CIRCLE_H_
#define EDCIRCLE_SRC_ED_CIRCLE_H_

#include <vector>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "ed_line.h"

struct Circle {
  cv::Point2f center;
  float radius;
};

struct Ellipse {
  float A, B, C, D, E, F;
  cv::Point center;
  cv::Size radius_ls;
  float angle;
};

class EDCircle {
 public:
  EDCircle();
  ~EDCircle();

  void detect( std::vector<std::vector<cv::Point> > edge_segment, cv::Size img_size);
  std::vector< Circle > get_circle();
  std::vector< Ellipse > get_ellipse();

 protected:
  std::vector<std::vector<cv::Point>> find_least_squares_circle_fit(std::vector<std::vector<cv::Point> > edge_segments);
  std::vector<std::vector<cv::Point>> find_least_squares_ellipse_fit(std::vector<std::vector<cv::Point> > edge_segments);
  bool least_squares_circle_fit(std::vector<cv::Point> edge_segment, Circle &circle);
  bool least_squares_ellipse_fit(std::vector<cv::Point> edge_segment, Ellipse &ellipse);

  std::vector< Circle> circle_v_;
  std::vector< Ellipse> ellipse_v_;

 private:
  EDLine edline_detector_;
};

#endif //EDCIRCLE_SRC_ED_CIRCLE_H_

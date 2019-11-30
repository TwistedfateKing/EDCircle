//
// Created by cjyj on 19. 11. 30..
//

#ifndef EDCIRCLE_SRC_ED_CIRCLE_H_
#define EDCIRCLE_SRC_ED_CIRCLE_H_

#include <vector>
#include <iostream>
#include "opencv2/opencv.hpp"

class EDCircle {
 public:
  EDCircle();
  ~EDCircle();

  void detect( std::vector<std::vector<cv::Point> > edge_segment);

 protected:
  std::vector<std::vector<cv::Point>> find_least_squares_circle_fit(std::vector<std::vector<cv::Point> > edge_segments);
  std::vector<std::vector<cv::Point>> find_least_squares_ellipse_fit(std::vector<std::vector<cv::Point> > edge_segments);
  bool least_squares_circle_fit(std::vector<cv::Point> edge_segment, cv::Point2f &circle_center, float &radius);
  bool least_squares_ellipse_fit(std::vector<cv::Point> edge_segment, float& a, float& b, float& c, float& d, float& e, float& f);
};

#endif //EDCIRCLE_SRC_ED_CIRCLE_H_

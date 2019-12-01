//
// Created by cjyj on 19. 12. 1..
//

#ifndef EDCIRCLE_SRC_ED_LINE_H_
#define EDCIRCLE_SRC_ED_LINE_H_

#include <vector>
#include <iostream>
#include "opencv2/opencv.hpp"

#define MIN_LINE_LENGTH 5

struct Line {
  cv::Point p1;
  cv::Point p2;
  float A, B; // y = Ax + B
};

class LineEquation {
 public:
  LineEquation();
  ~LineEquation();

  float ComputePointDistance2Line(cv::Point point);

 public:
  Line lineeq;
};

class EDLine {
 public:
  EDLine();
  ~EDLine();

  std::vector<Line> detect( std::vector<std::vector<cv::Point> > edge_segment, cv::Size img_size );

 protected:
  void LineFit(std::vector<cv::Point> pixelChain);
  void LeastSquaresLineFit(std::vector<cv::Point> pixelChain, int min_line_length, LineEquation &lineEquation, float &lineFitError);
  float ComputeError(LineEquation &lineEquation, std::vector<cv::Point> pixelChain);

 protected:
  std::vector<Line> ed_lines_;
  float line_valid_threshold_ = 12;
};

#endif //EDCIRCLE_SRC_ED_LINE_H_

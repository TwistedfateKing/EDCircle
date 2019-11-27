//
// Created by cjyj on 19. 11. 26..
//

#ifndef EDCIRCLE_SRC_EDGE_DRAWING_H_
#define EDCIRCLE_SRC_EDGE_DRAWING_H_

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

class EdgeDrawing {

 public:
  enum class EDGE_DIRECTION
  {
    VERTICAL = 0,
    HORIZONTAL = 1,
    OTHER = 2,
  };

 public:
  EdgeDrawing();
  ~EdgeDrawing();

  void detect(cv::Mat img);

 private:
  cv::Mat smooth(cv::Mat img); // step 1
  void computeEdgeMaps(cv::Mat img); // step 2

 private:
  cv::Mat G_; // gradient magnitude map
  cv::Mat D_; // direction map
  cv::Mat E_; // edge map

};

#endif //EDCIRCLE_SRC_EDGE_DRAWING_H_

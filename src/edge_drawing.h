//
// Created by cjyj on 19. 11. 26..
//

#ifndef EDCIRCLE_SRC_EDGE_DRAWING_H_
#define EDCIRCLE_SRC_EDGE_DRAWING_H_

#include <vector>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

class EdgeDrawing {

 public:
  enum class EDGE_DIRECTION
  {
    VERTICAL = 255,
    HORIZONTAL = 125,
    OTHER = 2,
  };

 public:
  EdgeDrawing();
  ~EdgeDrawing();

  void detect(cv::Mat img);

 private:
  cv::Mat smooth(cv::Mat img); // step 1
  void computeEdgeMaps(cv::Mat img); // step 2
  void extractAnchors();

 private:
  cv::Mat G_; // gradient magnitude map
  cv::Mat D_; // direction map
  cv::Mat E_; // edge map

  std::vector<cv::Point> anchors_;

 protected:
  float mag_threshold_ = 36;
  float anchor_threshold_ = 8;
  float anchor_scan_interval_ = 2;

};

#endif //EDCIRCLE_SRC_EDGE_DRAWING_H_

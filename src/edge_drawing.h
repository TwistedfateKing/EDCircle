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
  enum class EDGE_DIRECTION {
    VERTICAL = 255,
    HORIZONTAL = 125,
  };

 public:
  EdgeDrawing();
  ~EdgeDrawing();

  std::vector< std::vector<cv::Point> > detect(cv::Mat img);

 private:
  cv::Mat smooth(cv::Mat img); // step 1
  void computeEdgeMaps(cv::Mat img); // step 2
  void extractAnchors();
  void connectEdges();

  bool isAnchor(int x, int y);

  void edge_proceed_ori(int x, int y, std::vector<cv::Point> &edge_segment);
  void edge_proceed(int x, int y, std::vector<cv::Point> &edge_segment);
  void goLeft(int x, int y, std::vector<cv::Point> &edge_segment);
  void goRight(int x, int y, std::vector<cv::Point> &edge_segment);
  void goUp(int x, int y, std::vector<cv::Point> &edge_segment);
  void goDown(int x, int y, std::vector<cv::Point> &edge_segment);

  std::string direction_to_left(int x, int y);
  std::string direction_to_right(int x, int y);
  std::string direction_to_up(int x, int y);
  std::string direction_to_down(int x, int y);
  bool edge_process_stop_check(int x, int y);
  bool image_end_check(int x, int y);

 private:
  cv::Mat G_; // gradient magnitude map
  cv::Mat D_; // direction map
  cv::Mat E_; // edge map

  std::vector<cv::Point> anchors_;
  std::vector<std::vector<cv::Point> > edge_segments_;

  cv::Rect image_rect_;

 protected:
  float mag_threshold_ = 36;
  float anchor_threshold_ = 8;
  float anchor_scan_interval_ = 2;

  int edge_segment_drawing_threshold_ = 10;

};

#endif //EDCIRCLE_SRC_EDGE_DRAWING_H_

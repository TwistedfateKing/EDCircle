//
// Created by cjyj on 19. 11. 29..
//

#ifndef EDCIRCLE_SRC_EDGE_DRAWING_PARAMETER_FREE_H_
#define EDCIRCLE_SRC_EDGE_DRAWING_PARAMETER_FREE_H_

#include "edge_drawing.h"

class EdgeDrawingParameterFree : public EdgeDrawing {
 public:
  std::vector< std::vector<cv::Point> > detect(cv::Mat img);

 protected:
  void ValidateEdgeSegments();
  bool ValidateSegment(std::vector<cv::Point> S);

  void make_H();

 protected:
  int Np_ = 0;
  cv::Mat H_;
  std::vector< std::vector<cv::Point> > valid_edge_segments_;

};

#endif //EDCIRCLE_SRC_EDGE_DRAWING_PARAMETER_FREE_H_

//
// Created by cjyj on 19. 11. 29..
//

#ifndef EDCIRCLE_SRC_EDGE_DRAWING_PARAMETER_FREE_H_
#define EDCIRCLE_SRC_EDGE_DRAWING_PARAMETER_FREE_H_

#include "edge_drawing.h"

class EdgeDrawingParameterFree : public EdgeDrawing {
 public:
  std::vector< std::vector<cv::Point> > detect(cv::Mat img);
};

#endif //EDCIRCLE_SRC_EDGE_DRAWING_PARAMETER_FREE_H_

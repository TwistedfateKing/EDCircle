//
// Created by cjyj on 19. 11. 26..
//

#ifndef EDCIRCLE_SRC_EDGE_DRAWING_H_
#define EDCIRCLE_SRC_EDGE_DRAWING_H_

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

class EdgeDrawing {
 public:
  EdgeDrawing();
  ~EdgeDrawing();

  void detect(cv::Mat img);

};

#endif //EDCIRCLE_SRC_EDGE_DRAWING_H_

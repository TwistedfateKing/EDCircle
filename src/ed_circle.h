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

};

#endif //EDCIRCLE_SRC_ED_CIRCLE_H_

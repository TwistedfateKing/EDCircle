//
// Created by cjyj on 19. 11. 29..
//

#ifndef EDCIRCLE_SRC_EDGE_DRAWING_PARAMETER_FREE_H_
#define EDCIRCLE_SRC_EDGE_DRAWING_PARAMETER_FREE_H_

#include "edge_drawing.h"

class EdgeSegmentParameterFree : public EdgeDrawing {
 public:
  int set(int k);
  int k_;
};

#endif //EDCIRCLE_SRC_EDGE_DRAWING_PARAMETER_FREE_H_

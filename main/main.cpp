#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "src/edge_drawing.h"

int main(void)
{
  EdgeDrawing edge_detector();

  cv::Mat rena_img = cv::imread("main/rena.png");
  cv::imshow("rena", rena_img);
  cv::waitKey(0);

  return 1;
}

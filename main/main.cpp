#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "src/edge_drawing.h"

int main(void)
{
  EdgeDrawing edge_detector;

  cv::Mat rena_img = cv::imread("main/Lenna.png");
  cv::imshow("renna", rena_img);

  edge_detector.detect(rena_img);
  cv::waitKey(0);
  return 1;
}

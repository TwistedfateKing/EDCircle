#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "src/edge_drawing.h"
#include "src/edge_drawing_parameter_free.h"

int main(void)
{
  EdgeSegmentParameterFree edpf;

  cv::Mat rena_img = cv::imread("main/Lenna.png");
  cv::imshow("renna", rena_img);

  std::vector< std::vector<cv::Point> > edge_segs = edpf.detect(rena_img);

  cv::waitKey(0);
  return 1;
}

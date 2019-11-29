#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "src/edge_drawing.h"
#include "src/edge_drawing_parameter_free.h"

int main(void)
{
  cv::Mat rena_img = cv::imread("main/Lenna.png");
  cv::imshow("renna", rena_img);

  EdgeDrawing ed;
  std::vector< std::vector<cv::Point> > ret1 = ed.detect(rena_img);

  EdgeDrawingParameterFree edpf;
  std::vector< std::vector<cv::Point> > ret2 = edpf.detect(rena_img);

  cv::waitKey(0);
  return 1;
}

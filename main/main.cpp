#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

int main(void)
{
  
  cv::Mat rena_img = cv::imread("main/rena.png");
  cv::imshow("rena", rena_img);
  cv::waitKey(0);

  return 1;
}

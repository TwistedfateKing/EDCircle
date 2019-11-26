//
// Created by cjyj on 19. 11. 26..
//

#include "edge_drawing.h"

EdgeDrawing::EdgeDrawing() {

}

EdgeDrawing::~EdgeDrawing() {

}
void EdgeDrawing::detect(cv::Mat img) {

  //convert Color
  cv::Mat gray;
  if( img.channels() == 3 ) {
    cv::cvtColor(img, gray, CV_BGR2GRAY);
  }
  else {
    img.copyTo(gray);
  }
  cv::imshow("gray", gray);

  // step 1. Suppression of noise by Gaussian filtering
  cv::Mat smooth_image = smooth(gray);
  cv::imshow("smooth_image", smooth_image);

  // step 2. Computation of the gradient magnitude and edge direction maps
  cv::Mat mag, ori;
  computeEdgeMap(smooth_image, mag, ori);

  // step 3. Extraction of the anchors.
  // step 4. Connecting the anchors by smart routing.
}


cv::Mat EdgeDrawing::smooth(cv::Mat image) {
  cv::Mat smooth_img;

  cv::GaussianBlur(image, smooth_img, cv::Size(5,5), 1, 1);

  return smooth_img;
}

void EdgeDrawing::computeEdgeMap(cv::Mat img, cv::Mat mag, cv::Mat ori)
{

}
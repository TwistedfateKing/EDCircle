//
// Created by cjyj on 19. 11. 26..
//

#include "edge_drawing.h"

EdgeDrawing::EdgeDrawing()
{

}

EdgeDrawing::~EdgeDrawing()
{

}
void EdgeDrawing::detect(cv::Mat img)
{

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
  cv::Mat mag, egde_direction_map;
  computeEdgeMaps(smooth_image);

  // step 3. Extraction of the anchors.
  // step 4. Connecting the anchors by smart routing.
}


cv::Mat EdgeDrawing::smooth(cv::Mat image)
{
  cv::Mat smooth_img;
  cv::GaussianBlur(image, smooth_img, cv::Size(5,5), 1, 1);
  return smooth_img;
}

void EdgeDrawing::computeEdgeMaps(cv::Mat img)
{
  cv::Mat gx, gy;
  cv::Sobel(img, gx, CV_32F, 1, 0);
  cv::Sobel(img, gy, CV_32F, 0, 1);

  // mag G = |gx| + |gy|
  cv::Mat abs_gx, abs_gy;
  cv::convertScaleAbs( gx, abs_gx );
  cv::convertScaleAbs( gy, abs_gy );
  cv::add( abs_gx, abs_gy, G_ );

  D_ = cv::Mat::zeros(img.size(), CV_8UC1);
  // edge direction map
  for(int y = 0; y < img.rows; y++) {
    for(int x = 0; x < img.cols; x++) {
      float gx_val = gx.at<float>(y,x);
      float gy_val = gy.at<float>(y,x);

      // Vertical edge
      if( gx_val >= gy_val) {

      }
      // Horizontal
      else {

      }

    }
  }

  // Debug
//  cv::imshow( "Sobel Edge Detector", mag );
}
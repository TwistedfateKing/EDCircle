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
  computeEdgeMaps(smooth_image);

  // step 3. Extraction of the anchors.
  extractAnchors();

  // step 4. Connecting the anchors by smart routing.
  connectEdges();

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
  abs_gx = cv::abs(gx);
  abs_gy = cv::abs(gy);
  G_ = abs_gx + abs_gy;

  // mag thresholding
  for(int y = 0; y < img.rows; y++) {
    for(int x = 0; x < img.cols; x++) {
      float mag_val = G_.at<float>(y, x);
      if( mag_val < mag_threshold_ ) {
        G_.at<float>(y, x) = 0;
      }
    }
  }
  
  // edge direction map
  D_ = cv::Mat::zeros(img.size(), CV_8UC1);
  for(int y = 0; y < img.rows; y++) {
    for(int x = 0; x < img.cols; x++) {
      float gx_val = gx.at<float>(y, x);
      float gy_val = gy.at<float>(y, x);

      // Vertical edge
      if( gx_val >= gy_val) {
        D_.at<uchar>(y,x) = (uchar)EDGE_DIRECTION::VERTICAL;
      }
      // Horizontal
      else {
        D_.at<uchar>(y,x) = (uchar)EDGE_DIRECTION::HORIZONTAL;
      }
    }
  }

  //  std::cout << G_.type() << std::endl;
  // Debug
  cv::imshow( "Gradient mag", G_ );
  cv::imshow( "Direction map", D_ );
}

void EdgeDrawing::extractAnchors()
{
  anchors_.clear();

  for(int y = 0; y < D_.rows; y += anchor_scan_interval_) {
    for(int x = 0; x < D_.cols; x += anchor_scan_interval_) {

      // IsAnchor
      if( isAnchor(x,y) ) {
        anchors_.push_back(cv::Point(x,y));
      }
    }
  }

  // Debug
  cv::Mat anchor_Mat = cv::Mat::zeros(D_.size(), CV_8UC1);
  for( int i = 0 ; i < anchors_.size(); i++)
    anchor_Mat.at<uchar>(anchors_[i].y, anchors_[i].x) = 255;
  cv::imshow( "anchor", anchor_Mat );

}

void EdgeDrawing::connectEdges() {

  E_ = cv::Mat::zeros(D_.size(), CV_8UC1);

//  for( int i = 0 ; i < anchors_.size(); i++) {

//  }

}

bool EdgeDrawing::isAnchor(int x, int y) {

  uchar direction = D_.at<uchar>(y, x);

  if( direction == (uchar)EDGE_DIRECTION::HORIZONTAL ) {

    if( ( y-1 < 0 ) && ((y+1) >= D_.rows) )
      return false;

    float gxy = G_.at<float>(y, x);
    float gxy_up = G_.at<float>(y-1, x);
    float gxy_dw = G_.at<float>(y+1, x);
    if( ( (gxy - gxy_up) >= anchor_threshold_ ) && ( (gxy - gxy_dw ) >= anchor_threshold_ )) {
      return true;
    }
  }

  else {

    if( ( x-1 < 0 ) && ((x+1) >= D_.cols) )
      return false;

    float gxy = G_.at<float>(y, x);
    float gxy_lf = G_.at<float>(y, x-1);
    float gxy_rt = G_.at<float>(y, x+1);
    if( ( (gxy - gxy_lf) >= anchor_threshold_) && ( (gxy - gxy_rt) >= anchor_threshold_)) {
      return true;
    }
  }

  return false;
}
//
// Created by cjyj on 19. 11. 29..
//

#include "edge_drawing_parameter_free.h"

struct sort_gradient
{
  bool operator()(const std::pair<cv::Point, float> &left, const std::pair<cv::Point, float> &right)
  {
    return left.second > right.second;
  }
};

std::vector< std::vector<cv::Point> > EdgeDrawingParameterFree::detect(cv::Mat img) {

  mag_threshold_ = 8.48;
  anchor_threshold_ = 0;
  anchor_scan_interval_ = 1;
  edge_segment_drawing_threshold_ = 5;
  //convert Color
  cv::Mat gray;
  if( img.channels() == 3 ) {
    cv::cvtColor(img, gray, CV_BGR2GRAY);
  }
  else {
    img.copyTo(gray);
  }
  image_rect_ = cv::Rect(0,0, img.cols, img.rows);
  cv::imshow("gray", gray);

  // step 1. Suppression of noise by Gaussian filtering
  cv::Mat smooth_image = smooth(gray);
  cv::imshow("smooth_image", smooth_image);

  // step 2. Computation of the gradient magnitude and edge direction maps
  computeEdgeMaps(smooth_image);

  // step 3. Extraction of the anchors.
  extractAnchors();

  // sort anchor with anchor gradient.
  std::sort(anchors_.begin(), anchors_.end(), sort_gradient());

  // step 4. Connecting the anchors by smart routing.
  connectEdges();

  return edge_segments_;
}

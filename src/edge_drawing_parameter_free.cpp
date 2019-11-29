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

  mag_threshold_ = 12; // 6 + 6  // G = |gx| + |gy|
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

  // step 1. Suppression of noise by Gaussian filtering
  cv::Mat smooth_image = smooth(gray);

  // step 2. Computation of the gradient magnitude and edge direction maps
  computeEdgeMaps(smooth_image);

  // step 3. Extraction of the anchors.
  extractAnchors();

  // step 4. anchor with anchor gradient.
  std::sort(anchors_.begin(), anchors_.end(), sort_gradient());

  // step 5. Connecting the anchors by smart routing.
  connectEdges();

  // step 6. edge segment validation.
  ValidateEdgeSegments();

  return valid_edge_segments_;
}

void EdgeDrawingParameterFree::ValidateEdgeSegments() {

  // calculation Np_
  for(int i = 0; i < edge_segments_.size(); i++ ) {
    int length = edge_segments_[i].size();
    Np_ += length * (length - 1) / 2;
  }

  // distribution of the gradient
  make_H();

  // validation
  for( int i = 0; i < edge_segments_.size(); i++) {
    if( ValidateSegment( edge_segments_[i] ) ) {
      valid_edge_segments_.push_back(edge_segments_[i]);
    }
//    ValidateSegment( edge_segments_[i] );
  }

  // debug
  cv::Mat valide_edge_img = cv::Mat::zeros(D_.size(), CV_8UC3);
  for(int i =0 ; i < valid_edge_segments_.size(); i++) {
    cv::Vec3b ran_color = cv::Vec3b(rand()%255,rand()%255,rand()%255);
      for (int j = 0; j < valid_edge_segments_[i].size(); j++) {
        valide_edge_img.at<cv::Vec3b>(valid_edge_segments_[i][j]) = ran_color;
    }
  }
  cv::imshow( "Debug_7_Valid_Edge_segment", valide_edge_img );
}

bool EdgeDrawingParameterFree::ValidateSegment(std::vector<cv::Point> S )
{
  int l = S.size();
  if(l <= 0) {
    return false;
  }
  // find xmax
  float min_u = FLT_MAX;
  int min_index = -1;
  for ( int i = 0; i < S.size(); i++) {
    float g_val = G_.at<float>(S[i]);
    if (g_val < min_u) {
      min_u = g_val;
      min_index = i;
    }
  }

  float hu = H_.at<float>((int)(min_u));
  float NFA = Np_ * pow(hu, l);

  if ( NFA < 1) {
//    valid_edge_segments_.push_back(S);
    return true;
  }

  std::vector<cv::Point> S1(S.begin(), S.begin() + min_index);
  std::vector<cv::Point> S2(S.begin() + min_index + 1, S.end());

  return ValidateSegment( S1 ) || ValidateSegment( S2 );
}

void EdgeDrawingParameterFree::make_H()
{
  // make hist
  int maxv = 0;
  cv::Mat hist = cv::Mat::zeros(cv::Size(1, 255*3), CV_32SC1);
  for( int y = 0; y < G_.rows; y++) {
    for( int x = 0; x < G_.cols; x++) {
      int g_val = (int)G_.at<float>(cv::Point(x,y));
      hist.at<int>(g_val) += 1;
      if ( maxv < g_val) {
        maxv = g_val;
      }
    }
  }

  // make accum hist
  cv::Mat accum_hist(hist.size(), hist.type());
  accum_hist.at<int>(maxv) = hist.at<int>(maxv);
  for(int k = hist.rows - 2; k >= 0; --k) {
    accum_hist.at<int>(k) = hist.at<int>(k) + accum_hist.at<int>(k+1);
  }
  cv::Mat accum_hist_prob;
  accum_hist.convertTo(accum_hist_prob, CV_32FC1);
  accum_hist_prob = accum_hist_prob/accum_hist_prob.at<float>(1);

  // copy to H
  accum_hist_prob.copyTo(H_);

//  std::cout << "max : " << maxv << std::endl;
//  std::cout << hist.t() << std::endl;
//  std::cout << accum_hist.t() << std::endl;
//  std::cout << accum_hist_prob.t() << std::endl;
}
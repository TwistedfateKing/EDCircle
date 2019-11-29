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
std::vector< std::vector<cv::Point> > EdgeDrawing::detect(cv::Mat img)
{

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

  // step 4. Connecting the anchors by smart routing.
  connectEdges();

  return edge_segments_;
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

//  cv::Sobel(img, gx, CV_32F, 1, 0);
//  cv::Sobel(img, gy, CV_32F, 0, 1);
  float h[9] = {-1,-1,-1,0,0,0,1,1,1};
  float w[9] = {-1,0,1,-1,0,1,-1,0,1};
  cv::Mat kernel_h = cv::Mat( 3, 3, CV_32F, h);
  cv::Mat kernel_w = cv::Mat( 3, 3, CV_32F, w);
  cv::filter2D( img, gx, kernel_h.depth() , kernel_w );
  cv::filter2D( img, gy, kernel_w.depth() , kernel_h );

  // mag G = |gx| + |gy|
  cv::Mat abs_gx, abs_gy;
  abs_gx = cv::abs(gx);
  abs_gy = cv::abs(gy);

  G_ = cv::Mat::zeros(img.size(), CV_32FC1);

//  G_ = abs_gx + abs_gy;
  for(int y = 0; y < img.rows; y++) {
    for(int x = 0; x < img.cols; x++) {
      float abs_gx_val = abs_gx.at<float>(y, x);
      float abs_gy_val = abs_gy.at<float>(y, x);
      float sqrt_g_val = std::sqrt(abs_gx_val*abs_gx_val + abs_gy_val+abs_gy_val );
      G_.at<float>(y, x) = sqrt_g_val;
    }
  }

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
      float gx_val = abs_gx.at<float>(y, x);
      float gy_val = abs_gy.at<float>(y, x);

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
        anchors_.push_back( std::make_pair(cv::Point(x,y), G_.at<float>(y, x) ) );
      }
    }
  }

  // Debug
  cv::Mat anchor_Mat = cv::Mat::zeros(D_.size(), CV_8UC1);
  for( int i = 0 ; i < anchors_.size(); i++)
    anchor_Mat.at<uchar>(anchors_[i].first) = 255;
  cv::imshow( "anchor", anchor_Mat );

}

void EdgeDrawing::connectEdges() {

  E_ = cv::Mat::zeros(D_.size(), CV_8UC1);
  cv::Mat E_color = cv::Mat::zeros(D_.size(), CV_8UC3);
  for( int i = 0 ; i < anchors_.size(); i++) {
    int x = anchors_[i].first.x;
    int y = anchors_[i].first.y;

    std::vector<cv::Point> edge_segment;

    edge_proceed(x, y, edge_segment);

    if (edge_segment.size() > edge_segment_drawing_threshold_)
      edge_segments_.push_back( edge_segment );
  }

  // debug
  for(int i =0 ; i < edge_segments_.size(); i++) {
    cv::Vec3b ran_color = cv::Vec3b(rand()%255,rand()%255,rand()%255);
    if (edge_segments_[i].size() > edge_segment_drawing_threshold_) {
      for (int j = 0; j < edge_segments_[i].size(); j++) {
        E_color.at<cv::Vec3b>(edge_segments_[i][j].y, edge_segments_[i][j].x) = ran_color;
      }
    }
  }

  cv::imshow( "E", E_ );
  cv::imshow( "E_color", E_color );
}

bool EdgeDrawing::isAnchor(int x, int y) {

  uchar direction = D_.at<uchar>(y, x);

  if( direction == (uchar)EDGE_DIRECTION::HORIZONTAL ) {

    if( ( y-1 < 0 ) && ((y+1) >= D_.rows) )
      return false;

    float gxy = G_.at<float>(y, x);
    float gxy_up = G_.at<float>(y-1, x);
    float gxy_dw = G_.at<float>(y+1, x);
    if( ( (gxy - gxy_up) > anchor_threshold_ ) && ( (gxy - gxy_dw ) > anchor_threshold_ )) {
      return true;
    }
  }

  else {

    if( ( x-1 < 0 ) && ((x+1) >= D_.cols) )
      return false;

    float gxy = G_.at<float>(y, x);
    float gxy_lf = G_.at<float>(y, x-1);
    float gxy_rt = G_.at<float>(y, x+1);
    if( ( (gxy - gxy_lf) > anchor_threshold_) && ( (gxy - gxy_rt) > anchor_threshold_)) {
      return true;
    }
  }

  return false;
}

void EdgeDrawing::edge_proceed_ori(int x, int y, std::vector<cv::Point> &edge_segment)
{
  uchar direction = D_.at<uchar>(y, x);
  if( direction == (uchar)EDGE_DIRECTION::HORIZONTAL ) {
    // left
    goLeft(x, y, edge_segment);
    // right
    E_.at<uchar>(y, x) = 0;
    goRight(x, y, edge_segment);
  }
  else {
    // up
    goUp(x, y, edge_segment);
    // down
    E_.at<uchar>(y, x) = 0;
    goDown(x, y, edge_segment);
  }
}

void EdgeDrawing::edge_proceed(int x, int y, std::vector<cv::Point> &edge_segment)
{
  uchar direction = D_.at<uchar>(y, x);

  if( edge_process_stop_check(x, y) )
    return;

  E_.at<uchar>(y, x) = 255;
  edge_segment.push_back( cv::Point(x,  y) );

  if( direction == (uchar)EDGE_DIRECTION::HORIZONTAL ) {
    // left
    std::string left_dir = direction_to_left(x, y);
    int left_x = x, left_y = y;
    if ( left_dir == "up-left" ){
      left_x = x - 1;
      left_y = y - 1;
    }
    else if ( left_dir ==  "down-left" ){
      left_x = x - 1;
      left_y = y + 1;
    }
    else {
      left_x = x - 1;
    }
    if(image_rect_.contains( cv::Point(left_x, left_y)))
      goLeft(left_x, left_y, edge_segment);
    std::reverse(edge_segment.begin(), edge_segment.end());

    // right
    std::string right_dir = direction_to_right(x, y);
    int right_x = x, right_y = y;
    if ( right_dir == "up-right" ){
      right_x = x + 1;
      right_y = y - 1;
    }
    else if ( right_dir == "down-right" ){
      right_x = x + 1;
      right_y = y + 1;
    }
    else {
      right_x = x + 1;
    }
    if(image_rect_.contains( cv::Point(right_x, right_y)))
      goRight(right_x, right_y, edge_segment);

  }
  else {
    // up
    std::string up_dir = direction_to_up(x, y);
    int up_x = x, up_y = y;
    if ( up_dir == "up-left" ){
      up_x = x - 1;
      up_y = y - 1;
    }
    else if ( up_dir == "up-right"  ){
      up_x = x + 1;
      up_y = y - 1;
    }
    else {
      up_y = y - 1;
    }
    if(image_rect_.contains( cv::Point(up_x, up_y)))
      goUp(up_x, up_y, edge_segment);
    std::reverse(edge_segment.begin(), edge_segment.end());

    // down
    std::string down_dir = direction_to_down(x, y);
    int down_x = x, down_y = y;
    if ( down_dir == "down-left" ){
      down_x = x - 1;
      down_y = y + 1;
    }
    else if ( down_dir == "down-right" ){
      down_x = x + 1;
      down_y = y + 1;
    }
    else {
      down_y = y + 1;
    }
    if(image_rect_.contains( cv::Point(down_x, down_y)))
      goDown(down_x, down_y, edge_segment);

  }
}

void EdgeDrawing::goLeft(int x, int y, std::vector<cv::Point> &edge_segment)
{
  if( edge_process_stop_check(x, y) )
    return;

  E_.at<uchar>(y, x) = 255;
  edge_segment.push_back( cv::Point(x,  y) );

  if (image_end_check(x, y))
    return;

  float g_1 = G_.at<float>(y - 1, x - 1);
  float g_7 = G_.at<float>(y + 1, x - 1);

  // find dir
  std::string movement = direction_to_left(x, y);
  if ( movement == "up-left" ){
    x = x - 1;
    y = y - 1;
  }
  else if ( movement == "down-left" ){
    x = x - 1;
    y = y + 1;
  }
  else {
    x = x - 1;
  }

  uchar direction = D_.at<uchar>(y, x);
  if( direction == (uchar)EDGE_DIRECTION::HORIZONTAL ) {
    goLeft(x, y, edge_segment);
  }
  else {
    if(movement == "up-left"){
      goUp(x, y, edge_segment);
    }
    else if (movement == "down-left"){
      goDown(x, y, edge_segment);
    }
    else {
      if( g_1 > g_7) {
        goUp(x, y, edge_segment);
      }
      else{
        goDown(x, y, edge_segment);
      }
    }
  }

}

void  EdgeDrawing::goRight(int x, int y, std::vector<cv::Point> &edge_segment)
{
  if( edge_process_stop_check(x, y) )
    return;

  E_.at<uchar>(y, x) = 255;
  edge_segment.push_back( cv::Point(x,  y) );

  if (image_end_check(x, y))
    return;

  float g_3 = G_.at<float>(y - 1, x + 1);
  float g_9 = G_.at<float>(y + 1, x + 1);

  // find dir
  std::string movement = direction_to_right(x, y);
  if ( movement == "up-right" ){
    x = x + 1;
    y = y - 1;
  }
  else if ( movement == "down-right" ){
    x = x + 1;
    y = y + 1;
  }
  else {
    x = x + 1;
  }

  uchar direction = D_.at<uchar>(y, x);
  if( direction == (uchar)EDGE_DIRECTION::HORIZONTAL ) {
    goRight(x, y, edge_segment);
  }
  else {
    if (movement == "up-right") {
      goUp(x, y, edge_segment);
    } else if (movement == "down-right") {
      goDown(x, y, edge_segment);
    } else {
      if( g_3 > g_9) {
        goUp(x, y, edge_segment);
      }
      else{
        goDown(x, y, edge_segment);
      }
    }
  }
}

void EdgeDrawing::goUp(int x, int y, std::vector<cv::Point> &edge_segment)
{
  if( edge_process_stop_check(x, y) )
    return;

  E_.at<uchar>(y, x) = 255;
  edge_segment.push_back( cv::Point(x,  y) );

  if (image_end_check(x, y))
    return;

  float g_1 = G_.at<float>(y-1, x-1);
  float g_3 = G_.at<float>(y-1, x+1);

  // find dir
  std::string movement = direction_to_up(x, y);
  if( movement == "end of image")
    return;

  if ( movement == "up-left" ){
    x = x - 1;
    y = y - 1;
  }
  else if ( movement == "up-right" ){
    x = x + 1;
    y = y - 1;
  }
  else {
    y = y - 1;
    movement = "up";
  }

  uchar direction = D_.at<uchar>(y, x);
  if( direction == (uchar)EDGE_DIRECTION::HORIZONTAL ) {
    if (movement == "up-left") {
      goLeft(x, y, edge_segment);
    } else if (movement == "up-right") {
      goRight(x, y, edge_segment);
    } else {
      if( g_1 > g_3) {
        goLeft(x, y, edge_segment);
      }
      else{
        goRight(x, y, edge_segment);
      }
    }
  }
  else {
    goUp(x, y, edge_segment);
  }

}

void EdgeDrawing::goDown(int x, int y, std::vector<cv::Point> &edge_segment)
{
  if( edge_process_stop_check(x, y) )
    return;

  E_.at<uchar>(y, x) = 255;
  edge_segment.push_back( cv::Point(x,  y) );

  if (image_end_check(x, y))
    return;

  float g_7 = G_.at<float>(y+1, x-1);
  float g_9 = G_.at<float>(y+1, x+1);

  // find dir
  std::string movement = direction_to_down(x, y);
  if ( movement == "down-left" ){
    x = x - 1;
    y = y + 1;
  }
  else if ( movement == "down-right" ){
    x = x + 1;
    y = y + 1;
  }
  else {
    y = y + 1;
  }

  uchar direction = D_.at<uchar>(y, x);
  if( direction == (uchar)EDGE_DIRECTION::HORIZONTAL ) {
    if (movement == "down-left") {
      goLeft(x, y, edge_segment);
    } else if (movement == "down-right") {
      goRight(x, y, edge_segment);
    } else {
      if( g_7 > g_9) {
        goLeft(x, y, edge_segment);
      }
      else{
        goRight(x, y, edge_segment);
      }
    }
  }
  else {
    goDown(x, y, edge_segment);
  }
}

std::string EdgeDrawing::direction_to_left(int x, int y) {

  std::string ret = "left";
  if ( image_end_check(x,y) ) { // "end of img"
    return ret;
  }

  float g_1 = G_.at<float>(y - 1, x - 1);
  float g_4 = G_.at<float>(y + 0, x - 1);
  float g_7 = G_.at<float>(y + 1, x - 1);

  if ((g_1 > g_4) && (g_1 > g_7)) {
    ret =  "up-left";
  } else if ((g_7 > g_4) && (g_7 > g_1)) {
    ret =  "down-left";
  } else {
    ret =  "left";
  }
  return ret;
}


std::string EdgeDrawing::direction_to_right(int x, int y)
{
  std::string ret = "right";
  if ( image_end_check(x,y) ) {
    return ret;
  }

  float g_3 = G_.at<float>(y - 1, x + 1);
  float g_6 = G_.at<float>(y + 0, x + 1);
  float g_9 = G_.at<float>(y + 1, x + 1);

  // right
  if ( (g_3 > g_6) && (g_3 > g_9) ){
    ret = "up-right";
  }
  else if ( (g_9 > g_3) && (g_9 > g_6) ){
    ret = "down-right";
  }
  else {
    ret = "right";
  }
  return ret;
}
std::string EdgeDrawing::direction_to_up(int x, int y)
{
  std::string ret = "up";
  if ( image_end_check(x,y) ) {
    return ret;
  }

  float g_1 = G_.at<float>(y - 1, x - 1);
  float g_2 = G_.at<float>(y - 1, x + 0);
  float g_3 = G_.at<float>(y - 1, x + 1);

  if ( (g_1 > g_2) && (g_1 > g_3) ){
    ret = "up-left";
  }
  else if ( (g_3 > g_1) && (g_3 > g_2) ){
    ret = "up-right";
  }
  else {
    ret = "up";
  }
  return ret;
}
std::string EdgeDrawing::direction_to_down(int x, int y)
{
  std::string ret = "down";
  if ( image_end_check(x,y) ) {
    return ret;
  }

  float g_7 = G_.at<float>(y + 1, x - 1);
  float g_8 = G_.at<float>(y + 1, x + 0);
  float g_9 = G_.at<float>(y + 1, x + 1);

  if ( (g_7 > g_8) && (g_7 > g_9) ){
    ret = "down-left";
  }
  else if ( (g_9 > g_7) && (g_9 > g_8) ){
    ret = "down-right";
  }
  else {
    ret = "down";
  }
  return ret;
}

bool EdgeDrawing::edge_process_stop_check(int x, int y) {
  if( E_.at<uchar>(y, x) != 0 || G_.at<float>(y, x) == 0 )
    return true;
  return false;
}

bool EdgeDrawing::image_end_check(int x, int y) {
  if( x < 1 || y < 1 || x >= D_.cols - 1 || y >= D_.rows - 1 )
    return true;
  return false;
}
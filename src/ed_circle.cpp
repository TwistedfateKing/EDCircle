//
// Created by cjyj on 19. 11. 30..
//

#include "ed_circle.h"

EDCircle::EDCircle() {
  std::cout << "EDCircle()" << std::endl;
}
EDCircle::~EDCircle() {

}
void EDCircle::detect(std::vector<std::vector<cv::Point>> edge_segment) {
  std::cout << "EDCircle::detect()" << std::endl;

  // 1. Detect edge segments by EDPF and extract complete circles and ellipses.
//  Least-Squares Circle Fit
  std::vector<std::vector<cv::Point>> remain_edge_segment = find_least_squares_circle_fit(edge_segment);
  remain_edge_segment = find_least_squares_ellipse_fit(remain_edge_segment);
//  Least-Squares Circle Fit

  // 2. Convert the remaining edge segments into line segments.

  // 3. Detect arcs by combining line segments

  // 4. Join arcs to detect circle candidates.

  // 5. Join the remaining arcs to detect near-circular ellipse candidates.

  // 6. Validate the candidate circles/ellipses using the Helmholtz principle.

  // 7. Output the remaining valid circles/ellipses.

}
std::vector<std::vector<cv::Point>> EDCircle::find_least_squares_circle_fit(std::vector<std::vector<cv::Point> > edge_segment) {
  std::vector<std::vector<cv::Point> > edge_segment_l;

  for(int i = 0; i < edge_segment.size(); i++)
  {
    cv::Point2f circle_center;
    float radius;
    bool check = least_squares_circle_fit(edge_segment[i], circle_center, radius);
    if( check == true) {

    }
    else {
      edge_segment_l.push_back(edge_segment[i]);
    }
  }

  return edge_segment_l;
}

std::vector<std::vector<cv::Point>> EDCircle::find_least_squares_ellipse_fit(std::vector<std::vector<cv::Point> > edge_segment) {
  std::vector<std::vector<cv::Point> > edge_segment_l = edge_segment;
  return edge_segment_l;
}

bool EDCircle::least_squares_circle_fit(std::vector<cv::Point> edge_segment,
                                        cv::Point2f &circle_center,
                                        float &radius) {

  std::vector<cv::Point2f> test;
  test.push_back(cv::Point2f(0, 0));
  test.push_back(cv::Point2f(0.5, 0.25));
  test.push_back(cv::Point2f(1, 1));
  test.push_back(cv::Point2f(1.5, 2.25));
  test.push_back(cv::Point2f(2, 4));
  test.push_back(cv::Point2f(2.5, 6.25));
  test.push_back(cv::Point2f(3, 9));

  int N = test.size();
  float x_mean = 0;
  float y_mean = 0;
  float Suu = 0;
  float Suv = 0;
  float Svv = 0;
  float Suuu = 0;
  float Svvv = 0;
  float Suvv = 0;
  float Svuu = 0;

  for(int i = 0; i < N; i++) {
    x_mean += test[i].x;
    y_mean += test[i].y;
  }
  x_mean = x_mean / (float)N;
  y_mean = y_mean / (float)N;

  for(int i = 0; i < N; i++) {
    float u = test[i].x - x_mean;
    float v = test[i].y - y_mean;
    Suu += u*u;
    Suv += u*v;
    Svv += v*v;
    Suuu += u*u*u;
    Svvv += v*v*v;
    Suvv += u*v*v;
    Svuu += v*u*u;
  }

  // Eq 4, Eq 5
  // AX = B , X = A-1 * B
  cv::Mat A = cv::Mat::zeros(cv::Size(2,2), CV_32FC1);
  cv::Mat B = cv::Mat::zeros(cv::Size(1,2), CV_32FC1);
  A.at<float>(0,0) = Suu;
  A.at<float>(0,1) = Suv;
  A.at<float>(1,0) = Suv;
  A.at<float>(1,1) = Svv;
  B.at<float>(0) = (Suuu + Suvv)/2.0f;
  B.at<float>(1) = (Svvv + Svuu)/2.0f;
  cv::Mat uv_center = A.inv() * B;

  std::cout << A << std::endl;
  std::cout << B << std::endl;
  std::cout << uv_center << std::endl;
  float uc = uv_center.at<float>(0);
  float vc = uv_center.at<float>(1);
  float x_center = uc + x_mean;
  float y_center = vc + y_mean;
  float R = sqrt(uc*uc + vc*vc + (Suu+Svv)/N);

  return false;
}

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
  std::vector<std::vector<cv::Point>> remain_edge_segment;
  remain_edge_segment = find_least_squares_circle_fit(edge_segment); //  Least-Squares Circle Fit
  remain_edge_segment = find_least_squares_ellipse_fit(remain_edge_segment);   //  Least-Squares ellipse Fit

  // 2. Convert the remaining edge segments into line segments.

  // 3. Detect arcs by combining line segments

  // 4. Join arcs to detect circle candidates.

  // 5. Join the remaining arcs to detect near-circular ellipse candidates.

  // 6. Validate the candidate circles/ellipses using the Helmholtz principle.

  // 7. Output the remaining valid circles/ellipses.

}
std::vector<std::vector<cv::Point>> EDCircle::find_least_squares_circle_fit(std::vector<std::vector<cv::Point> > edge_segment) {
  std::vector<std::vector<cv::Point> > edge_segment_remain;

  for(int i = 0; i < edge_segment.size(); i++) {
    Circle circle;
    bool check = least_squares_circle_fit(edge_segment[i], circle);
    if( check ) {
      std::cout << " Find Circle " << std::endl;
      circle_v_.push_back(circle);
    }
    else {
      edge_segment_remain.push_back(edge_segment[i]);
    }
  }

  return edge_segment_remain;
}

std::vector<std::vector<cv::Point>> EDCircle::find_least_squares_ellipse_fit(std::vector<std::vector<cv::Point> > edge_segment) {
  std::vector<std::vector<cv::Point> > edge_segment_remain;

  for(int i = 0; i < edge_segment.size(); i++) {
    Ellipse ellipse;
    bool check = least_squares_ellipse_fit(edge_segment[i], ellipse);
    if( check ) {
      std::cout << " Find Ellipse " << std::endl;
      ellipse_v_.push_back(ellipse);
    }
    else {
      edge_segment_remain.push_back(edge_segment[i]);
    }
  }

  return edge_segment_remain;
}

bool EDCircle::least_squares_circle_fit(std::vector<cv::Point> edge_segment, Circle &circle) {
  int N = edge_segment.size();
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
    x_mean += edge_segment[i].x;
    y_mean += edge_segment[i].y;
  }
  x_mean = x_mean / (float)N;
  y_mean = y_mean / (float)N;

  for(int i = 0; i < N; i++) {
    float u = edge_segment[i].x - x_mean;
    float v = edge_segment[i].y - y_mean;
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

//  std::cout << A << std::endl;
//  std::cout << B << std::endl;
//  std::cout << uv_center << std::endl;
  float uc = uv_center.at<float>(0);
  float vc = uv_center.at<float>(1);
  float x_center = uc + x_mean;
  float y_center = vc + y_mean;
  float R = sqrt(uc*uc + vc*vc + (Suu+Svv)/N);

  // error check
  float error = 0;
  bool ret = true;
  for( int i = 0 ; i < N ; i++){
    float x = edge_segment[i].x;
    float y = edge_segment[i].y;
    error = fabs(sqrt((x - x_center)*(x - x_center) + (y - y_center)*(y - y_center) ) - R);
    if( error > 1.5) {
      ret = false;
      break;
    }
  }

  circle.center.x = x_center;
  circle.center.y = y_center;
  circle.radius = R;

  return ret;
}
bool EDCircle::least_squares_ellipse_fit(std::vector<cv::Point> edge_segment, Ellipse &ellipse) {

  int N = edge_segment.size();
  float mx, my, sx, sy;
  mx = my = sx = sy = 0;
  cv::Mat D1 = cv::Mat::zeros(cv::Size(3, N),CV_32FC1);
  cv::Mat D2 = cv::Mat::zeros(cv::Size(3, N),CV_32FC1);

  float xmin,xmax,ymin,ymax;
  xmin = edge_segment[0].x;
  xmax = edge_segment[0].x;
  ymin = edge_segment[0].y;
  ymax = edge_segment[0].y;
  for(int i = 0; i < N; i++) {
    float x = edge_segment[i].x;
    float y = edge_segment[i].y;
    mx += x;
    my += y;
    // min max
    if(xmax < x)
      xmax = x;
    if(xmin > x)
      xmin = x;
    if(ymax < y)
      ymax = y;
    if(ymin > y)
      ymin = y;
  }
  mx = mx / (float)N;
  my = my / (float)N;
  std::cout << mx << std::endl;
  std::cout << my << std::endl;

  // Make D1, D2 matrix
  for ( int i = 0; i < N; i++) {
    float x1 = (edge_segment[i].x-mx);
    float y1 = (edge_segment[i].y-my);
    D1.at<float>(i, 0) = (float)x1*x1;
    D1.at<float>(i, 1) = (float)x1*y1;
    D1.at<float>(i, 2) = (float)y1*y1;
    D2.at<float>(i, 0) = (float)x1;
    D2.at<float>(i, 1) = (float)y1;
    D2.at<float>(i, 2) = (float)1;
  }

//  std::cout << D1 << std::endl;
//  std::cout << D2 << std::endl;
  cv::Mat S1 = D1.t() * D1;
//  std::cout << "S1" << S1 << std::endl;
  cv::Mat S2 = D1.t() * D2;
//  std::cout << "S2" <<S2 << std::endl;
  cv::Mat S3 = D2.t() * D2;
//  std::cout << "S3" << S3 << std::endl;

  cv::Mat T = -S3.inv()*S2.t();
//  std::cout << "T" << T << std::endl;

  cv::Mat M = S1 + S2*T;
//  std::cout << "M" << M << std::endl;
  cv::Mat M2 = cv::Mat::zeros(cv::Size(3,3), M.type());
  M2.at<float>(0,0) = M.at<float>(2,0)/2.0f;
  M2.at<float>(0,1) = M.at<float>(2,1)/2.0f;
  M2.at<float>(0,2) = M.at<float>(2,2)/2.0f;
  M2.at<float>(1,0) = -M.at<float>(1,0);
  M2.at<float>(1,1) = -M.at<float>(1,1);
  M2.at<float>(1,2) = -M.at<float>(1,2);
  M2.at<float>(2,0) = M.at<float>(0,0)/2.0f;
  M2.at<float>(2,1) = M.at<float>(0,1)/2.0f;
  M2.at<float>(2,2) = M.at<float>(0,2)/2.0f;
//  std::cout << "M2" << M2 << std::endl;

  cv::Mat eigenvalues(1, 3, M2.type()), eigenvectors(3, 3, M2.type());
  cv::eigenNonSymmetric(M2, eigenvalues, eigenvectors );

  eigenvectors = -eigenvectors;
//  std::cout << "eigenvalues" << eigenvalues << std::endl;
//  std::cout << "eigenvectors" << eigenvectors << std::endl;

  float check = 4*eigenvectors.at<float>(0,0) *eigenvectors.at<float>(0,2) - eigenvectors.at<float>(0,1)*eigenvectors.at<float>(0,1);
  if(check < 0.000001)
    return false;

//  float check = eigenvalues.at<float>(0);
//  if(check < 0 || check > 99999)
//    return false;


  cv::Mat eigenvector_top = cv::Mat::zeros(cv::Size(1,3), eigenvectors.type());
  cv::Mat T_eigenvector_top = cv::Mat::zeros(cv::Size(1,3), eigenvectors.type());
  eigenvector_top.at<float>(0) = eigenvectors.at<float>(0,0);
  eigenvector_top.at<float>(1) = eigenvectors.at<float>(0,1);
  eigenvector_top.at<float>(2) = eigenvectors.at<float>(0,2);
  T_eigenvector_top = T*eigenvector_top;
//  std::cout << "eigenvector_top" << eigenvector_top <<  std::endl;
//  std::cout << "T*eigenvector" << T_eigenvector_top <<  std::endl;


  float a = eigenvector_top.at<float>(0);
  float b = eigenvector_top.at<float>(1);
  float c = eigenvector_top.at<float>(2);
  float d = T_eigenvector_top.at<float>(0)-2*eigenvector_top.at<float>(0)*mx-eigenvector_top.at<float>(1)*my;
  float e = T_eigenvector_top.at<float>(1)-2*eigenvector_top.at<float>(2)*my-eigenvector_top.at<float>(1)*mx;
  float f  = T_eigenvector_top.at<float>(2) +
      eigenvector_top.at<float>(0)*mx*mx +
      eigenvector_top.at<float>(2)*my*my +
      eigenvector_top.at<float>(1)*mx*my -
      T_eigenvector_top.at<float>(0)*mx -
      T_eigenvector_top.at<float>(1)*my;

  float norm = sqrt(a*a+b*b+c*c+d*d+e*e+f*f);
  a /= norm;
  b /= norm;
  c /= norm;
  d /= norm;
  e /= norm;
  f /= norm;

  double tmp1 = b*b - 4*a*c;
  double tmp2 = sqrt((a-c)*(a-c)+b*b);
  double tmp3 = a*e*e + c*d*d - b*d*e + tmp1*f;

  double r1 = -sqrt(2*tmp3*(a+c+tmp2)) / tmp1;
  double r2 = -sqrt(2*tmp3*(a+c-tmp2)) / tmp1;
  float rl = r1>=r2 ? r1 : r2;
  float rs = r1<=r2 ? r1 : r2;

  float cx = (2*c*d - b*e) / tmp1;
  float cy = (2*a*e - b*d) / tmp1;

  float phi = 0.5 * atan2(b, a-c);
  if(r1>r2)
    phi += M_PI_2;

  // error check
  float error = 0;
  bool ret = true;
  for( int i = 0 ; i < N ; i++){
    float x = edge_segment[i].x;
    float y = edge_segment[i].y;

    float e = a*x*x + b*x*y + c*y*y + d*x + e*y + f;

//    error = fabs(e);
//    if( error > 100) {
//      ret = false;
//      break;
//    }
  }

  ellipse.A = a;
  ellipse.B = b;
  ellipse.C = c;
  ellipse.D = d;
  ellipse.E = e;
  ellipse.F = f;

  ellipse.center.x  = cx;
  ellipse.center.y  = cy;
  ellipse.radius_ls.width  = rl;
  ellipse.radius_ls.height  = rs;
  ellipse.angle  = phi;

  return ret;
}
std::vector<Circle> EDCircle::get_circle() {
  return circle_v_;
}
std::vector<Ellipse> EDCircle::get_ellipse() {
  return ellipse_v_;
}

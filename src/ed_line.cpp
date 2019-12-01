//
// Created by cjyj on 19. 12. 1..
//

#include "ed_line.h"
EDLine::EDLine() {

}
EDLine::~EDLine() {

}
std::vector<Line> EDLine::detect(std::vector<std::vector<cv::Point> > edge_segment, cv::Size img_size ) {
  // valid line length : -4log(N) / log (P);
  line_valid_threshold_ = -4.0f * log((float)std::min(img_size.width, img_size.height)) / log(0.125f);

  for (int i = 0; i < edge_segment.size(); i++) {
    // fit line

    // validation

  }
}

void EDLine::LineFit(std::vector<cv::Point> pixelChain) {
  int initial_length_of_pixelChain = pixelChain.size();
  float lineFitError = FLT_MAX; // current line fit error
  LineEquation lineEquation;    // y = ax + b OR x = ay + b

  std::vector<cv::Point> pixelChain_pop;

  while (pixelChain.size() > MIN_LINE_LENGTH){
    LeastSquaresLineFit(pixelChain, MIN_LINE_LENGTH, lineEquation, lineFitError);

    if (lineFitError <= 1.0)
      break; // OK. An initial line segment detected

    // Skip the first pixel & try with the remaining pixels
    pixelChain_pop.push_back( pixelChain.back());
    pixelChain.pop_back();
  } // end-while

  if (lineFitError > 1.0) return; // no initial line segment. Done.

  // An initial line segment detected. Try to extend this line segment
  int lineLen = MIN_LINE_LENGTH;
  while (lineLen < initial_length_of_pixelChain) {
    double d = ComputeError(lineEquation, pixelChain);
    if (d > 1.0) break;
    lineLen++;
  } //end-while
  // End of the current line segment. Compute the final line equation & output it.
  LeastSquaresLineFit(pixelChain, lineLen, lineEquation, lineFitError);
//  Output ‘‘lineEquation’’
  // Extract line segments from the remaining pixels
  LineFit(pixelChain);
} //end-LineFit

void EDLine::LeastSquaresLineFit(std::vector<cv::Point> pixelChain, int min_line_length, LineEquation &lineEquation, float &lineFitError) {
  float Sxx = 0.f;
  float Sxy = 0.f;
  float x_mean = 0.f;
  float y_mean = 0.f;

  int N = pixelChain.size();

  if ( N < min_line_length )
    return;

  for ( int i = 0; i < N; i++ ) {
    x_mean += pixelChain[i].x;
    y_mean += pixelChain[i].y;
  }
  x_mean = x_mean / ( float ) N;
  y_mean = y_mean / ( float ) N;

  for ( int  i = 0; i < N; i++ ) {
    float nx = pixelChain[i].x - x_mean;
    float ny = pixelChain[i].y - y_mean;
    Sxy += nx * ny;
    Sxx += nx * nx;
  }

  float A = Sxy / Sxx;
  float B = y_mean - A * x_mean;

  lineEquation.lineeq.A = A;
  lineEquation.lineeq.B = B;

  // error
  lineFitError = ComputeError(lineEquation, pixelChain);
}

float EDLine::ComputeError(LineEquation &lineEquation, std::vector<cv::Point> pixelChain) {
  int N = pixelChain.size();
  float accum_error = 0;
  for(int i = 0; i < N; i++) {
    accum_error += lineEquation.ComputePointDistance2Line(pixelChain[i]);
  }
  return accum_error / (float)N;
}

LineEquation::LineEquation() {

}
LineEquation::~LineEquation() {

}

float LineEquation::ComputePointDistance2Line(cv::Point point) {
  float A = lineeq.A;
  float B = lineeq.B;
  float error = 0;
  float x = point.x;
  float y = point.y;
  error = fabs( A*x - y + B ) / sqrt(A*A + 1);
  return error;
}

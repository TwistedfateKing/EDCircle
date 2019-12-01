//
// Created by cjyj on 19. 12. 1..
//

#include "ed_line.h"
EDLine::EDLine() {
  std::cout << "EDline init" << std::endl;
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

void EDLine::LineFit(std::vector<cv::Point> pixelChain, int noPixels) {
  double lineFitError = INFINITY; // current line fit error
  LineEquation lineEquation; // y = ax + b OR x = ay + b
  while (noPixels > MIN_LINE_LENGTH){
    LeastSquaresLineFit(pixelChain, MIN_LINE_LENGTH, lineEquation, lineFitError);
    if (lineFitError <= 1.0) break; // OK. An initial line segment detected
    pixelChain++; // Skip the first pixel & try with the remaining pixels
    noPixels–; // One less pixel
  } // end-while

  if (lineFitError > 1.0) return; // no initial line segment. Done.

  // An initial line segment detected. Try to extend this line segment
  int lineLen = MIN_LINE_LENGTH;
  while (lineLen < noPixels){
  double d = ComputePointDistance2Line(lineEquation, pixelChain[lineLen]);
  if (d > 1.0) break;
  lineLen++;
  } //end-while
  // End of the current line segment. Compute the final line equation & output it.
  LeastSquaresLineFit(pixelChain, lineLen, &lineEquation);
  Output ‘‘lineEquation’’
  // Extract line segments from the remaining pixels
  LineFit(pixelChain + lineLen, noPixels-lineLen);
} //end-LineFit

void EDLine::LeastSquaresLineFit(std::vector<cv::Point> pixelChain, int min_line_length, LineEquation &lineEquation, double &lineFitError){

}


LineEquation::LineEquation() {

}
LineEquation::LineEquation(float A, float B) {

}
LineEquation::~LineEquation() {

}

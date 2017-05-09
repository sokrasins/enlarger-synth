#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
  VideoCapture stream1(0);

  if( !stream1.isOpened() ) {
    cout << "cannot open camera";
  }

  while( true ) {
    Mat cameraFrame;
    stream1.read(cameraFrame);
    imshow("cam", cameraFrame);
  
    if( waitKey(30) >= 0 ) {break;}
  }

  return 0;
}

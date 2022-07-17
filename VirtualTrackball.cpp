#include "VirtualTrackball.h"
#include "trackball.h"


VirtualTrackball::VirtualTrackball( int w, int h ) {
  width = w;
  height = h;

  currentX = 0;
  currentY = 0;

  mousePressed = false;
  
  trackball( currentQuat, 0.0, 0.0, 0.0, 0.0 );
}

VirtualTrackball::~VirtualTrackball() {
  ;
}

void VirtualTrackball::setWindowSize( int w, int h ) {
  width = w;
  height = h;
}

void VirtualTrackball::mouseDown( int x, int y ) {
  currentX = x;
  currentY = y;
  mousePressed = true;
}

void VirtualTrackball::mouseMotion( int x, int y ) {
  trackball( incrementQuat,
	     (2.0f*currentX - width) / width,
	     (height - 2.0f*currentY) / height,
	     (2.0f*x - width) / width,
	     (height - 2.0f*y) / height);

  add_quats(incrementQuat, currentQuat, currentQuat);
  
  currentX = x;
  currentY = y;
}

void VirtualTrackball::mouseUp() {
  mousePressed = false;
}

bool VirtualTrackball::isMouseDown() {
  return mousePressed;
}

float *VirtualTrackball::getMatrix() {
  build_rotmatrix( matrix, currentQuat );
  return &(matrix[0][0]);
}


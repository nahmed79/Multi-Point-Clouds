#ifndef DISPLAY_SCENEVIEWER_C
#define DISPLAY_SCENEVIEWER_C

#include "SceneViewer.h"

#include "vec/Vector.hh"
//#include <base/values/Math.hh>

using namespace std;
using namespace base;

SceneViewer::SceneViewer( const unsigned int &winWidth, 
			  const unsigned int &winHeight,
			  const base::Vec3f &bboxmin, 
			  const base::Vec3f &bboxmax )
{
  init();
  d_mouseState = 0; 
  d_oldX =0; d_oldY = 0; 
  d_ldir = d_initialLight;
  d_vdir = d_initialView;
  d_vPos = d_initialView;
  d_view_tb = new VirtualTrackball( winWidth, winHeight );
  d_bboxmin = bboxmin;
  d_bboxmax = bboxmax;

  d_winWidth = winWidth;
  d_winHeight = winHeight;
  d_aspect = (float) winWidth / (float) winHeight;

  d_viewdist = 0.0f;

  d_nearplane = 0.01f;
  d_farplane = 1000.0f;
  d_fov = 40.0f;

  d_valid = true;
  computeEyeAt();
}


void
SceneViewer::MouseCallback(int button, int state, int x, int y)
{
  if ( !d_valid ){
    fprintf( stderr, "Error in SceneViewer::MouseCallback:\n");
    fprintf( stderr, "   Invalid Object.\n");
  }
  if( button == GLUT_LEFT_BUTTON ) {
    if( state == GLUT_DOWN ) {
      d_view_tb->mouseDown( x, y );
      d_mv_changed = true;
    }
    if( state == GLUT_UP ) {
      d_view_tb->mouseUp();
      d_mv_changed = true;
    }
  }
  if (state == GLUT_DOWN) {

    switch (button) {
    case GLUT_LEFT_BUTTON: d_mouseState |= MOUSE_LEFT; break;
    case GLUT_MIDDLE_BUTTON: d_mouseState |= MOUSE_MIDDLE; break;
    case GLUT_RIGHT_BUTTON: d_mouseState |= MOUSE_RIGHT; break;
    }
  }
  else {
    switch (button) {
    case GLUT_LEFT_BUTTON: d_mouseState = d_mouseState ^ MOUSE_LEFT; break;
    case GLUT_MIDDLE_BUTTON: d_mouseState =  d_mouseState ^ MOUSE_MIDDLE; break;
    case GLUT_RIGHT_BUTTON: d_mouseState =  d_mouseState ^ MOUSE_RIGHT; break;
    }
  }

  // store old status 
  d_oldX = x; d_oldY = y; d_oldZ = d_viewdist;
  d_oldEye = d_eye; 
  d_oldAt = d_at; 
  d_oldVdir = (d_eye - d_at);
  if (d_oldVdir.norm() < 1E-10) d_oldVdir = d_vdir;
//  if (isZero(d_oldVdir.norm())) d_oldVdir = d_vdir; 
  d_oldVdir.normalize(); 

  // compute a basis for Translation 
  d_baseZ = d_vdir; 
  d_baseX = ( d_baseZ % d_up );
  d_baseX.normalize(); // x
  d_baseY = d_baseX % d_baseZ;
  d_baseY.normalize(); // y
}

void
SceneViewer::MotionCallback(int x, int y )
{
  if ( !d_valid ){
    fprintf( stderr, "Error in SceneViewer::MotionCallback:\n");
    fprintf( stderr, "   Invalid Object.\n");
  }
  if (d_mouseState) {
    float dx = (float)x - d_oldX; 
    float dy = (float)y - d_oldY; 
    dx /= d_winWidth * 0.5f; 
    dy /= d_winHeight * 0.5f;
    dx *= d_scale;
    dy *= d_scale;

    Matrix4x4f mat; 

    switch (d_mouseState) {
      /*    case MOUSE_LEFT | MOUSE_MIDDLE: 
      d_mv_changed = true;
      //      d_at  = d_oldAt - d_oldVdir * dy; 


      d_viewdist = d_oldZ + dy; 
      return true; */
      
    case MOUSE_LEFT: 
      
      d_view_tb->mouseMotion( x, y );
      
      d_mv_changed = true;
      
      mat = d_view_tb->getMatrix();
      mat.invert();
      d_vdir = mat.transformPoint( d_initialView );
      d_vdir.normalize();
      
    case MOUSE_MIDDLE: 
      d_at  = d_oldAt + Vec3f(-1.,0,0) * dx + Vec3f(0,1.,0) * dy; 
      
      d_mv_changed = true;
    }
  }
}

void SceneViewer::WheelCallback(float delta) {
  d_mv_changed = true;
  d_viewdist = d_oldZ + delta;
  d_oldZ = d_viewdist;
}
 
void
SceneViewer::ReshapeCallback( int width, int height )
{
  if ( !d_valid ){
    fprintf( stderr, "Error in SceneViewer::ReshapeCallback:\n");
    fprintf( stderr, "   Invalid Object.\n");
  }

  glViewport( 0, 0, width, height );

  d_winWidth = width;
  d_winHeight = height;
  d_aspect = (float) width / (float) height;
  d_p_changed = true;
}

#endif /* DISPLAY_SCENEVIEWER_C */








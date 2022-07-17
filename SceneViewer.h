#ifndef DISPLAY_SCENEVIEWER_H
#define DISPLAY_SCENEVIEWER_H

#ifdef _WIN32
// this header file must be included before all the others
#define NOMINMAX
#include <windows.h>
#endif

#include<GL/glut.h>

#include <string>
#include "vec/Vector.hh"
#include "vec/Matrix4x4.hh"
#include "VirtualTrackball.h"

class SceneViewer {

public:

  // default constructor 
  inline SceneViewer();
  // copy constructor
  inline SceneViewer( const SceneViewer &);

  SceneViewer( const unsigned int &winWidth, const unsigned int &winHeight,
	       const base::Vec3f &bboxmin, const base::Vec3f &bboxmax );
  // destructor
  inline ~SceneViewer();
  // init routine
  inline void init();
  // assignment operator
  inline void operator=( const SceneViewer &);

  // default for both is (0,0,1);
  // no glutPostRedisplay called for changing of
  // initial values
  inline void setInitialLight( const base::Vec3f &_light );
  inline void setInitialView( const base::Vec3f &_view );
  inline void setUp( const base::Vec3f &_up );
  inline base::Vec3f getUp() { return d_up; }

  // get matrix data to put into glMultMatrix before display.
  inline const float *getModelviewMatrix();
  inline const float *getProjectionMatrix();
  inline base::Vec3f getViewDir() const;
  inline base::Vec3f getViewPos() {
    base::Vec3f eye, at, up; 
    float fov; 
    getCam(eye, at, up, fov); 
    return eye; 
  }

  inline void setViewPos(const base::Vec3f _eye) {
    d_eye = _eye; 
    d_mv_changed = true; 
  }

  inline base::Vec3f getLightDir() const;
  inline void getLightFrame(base::Vec3f& _ldir,
			    base::Vec3f& _ldirx, 
			    base::Vec3f& _ldiry) const;
  
  // put these in the glut callbacks
  // return values: if true post a redisplay
  // reserve cases 30-40 in control_callback for SceneViewer
  void MouseCallback(int button, int state, int x, int y);
  void MotionCallback(int x, int y);
  void ReshapeCallback(int width, int height );
  void WheelCallback(float delta);

  inline float getAspect() { return d_aspect; }
  inline float getNear() { return d_nearplane; }
  inline float getFar() { return  d_farplane; }

  inline void getCam(base::Vec3f &_eye, base::Vec3f &_at,
		     base::Vec3f &_up, float &_fov);
    

protected:

  inline void computeEyeAt(); 
protected:
  bool d_valid;

  int d_mouseState; 

  enum { MOUSE_LEFT = 1, 
	 MOUSE_MIDDLE = 2,  MOUSE_RIGHT = 4}; 
  

  base::Vec3f       d_initialLight;
  base::Vec3f       d_initialView;
  base::Vec3f       d_initialUp;
  base::Vec3f       d_vdir, d_ldir; // view dir, light dir
  base::Vec3f       d_ldirx, d_ldiry; // environmental frame
  
  base::Vec3f       d_bboxmin, d_bboxmax;
  base::Vec3f       d_vPos;
  base::Vec3f       d_eye;
  base::Vec3f       d_at;
  base::Vec3f       d_up;

  // various stuff to store conditions when pressing any mouse button
  int d_oldX, d_oldY; 
  float d_oldZ; 

  base::Vec3f       d_oldEye; 
  base::Vec3f       d_oldAt; 
  base::Vec3f       d_oldVdir; 
  base::Vec3f       d_baseX; // basis vector for translation 
  base::Vec3f       d_baseY; // basis vector for translation
  base::Vec3f       d_baseZ; // basis vector for translation
  float d_scale; // how much to scale translations


  base::Matrix4x4f  d_mat_mv, d_mat_p; // modelview and projection matrices
  bool              d_mv_changed, d_p_changed; // update matrix calculation
  float             d_aspect;	// aspect ratio
  unsigned int      d_winWidth, d_winHeight;

  // GUI stuff
  float d_viewdist; // view distance
  VirtualTrackball *d_view_tb;	// view rotation trackball
  float d_nearplane; // near clipping plane
  float d_farplane; // far clipping plane
  float d_fov; // field of view
};

inline
SceneViewer::SceneViewer()
{
  init();
}

inline
SceneViewer::SceneViewer( const SceneViewer &v )
{
  init();
  (*this) = v;
}

inline 
SceneViewer::~SceneViewer()
{
  if ( d_view_tb ) delete d_view_tb;	// view rotation trackball

}

inline void 
SceneViewer::init()
{
  d_valid = false;
  
  d_initialLight = base::Vec3f(0,0,1);
  d_initialView = base::Vec3f(0,0,1);
  d_initialUp = base::Vec3f(0,1,0);
  d_up = base::Vec3f(0,1,0);

  d_vdir = base::Vec3f(0,0,0);
  d_ldir = base::Vec3f(0,0,0);
  d_ldirx = base::Vec3f(1,0,0);
  d_ldiry = base::Vec3f(0,1,0);


  d_bboxmax = d_bboxmin = base::Vec3f( 0,0,0 );
  d_eye = base::Vec3f(0,0,0);
  d_at = base::Vec3f(0,0,0);
  d_aspect = 0;
  d_winWidth = 0;
  d_winHeight = 0;

  d_mat_mv.identity();
  d_mat_p.identity();
  d_mv_changed = true;
  d_p_changed = true;

  // GUI stuff
  d_view_tb = NULL;
}

inline void
SceneViewer::operator=( const SceneViewer &v)
{
  d_valid = v.d_valid;

  d_initialLight = v.d_initialLight;
  d_initialView = v.d_initialView;
  d_vdir = v.d_vdir;
  d_ldir = v.d_ldir;
  d_bboxmin = v.d_bboxmin;
  d_bboxmax = v.d_bboxmax;
  d_eye = v.d_eye;
  d_at = v.d_at;
  d_up = v.d_up;
  d_aspect = v.d_aspect;
  d_winWidth = v.d_winWidth;
  d_winHeight = v.d_winHeight;
  
  d_mat_mv = v.d_mat_mv;
  d_mat_p = v.d_mat_p;
  d_mv_changed = v.d_mv_changed;
  d_p_changed = v.d_p_changed;
  
  fprintf( stderr, "Warning from SceneViewer::operator=:\n");
  fprintf( stderr, "   Calling assignment operator not really a good idea.\n");
  fprintf( stderr, "   for this class, can't rebuild gui stuff.\n");
  // would have to save GLUI* and user's control callback

  d_viewdist = v.d_viewdist;

  d_nearplane = v.d_nearplane;
  d_farplane = v.d_farplane;
  d_fov = v.d_fov;
  d_view_tb = v.d_view_tb;
}

inline void
SceneViewer::setInitialLight( const base::Vec3f &_light )
{
  if ( !d_valid ){
    fprintf( stderr, "Error in SceneViewer::setInitialLight:\n");
    fprintf( stderr, "    Invalid object.\n");
    return;
  }
  d_initialLight = _light;
  d_ldirx = base::Vec3f(1,0,0);
  d_ldiry = base::Vec3f(0,1,0);
}

inline void
SceneViewer::setInitialView( const base::Vec3f &_view )
{
  if ( !d_valid ){
    fprintf( stderr, "Error in SceneViewer::setInitialView:\n");
    fprintf( stderr, "    Invalid object.\n");
    return;
  }
   d_initialView = _view;
}

inline void
SceneViewer::setUp( const base::Vec3f &_up )
{
  if ( !d_valid ){
    fprintf( stderr, "Error in SceneViewer::setUp:\n");
    fprintf( stderr, "    Invalid object.\n");
    return;
  }
  d_up = _up;
  d_mv_changed = true;
}


inline const float*
SceneViewer::getModelviewMatrix()
{
  if ( !d_valid ){
    fprintf( stderr, "Error in SceneViewer::getModelviewMatrix:\n");
    fprintf( stderr, "    Invalid object.\n");
    return NULL;
  }
  if ( d_mv_changed ){

    base::Vec3f center = (d_bboxmin + d_bboxmax) * 0.5;

    d_mat_mv.identity();
    d_mat_mv.lookAt( d_eye, d_at, d_up );
    d_mat_mv.translate( 0.0, 0.0, d_viewdist );

    d_mat_mv.translate( center );
    d_mat_mv = d_mat_mv * d_view_tb->getMatrix();
    d_mat_mv.translate( -center );
    
    d_mv_changed = false;
  }
  return d_mat_mv.getRawData();
}

inline const float*
SceneViewer::getProjectionMatrix()
{
  if ( !d_valid ){
    fprintf( stderr, "Error in SceneViewer::getProjectionMatrix:\n");
    fprintf( stderr, "    Invalid object.\n");
    return NULL;
  }
  if ( d_p_changed ){
    
    d_mat_p.identity();
    d_mat_p.perspective( d_fov, d_aspect, d_nearplane, d_farplane );
    d_p_changed = false;
  }
  return d_mat_p.getRawData();
}

inline base::Vec3f
SceneViewer::getViewDir() const
{
  if ( !d_valid ){
    fprintf( stderr, "Error in SceneViewer::getViewDir:\n");
    fprintf( stderr, "    Invalid object.\n");
    return base::Vec3f(0,0,0);
  }
  return d_vdir;
}
inline base::Vec3f 
SceneViewer::getLightDir() const
{
  if ( !d_valid ){
    fprintf( stderr, "Error in SceneViewer::getLightDir:\n");
    fprintf( stderr, "    Invalid object.\n");
    return base::Vec3f(0,0,0);
  }
  return d_ldir;
}

inline void 
SceneViewer::getLightFrame(base::Vec3f& _ldir,
			  base::Vec3f& _ldirx, 
			  base::Vec3f& _ldiry) const {
  _ldir = d_ldir; 
  _ldirx = d_ldirx;
  _ldiry = d_ldiry; 
}

inline void
SceneViewer::computeEyeAt()
{
  if ( !d_valid || d_bboxmax == base::Vec3f(0,0,0) ||
       d_bboxmin == base::Vec3f(0,0,0) || d_fov == 0.0 ){
    fprintf( stderr, "Error in SceneViewer::computeEyeAt:\n");
    fprintf( stderr, "   Invalid object.\n");
    return;
  }
  d_at = ( d_bboxmin + d_bboxmax ) * 0.5;

  float size = (d_bboxmax - d_bboxmin).norm();
  d_scale = size / 2.0f; 
  float dist = size / (float)tan( (float)(d_fov) * 2.0f * M_PI / 360.0f);
  d_eye = d_at + d_initialView*dist;
}

inline void 
SceneViewer::getCam(base::Vec3f &_eye, base::Vec3f &_at,
		    base::Vec3f &_up, float &_fov) {

  getModelviewMatrix();
//  d_mat_mv.print("mv");  
  base::Matrix4x4f dd(d_mat_mv); 
  dd.invert();  
  _eye = dd.transformPoint( base::Vec3f(0.,0.,0.) ); 
  base::Vec3f vd = dd.transformVector( base::Vec3f(0.,0.,-1.)); 
//  std::cout << "vd: " << vd << std::endl; 
  _at = _eye + vd; 
  
  _up = dd.transformVector( base::Vec3f(0.,1.,0.)); 
  
  _fov = d_fov;


//  dd.print("invert");
}

 
#endif /* DISPLAY_SCENEVIEWER_H */








#ifndef BUMPMAPS_VIRTUALTRACKBALL_HH
#define BUMPMAPS_VIRTUALTRACKBALL_HH

/** \file  VirtualTrackball.hh
    \brief This class implements a virtual trackball.
           It can be used to generate an OpenGL ModelView Matrix
 */


  /** \class VirtualTrackball VirtualTrackball.hh
      This class implements a virtual trackball.
      It can be used to generate an OpenGL ModelView Matrix */
  
class VirtualTrackball {
public:
  /** Initializes the virtual trackball with the window width and height. */
  VirtualTrackball( int width, int height );

  /** Deconstructor */
  ~VirtualTrackball();

  /** set a new window size (usually after resize events) */
  void setWindowSize( int width, int height );

  /** call this method when the mouse button was pressed */
  void mouseDown( int x, int y );

  /** During mouse motions, this method should be invoked */
  void mouseMotion( int x, int y );

  /** Call when mouse button was released */
  void mouseUp();

  /** Check if mouse is still pressed. This can be used to find 
      out, if you should do the mouseMotion() call at all! */
  bool isMouseDown();
  
  /** for idle functions, use this to continue motion */
  void continueMotion();

  /** return the rotation matrix */
  float *getMatrix();
  
protected:

  int currentX, currentY;
  int width, height;

  bool mousePressed;

  float currentQuat[4], incrementQuat[4];

  float matrix[4][4];
};


#endif /* BUMPMAPS_VIRTUALTRACKBALL_HH */

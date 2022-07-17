#include "SceneViewer.h"

class CustomSceneViewer : public SceneViewer {
public:
  CustomSceneViewer( const unsigned int &winWidth, const unsigned int &winHeight, const base::Vec3f &bboxmin, const base::Vec3f &bboxmax )  : SceneViewer( winWidth, winHeight, bboxmin, bboxmax ) {}

  inline void setBox(const base::Vec3f &bboxmin, const base::Vec3f &bboxmax) {
    d_bboxmin = bboxmin;
    d_bboxmax = bboxmax;
    d_mv_changed = true;
    computeEyeAt();
  }

  inline void setEye( const base::Vec3f &_eye ) {
    if ( !d_valid ){
      fprintf( stderr, "Error in SceneViewer::setEye:\n");
      fprintf( stderr, "    Invalid object.\n");
      return;
    }
    d_eye = _eye;
    d_mv_changed = true;
  }
};

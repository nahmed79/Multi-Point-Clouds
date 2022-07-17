#define _CRT_SECURE_NO_WARNINGS

#ifndef KINECT_DATA_HH
#define KINECT_DATA_HH

#ifndef M_PI
#define M_PI 3.141592653589793238462643f
#endif

#include <opencv\cv.h>
#include <opencv\highgui.h>

#include "vec\Vector.hh"
#include "vec\Matrix4x4.hh"

class Kinect_Camera
{
public:

	Kinect_Camera()
	{
		near_plane = 0.08f;
		far_plane = 12.0f;

		rgb_int =  0;
		depth_int = 0;
		drgb_R = 0;
		drgb_T = 0;
	}
	~Kinect_Camera();

	void init(const char* kinect_calibration_filename);

	void raw_depth_to_meters_3d(int raw_depth, int row, int col, float *vec3, int offset = 8);
	void depth_to_meters_3d_and_rgb(int x_d, int y_d, int raw_depth, float *xyz3d, float *rgb2d);

	void printM(CvMat* M, bool colmajor = true);
	void printM(float* M, int rows, int cols, bool colmajor = true);

	float rgb_KK[16];
	float depth_KK[16];
	float drgb_RT[16];

	float depth_baseline;
	float depth_offset;

	void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);
	void glhFrustumf2(float *matrix, float left, float right, float bottom, float top, float znear, float zfar);

	float near_plane, far_plane;

public:

	float raw_depth_to_meters(int raw_depth);
	float raw_depth_baseline(int raw_depht);
	float fix_depth_geometry(int orig_depth, int x_d, int y_d);

	void convertKKopencv2opengl(CvMat* opencvKK, float width, float height, float near_plane, float far_plane, float *openglKK);
	void convertRTopencv2opengl(CvMat* opencvR, CvMat* opencvT, float *openglRT);

	CvMat* rgb_int;
	CvMat* depth_int;

	CvMat* drgb_R;
	CvMat* drgb_T;

	base::Matrix4x4f RT_rgb;


	float fx_d,fy_d,cx_d,cy_d;
	float fx_rgb,fy_rgb,cx_rgb,cy_rgb;

	int d_width, d_height;
	int rgb_width, rgb_height;
};

#endif
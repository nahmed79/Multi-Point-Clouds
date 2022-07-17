#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
using namespace std;

#include "Kinect_Camera.h"

void Kinect_Camera::init(const char* kinect_calibration_filename)
{
	cout<<"Loading "<<kinect_calibration_filename<<endl;

	rgb_int = (CvMat*)cvLoad(kinect_calibration_filename, NULL, "rgb_intrinsics");
	//cout << '\n' << "rgb_intrinsics opencv (kinect_calibration.yml)" << endl;
	//printM(rgb_int);

	fx_rgb = (float)cvGetReal2D( rgb_int, 0, 0 );
	fy_rgb = (float)cvGetReal2D( rgb_int, 1, 1 );
	cx_rgb = (float)cvGetReal2D( rgb_int, 0, 2 );
	cy_rgb = (float)cvGetReal2D( rgb_int, 1, 2 );

	CvMat* rgb_size = (CvMat*)cvLoad(kinect_calibration_filename, NULL, "rgb_size");
	rgb_width = (int)cvGetReal2D( rgb_size, 0, 0 );
	rgb_height = (int)cvGetReal2D( rgb_size, 0, 1 );
	cvReleaseMat(&rgb_size);

	convertKKopencv2opengl(rgb_int, (float)rgb_width, (float)rgb_height, near_plane, far_plane, rgb_KK);
	//cout << '\n' << "rgb_intrinsics converted to opengl" << endl;
	//printM(rgb_KK, 4, 4);

	//      depth

	CvMat* d_baseline_offset = (CvMat*)cvLoad(kinect_calibration_filename, NULL, "depth_base_and_offset");
	depth_offset = (float)cvGetReal2D( d_baseline_offset, 0, 1 ); //offset
	depth_baseline = (float)cvGetReal2D( d_baseline_offset, 0, 0 ); //baseline
	cvReleaseMat(&d_baseline_offset);

	//cout << '\n' << "depth_base_and_offset (kinect_calibration.yml)" << endl;
	//cout << "offset: " << depth_offset << endl;
	//cout << "baseline: " << depth_baseline << endl;

	depth_int = (CvMat*)cvLoad(kinect_calibration_filename, NULL, "depth_intrinsics");
	//cout << '\n' << "depth_intrinsics opencv (kinect_calibration.yml)" << endl;
	//printM(depth_int);

	fx_d = (float)cvGetReal2D( depth_int, 0, 0 );
	fy_d = (float)cvGetReal2D( depth_int, 1, 1 );
	cx_d = (float)cvGetReal2D( depth_int, 0, 2 );
	cy_d = (float)cvGetReal2D( depth_int, 1, 2 );

	CvMat* d_size = (CvMat*)cvLoad(kinect_calibration_filename, NULL, "depth_size");
	d_width = (int)cvGetReal2D( d_size, 0, 0 );
	d_height = (int)cvGetReal2D( d_size, 0, 1 );
	cvReleaseMat(&d_size);

	convertKKopencv2opengl(depth_int, (float)d_width, (float)d_height, near_plane, far_plane, depth_KK);
	//cout << '\n' << "depth_intrinsics converted to opengl" << endl;
	//printM(depth_KK, 4, 4);

	drgb_R = (CvMat*)cvLoad(kinect_calibration_filename, NULL, "R");
	//cout << '\n' << "drgb_R opencv (kinect_calibration.yml)" << endl;
	//printM(drgb_R);

	drgb_T = (CvMat*)cvLoad(kinect_calibration_filename, NULL, "T");
	//cout << '\n' << "drgb_T opencv (kinect_calibration.yml)" << endl;
	//printM(drgb_T);

	convertRTopencv2opengl(drgb_R, drgb_T, drgb_RT);
	//cout << '\n' << "drgb_RT converted to opengl" << endl;
	//printM(drgb_RT, 4, 4);

	RT_rgb = base::Matrix4x4f(drgb_RT);

	RT_rgb.invert();
}

//http://opencv.willowgarage.com/wiki/Posit
//http://www.songho.ca/opengl/gl_projectionmatrix.html
//http://www.songho.ca/opengl/gl_transform.html

void Kinect_Camera::convertRTopencv2opengl(CvMat* opencvR, CvMat* opencvT, float *openglRT)
{
	//      opencv: row-major       
	//      R x axis
	float xx = (float)cvGetReal2D( opencvR, 0, 0 );
	float xy = (float)cvGetReal2D( opencvR, 1, 0 );
	float xz = (float)cvGetReal2D( opencvR, 2, 0 );
	//      R y axis        
	float yx = (float)cvGetReal2D( opencvR, 0, 1 );
	float yy = (float)cvGetReal2D( opencvR, 1, 1 );
	float yz = (float)cvGetReal2D( opencvR, 2, 1 );
	//      R z axis        
	float zx = (float)cvGetReal2D( opencvR, 0, 2 );
	float zy = (float)cvGetReal2D( opencvR, 1, 2 );
	float zz = (float)cvGetReal2D( opencvR, 2, 2 );
	//      T
	float tx = (float)cvGetReal2D( opencvT, 0, 0 );
	float ty = (float)cvGetReal2D( opencvT, 1, 0 );
	float tz = (float)cvGetReal2D( opencvT, 2, 0 ); 

	//      opengl: col-major       
	openglRT[0] = xx;       openglRT[4] = yx;       openglRT[8] = zx;       openglRT[12] = tx;
	openglRT[1] = xy;       openglRT[5] = yy;       openglRT[9] = zy;       openglRT[13] = ty;
	openglRT[2] = xz;       openglRT[6] = yz;       openglRT[10] = zz;      openglRT[14] = tz;
	openglRT[3] = 0.;       openglRT[7] = 0.;       openglRT[11] = 0.;      openglRT[15] = 1.;
}

Kinect_Camera::~Kinect_Camera()
{
	if ( rgb_int )
		cvReleaseMat(&rgb_int); 
	if ( depth_int )
		cvReleaseMat(&depth_int);
	if ( drgb_R )
		cvReleaseMat(&drgb_R);
	if ( drgb_T)
		cvReleaseMat(&drgb_T);
}

void Kinect_Camera::convertKKopencv2opengl(CvMat* opencvKK, float width, float height, float near, float far, float* openglKK)
{
	float fx = (float)cvGetReal2D( opencvKK, 0, 0 );
	float fy = (float)cvGetReal2D( opencvKK, 1, 1 );
	float cx = (float)cvGetReal2D( opencvKK, 0, 2 );
	float cy = (float)cvGetReal2D( opencvKK, 1, 2 );

	float A = 2.0f*fx/width;
	float B = 2.0f*fy/height;
	float C = 2.0f*(cx/width)-1.0f;
	float D = 2.0f*(cy/height)-1.0f;
	float E = -(far_plane+near_plane)/(far_plane-near_plane);
	float F = -2.0f*far_plane*near_plane/(far_plane-near_plane);

	//      col-major
	openglKK[0]= A;         openglKK[4]= 0.;        openglKK[8]= C;         openglKK[12]= 0.;
	openglKK[1]= 0.;        openglKK[5]= B;         openglKK[9]= D;         openglKK[13]= 0.;
	openglKK[2]= 0.;        openglKK[6]= 0.;        openglKK[10]= E;        openglKK[14]= F;
	openglKK[3]= 0.;        openglKK[7]= 0.;        openglKK[11]= -1.;      openglKK[15]= 0.;       
}

//http://openkinect.org/wiki/Imaging_Information
//http://nicolas.burrus.name/index.php/Research/KinectCalibration

void Kinect_Camera::depth_to_meters_3d_and_rgb(int x_d, int y_d, int raw_depth, float* xyz3d, float *rgb2d)
{
	raw_depth_to_meters_3d(raw_depth, x_d, y_d, xyz3d, -8);

	base::Vec3f _p3d = base::Vec3f(xyz3d[0],xyz3d[1],xyz3d[2]);
	base::Vec3f p3d_rgb = RT_rgb.transformPoint(_p3d);

	rgb2d[0] = (p3d_rgb[0] * fx_rgb /p3d_rgb[2]) + cx_rgb;
	rgb2d[1] = (p3d_rgb[1] * fy_rgb /p3d_rgb[2]) + cy_rgb;

	if (rgb2d[0] > rgb_width)
		rgb2d[0] = (float)rgb_width;
	if (rgb2d[1] > rgb_height)
		rgb2d[1] = (float)rgb_height;
}

void Kinect_Camera::raw_depth_to_meters_3d(int raw_depth, int x_d, int y_d, float* vec3, int offset)
{
	//      z
	vec3[2] = raw_depth_baseline(raw_depth);
	//      x
	vec3[0] = (x_d + offset - cx_d) * vec3[2] / fx_d; //burrus      
	//      y
	vec3[1] = (y_d - cy_d) * vec3[2] / fy_d; //burrus
}               

float Kinect_Camera::raw_depth_to_meters(int raw_depth)
{
	if (raw_depth < 2047)
	{
		const float k1 = 0.1236f;
		const float k2 = 2842.5f;
		const float k3 = 1.1863f;
		const float k4 = 0.0370f;
		return k1 * tanf((raw_depth / k2) + k3) - k4; // calculate in meters
	}
	return 0;
}

//see Burrus RGBDProcessor::computeKinectDepthBaseline()
float Kinect_Camera::raw_depth_baseline(int raw_depth)
{
	float depth = 0;
	if (raw_depth < 2047)
	{
		float meanFocal = (fx_d + fy_d)/2.0f;
		depth = meanFocal * 8.0f * depth_baseline / (depth_offset - raw_depth);
	}
	if (depth < 0)
		depth = 0;
	else if (depth > 10)
		depth = 10;
	return depth;
}

//see Burrus RGBDProcessor::fixDepthGeometry()
float Kinect_Camera::fix_depth_geometry(int orig_depth, int x_d, int y_d)
{
	float dx = x_d - cx_d;
	float dy = y_d - cy_d;

	base::Vec3f v(dx/fx_d, dy/fy_d, 1);
	float norm = sqrt(v.dot(v));
	v = v * (1.0f/norm);
	v *= (float)orig_depth;

	float depth_z = v[2];
	return depth_z;
}

void Kinect_Camera::printM(CvMat* M, bool colmajor)
{
	int i,j;
	if (colmajor)
	{
		for (i = 0; i < M->rows; i++)
		{
			printf("\n");                           
			switch( CV_MAT_DEPTH(M->type) )
			{
			case CV_32F:
			case CV_64F:
				for (j = 0; j < M->cols; j++)
					printf("%9.3f ", (float)cvGetReal2D( M, i, j ));
				break;
			case CV_8U:
			case CV_16U:
				for (j = 0; j < M->cols; j++)
					printf("%6d",(int)cvGetReal2D( M, i, j ));
				break;
			default:
				break;
			}
		}
	}
	else
	{
		for (j = 0; j < M->cols; j++)
		{
			printf("\n");                           
			switch( CV_MAT_DEPTH(M->type) )
			{
			case CV_32F:
			case CV_64F:
				for (i = 0; i < M->rows; i++)
					printf("%9.3f ", (float)cvGetReal2D( M, i, j ));
				break;
			case CV_8U:
			case CV_16U:
				for (i = 0; i < M->rows; i++)
					printf("%6d",(int)cvGetReal2D( M, i, j ));
				break;
			default:
				break;
			}
		}
	}
	printf("\n");
}

void Kinect_Camera::printM(float* M, int rows, int cols, bool colmajor)
{
	if (colmajor)
	{
		for (int j = 0; j < cols; j++)
		{
			printf("\n");
			for (int i = 0; i < rows; i++)
			{
				printf("%9.3f ", M[i*cols+j]);
			}       
		}
	}
	else
	{
		for (int i = 0; i < rows; i++)
		{
			printf("\n");
			for (int j = 0; j < cols; j++)
			{
				printf("%9.3f ", M[i*cols+j]);
			}       
		}
	}
	printf("\n");
}

void Kinect_Camera::glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar)
{
	float ymax, xmax;
	ymax = znear * tanf(fovyInDegrees * M_PI / 360.0f);
	xmax = ymax * aspectRatio;
	glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void Kinect_Camera::glhFrustumf2(float *matrix, 
								 float left, float right, 
								 float bottom, float top,
								 float znear, float zfar)
{
	float temp, temp2, temp3, temp4;
	temp = 2.0f * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	matrix[0] = temp / temp2;
	matrix[1] = 0.0;
	matrix[2] = 0.0;
	matrix[3] = 0.0;
	matrix[4] = 0.0;
	matrix[5] = temp / temp3;
	matrix[6] = 0.0;
	matrix[7] = 0.0;
	matrix[8] = (right + left) / temp2;
	matrix[9] = (top + bottom) / temp3;
	matrix[10] = (-zfar - znear) / temp4;
	matrix[11] = -1.0;
	matrix[12] = 0.0;
	matrix[13] = 0.0;
	matrix[14] = (-temp * zfar) / temp4;
	matrix[15] = 0.0;
}
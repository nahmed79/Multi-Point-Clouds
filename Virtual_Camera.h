#ifndef VIRTUAL_CAMERA_HH
#define VIRTUAL_CAMERA_HH

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#include "vec\Matrix4x4.hh"

class Virtual_Camera
{
public:
	double fc[2]; // Focal length
	double cc[2]; // Principal Point
	double kc[5]; // Distortion coefficients
	double nx, ny; // Image size

	float omc_x, omc_y, omc_z; // Rotation angles
	float tc_x, tc_y, tc_z; // Translation

	float r_omc_x, r_omc_y, r_omc_z; // Relative rotation angles
	float r_tc_x, r_tc_y, r_tc_z; // Relative translation

	float r_tform[9]; // For testing relative rotation
	base::Matrix4x4f r_pc; // For testing purpose

	float tform[9]; // For testing rotation
	base::Matrix4x4f pc; // for testing purpose

	base::Matrix4x4f modelview;
	base::Matrix4x4f projection;
	base::Matrix4x4f mvp; // modelview and projection

	base::Matrix4x4f r_modelview;

	base::Matrix4x4f t1, t1_t, t2, t2_t;

	bool init(const string& filename);
};

#endif
#define _CRT_SECURE_NO_WARNINGS

#ifndef SCENE_CONFIG_HH
#define SCENE_CONFIG_HH

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Kinect_Camera.h"
#include "Virtual_Camera.h"

using namespace std;

class Scene_Config
{
public:
	string name;
	int num_frames;
	int start_frame, end_frame;
	int num_cameras;
	int num_background_frames;

	vector<string> camera_str;
	vector<int> camera_start;

	string base_str;
	string image_str, depth_str, internal_calib_str, overlap_image_str;
	string background_str, cleaned_depth_str, point_cloud_str;
	string cleaned_point_cloud_str, transformed_depth_str;
	string sil_img_str, sil_depth_str;
	string skeleton_str, external_calib_str;

	bool init(const string& conf_file);

	vector<Kinect_Camera> kc;
	vector<Virtual_Camera> vc;

};

#endif
#include "Scene_Config.h"

bool Scene_Config::init(const string& conf_file)
{
	ifstream in(conf_file);

	if ( !in.good() )
		return false;

	in>>name;
	in>>num_frames;
	in>>start_frame>>end_frame;

	in>>num_background_frames;

	in>>num_cameras;

	camera_str.resize(num_cameras);
	camera_start.resize(num_cameras);

	for(int i = 0; i < num_cameras; i++)
		in>>camera_str[i]>>camera_start[i];

	in>>base_str;
	in>>image_str;
	in>>depth_str;
	in>>internal_calib_str;
	in>>overlap_image_str;
	in>>background_str>>cleaned_depth_str>>point_cloud_str;
	in>>cleaned_point_cloud_str>>transformed_depth_str;
	in>>sil_img_str>>sil_depth_str;
	in>>skeleton_str>>external_calib_str;

	kc.resize(num_cameras);
	vc.resize(num_cameras);

	char kinect_cam_file[512], virtual_cam_file[512];

	for(int i = 0; i < num_cameras; i++)
	{
		//sprintf(kinect_cam_file, internal_calib_str.c_str(), camera_str[i].c_str());
		sprintf(virtual_cam_file, external_calib_str.c_str(), camera_str[i].c_str());

		//kc[i].init(kinect_cam_file);
		vc[i].init(virtual_cam_file);
	}

	return true;
}
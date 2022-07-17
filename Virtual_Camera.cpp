#include "Virtual_Camera.h"

using namespace base;

bool Virtual_Camera::init(const string& filename)
{
	cout<<"Loading "<<filename<<endl;

	ifstream in(filename);

	if ( !in.good() )
	{
		cout<<"Cannot load "<<filename<<endl;
		return false;
	}
	
	/*in>>fc[0]>>fc[1];
	in>>cc[0]>>cc[1];
	in>>kc[0]>>kc[1]>>kc[2]>>kc[3]>>kc[4];

	in>>omc_x>>omc_y>>omc_z;
	in>>tc_x>>tc_y>>tc_z;

	tc_x /= 1000;
	tc_y /= 1000;
	tc_z /= 1000;


	float test[16];

	for(int i = 0; i < 9; i++)
		in>>r_tform[i];

	test[0] = r_tform[0];		test[4] = r_tform[1];		test[8] = r_tform[2];		test[12] = tc_x;
	test[1] = r_tform[3];		test[5] = r_tform[4];		test[9] = r_tform[5];		test[13] = tc_y;
	test[2] = r_tform[6];		test[6] = r_tform[7];		test[10] = r_tform[8];		test[14] = tc_z;
	test[3] = 0.0f;				test[7] = 0.0f;				test[11] = 0.0f;			test[15] = 1.0f;

	pc = Matrix4x4f(test);

	in>>r_omc_x>>r_omc_y>>r_omc_z;
	in>>r_tc_x>>r_tc_y>>r_tc_z;

	r_tc_x = r_tc_y = r_tc_z = 0.0;

	for(int i = 0; i < 9; i++)
		in>>tform[i];

	test[0] = tform[0];		test[4] = tform[1];		test[8] = tform[2];		test[12] = r_tc_x;
	test[1] = tform[3];		test[5] = tform[4];		test[9] = tform[5];		test[13] = r_tc_y;
	test[2] = tform[6];		test[6] = tform[7];		test[10] = tform[8];	test[14] = r_tc_z;
	test[3] = 0.0f;			test[7] = 0.0f;			test[11] = 0.0f;		test[15] = 1.0f;

	r_pc = Matrix4x4f(test);

	//r_pc.invert();*/

	float tform[16], test[16];

	for(int i = 0; i < 16; i++)
		in>>tform[i];

	test[0] = tform[0];		test[4] = tform[1];		test[8] = tform[2];		test[12] = tform[3];
	test[1] = tform[4];		test[5] = tform[5];		test[9] = tform[6];		test[13] = tform[7];
	test[2] = tform[8];		test[6] = tform[9];		test[10] = tform[10];	test[14] = tform[11];
	test[3] = tform[12];	test[7] = tform[13];	test[11] = tform[14];	test[15] = tform[15];

	t1 = Matrix4x4f(test);
	t1_t = Matrix4x4f(t1.getRawData());
	t1_t.transpone();

	//t1.transpone();

	t1.print("T1");

	/*for(int i=0; i < 16; i++)
		in>>test[i];

	t2 = Matrix4x4f(test);
	t2_t = Matrix4x4f(t2.getRawData());
	t2_t.transpone();*/

	in.close();

	return true;
}
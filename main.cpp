#include "main.h"
#include "Scene_Config.h"

float colors[][3] = { {1, 0, 1} , {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0}, {0.5, 0.5, 1}, {1, 0.5, 0.5}, {0.5f, 1.0f, 0.5f}, {0.5, 0.5, 0.5} };

CustomSceneViewer *scene=NULL;
int w=800,h=600;

Scene_Config conf;

bool draw_color = false;

struct PointRGB
{
	Point p;
	float color[3];
};

vector< vector<PointRGB> > Model;

struct Joint
{
	float x, y, z;
	int start, end;
};

Joint Skeleton[8][20];

Bounding_Box root_bbox;
bool bbox_init = false;

int selected = 0;

Bounding_Box init_bounding_box(vector< vector<PointRGB> >& cloud);

GLfloat rotate_x[6] = {0.0f}, rotate_y[6] = {0.0f}, rotate_z[6] = {0.0f};
GLfloat tr_x[6] = {0.0f}, tr_y[6] = {0.0f}, tr_z[6] = {0.0f};
GLfloat sc_x[6]= {1.0f}, sc_y[6] = {1.0f}, sc_z[6] = {1.0f};
GLfloat chg_rot = 0.1f;
GLfloat chg_tr = 0.05f;
GLfloat chg_sc = 0.01f;

void loadCurrentFrame();

void loadNextFrame()
{
	for(int i = 0; i < conf.num_cameras; i++)
		conf.camera_start[i]++;

	loadCurrentFrame();
}

void loadPreviousFrame()
{
	for(int i = 0; i < conf.num_cameras; i++)
		conf.camera_start[i]--;

	loadCurrentFrame();
}

void glInit(int *argcp, char **argv)
{
	glutInit(argcp, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(w,h);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Point Cloud Viewer");

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glClearColor (1.0, 1.0, 1.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1,1,1,0);
}

void draw_bounding_box(const Bounding_Box& bbox, const float r, const float g, const float b)
{
	glDisable(GL_LIGHTING);

	glColor3f(r, g, b);

	//y-z-min_x face

	glBegin(GL_LINES);
	glVertex3f(bbox.min_x, bbox.min_y, bbox.min_z);
	glVertex3f(bbox.min_x, bbox.min_y, bbox.max_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.min_x, bbox.min_y, bbox.max_z);
	glVertex3f(bbox.min_x, bbox.max_y, bbox.max_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.min_x, bbox.max_y, bbox.max_z);
	glVertex3f(bbox.min_x, bbox.max_y, bbox.min_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.min_x, bbox.max_y, bbox.min_z);
	glVertex3f(bbox.min_x, bbox.min_y, bbox.min_z);
	glEnd();

	//y-z-max_x face

	glBegin(GL_LINES);
	glVertex3f(bbox.max_x, bbox.min_y, bbox.min_z);
	glVertex3f(bbox.max_x, bbox.min_y, bbox.max_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.max_x, bbox.min_y, bbox.max_z);
	glVertex3f(bbox.max_x, bbox.max_y, bbox.max_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.max_x, bbox.max_y, bbox.max_z);
	glVertex3f(bbox.max_x, bbox.max_y, bbox.min_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.max_x, bbox.max_y, bbox.min_z);
	glVertex3f(bbox.max_x, bbox.min_y, bbox.min_z);
	glEnd();

	// x-z-min_y face

	glBegin(GL_LINES);
	glVertex3f(bbox.min_x, bbox.min_y, bbox.min_z);
	glVertex3f(bbox.min_x, bbox.min_y, bbox.max_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.min_x, bbox.min_y, bbox.max_z);
	glVertex3f(bbox.max_x, bbox.min_y, bbox.max_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.max_x, bbox.min_y, bbox.max_z);
	glVertex3f(bbox.max_x, bbox.min_y, bbox.min_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.max_x, bbox.min_y, bbox.min_z);
	glVertex3f(bbox.min_x, bbox.min_y, bbox.min_z);
	glEnd();

	// x-z-max_y face

	glBegin(GL_LINES);
	glVertex3f(bbox.min_x, bbox.max_y, bbox.min_z);
	glVertex3f(bbox.min_x, bbox.max_y, bbox.max_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.min_x, bbox.max_y, bbox.max_z);
	glVertex3f(bbox.max_x, bbox.max_y, bbox.max_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.max_x, bbox.max_y, bbox.max_z);
	glVertex3f(bbox.max_x, bbox.max_y, bbox.min_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.max_x, bbox.max_y, bbox.min_z);
	glVertex3f(bbox.min_x, bbox.max_y, bbox.min_z);
	glEnd();

	// x-y-min_z face

	glBegin(GL_LINES);
	glVertex3f(bbox.min_x, bbox.min_y, bbox.min_z);
	glVertex3f(bbox.min_x, bbox.max_y, bbox.min_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.min_x, bbox.max_y, bbox.min_z);
	glVertex3f(bbox.max_x, bbox.max_y, bbox.min_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.max_x, bbox.max_y, bbox.min_z);
	glVertex3f(bbox.max_x, bbox.min_y, bbox.min_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.max_x, bbox.min_y, bbox.min_z);
	glVertex3f(bbox.min_x, bbox.min_y, bbox.min_z);
	glEnd();

	// x-y-max_z face (It is useless to draw this face, isn't it? :-)

	glBegin(GL_LINES);
	glVertex3f(bbox.min_x, bbox.min_y, bbox.max_z);
	glVertex3f(bbox.min_x, bbox.max_y, bbox.max_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.min_x, bbox.max_y, bbox.max_z);
	glVertex3f(bbox.max_x, bbox.max_y, bbox.max_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.max_x, bbox.max_y, bbox.max_z);
	glVertex3f(bbox.max_x, bbox.min_y, bbox.max_z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(bbox.max_x, bbox.min_y, bbox.min_z);
	glVertex3f(bbox.min_x, bbox.min_y, bbox.min_z);
	glEnd();

	glEnable(GL_LIGHTING);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glMultMatrixf( scene->getProjectionMatrix() );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glMultMatrixf( scene->getModelviewMatrix() );
	glLineWidth(1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	float mColor[] = {255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mColor);


	glDisable(GL_LIGHTING);

	for(size_t c = 0; c < Model.size(); c++)
	{
		glPushMatrix();
		glRotatef(rotate_x[c], 1.0f, 0.0f, 0.0f);
		glRotatef(rotate_y[c], 0.0f, 1.0f, 0.0f);
		glRotatef(rotate_z[c], 0.0f, 0.0f, 1.0f);

		glTranslatef(tr_x[c], tr_y[c], tr_z[c]);

		for(int i = 0; i < 20; i++)
		{
			glColor3f(colors[c+4][0], colors[c+4][1], colors[c+4][2]);

			int start = Skeleton[c][i].start;
			int end = Skeleton[c][i].end;

			glPointSize(8.0);

			glBegin(GL_POINTS);
			glVertex3f(Skeleton[c][end].x, Skeleton[c][end].y, Skeleton[c][end].z);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(Skeleton[c][start].x, Skeleton[c][start].y, Skeleton[c][start].z);
			glVertex3f(Skeleton[c][end].x, Skeleton[c][end].y, Skeleton[c][end].z);
			glEnd();
		}

		glPointSize(1.0);

		glBegin(GL_POINTS);

		for(size_t i = 0; i < Model[c].size(); i++)
		{
			if ( !draw_color )
				glColor3f(Model[c][i].color[0], Model[c][i].color[1], Model[c][i].color[2]);
			else
			{
				glColor3f(colors[c][0], colors[c][1], colors[c][2]);

				if ( c == selected )
					glColor3f(colors[8][0], colors[8][1], colors[8][2]);
			}
			glVertex3f(Model[c][i].p.x, Model[c][i].p.y, Model[c][i].p.z);



		}

		glEnd();

		glPopMatrix();
	}


	glLineWidth(1.0f);

	if ( bbox_init )
		draw_bounding_box(root_bbox, 0, 0, 0);


	glutSwapBuffers();
}


void mouse(int button, int state, int x, int y)
{
	if (button == 3)
		scene->WheelCallback(-0.5f);
	else if (button == 4)
		scene->WheelCallback(0.5f);
	else
		scene->MouseCallback(button, state, x, y);
	glutPostRedisplay();
}

void resetAll()
{
	rotate_x[selected] = rotate_y[selected] = rotate_z[selected] = 0.0f;
	tr_x[selected] = tr_y[selected] = tr_z[selected] = 0.0f;
	sc_x[selected] = sc_y[selected] = sc_z[selected] = 1.0f;
}

void keyboard(unsigned char key, int x, int y)
{	
	if ( key == 'C' || key == 'c' )
		draw_color = !draw_color;

	if ( key == '9' )
		chg_rot += 0.01f;

	if ( key == '0' )
		chg_rot -= 0.01f;

	if ( key == 'V' || key == 'v' )
		chg_tr += 0.01f;

	if ( key == 'B' || key == 'b' )
		chg_tr -= 0.01f;

	if ( key == 'S' || key == 's' )
		tr_y[selected] += chg_tr;
	else if ( key == 'W' || key == 'w' )
		tr_y[selected] -= chg_tr;
	if ( key == 'A' || key == 'a' )
		tr_x[selected] -= chg_tr;
	else if ( key == 'D' || key == 'd' )
		tr_x[selected] += chg_tr;
	if ( key == 'Q' || key == 'q' )
		tr_z[selected] += chg_tr;
	else if ( key == 'E' || key == 'e' )
		tr_z[selected] -= chg_tr;

	if ( key == 'U' || key == 'u' )
		sc_y[selected] += chg_sc;
	else if ( key == 'J' || key == 'j' )
		sc_y[selected] -= chg_sc;
	if ( key == 'H' || key == 'h' )
		sc_x[selected] -= chg_sc;
	else if ( key == 'K' || key == 'k' )
		sc_x[selected] += chg_sc;
	if ( key == 'Y' || key == 'y' )
		sc_z[selected] += chg_sc;
	else if ( key == 'I' || key == 'i' )
		sc_z[selected] -= chg_sc;

	if ( key == 'X' || key == 'x' )
		resetAll();

	if ( key == '1' )
		rotate_x[selected] -= chg_rot;
	else if ( key == '2' )
		rotate_x[selected] += chg_rot;
	else if ( key == '3' )
		rotate_y[selected] -= chg_rot;
	else if ( key == '4' )
		rotate_y[selected] += chg_rot;
	else if ( key == '5' )
		rotate_z[selected] -= chg_rot;
	else if ( key == '6' )
		rotate_z[selected] += chg_rot;

	if ( rotate_x[selected] < 0.0f )
		rotate_x[selected] = 360.0f + rotate_x[selected];

	if ( rotate_x[selected] > 360.0f )
		rotate_x[selected] = rotate_x[selected] - 360.0f;

	if ( rotate_y[selected] < 0.0f )
		rotate_y[selected] = 360.0f + rotate_y[selected];

	if ( rotate_y[selected] > 360.0f )
		rotate_y[selected] = rotate_y[selected] - 360.0f;

	if ( rotate_z[selected] < 0.0f )
		rotate_z[selected] = 360.0f + rotate_z[selected];

	if ( rotate_z[selected] > 360.0f )
		rotate_z[selected] = rotate_z[selected] - 360.0f;

	cout<<rotate_x[selected]<<" "<<rotate_y[selected]<<" "<<rotate_z[selected]<<endl;
	cout<<tr_x[selected]<<" "<<tr_y[selected]<<" "<<tr_z[selected]<<endl;
	cout<<sc_x[selected]<<" "<<sc_y[selected]<<" "<<sc_z[selected]<<endl;
	cout<<chg_rot<<" "<<chg_tr<<" "<<chg_sc<<endl;

	if ( key == 'Z' || key == 'z' )
	{
		base::Matrix4x4f current;
		current.identity();
		current.rotateX(rotate_x[selected]);
		current.rotateY(rotate_y[selected]);
		current.rotateZ(rotate_z[selected]);
		current.translate(tr_x[selected], tr_y[selected], tr_z[selected]);

		current.print("Current");
	}

	if ( key == 'n' )
	{
		selected --;

		if ( selected < 0 )
			 selected = conf.num_cameras + selected;
	}

	if ( key == 'm' )
	{
		selected++;

		if ( selected >= conf.num_cameras )
			selected = conf.num_cameras - selected;
	}

	if ( key == '=' )
		loadNextFrame();

	if ( key == '-' )
		loadPreviousFrame();

	if ( key == '[')
	{
		conf.camera_start[selected]--;
		loadCurrentFrame();
	}

	if ( key == ']')
	{
		conf.camera_start[selected]++;
		loadCurrentFrame();
	}

	cout<<"Selected: "<<selected<<endl;

	glutPostRedisplay();
}

void reshape(int width, int height )
{
	w=width; h=height;
	scene->ReshapeCallback(width, height);
	glutPostRedisplay();
}

void motion(int x, int y)
{
	scene->MotionCallback(x,y);
	glutPostRedisplay();
}

void wheel(float d)
{
	scene->WheelCallback(d);
	glutPostRedisplay();
}


void readModel()
{
	for(int c = 0; c < conf.num_cameras; c++)
	{
		char filename[512];
		sprintf(filename, conf.point_cloud_str.c_str(), conf.camera_str[c].c_str(), conf.camera_start[c]);
		//sprintf(filename, conf.cleaned_point_cloud_str.c_str(), conf.camera_str[i].c_str(), conf.camera_start[i]);
		ifstream in(filename);
		if(!in)
			continue;

		cout<<"Loading "<<filename<<"... ";

		for(int i = 0; i < 20; i++)
		{
			int dummy, dx, dy, cx, cy;
			unsigned short depth;

			//in>>dummy>>Skeleton[i].x>>Skeleton[i].y>>Skeleton[i].z>>dx>>dy>>depth>>cx>>cy>>Skeleton_SDK[i].x>>Skeleton_SDK[i].y>>Skeleton_SDK[i].z>>cx>>cy;
			in>>Skeleton[c][i].x>>Skeleton[c][i].y>>Skeleton[c][i].z>>dx>>dy>>depth>>cx>>cy;
			in>>dummy>>dummy>>Skeleton[c][i].start>>Skeleton[c][i].end;

			base::Vec3f sPos = base::Vec3f(Skeleton[c][i].x, Skeleton[c][i].y, Skeleton[c][i].z);
			sPos = conf.vc[c].t1.transformPoint(sPos);

			Skeleton[c][i].x = sPos[0];
			Skeleton[c][i].y = sPos[1];
			Skeleton[c][i].z = sPos[2];
		}

		Model[c].clear();

		while( !in.eof() )
		{
			PointRGB pr;

			in>>pr.p.x>>pr.p.y>>pr.p.z;

			int r, g, b;

			in>>r>>g>>b;

			int x,y;

			in>>x>>y;

			pr.color[0] = r / 255.0f;
			pr.color[1] = g / 255.0f;
			pr.color[2] = b / 255.0f;

			Model[c].push_back(pr);
		}

		cout<<Model[c].size()<<endl;

		cout<<"Finished!"<<endl;
	}
}

Bounding_Box init_bounding_box(vector< vector<PointRGB> >& cloud)
{
	Bounding_Box bbox;

	bbox.max_x = bbox.max_y = bbox.max_z = -10000;
	bbox.min_x = bbox.min_y = bbox.min_z = 10000;

	for(size_t c = 0; c < cloud.size(); c++)
	{
		cout<<Model[c].size()<<endl;

		for(size_t i = 0; i < cloud[c].size(); i++)
		{
			base::Vec3f v(cloud[c][i].p.x, cloud[c][i].p.y, cloud[c][i].p.z);
			//v = conf.vc[c].r_pc.transformPoint(v);

			cloud[c][i].p.x = v[0];
			cloud[c][i].p.y = v[1];
			cloud[c][i].p.z = v[2];
			
			if ( !c || c )
			{
				v = conf.vc[c].t1.transformPoint(v);

				//conf.vc[c].t1.print("C");

				cloud[c][i].p.x = v[0];
				cloud[c][i].p.y = v[1];
				cloud[c][i].p.z = v[2];
			}

			if ( cloud[c][i].p.x < bbox.min_x )
				bbox.min_x = cloud[c][i].p.x;

			if ( cloud[c][i].p.x > bbox.max_x )
				bbox.max_x = cloud[c][i].p.x;

			if ( cloud[c][i].p.y < bbox.min_y )
				bbox.min_y = cloud[c][i].p.y;

			if ( cloud[c][i].p.y > bbox.max_y )
				bbox.max_y = cloud[c][i].p.y;

			if ( cloud[c][i].p.z < bbox.min_z )
				bbox.min_z = cloud[c][i].p.z;

			if ( cloud[c][i].p.z > bbox.max_z )
				bbox.max_z = cloud[c][i].p.z;
		}
	}

	bbox_init = true;

	return bbox;
}

void loadCurrentFrame()
{
	readModel();
	root_bbox = init_bounding_box(Model);
}

int main(int argc, char **argv)
{
	string conf_file;

	if ( argc == 2 )
		conf_file = string(argv[1]);
	else
	{
		cout<<"Usage: Multi_Point_Cloud_Viewer conf_file \n";
		return 0;
	}

	conf.init(conf_file);

	Model.resize(conf.num_cameras);

	loadCurrentFrame();

	if ( !bbox_init )
		scene = new CustomSceneViewer(w,h,base::Vec3f(-10, -10, -10),base::Vec3f(10, 10, 10));
	else
		scene = new CustomSceneViewer(w,h,base::Vec3f(root_bbox.min_x, root_bbox.min_y, root_bbox.min_z), base::Vec3f(root_bbox.max_x, root_bbox.max_y, root_bbox.max_z));
	glInit(&argc, argv);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include <GL/glut.h>
#define PI 3.1416
#define pi (2 * acos(0.0))
#define LEFT +1
#define RIGHT -1

using namespace std;
double cameraHeight;
double cameraAngle;
int drawgrid = 1;
int drawaxes;
double spinAngle;


class line;
struct point
{
	double x, y, z;
	point(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void print()
	{
		cout << x << " " << y << " " << z << endl;
	}
	point() {}
	point addPoints(point b)
	{
		return point(x + b.x, y + b.y, z + b.z);
	}
};

point addPoints(point a, point b)
{
	point ret;
	ret.x = a.x + b.x, ret.y = a.y + b.y, ret.z = a.z + b.z;
	return ret;
}

point subPoints(point a, point b)
{
	point ret;
	ret.x = a.x - b.x, ret.y = a.y - b.y, ret.z = a.z - b.z;
	return ret;
}

point rotate(point toRotate, point ref, int whichDir)
{
	point n, ret;
	double theta = 0.03 * whichDir;

	n.x = ref.y * toRotate.z - ref.z * toRotate.y;
	n.y = ref.z * toRotate.x - ref.x * toRotate.z;
	n.z = ref.x * toRotate.y - ref.y * toRotate.x;

	ret.x = toRotate.x * cos(theta) + n.x * sin(theta);
	ret.y = toRotate.y * cos(theta) + n.y * sin(theta);
	ret.z = toRotate.z * cos(theta) + n.z * sin(theta);

	return ret;
}

class line
{
public:
	point p1, p2;
	line(point p1, point p2)
	{
		this->p1 = p1;
		this->p2 = p2;
	}
	line() {}
	void setPoints(point p1, point p2)
	{
		this->p1 = p1;
		this->p2 = p2;
	}
	void print()
	{
		p1.print();
		p2.print();
	}
	point getDirectionVector()
	{
		point dir = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
		return dir;
	}

	line moveLineAlong(point unitVector, float distance)
	{
		p1 = {p1.x + unitVector.x * distance, p1.y + unitVector.y * distance, p1.z + unitVector.z * distance};
		p2 = {p2.x + unitVector.x * distance, p2.y + unitVector.y * distance, p2.z + unitVector.z * distance};
		return *this;
	}
	line rotateLine(int whichDir)
	{
		point tmp = rotate(getDirectionVector(), point(0, 0, 1), whichDir);
		p2 = {tmp.x + p1.x, tmp.y + p1.y, tmp.z + p1.z};
		return *this;
	}
};
class Vector
{

public:
	point p1;
	Vector(point p1)
	{
		this->p1 = p1;
	}
	Vector(line l)
	{
		p1 = l.getDirectionVector();
	}
	Vector() {}
	void setVector(point p1)
	{
		this->p1 = p1;
	}
	double getMagnitude()
	{
		return sqrt(p1.x * p1.x + p1.y * p1.y + p1.z * p1.z);
	}
	void print()
	{
		p1.print();
	}
	point getDirectionPoint()
	{
		return p1;
	}
	Vector crossProduct(Vector v)
	{
		point p2 = v.getDirectionPoint();
		point n, ret;

		n.x = p1.y * p2.z - p1.z * p1.y;
		n.y = p1.z * p2.x - p2.x * p2.z;
		n.z = p1.x * p2.y - p1.y * p2.x;
		return Vector(n);
	}

	Vector rotate90(Vector ref, bool atRight)
	{

		Vector cp;
		if (atRight)
		{
			cp = crossProduct(ref);
		}
		else
		{
			cp = ref.crossProduct(*this);
		}
		return cp;
	}

	double dotProduct(Vector v)
	{
		double ret;
		point p2 = v.getDirectionPoint();
		ret = p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
		return ret;
	}
	double angleBetween(Vector v)
	{
		double cp = dotProduct(v);
		cp /= (getMagnitude() * v.getMagnitude());
		Vector rightVect = rotate90( Vector(point(0,0,1)), true);
		if(v.dotProduct(rightVect) < 0){
			return acos(cp);
		}else{
			return -acos(cp);
		}
	}
};
line rotateLine(line toRotateLine, line refLine, int whichDir)
{
	point n, ret;
	Vector ref(refLine);
	Vector toRotate(toRotateLine);
	double theta = 0.03 * whichDir;
	n = ref.crossProduct(toRotate).getDirectionPoint();

	ret.x = toRotate.getDirectionPoint().x * cos(theta) + n.x * sin(theta);
	ret.y = toRotate.getDirectionPoint().y * cos(theta) + n.y * sin(theta);
	ret.z = toRotate.getDirectionPoint().z * cos(theta) + n.z * sin(theta);

	line rotatedLine(toRotateLine.p1, toRotateLine.p2.addPoints(ret));
	return rotatedLine;
}
void drawAxes()
{
	if (drawaxes == 1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		{
			glVertex3f(100, 0, 0);
			glVertex3f(-100, 0, 0);

			glVertex3f(0, -100, 0);
			glVertex3f(0, 100, 0);

			glVertex3f(0, 0, 100);
			glVertex3f(0, 0, -100);
		}
		glEnd();
	}
}

void drawGrid()
{
	int i;
	if (drawgrid == 1)
	{
		glColor3f(0.6, 0.6, 0.6); //grey
		glBegin(GL_LINES);
		{
			for (i = -8; i <= 8; i++)
			{

				//lines parallel to Y-axis
				glVertex3f(i * 10, -90, 0);
				glVertex3f(i * 10, 90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i * 10, 0);
				glVertex3f(90, i * 10, 0);
			}
		}
		glEnd();
	}
}


float forwardBy = 1;
double cylRad = 16;
double cylHeight = 8;
double rotateAngle = (forwardBy / cylRad) * (180 / PI);
line mainAxis, frontAxis;

void drawCylinder(double radius, double height, int slices)
{
	//glColor3f(0, 1, 0);

	int i;
	double shade = 0.5;
	struct point points[100];

	Vector mainAxisVector(mainAxis);
	Vector originVector(point(0,1,0));
	float theta = originVector.angleBetween(mainAxisVector);

	glTranslatef(mainAxis.p1.x, mainAxis.p1.y, mainAxis.p1.z);
	glRotatef(theta * 180 / PI, 0, 0, 1);
	glRotatef(-spinAngle , 0, 1, 0);

	//generate points
	for (i = 0; i <= slices; i++)
	{

		points[i].x = radius * cos(((double)i / (double)slices) * 2 * pi);
		points[i].z = radius * sin(((double)i / (double)slices) * 2 * pi);
		points[i].y = 0;
	}

	//draw triangles using generated points
	glColor3f(shade, shade, shade);
	glBegin(GL_QUADS);
	{
		glVertex3f(-cylRad, 1/4.0*cylHeight , 0);
		glVertex3f(-cylRad, -1/4.0*cylHeight , 0);
		glVertex3f(cylRad, -1/4.0*cylHeight , 0);
		glVertex3f(cylRad, 1/4.0*cylHeight , 0);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glVertex3f(0,1/4.0*cylHeight ,-cylRad);
		glVertex3f( 0,-1/4.0* cylHeight,-cylRad );
		glVertex3f(0, -1/4.0*cylHeight ,cylRad );
		glVertex3f(0, 1/4.0* cylHeight ,cylRad );
	}
	glEnd();

	for (i = 0; i < slices; i++)
	{
		//create shading effect
		if (i < slices / 2)
			shade = 2 * (double)i / (double)slices;
		else
			shade = 2 * (1.0 - (double)i / (double)slices);
		glColor3f(shade, shade, shade);
		glBegin(GL_QUADS);
		{
			glVertex3f(points[i].x, points[i].y + cylHeight / 2.0, points[i].z);
			glVertex3f(points[i + 1].x, points[i].y + cylHeight / 2.0, points[i + 1].z);
			glVertex3f(points[i + 1].x, points[i].y - cylHeight / 2, points[i + 1].z);
			glVertex3f(points[i].x, points[i].y - cylHeight / 2, points[i].z);
		}
		glEnd();
	}
}

void keyboardListener(unsigned char key, int x, int y)
{
	//tmpAxis = mainAxis;
	point rotationVector;
	switch (key)
	{

	case '1':
		drawgrid = 1 - drawgrid;
		break;
	case 'w':
		spinAngle += rotateAngle;
		mainAxis.moveLineAlong(frontAxis.getDirectionVector(), forwardBy);
		frontAxis.moveLineAlong(frontAxis.getDirectionVector(), forwardBy);
		break;
	case 'a':
		mainAxis.rotateLine(LEFT);
		frontAxis.rotateLine(LEFT);
		break;
	case 's':
		spinAngle -= rotateAngle;
		mainAxis.moveLineAlong(frontAxis.getDirectionVector(), -forwardBy);
		frontAxis.moveLineAlong(frontAxis.getDirectionVector(), -forwardBy);
		break;
	case 'd':
		mainAxis.rotateLine(RIGHT);
		frontAxis.rotateLine(RIGHT);
		break;
	default:
		break;
	}
}

void specialKeyListener(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_DOWN: //down arrow key
		cameraHeight -= 3.0;
		break;
	case GLUT_KEY_UP: // up arrow key
		cameraHeight += 3.0;
		break;

	case GLUT_KEY_RIGHT:
		cameraAngle += 0.03;
		break;
	case GLUT_KEY_LEFT:
		cameraAngle -= 0.03;
		break;

	case GLUT_KEY_PAGE_UP:
		break;
	case GLUT_KEY_PAGE_DOWN:
		break;

	case GLUT_KEY_INSERT:
		break;

	case GLUT_KEY_HOME:
		break;
	case GLUT_KEY_END:
		break;

	default:
		break;
	}
}

void display()
{

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0); //color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?
	gluLookAt(100 * cos(cameraAngle), 100 * sin(cameraAngle), cameraHeight, 0, 0, 0, 0, 0, 1);

	//gluLookAt(100,100,60,	0,0,0,	0,0,1);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

	drawCylinder(cylRad, cylHeight, 30);

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate()
{
	//angle += 0.05;

	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init()
{
	//codes for initialization
	drawgrid = 1;
	drawaxes = 0;
	cameraHeight = 150.0;
	cameraAngle = 1.0;
	spinAngle = 0;

	point center = {0, 0, cylRad};
	point right = {0, 1, cylRad};
	point front = {-1, 0, cylRad};
	frontAxis.setPoints(center, front);
	mainAxis.setPoints(center, right);
	//clear the screen
	glClearColor(0, 0, 0, 0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80, 1, 1, 1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); //Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST); //enable Depth Testing

	glutDisplayFunc(display); //display callback function
	glutIdleFunc(animate);	//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	glutMainLoop(); //The main loop of OpenGL

	return 0;
}

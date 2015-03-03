//*************************************************************
// Ahmad Pahlavan Tafti
// Date       : 21 April 2013
//*************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glut.h> 
#include <iostream>

// Declaring internal functions
void draw();
void createMenu(void);
void normalize(float* v);
void menu(int val);


//Declaring StructS
typedef struct {
	float x;
	float y;
	float z;
}FLTVECT;

typedef struct {
	int a;
	int b;
	int c;
}INT3VECT;

typedef struct {
	float nrmf0;
	float nrmf1;
	float nrmf2;
}NORMALF;

typedef struct {
	float nrmv0;
	float nrmv1;
	float nrmv2;
}NORMALV;

typedef struct {
	int nv;
	int nf;
	FLTVECT *vertex;
	INT3VECT *face;
	NORMALF *normalf;
	NORMALV *normalv;
}SurFacemesh;

// Rotation variables
float xrot = 0.0f;
float yrot = 0.0f;

// Translation variables
float xt=0.0f;
float yt=0.0f;
float zt=0.0f;

//Scaling variables
float sx=0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;
char KEY='t';
float Scale;

static int menuExit;
static int Polygonmode;
static int Lighting;
static int Shading;
static int val = 1;

int num,n,m;
int a,b,c,d,e;
float x,y,z;
int counter=0;
float swapl[3];
float deep;
bool fullscreen = false;
bool mouseDown = false;

int speed=100;

int flag=0;
//Lighting: position and color
GLfloat lightposition[]= {300.0, 300.0, 500.0, 1.0};
GLfloat lightposition1[]= {-3.0, 3.0, 1.0, 1.0};
GLfloat light1color[]={1.0,0.0,0.0,1.0};


// Function for reading OFF format (file)
SurFacemesh* readPolygon()
{
	float v1[3] , v2[3], v3[3];
	GLfloat v12[3], v23[3], v13[3], normal[3];
	SurFacemesh *surfmesh;
	char line[256];
	FILE *fin;
	if ((fin=fopen("d:\sample_polygon.off", "r"))==NULL){
		printf("read error...\n");
		exit(0);
	};
	/* OFF format */
	while (fgets(line,256,fin) != NULL) {
		if (line[0]=='O' && line[1]=='F' && line[2]=='F')
			break;
	}
	fscanf(fin,"%d %d %d\n",&m,&n,&num);
	surfmesh = (SurFacemesh*)malloc(sizeof(SurFacemesh));
	surfmesh->nv = m;
	surfmesh->nf = n;
	surfmesh->vertex = (FLTVECT *)malloc(sizeof(FLTVECT)*surfmesh->nv);
	surfmesh->face = (INT3VECT *)malloc(sizeof(INT3VECT)*surfmesh->nf);
	surfmesh->normalv = (NORMALV *)malloc(sizeof(NORMALV)*surfmesh->nv);
	surfmesh->normalf = (NORMALF *)malloc(sizeof(NORMALF)*surfmesh->nf);
	for (n = 0; n < surfmesh->nv; n++) 
	{
		fscanf(fin,"%f %f %f\n",&x,&y,&z);
		surfmesh->vertex[n].x = x;
		surfmesh->vertex[n].y = y;
		surfmesh->vertex[n].z = z;
	}

	for (n = 0; n < surfmesh->nf; n++) 
	{
		fscanf(fin,"%d %d %d %d\n",&a,&b,&c,&d);
		surfmesh->face[n].a = b;
		surfmesh->face[n].b = c;
		surfmesh->face[n].c = d;
		if(a != 3)
			printf("Errors: reading surfmesh .... \n");
	}
	fclose(fin);

	
	  return surfmesh;
}  


// Surface mesh obtained from OFF file
  SurFacemesh* surfmesh = readPolygon();


// Drawing Polygon Based on Menu Selection
 void draw()
{
		
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		for (n=0;n<surfmesh->nf;n++)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_TRIANGLES);
			glVertex3f(surfmesh->vertex[surfmesh->face[n].a].x, surfmesh->vertex[surfmesh->face[n].a].y, surfmesh->vertex[surfmesh->face[n].a].z);
			glVertex3f(surfmesh->vertex[surfmesh->face[n].b].x, surfmesh->vertex[surfmesh->face[n].b].y, surfmesh->vertex[surfmesh->face[n].b].z);
			glVertex3f(surfmesh->vertex[surfmesh->face[n].c].x, surfmesh->vertex[surfmesh->face[n].c].y, surfmesh->vertex[surfmesh->face[n].c].z);
			glEnd();
		}
			
	glDisable (GL_POLYGON_OFFSET_FILL);
	}
	
 
	
 //Initialization Function 	
 bool init()
  {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	return true;
 }


void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// Viewing Point
	gluLookAt(
		0.0f, 30.0f, 60.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	//Rotation	
	if (KEY=='r')
	{
		glRotatef(xrot, 1.0f, 0.0f, 0.0f);
		glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	}
	//Scaling
	else if (KEY=='s')
	{
		glScalef(sx,sx,sx);
	}
	//Translation
	else if (KEY=='t')
	{
		glTranslatef(xt,-yt,zt);
	}

	for (int i=1;i<215;i++)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Scale=(float) ((-0.04/214)*(i-1)+1);
		glRotatef((float) i/3, 1.0f, 0.0f, 0.0f);
		glTranslatef((float) i/120,-yt,zt);
		glScalef(Scale,Scale,Scale);
		readPolygon();
		draw();
		glutSwapBuffers ( );
     	glPushMatrix();
	
	}
	
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}



void resize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(70.0f, 1.0f * w / h, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle()
{

	if (!mouseDown && KEY=='r')
	{
		xrot += 0.3f;
		yrot += 0.4f;
	}

	glutPostRedisplay();
}


void ontime(int time)
{
	xrot+=3;
	yrot+=3;
	glutPostRedisplay();
	if (flag==0)
	{
		glutTimerFunc(speed,ontime,1);
	}
}


//Main body
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(700, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("CS 718- Project 3: Key Frame; Animation");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	if (!init())
		return 1;
	glutTimerFunc(speed,ontime,1);
	glutMainLoop();
	return 0;
}

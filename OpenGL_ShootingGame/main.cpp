#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double tx=0;
double dx=0;
double a;

float posx=100;
float posy=100;
float posz=100;

float lx=-1/sqrt(2);
float ly=-1/sqrt(2);
float lz=0;

float rx=-1/sqrt(2);
float ry=-1/sqrt(2);
float rz=0;

float ux=0;
float uy=0;
float uz=1;

double up=0;
double right=0;
double shoot=0;

double vanish=0;

int n[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


double p=0;
double q=0;

int k=0;
int l=0;


struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);

                glVertex3f(0, i*10, -90);
				glVertex3f(0, i*10, 90);



			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}
void drawCube(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,a);
		glVertex3f( a,-a,a);
		glVertex3f(-a,-a,a);
		glVertex3f(-a, a,a);

		glVertex3f(a,a,-a);
		glVertex3f(a,-a,-a);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a,a,-a);

		glVertex3f(a,-a,a);
		glVertex3f(a,a,a);
		glVertex3f(a,a,-a);
		glVertex3f(a,-a,-a);


		glVertex3f(-a, -a,a);
		glVertex3f(-a,a,a);
		glVertex3f(-a,a,-a);
		glVertex3f(-a,-a,-a);


		glVertex3f( a,a,a);
		glVertex3f( a,a,-a);
		glVertex3f(-a,a,-a);
		glVertex3f(-a,a,a);

		glVertex3f(a,-a,a);
		glVertex3f(a,-a,-a);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a,-a,a);
	}glEnd();
}


void drawCylinder(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw quads using generated points
    for(i=0;i<segments;i++)
    {

        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,height/2);
			glVertex3f(points[i+1].x,points[i+1].y,height/2);
			glVertex3f(points[i].x,points[i].y,-height/2);
			glVertex3f(points[i+1].x,points[i+1].y,-height/2);
        }
        glEnd();
    }
}

void drawSphere(double radius,int slices,int stacks)
{

	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{

		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


double moveBus=0;
double z = 0;

void drawSS()
{
    ///Board
    glPushMatrix();
    {

        glColor3f(.7, .7, .7);
        glTranslatef(0, 0, -20);
        glScalef(4, 4, .1);
        drawCube(32);

    }glPopMatrix();

    ///Balloon


    glPushMatrix();///0
    {

        glColor3f(.1, .41, .5);
        glTranslatef(-64, -64, 0);
        if( vanish==1 && (-69<=right && -59>=right) && (-69 <= up && -59 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[0]=1;
        }


        if(n[0]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();

    glPushMatrix();///1
    {
        glColor3f(.3, .8, .1);
        glTranslatef(-64, -32, 0);
        if( vanish==1 && (-69<=right && -59>=right) && (-37 <= up && -27 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[1]=1;
        }


        if(n[1]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();


    glPushMatrix();///2
    {
        glColor3f(.2, .5, .9);
        glTranslatef(-64, 0, 0);
        if( vanish==1 && (-69<=right && -59>=right) && (-5 <= up && 5 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[2]=1;
        }


        if(n[2]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }
    }glPopMatrix();

    glPushMatrix();///3
    {
        glColor3f(.1, .1, .1);
        glTranslatef(-64, 32, 0);
        if( vanish==1 && (-69<=right && -59>=right) && (59 <= up && 69 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[3]=1;
        }

        if(n[3]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();

    glPushMatrix();///4
    {
        glColor3f(.5, .5, .5);
        glTranslatef(-64, 64, 0);
        if( vanish==1 && (-69<=right && -59>=right) && (59 <= up && 69 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[4]=1;
        }

        if(n[4]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();

    glPushMatrix();///5
    {
        glColor3f(.9, .9, .9);
        glTranslatef(-32, -64, 0);
        if( vanish==1 && (-37<=right && -27>=right) && (-69 <= up && -59 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[5]=1;
        }

        if(n[5]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();

    glPushMatrix();///6
    {
        glColor3f(.7, .2, .7);
        glTranslatef(-32, -32, 0);
        if( vanish==1 && (-37<=right && -27>=right) && (-37 <= up && -27 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[16]=1;
        }

        if(n[16]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();

    glPushMatrix();///7
    {
        glColor3f(.2, .4, .6);
        glTranslatef(-32, 0, 0);
        if( vanish==1 && (-37<=right && -27>=right) && (-5 <= up && 5 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[7]=1;
        }

        if(n[7]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();

    glPushMatrix();///8
    {
        glColor3f(.4, .6, .8);
        glTranslatef(-32, 32, 0);
        if( vanish==1 && (-37<=right && -27>=right) && (27 <= up && 37 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[8]=1;
        }

        if(n[8]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();

    glPushMatrix();///9
    {
        glColor3f(.8, .2, .6);
        glTranslatef(-32, 64, 0);
        if( vanish==1 && (-37<=right && -27>=right) && (59 <= up && 69 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[9]=1;
        }

        if(n[9]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();

    glPushMatrix();///10
    {
        glColor3f(.3, .8, .1);
        glTranslatef(0, -64, 0);
        if( vanish==1 && (-5<=right && 5>=right) && (-69 <= up && -59 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[10]=1;
        }

        if(n[10]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();

    glPushMatrix();///11
    {
        glColor3f(.5, .9, .2);
        glTranslatef(0, -32, 0);
        if( vanish==1 && (-5<=right && 5>=right) && (-37 <= up && -27 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[11]=1;
        }

        if(n[11]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();

    glPushMatrix();///12
    {
        glColor3f(.3, .6, .9);
        glTranslatef(0, 0, 0);
        if( vanish==1 && (-5<=right && 5>=right) && (-5 <= up && 5 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[12]=1;
        }

        if(n[12]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();
    glPushMatrix();///13
    {
        glColor3f(.9, .6, .3);
        glTranslatef(0, 32, 0);
        if( vanish==1 && (-5<=right && 5>=right) && (27 <= up && 37 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[13]=1;
        }

        if(n[13]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();
    glPushMatrix();///14
    {
        glColor3f(.4, .6, .1);
        glTranslatef(0, 64, 0);
        if( vanish==1 && (-5<=right && 5>=right) && (59 <= up && 69 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[14]=1;
        }


        if(n[14]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();
    glPushMatrix();///15
    {
        glColor3f(.3, .6, .9);
        glTranslatef(32, -64, 0);
        if( vanish==1 && (27<=right && 37>=right) && (-69 <= up && -59 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[15]=1;
        }

        if(n[15]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();
    glPushMatrix();///16
    {
        glColor3f(.6, .9, .3);
        glTranslatef(32, -32, 0);
        if( vanish==1 && (27<=right && 37>=right) && (-37 <= up && -27 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[16]=1;
        }

        if(n[16]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();
    glPushMatrix();///17
    {
        glColor3f(.5, .1, .9);
        glTranslatef(32, 0, 0);
        if( vanish==1 && (27<=right && 37>=right) && (-5 <= up && 5 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[17]=1;
        }

        if(n[17]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();
    glPushMatrix();///18
    {
        glColor3f(.2, .8, .5);
        glTranslatef(32, 32, 0);
        if( vanish==1 && (27<=right && 37>=right) && (27 <= up && 27 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[18]=1;
        }

        if(n[18]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();
    glPushMatrix();///19
    {
        glColor3f(.2, .6, .9);
        glTranslatef(32, 64, 0);
        if( vanish==1 && (27<=right && 37>=right) && (59 <= up && 69 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[19]=1;
        }

        if(n[19]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();
    glPushMatrix();///20
    {
        glColor3f(.1, .4, .8);
        glTranslatef(64, -64, 0);
        if( vanish==1 && (59<=right && 69>=right) && (-69 <= up && -59 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[20]=1;
        }


        if(n[20]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();
    glPushMatrix();///21
    {
        glColor3f(.9, .3, .6);
        glTranslatef(64, -32, 0);
        if( vanish==1 && (-69<=right && -59>=right) && (-37 <= up && -27 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[21]=1;
        }


        if(n[21]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();
    glPushMatrix();///22
    {
        glColor3f(.2, .8, .2);
        glTranslatef(64, 0, 0);
        if( vanish==1 && (-69<=right && -59>=right) && (-5 <= up && 5 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[22]=1;
        }


        if(n[22]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();
    glPushMatrix();///23
    {
        glColor3f(.8, .8, .8);
        glTranslatef(64, 32, 0);
        if( vanish==1 && (-69<=right && -59>=right) && (27 <= up && 37 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[23]=1;
        }


        if(n[23]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();
    glPushMatrix();///24
    {
        glColor3f(.3, .3, .3);
        glTranslatef(64, 64, 0);
        if( vanish==1 && (-69<=right && -59>=right) && (59 <= up && 69 >= up) )
        {
            glScalef(0,0,0);
            vanish=0;
            n[24]=1;
        }


        if(n[24]!=1 && vanish == 0){
            drawSphere(8, 40, 40);
        }

    }glPopMatrix();




    ///Pistol
    glPushMatrix();
    {
        glTranslatef(right, up, 0);
        glPushMatrix();
        {
            glColor3f(1, 1, 0);
            glTranslatef(0, 0, 170);
            glScalef(1, 1, 12);
            drawCylinder(2, 2, 50);

        }glPopMatrix();

        glPushMatrix();
        {
            glColor3f(0, 0, 1);
            glTranslatef(0, -6, 178);
            glScalef(1, 4, 1);
            drawCube(1);

        }glPopMatrix();

        glPushMatrix();
        {
            glColor3f(0, 0, 1);
            glTranslatef(0, 2, 160);
            glScalef(.2, .6, 1);
            drawCube(1);

        }glPopMatrix();


        ///laser
        glPushMatrix();
        {
            glColor3f(0, 1, 0);
            glTranslatef(0, 0, 70);
            glScalef(.1, .1, 100);
            drawCylinder(2, 2, 50);

        }glPopMatrix();


        ///Bullet
        glPushMatrix();
        {
            glTranslatef(0, 0, -z);
            glTranslatef(0, 0, 165);
            glColor3f(1, 0, 0);
            drawSphere(1, 10, 10);

        }glPopMatrix();
    }glPopMatrix();


}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){


        case 'w':                       ///Up
            up+=.5;
			break;

        case 'd':                       ///right
            right+=.5;
			break;
        case 's':                       ///down
            up-=.5;
			break;
        case 'a':                       ///left
            right-=.5;
			break;
        case 'f':                       ///left
            shoot=1;
			break;


        case 'k':
            posx+=lx;
            posy+=ly;
            posz+=lz;
            break;

        case 'i':
            posx-=lx;
            posy-=ly;
            posz-=lz;
            break;

         case 'n':
            posx+=rx;
            posy+=ry;
            posz+=rz;
            break;

        case 'm':
            posx-=rx;
            posy-=ry;
            posz-=rz;
            break;

        case 'j':
            posx+=ux;
            posy+=uy;
            posz+=uz;
            break;

        case 'l':
            posx-=ux;
            posy-=uy;
            posz-=uz;
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
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


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the glTranslatef(tx,0,0);camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(100,100,300,	0,0,0,	0,1,0);
	//gluLookAt(posx,posy,posz,	posx+lx,posy+ly,posz+lz,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	//drawGrid();

    glColor3f(1,0,0);
    //drawSquare(10);
    //drawCube(20);

    drawSS();
    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){

    if(shoot==1){
        z+=5;

        if(z>160){
            z=0;
            shoot = 0;
            vanish = 1;
        }
    }




	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

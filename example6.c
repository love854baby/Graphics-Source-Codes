//{\small\begin{verbatim}
/* example6.c */

/* 
 *  By Ichiro Suzuki. Parts of this program might have been adopted 
 *  from or inspired by sample programs in "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*  
 *  - extension of previous examples
 *  - draws a cube on a tabletop
 *  - top of cube opens and closes by "o" and "c" to show
 *    a sphere inside
 *  - toggles between wireframe and fill by "w"
 */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265
static int step = 0;

/* camera location in longitude and latitude */
static float alpha = 0.0;
static float beta = PI/6.0;

/* position of camera */
static GLdouble cpos[3]; 

/* position of light source */
static GLfloat lpos[] = {-2.5, 2.5, 3.0, 1.0};

/* vectors to specify material properties */ 
static GLfloat black[]   = {0.0, 0.0, 0.0, 1.0};
static GLfloat white[]   = {1.0, 1.0, 1.0, 1.0};
static GLfloat gray[]    = {0.5, 0.5, 0.5, 1.0};
static GLfloat red[]     = {1.0, 0.0, 0.0, 1.0};
static GLfloat green[]   = {0.0, 1.0, 0.0, 1.0};
static GLfloat blue[]    = {0.0, 0.0, 1.0, 1.0};
static GLfloat yellow[]  = {1.0, 1.0, 0.0, 1.0}; 
static GLfloat magenta[] = {1.0, 0.0, 1.0, 1.0}; 
static GLfloat cyan[]    = {0.0, 1.0, 1.0, 1.0}; 
static GLfloat darkcyan[]= {0.0, 0.4, 0.4, 1.0}; 

/* vectors to specify cube */ 
static GLfloat vdata[8][3] = 
 { 
   {-1.0, 0.0,-1.0},  /* 0 */
   { 1.0, 0.0,-1.0},  /* 1 */
   { 1.0, 2.0,-1.0},  /* 2 */
   {-1.0, 2.0,-1.0},  /* 3 */
   {-1.0, 0.0, 1.0},  /* 4 */
   { 1.0, 0.0, 1.0},  /* 5 */
   { 1.0, 2.0, 1.0},  /* 6 */
   {-1.0, 2.0, 1.0}   /* 7 */
 };

void writemessage()
{
   printf("\n\
   example6.c by Ichiro Suzuki suzuki@cs.uwm.edu\n\
   uses double buffering for smooth animation\n\n\
   x, X, y, Y, z, Z ---------- change light source position\n\
   arrow keys ---------------- change camera position\n\
   o, c ---------------------- open/close top\n\
   w ------------------------- toggle between fill and wire-frame\n\
   ESC ----------------------- exit\n\
   \n");
}

void init(void) 
{
   writemessage(); 

   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   /* initially GL_FILL mode (default), later GL_LINE to show wireframe */
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

   glEnable(GL_LIGHTING);   
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   glEnable(GL_LIGHT0);
}


void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);

   glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
   static int i;
   static double angle, Y, Z, normalY, normalZ;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* initialize (modelview) matrix */
   glLoadIdentity(); 
   
   /* update camera position */
   cpos[0] = 10.0 * cos(beta) * sin(alpha);
   cpos[1] = 10.0 * sin(beta);
   cpos[2] = 10.0 * cos(beta) * cos(alpha);
   gluLookAt(cpos[0],cpos[1],cpos[2], 0.0,1.0,0.0, 0.0,1.0,0.0);

   /* set current light source position */
   glLightfv(GL_LIGHT0, GL_POSITION, lpos);

   /* draw sphere to show light source */
   glMaterialfv(GL_FRONT, GL_EMISSION, white); 
   glPushMatrix();
     glTranslatef(lpos[0],lpos[1],lpos[2]);
     glutSolidSphere(0.1, 10, 8);
   glPopMatrix();

   /* remaining objects do not look as if they emit light */
   glMaterialfv(GL_FRONT, GL_EMISSION, black);

   /* draw a tabletop */   
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gray);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
   glNormal3f( 0.0, 1.0, 0.0);  
   glBegin(GL_POLYGON);
      glVertex3f(-3.0, -0.001,-3.0);
      glVertex3f(-3.0, -0.001, 3.0);
      glVertex3f( 3.0, -0.001, 3.0);
      glVertex3f( 3.0, -0.001,-3.0);
   glEnd();

   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
   glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, darkcyan);
   glMaterialfv(GL_BACK, GL_SPECULAR, black);

   /* back */      
   glBegin(GL_POLYGON);
       glNormal3f(0.0, 0.0, -1.0);   
       glVertex3fv(&vdata[1][0]);
       glVertex3fv(&vdata[0][0]);
       glVertex3fv(&vdata[3][0]);
       glVertex3fv(&vdata[2][0]); 
   glEnd();

   /* left */
   glBegin(GL_POLYGON);
       glNormal3f(-1.0, 0.0, 0.0);  
       glVertex3fv(&vdata[0][0]);
       glVertex3fv(&vdata[4][0]);
       glVertex3fv(&vdata[7][0]);
       glVertex3fv(&vdata[3][0]); 
   glEnd();

   /* right */
   glBegin(GL_POLYGON);
       glNormal3f(1.0, 0.0, 0.0);  
       glVertex3fv(&vdata[5][0]);
       glVertex3fv(&vdata[1][0]);
       glVertex3fv(&vdata[2][0]);
       glVertex3fv(&vdata[6][0]); 
   glEnd();

   /* front */
   glBegin(GL_POLYGON);
       glNormal3f(0.0, 0.0, 1.0);  
       glVertex3fv(&vdata[4][0]);
       glVertex3fv(&vdata[5][0]);
       glVertex3fv(&vdata[6][0]);
       glVertex3fv(&vdata[7][0]); 
   glEnd();

   /* bottom */
   glBegin(GL_POLYGON);
       glNormal3f(0.0, -1.0, 0.0);  
       glVertex3fv(&vdata[0][0]);
       glVertex3fv(&vdata[1][0]);
       glVertex3fv(&vdata[5][0]);
       glVertex3fv(&vdata[4][0]); 
   glEnd();

   /* top */
   angle = ((float)step/180) * PI;
   Y =  2.0 + 2.0 * sin(angle);
   Z = -1.0 + 2.0 * cos(angle);
   normalY = cos(angle);
   normalZ = -sin(angle);

   glNormal3f(0, normalY, normalZ);
   glBegin(GL_QUADS);
     glVertex3f( 1.0,   Y,   Z);
     glVertex3f( 1.0, 2.0,-1.0);
     glVertex3f(-1.0, 2.0,-1.0);
     glVertex3f(-1.0,   Y,   Z);
   glEnd();

   /* sphere inside cube */
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
   glPushMatrix();
     glTranslatef(0.0, 1.0, 0.0);
     glutSolidSphere(1.0, 20.0, 16.0);
   glPopMatrix();

   glFlush();
   glutSwapBuffers();
}

/* exit program with <ESC>
 * light source position controlled by x, X, y, Y, z, Z
 * o and c opens/closes the top
 * w to toggle between filled and wire-frame rendering */
void keyboard(unsigned char key, int x, int y)
{
   static int polygonmode[2]; 

   switch (key) {
      case 27:
         exit(0);
         break; 

      /* open and close the top */
      case 'o':
         if (step < 180) step = step +2;
         glutPostRedisplay();
         break;
      case 'c':
         if (step > 0) step = step -2;
         glutPostRedisplay();
         break;

      /* w for switching between GL_FILL and GL_LINE 
       * Using a query command glGetIntegerv() we obtain two integers
       * indicating the current GL_POLYGON_MODE for FRONT and BACK faces. 
       * Look up glIsEnabled() that may be found useful. */
      case 'w':
         glGetIntegerv(GL_POLYGON_MODE,polygonmode);
         if (polygonmode[0]==GL_FILL) 
              glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         glutPostRedisplay();
         break;

      /* control lpos */
      case 'x':                
         lpos[0] = lpos[0] + 0.2;
         glutPostRedisplay();
         break;
      case 'X':                 
         lpos[0] = lpos[0] - 0.2;
         glutPostRedisplay();
         break;
      case 'y':                
         lpos[1] = lpos[1] + 0.2;
         glutPostRedisplay();
         break;
      case 'Y':                 
         lpos[1] = lpos[1] - 0.2;
         glutPostRedisplay();
         break;
      case 'z':                
         lpos[2] = lpos[2] + 0.2;
         glutPostRedisplay();
         break;
      case 'Z':                 
         lpos[2] = lpos[2] - 0.2;
         glutPostRedisplay();
         break;

      default:
         break;
   }
}

void specialkey(GLint key, int x, int y)
{
   switch (key) {
      case GLUT_KEY_RIGHT:                
         alpha = alpha + PI/180;
         if (alpha > 2*PI) alpha = alpha - 2*PI;
         glutPostRedisplay();
         break;
      case GLUT_KEY_LEFT:                 
         alpha = alpha - PI/180;
         if (alpha < 0) alpha = alpha + 2*PI;
         glutPostRedisplay();
         break;
      case GLUT_KEY_UP:                
         if (beta < 0.45*PI) beta = beta + PI/180;
         glutPostRedisplay();
         break;
      case GLUT_KEY_DOWN:                 
         if (beta > 0.05*PI) beta = beta - PI/180;
         glutPostRedisplay();
         break;

      default:
         break;  
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 800); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialkey);
   glutMainLoop();
   return 0;
}
//\end{verbatim}}
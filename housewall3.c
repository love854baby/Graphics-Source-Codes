//{\small\begin{verbatim}
/* housewall3.c */

/* 
 *  By Ichiro Suzuki. Parts of this program might have been adopted 
 *  from or inspired by sample programs in "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*  
 *  - draws a floor and a triangulated wall with a doorway
 *  - hit w to see wireframe
 *  - has standard controls for light and viewer
 */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265

/* camera location in longitude and latitude */
static float alpha = 0.0;
static float beta  = PI/6.0;;

/* position of camera */
static GLdouble cpos[3]; 

/* position of light source */
static GLfloat lpos[]    = {6.0, 8.0, 5.0, 1.0};

/* vectors to specify material properties */ 
static GLfloat black[]   = {0.0, 0.0, 0.0, 1.0};
static GLfloat white[]   = {1.0, 1.0, 1.0, 1.0};
static GLfloat gray[]    = {0.5, 0.5, 0.5, 1.0};
static GLfloat yellow[]  = {1.0, 1.0, 0.0, 1.0}; 

/* draw a front wall */
static void drawfrontwall()
{
   /* specify material properties */
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
   glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, white);

   glNormal3f( 0.0, 0.0, 1.0);  

   glBegin(GL_TRIANGLES);
      glVertex3f(-3.0, 5.0, 0.0); //2
      glVertex3f(-3.0, 0.0, 0.0); //0
      glVertex3f(-1.0, 0.0, 0.0); //1

      glVertex3f(-3.0, 5.0, 0.0); //2
      glVertex3f(-1.0, 0.0, 0.0); //1
      glVertex3f(-1.0, 4.0, 0.0); //3

      glVertex3f(-3.0, 5.0, 0.0); //2
      glVertex3f(-1.0, 4.0, 0.0); //3
      glVertex3f( 0.0, 8.0, 0.0); //4

      glVertex3f( 0.0, 8.0, 0.0); //4
      glVertex3f(-1.0, 4.0, 0.0); //3
      glVertex3f( 1.0, 4.0, 0.0); //5

      glVertex3f( 3.0, 5.0, 0.0); //6
      glVertex3f( 0.0, 8.0, 0.0); //4
      glVertex3f( 1.0, 4.0, 0.0); //5

      glVertex3f( 3.0, 5.0, 0.0); //6
      glVertex3f( 1.0, 4.0, 0.0); //5
      glVertex3f( 1.0, 0.0, 0.0); //7

      glVertex3f( 3.0, 5.0, 0.0); //6
      glVertex3f( 1.0, 0.0, 0.0); //7
      glVertex3f( 3.0, 0.0, 0.0); //8
   glEnd();
}

void writemessage()
{
   printf("\n\
   housewall2.c by Ichiro Suzuki suzuki@cs.uwm.edu\n\n\
   x, X, y, Y, z, Z ---------- change light source position\n\
   arrow keys ---------------- change camera position\n\
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

   /* initially GL_FILL mode, later GL_LINE to show wireframe */
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
   gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 1.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* initialize (modelview) matrix */
   glLoadIdentity(); 
   
   /* update camera position */
   cpos[0] = 20.0 * cos(beta) * sin(alpha);
   cpos[1] = 20.0 * sin(beta);
   cpos[2] = 20.0 * cos(beta) * cos(alpha);
   gluLookAt(cpos[0],cpos[1],cpos[2], 0.0,3.0,0.0, 0.0,1.0,0.0);

   /* set current light source position */
   glLightfv(GL_LIGHT0, GL_POSITION, lpos);

   /* draw sphere to show light source */
   glMaterialfv(GL_FRONT, GL_EMISSION, white); 
   glPushMatrix();
     glTranslatef(lpos[0],lpos[1],lpos[2]);
     glutSolidSphere (0.1, 10, 8);
   glPopMatrix();

   /* remaining objects do not emit light */
   glMaterialfv(GL_FRONT, GL_EMISSION, black);

   /* draw floor */
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
   glBegin(GL_POLYGON);
      glNormal3f( 0.0, 1.0, 0.0);  
      glVertex3f(-3.0, 0.0, 0.0);
      glVertex3f( 3.0, 0.0, 0.0);
      glVertex3f( 3.0, 0.0,-10.0);
      glVertex3f(-3.0, 0.0,-10.0);
   glEnd();

   /* draw front wall */
   drawfrontwall();

   glFlush();
   glutSwapBuffers();
}

/* exit program with <ESC>
 * light source position controlled by x, X, y, Y, z, Z
 * w to toggle between filled and wire-frame rendering */
void keyboard(unsigned char key, int x, int y)
{
   static int polygonmode[2]; 

   switch (key) {
      case 27:
         exit(0);
         break; 

      case 'w':
         glGetIntegerv(GL_POLYGON_MODE,polygonmode);
         if (polygonmode[0]==GL_FILL) 
              glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         glutPostRedisplay();
         break;

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

//{\small\begin{verbatim}
/* example5.c */

/* 
 *  By Ichiro Suzuki. Parts of this program might have been adopted 
 *  from or inspired by sample programs in "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*  
 *  - extension of previous examples
 *  - camera position controlled by arrow keys
 */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265

/* camera location in longitude and latitude */
static float alpha = 0.0;
static float beta = PI/6.0;

/* position of camera */
static GLdouble cpos[3]; 

/* position of light source */
static GLfloat lpos[] = {-3.0, 3.0, 1.0, 1.0};

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

void writemessage()
{
   printf("\n\
   example5.c by Ichiro Suzuki suzuki@cs.uwm.edu\n\
   uses double buffering for smooth animation\n\n\
   x, X, y, Y, z, Z ---------- change light source position\n\
   arrow keys ---------------- change camera position\n\
   ESC ----------------------- exit\n\
   \n");
}

void init(void) 
{
   writemessage(); 

   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);
   
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
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* Note that the current matrix is the modelview matrix because of
    * glMatrixMode(GL_MODELVIEW) at the end of reshape().
    * Thus next line initializes the modelview matrix */
   glLoadIdentity(); 
   
   /* Camera position is computed from alpha and beta and 
    * set by gluLookat().
    * Then LIGHT0 position is set and all objects are drawn.
    * The order is important (there are exceptions, more later) */

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
   glMaterialfv(GL_FRONT, GL_EMISSION, black);

   /* draw a tabletop */   
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gray);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
   glNormal3f( 0.0, 1.0, 0.0);  
   glBegin(GL_POLYGON);
      glVertex3f(-3.0, 0.0,-3.0);
      glVertex3f(-3.0, 0.0, 3.0);
      glVertex3f( 3.0, 0.0, 3.0);
      glVertex3f( 3.0, 0.0,-3.0);
   glEnd();

   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
   glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, cyan);

   glNormal3f(0.0,0.0,1.0); 
   glBegin(GL_POLYGON);       
       glVertex3f( 0.0,2.0,2.0);
       glVertex3f(-1.0,0.0,2.0);
       glVertex3f( 1.0,0.0,2.0);
   glEnd();

   glNormal3f(0.0,0.0,-1.0);  
   glBegin(GL_POLYGON);
       glVertex3f( 0.0,0.0,0.0);
       glVertex3f( 0.0,2.0,0.0);   
       glVertex3f( 2.0,2.0,0.0);          
       glVertex3f( 2.0,0.0,0.0);       
   glEnd();

   glNormal3f(0.0,0.0,1.0);  
   glBegin(GL_POLYGON); 
       glVertex3f(-2.0,0.0,-1.0);
       glVertex3f( 2.5,0.0,-1.0);
       glVertex3f( 2.5,1.0,-1.0);           
       glVertex3f(-2.0,1.0,-1.0); 
   glEnd();

   glFlush();
   glutSwapBuffers();
}

/* exit program with <ESC> 
 * light source position controlled by x, X, y, Y, z, Z */
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
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

/* special function callback registered in main()
/* handles function, arrow and special-purpose keys
 * alpha and beta are increased/decreased by 1 degree */
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

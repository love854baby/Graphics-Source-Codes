//{\small\begin{verbatim}
/* example2.c */

/* 
 *  By Ichiro Suzuki. Parts of this program might have been adopted 
 *  from or inspired by sample programs in "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*  
 *  - extension of previous example
 *  - reshape function maintains correct aspect ratio of objects
 *    when window is resized
 *  - keyboard callback added to exit with <ESC>
 */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

void init(void) 
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);
      
   glEnable(GL_LIGHTING);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   glEnable(GL_LIGHT0);
   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();       
   gluLookAt(0.0,5.0,10.0, 0.0,1.0,0.0, 0.0,1.0,0.0);
}

/* reshape callback, registered in main()
 *  - called when window created, resized
 *  - moved matrix setup from init() to here
 *  - w and h are the window size in pixels
 *  - glViewport(0,0,w,h) sets viewport within window
 *  - gluPerspective sets aspect ratio to w/h (same as that of window)
 *  - no image distortion */
void reshape(int w, int h)
{
  
   /* sets viewport, the portion of window where rendering takes place
    * defauls is entire window, as set here */
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
      /* set up viewing frustum by specifying 
       *  - field of view of 45 degrees in y direction, 
       *  - aspect ratio of w/h, and 
       *  - distances to front and rear faces 
       * aspect ratio w/h matches that of window
       *  - no image distortion even if window resized */

      /* The current matrix is GL_PROJECTION when we exit reshape().
       * Must switch to GL_MODELVIEW before doing any transformation
       * (there is none in this example) */
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* set light source position before drawing objects */
   glLightfv(GL_LIGHT0, GL_POSITION, lpos);

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
}

/* keyboard callback registered in main()
 * - "key" is key hit, x and y give the position of pointer
 * - <ESC> to exit program */
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break; 
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 800); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
//\end{verbatim}}


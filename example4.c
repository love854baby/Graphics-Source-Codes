//{\small\begin{verbatim}
/* example4.c */

/* 
 *  By Ichiro Suzuki. Parts of this program might have been adopted 
 *  from or inspired by sample programs in "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*  
 *  - extension of previous examples
 *  - uses double buffering for smooth animation
 *  - assigns various colors to "front" vertices, see effect of interpolation
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

void writemessage()
{
   printf("\n\
   example4.c by Ichiro Suzuki suzuki@cs.uwm.edu\n\
   uses double buffering for smooth animation\n\n\
   x, X, y, Y, z, Z ---------- change light source position\n\
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

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();       
   gluLookAt(0.0,5.0,10.0, 0.0,1.0,0.0, 0.0,1.0,0.0);
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

   glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, cyan);

   /* assign various colors to "front" vertices */
   glNormal3f(0.0,0.0,1.0); 
   glBegin(GL_POLYGON);    
       glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
       glVertex3f( 0.0,2.0,2.0);
       glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
       glVertex3f(-1.0,0.0,2.0);
       glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
       glVertex3f( 1.0,0.0,2.0);
   glEnd();

   /* same here, but only BACK face is visible */
   glNormal3f(0.0,0.0,-1.0);  
   glBegin(GL_POLYGON); 
       glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
       glVertex3f( 0.0,0.0,0.0);
       glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, magenta);
       glVertex3f( 0.0,2.0,0.0);   
       glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
       glVertex3f( 2.0,2.0,0.0); 
       glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
       glVertex3f( 2.0,0.0,0.0);       
   glEnd();

   glNormal3f(0.0,0.0,1.0);  
   glBegin(GL_POLYGON); 
       glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
       glVertex3f(-2.0,0.0,-1.0);
       glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
       glVertex3f( 2.5,0.0,-1.0);
       glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
       glVertex3f( 2.5,1.0,-1.0);  
       glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
       glVertex3f(-2.0,1.0,-1.0); 
   glEnd();

   glFlush();
   glutSwapBuffers();
      /* double buffering for smooth animation
       * - use two frame buffers
       * - write into one while the other is displayed 
       * - glutSwapBuffers() will swap the two buffers when writing
       *   is complete to show the new buffer
       * - compare to example3.c without double buffering */
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

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   /* Note that GLUT_DOUBLE is specified */
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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
//{\small\begin{verbatim}
/* example3.c */

/* 
 *  By Ichiro Suzuki. Parts of this program might have been adopted 
 *  from or inspired by sample programs in "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*  
 *  - extension of previous examples
 *  - shows a sphere to represent light source
 *  - light source position controlled by x, X, y, Y, z, Z
 *  - animation is not smooth because of singlel buffer mode
 *  - prints a message in command prompt window
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
   example3.c by Ichiro Suzuki suzuki@cs.uwm.edu\n\
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

   /* In display() we execute glTranslate() that requires
    * the current matrix to be the model-view matrix.
    * In this case it is a common practice to execute 
    * glMatrixMode(GL_MODELVIEW) when we exit reshape(). */
   glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* set current light source position */
   glLightfv(GL_LIGHT0, GL_POSITION, lpos);

   /* draw sphere to show light source */   
   /* set emission component so sphere looks AS IF it emits light */
   glMaterialfv(GL_FRONT, GL_EMISSION, white); 

   glPushMatrix();
     glTranslatef(lpos[0],lpos[1],lpos[2]);
     glutSolidSphere(0.1, 10, 8);
   glPopMatrix();
      /* we will discuss glTranslatef(), glPushMatrix(), glPopMatrix();
       * in detail later.  for now ....
       * - glutSolidSphere() draws a sphere centered at (0,0,0)
       * - glTranslatef(x,y,z)
       *   - EVERYTHING drawn subsequently are translated by x,y,z
       *   - so the sphere drawn by glutSolidSphere() appears 
       *     in the right place
       *   - but that means the other objects will be translated also
       *     and appear in a wrong place!!
       * - glPushMatrix() ... glPopMatrix() limits the effect of 
       *   translation to ... 
       * - so the sphere appears in various places, but other
       *   objects are not affected 
       * - Must set the model-view matrix as the current matrix 
       *   before executing glTranslatef(), and glPushMatrix() and 
       *   glPopMatrix() that surround it. */

   /* remaining objects do not look AS IF they emit light */
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
         /* Mark current window as needing to be redrawn, so that
          * display callback will be called at the next opportunity */
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

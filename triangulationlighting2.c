//{\small\begin{verbatim}
/* triangulationlighting2.c */

/* 
 *  By Ichiro Suzuki. Parts of this program might have been adopted 
 *  from or inspired by sample programs in "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*  
 *  - shows the effect of triangulation on lighting
 *
 *  - initially left square has 2048 triangles
 *  - right square has only two triangles
 *  - hit w to see wireframe
 *  - GL_SMOOTH interpolates brightness/color values across a surface
 *    based on values of vertices
 *  - n to increase the number of triangles of left square
 *  - N to decrease the number of triangles of left square
 *  - l to switch between two light sources (LIGHT1 and LIGHT2) 
 *    and a spotlight (LIGHT0)
 */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define min(x,y) ((x)<(y) ? (x) : (y))
#define max(x,y) ((x)>(y) ? (x) : (y))

static GLint   nleft = 32;

static GLfloat light0_position[]       = {1.1, 1.0, 0.5, 1.0};
static GLfloat light0_spot_direction[] = {0.0, -1.0, 0.0}; 

static GLfloat light1_position[]       = {0.5, 0.2, 0.5, 1.0};
static GLfloat light2_position[]       = {1.7, 0.2, 0.5, 1.0};

/* default diffuse and specular components are 
   (1,1,1,1) for LIGHT0 and (0,0,0,1) for other lights.*/
static GLfloat reddishcolor[]          = {1.0, 0.4, 0.1, 1.0};
static GLfloat light_diffuse[]         = {1.0, 0.4, 0.1, 1.0}; 
static GLfloat light_specular[]        = {1.0, 0.4, 0.1, 1.0}; 

/* vectors to specify material properties */ 
static GLfloat black[]   = {0.0, 0.0, 0.0, 1.0};
static GLfloat white[]   = {1.0, 1.0, 1.0, 1.0};

/* draw a table */
static void drawtable(int n)
{
   GLint i,j;
   GLfloat xl,xh,zl,zh;

   /* specify material properties */
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
   glMaterialf(GL_FRONT, GL_SHININESS, 64);
   glMaterialfv(GL_FRONT, GL_EMISSION, black);

   glNormal3f( 0.0, 1.0, 0.0);  
   for (i=0; i<n; i++)
   {
      for (j=0; j<n; j++)
      {
         /* four corners of square */
         xl = (GLfloat) i/n;
         xh = (GLfloat) (i+1)/n;
         zl = (GLfloat) j/n;
         zh = (GLfloat) (j+1)/n;

         glBegin(GL_TRIANGLES);
            glVertex3f( xl, 0.0, zl);
            glVertex3f( xl, 0.0, zh);
            glVertex3f( xh, 0.0, zh);

            glVertex3f( xl, 0.0, zl);
            glVertex3f( xh, 0.0, zh);
            glVertex3f( xh, 0.0, zl);
         glEnd();
      };
   };
}

void init(void) 
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT1);
   glEnable(GL_LIGHT2);
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30.0, (GLfloat) w / (GLfloat) h, 2.0, 20.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(1.1, 2.0, 4.0, 1.1, 0.5, 0.0, 0.0, 1.0, 0.0);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   if (glIsEnabled(GL_LIGHT0)) {
     /* Set light0 and draw sphere at its position */
     glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_spot_direction);
     glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
     glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 4.0);

     glPushMatrix();
       glMaterialfv(GL_FRONT, GL_EMISSION, white);
       glTranslatef(light0_position[0],light0_position[1],light0_position[2]);
       glutSolidSphere (0.02, 10, 8);
     glPopMatrix();
   }
   else {
     /* Set light1 and light2 and draw spheres at their positions.
        Default diffuse and specular components are 
        (1,1,1,1) for LIGHT0 and (0,0,0,1) for other lights.
        Must set (ambient)/diffuse/specular light intensity for LIGHT1 and 2 */
     glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
     glLightfv(GL_LIGHT1, GL_DIFFUSE, reddishcolor);
     glLightfv(GL_LIGHT1, GL_SPECULAR, reddishcolor);
     glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION,1.0);
     glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION,1.0);

     glPushMatrix();
       glMaterialfv(GL_FRONT, GL_EMISSION, reddishcolor);
       glTranslatef(light1_position[0],light1_position[1],light1_position[2]);
       glutSolidSphere (0.02, 10, 8);
     glPopMatrix();

     glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
     glLightfv(GL_LIGHT2, GL_DIFFUSE, reddishcolor);
     glLightfv(GL_LIGHT2, GL_SPECULAR, reddishcolor);
     glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION,1.0);
     glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION,1.0);

     glPushMatrix();
       glMaterialfv(GL_FRONT, GL_EMISSION, reddishcolor);
       glTranslatef(light2_position[0],light2_position[1],light2_position[2]);
       glutSolidSphere (0.02, 10, 8);
     glPopMatrix();
   };

   drawtable(nleft);
   
   glPushMatrix();
     glTranslatef(1.2, 0.0, 0.0);
     drawtable(1);
   glPopMatrix();

   glFlush();
   glutSwapBuffers();
}

/* exit program with "esc" key */
/* "w" toggles between fill and wireframe modes */
void keyboard(unsigned char key, int x, int y)
{
   static int polygonmode[2];

   switch (key) {
      case 27:
         exit(0);
         break; 
      case 'l':
         /* test if GL_LIGHT0 is enabled */
         if (glIsEnabled(GL_LIGHT0)) {
           glDisable(GL_LIGHT0);
           glEnable(GL_LIGHT1);
           glEnable(GL_LIGHT2);
         } 
         else {
           glDisable(GL_LIGHT1);
           glDisable(GL_LIGHT2);
           glEnable(GL_LIGHT0);
         };
         glutPostRedisplay();
         break;

      case 'w':
         glGetIntegerv(GL_POLYGON_MODE,polygonmode);
         if (polygonmode[0]==GL_FILL) 
              glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         glutPostRedisplay();
         break;

      case 'n':
         if (nleft < 8) {
           nleft = nleft + 1;
         }
         else {
           nleft = min(256, nleft * 2);
         };
         printf("Left square has %d triangles\n", nleft*nleft*2);
         glutPostRedisplay();
         break;

      case 'N':
         if (nleft >= 16) {
           nleft = nleft/2;
         }
         else {
           nleft = max(1, nleft-1);
         };
         printf("Left square has %d triangles\n", nleft*nleft*2);
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
   glutInitWindowSize(1000, 800); 
   glutInitWindowPosition(0, 0);
   glutCreateWindow(argv[0]);
   init();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
//\end{verbatim}}
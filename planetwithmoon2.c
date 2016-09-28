//{\small\begin{verbatim}
/* planetwithmoon2.c */

/* 
 *  By Ichiro Suzuki. Parts of this program might have been adopted 
 *  from or inspired by sample programs in "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/* 
 *  - modified from planet.c
 *  - added a moon to the planet (earth)
 *  - added light source at the poition of the sun
 *  - usual mouse/keyboard inputs: left clik to start animation
 *  - "w" to see wireframe
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <Windows.h> /* for Sleep() */

static GLfloat year = 0.0, day = 0.0, moonday=0.0;

static GLfloat position[] = {0.0, 0.0, 0.0, 1.0};
static GLdouble cpos[]    = {2.0, 4.0, 25.0}; 
static GLfloat lmodel_ambient[] = {0.3, 0.3, 0.3, 1.0 };

/* vectors to specify material properties */ 
static GLfloat mediumred[]= {0.6, 0.0, 0.0, 1.0};
static GLfloat none[]     = {0.0, 0.0, 0.0, 1.0};
static GLfloat white[]    = {1.0, 1.0, 1.0, 1.0};
static GLfloat cyan[]     = {0.0, 1.0, 1.0, 1.0}; 
static GLfloat green[]    = {0.0, 1.0, 0.0, 1.0};
static GLfloat red[]      = {1.0, 0.0, 0.0, 1.0};

void init(void) 
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   /* will normally show only front face, but...
   glPolygonMode(GL_FRONT,GL_FILL);
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);
   */

   /* since back "face" appears in wireframe mode */ 
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 1.0, 50.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   /* set camera position */
   gluLookAt(cpos[0], cpos[1], cpos[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

void display(void)
{

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix();

   /* light source position */
   glLightfv(GL_LIGHT0, GL_POSITION, position);

   /* to add more ambient light */
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

   glMaterialfv(GL_FRONT, GL_SPECULAR, white);
   glMaterialf(GL_FRONT, GL_SHININESS, 64);

   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mediumred);

   glutSolidSphere(1.0, 12, 12);      /* draw sun */
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, none);

   glRotatef(year, 0.0, 1.0, 0.0);    /* find direction of earth */
   glTranslatef(8.0, 0.0, 0.0);       /* go to position of earth */
   glRotatef(-year, 0.0, 1.0, 0.0);   /* restore initial orientation of earth */

   glPushMatrix();  /* memorize state just after reaching position of earth */

   glRotatef(day, 0.0, 1.0, 0.0);     /* rotate earth about its axis */
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cyan);
   glutSolidSphere(1.0, 8, 8);        /* draw earth */

   glPopMatrix();   /* return to state just after reaching earth */

   glRotatef(moonday, 0.0, 1.0, 0.0); /* find direction of moon */
   glTranslatef(3.0, 0.0, 0.0);       /* go to position of moon */
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
   glutSolidSphere(0.5, 8, 8);        /* draw moon*/

   glPopMatrix();

   glFlush();
   glutSwapBuffers();

}

void revolve(void)
{
   /* 1 year = 36 days */
   day = day + 2.0;
   if (day > 360.0) day = day - 360.0;
   year = year + 0.2;
   if (year > 360.0) year = year - 360.0;
   moonday = moonday + 0.5;
   if (moonday > 360.0) moonday = moonday - 360.0;
   
   Sleep(30);  /* suspend execution for given milliseconds */

   glutPostRedisplay();  
}


void mouse(int button, int state, int x, int y) 
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(revolve); 
         break;
      case GLUT_MIDDLE_BUTTON:
      case GLUT_RIGHT_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
         break;
      default:
         break;
   }
}

void keyboard(unsigned char key, int x, int y)
{
   static int polygonmode[2];

   switch (key) {
      case 'w':
         glGetIntegerv(GL_POLYGON_MODE,polygonmode);
         if (polygonmode[0]==GL_FILL) 
              glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         glutPostRedisplay();
         break;

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
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(1000, 800); 
   glutInitWindowPosition(0, 0);
   glutCreateWindow(argv[0]);
   init();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
//\end{verbatim}}
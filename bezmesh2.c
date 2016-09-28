//{\small\begin{verbatim}
/* bezmesh2.c 
 * modified by Ichiro Suzuki
 * - can see wireframe
 */

/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*  bezmesh.c
 *  This program renders a lighted, filled Bezier surface,
 *  using two-dimensional evaluators.
 */

#include <stdlib.h>
#include <GL/glut.h>

static int filled=1;

GLfloat ctrlpoints[4][4][3] = {
   { {-1.5, -1.5, 4.0},
     {-0.5, -1.5, 2.0},
     {0.5, -1.5, -1.0},
     {1.5, -1.5, 2.0}},
   { {-1.5, -0.5, 1.0},
     {-0.5, -0.5, 3.0},
     {0.5, -0.5, 0.0},
     {1.5, -0.5, -1.0}},
   { {-1.5, 0.5, 4.0},
     {-0.5, 0.5, 0.0},
     {0.5, 0.5, 3.0},
     {1.5, 0.5, 4.0}},
   { {-1.5, 1.5, -2.0},
     {-0.5, 1.5, -2.0},
     {0.5, 1.5, 0.0},
     {1.5, 1.5, -1.0}}
};

void initlights(void)
{
   GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
   GLfloat darkgray[] = {0.2, 0.2, 0.2, 1.0};   
   GLfloat gray[]     = {0.6, 0.6, 0.6, 1.0};
   GLfloat white[]    = {1.0, 1.0, 1.0, 1.0};
   GLfloat medium[]   = {50.0}; /* for shininess */

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   glLightfv(GL_LIGHT0, GL_AMBIENT, darkgray);
   glLightfv(GL_LIGHT0, GL_POSITION, position);

   glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
   glMaterialfv(GL_FRONT, GL_SPECULAR, white);
   glMaterialfv(GL_FRONT, GL_SHININESS, medium);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   glRotatef(85.0, 1.0, 1.0, 1.0);
   if (filled) glEvalMesh2(GL_FILL, 0, 20, 0, 20);
   else glEvalMesh2(GL_LINE, 0, 20, 0, 20);
   glPopMatrix();
   glFlush();
}

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   glEnable(GL_DEPTH_TEST);
   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, &ctrlpoints[0][0][0]);
   glEnable(GL_MAP2_VERTEX_3);
   glEnable(GL_AUTO_NORMAL);
   glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
   initlights();       /* for lighted version only */
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-4.0, 4.0, -4.0*(GLfloat)h/(GLfloat)w, 4.0*(GLfloat)h/(GLfloat)w, 
              -4.0, 4.0);
   else
      glOrtho(-4.0*(GLfloat)w/(GLfloat)h, 4.0*(GLfloat)w/(GLfloat)h, -4.0, 4.0,
              -4.0, 4.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
   
   switch (key) {
      case 27:
         exit(0);
         break;

      case 'w':
         if (filled) filled = 0; 
       	 else filled = 1;
          
         glutPostRedisplay();
         break;
   }
}

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (800, 800);
   glutInitWindowPosition (100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
//\end{verbatim}}
//{\small\begin{verbatim}
/* pickdepth4.c
 * Modified by Ichiro Suzuki.
 * Three rectangles and projection are slightly different from original.
 *
 * - Name stack remains untouched between first and second rectangles
 *   (so both first and second rectangles are given the name "1").
 * - Clicking in their intersection causes only one hit with 
 *   two different z-values.
 */

/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*
 * pickdepth.c
 * Picking is demonstrated in this program.  In 
 * rendering mode, three overlapping rectangles are 
 * drawn.  When the left mouse button is pressed, 
 * selection mode is entered with the picking matrix.  
 * Rectangles which are drawn under the cursor position
 * are "picked."  Pay special attention to the depth 
 * value range, which is returned.
 */


#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

int ww, hh;

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
   glDepthRange(0.0, 1.0);  /* The default z mapping */
}

/* Three rectangles are drawn with different z values.
.* In selection mode, 
 * yellow and cyan have name 1, and magenta has name 3.
 */
void drawRects(GLenum mode)
{
   if (mode == GL_SELECT) glLoadName(1); // replace stack top by 1
   glBegin(GL_QUADS);
   glColor3f(1.0, 1.0, 0.0); // yellow
   glVertex3i(2, 0, 0);
   glVertex3i(2, 5, 0);
   glVertex3i(6, 5, 0);
   glVertex3i(6, 0, 0);
   glEnd();

   // deleted!  
   //if (mode == GL_SELECT) glLoadName(2);
   // name stack never modified
   // yellow and cyan squares have the same name and generate one hit
   glBegin(GL_QUADS);
   glColor3f(0.0, 1.0, 1.0); // cyan
   glVertex3i(3, 1, -1);
   glVertex3i(3, 6, -1);
   glVertex3i(8, 6, -1);
   glVertex3i(8, 1, -1);
   glEnd();

   if (mode == GL_SELECT) glLoadName(3);
   glBegin(GL_QUADS);
   glColor3f(1.0, 0.0, 1.0); // magenta
   glVertex3i(0, 2, -2);
   glVertex3i(0, 7, -2);
   glVertex3i(5, 7, -2);
   glVertex3i(5, 2, -2);
   glEnd();
}

/*  processHits() prints out the contents of the 
 *  selection array.
 */
void processHits(GLuint hits, GLuint buffer[])
{
   unsigned int i, j;
   GLuint names, *ptr;

   printf("number of hits = %d\n", hits); // number of hits
   ptr = (GLuint *) buffer;
   for (i = 0; i < hits; i++) {  /* for each hit  */ 
      // number of names on stack
      names = *ptr;
      printf(" number of names on stack for this hit = %d\n", names); ptr++;  
      // min and max z-values of vertices of object hit
      // z-values are in the range 0 to 2^32-1 = 0xffffffff = 4294967295
      printf(" z1 is %g;",  (float) *ptr/0xffffffff); ptr++;
      printf(" z2 is %g\n", (float) *ptr/0xffffffff); ptr++;
      // names on stack, bottom to top
      printf(" the names are ");
      for (j = 0; j < names; j++) {printf("%d ", *ptr); ptr++;}     
      printf("\n");
   }
   printf("\n");
}

/*  pickRects() sets up selection mode, name stack, 
 *  and projection matrix for picking.  Then the objects 
 *  are drawn.
 */
#define BUFSIZE 512

void pickRects(int button, int state, int x, int y)
{
   GLuint selectBuf[BUFSIZE];
   GLint hits;
   GLint viewport[4];

   if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
      return;

   // viewport dimension 
   // viewport[0]=0, viewport[1]=0, viewport[2]=ww, viewport[3]=hh
   glGetIntegerv(GL_VIEWPORT, viewport); 

   glSelectBuffer(BUFSIZE, selectBuf); // specify array to hold hit records

   (void) glRenderMode(GL_SELECT); // enter selection mode

   glInitNames(); // clear name stack so it is empty
   glPushName(0); // load something so we can call glLoadName() later

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   // create 5x5 pixel picking region near cursor location
   gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3]-y), 5.0, 5.0, viewport);
   glOrtho(0.0, (float)ww/20, 0.0, (float)hh/20, -0.5, 2.5);
   // redraw objects in selection mode, framebuffer does not change
   drawRects(GL_SELECT);
   glPopMatrix();
   glFlush();

   hits = glRenderMode(GL_RENDER); //exit selection mode, get number of hits
   processHits(hits, selectBuf);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   drawRects(GL_RENDER);
   glFlush();
}

void reshape(int w, int h)
{
   // need window size when setting gluPickMatrix() in pickRects
   ww=w;
   hh=h;  
        
   glViewport(0, 0, (GLsizei) ww, (GLsizei) hh);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, (float)ww/20, 0.0, (float)hh/20, -0.5, 2.5);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

/*  Main Loop
 */
int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (200, 200);
   glutInitWindowPosition (100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutMouseFunc(pickRects);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0; 
}
//\end{verbatim}}
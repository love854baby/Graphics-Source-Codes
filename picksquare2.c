//{\small\begin{verbatim}
/* picksquare_mod1 
 * Modified by Ichiro Suzuki.
 *
 * - Uses perspective projection.
 * - Selected squares change color.
 */
 
/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*
 * picksquare.c
 * Use of multiple names and picking are demonstrated.  
 * A 3x3 grid of squares is drawn.  When the left mouse 
 * button is pressed, all squares under the cursor position 
 * have their color changed.
 */

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

int board[3][3]; /* to control amount of blue for each square */
int ww, hh;

/*  Clear blue value for every square on the board */
void init(void)
{
   int i, j;
   for (i = 0; i < 3; i++) 
      for (j = 0; j < 3; j ++)
         board[i][j] = 0;
   glClearColor (0.0, 0.0, 0.0, 0.0);
}

/*  The nine squares are drawn.  In selection mode, each 
 *  square is given two names:  one for the row and the 
 *  other for the column on the grid.  The color of each 
 *  square is determined by its position on the grid, and 
 *  the value in the board[][] array.
 */
void drawSquares(GLenum mode)
{
   GLuint i, j;
   for (i = 0; i < 3; i++) {
      if (mode == GL_SELECT) glLoadName(i);
      for (j = 0; j < 3; j ++) {
         if (mode == GL_SELECT) glPushName (j);
         glColor3f((GLfloat)i/3.0, (GLfloat)j/3.0, (GLfloat)board[i][j]/3.0);
         glRecti(i, j, i+1, j+1);
         if (mode == GL_SELECT) glPopName();
      }
   }
}

/*  processHits prints out the contents of the 
 *  selection array.
 */
void processHits (GLuint hits, GLuint buffer[])
{
   unsigned int i, j;
   GLuint ii, jj, names, *ptr;

   printf ("hits = %d\n", hits);
   ptr = (GLuint *) buffer;
   for (i = 0; i < hits; i++) {        /*  for each hit  */
      names = *ptr;
      printf(" number of names on stack for this hit = %d\n", names); ptr++;
      // z-values are in the range 0 to 2^32-1 = 0xffffffff = 4294967295
      printf(" z1 is %g;",  (float) *ptr/0xffffffff); ptr++;
      printf(" z2 is %g\n", (float) *ptr/0xffffffff); ptr++;
      printf(" names on stack are ");
      for (j = 0; j < names; j++) { /*  for each name */
         printf ("%d ", *ptr);
         if (j == 0)  /*  set row and column  */
            ii = *ptr;
         else if (j == 1)
            jj = *ptr;
         ptr++;
      }
      printf("\n");
      board[ii][jj] = (board[ii][jj] + 1) % 3;
   }
}

/*  pickSquares() sets up selection mode, name stack, 
 *  and projection matrix for picking.  Then the 
 *  objects are drawn.
 */
#define BUFSIZE 512

void pickSquares(int button, int state, int x, int y)
{
   GLuint selectBuf[BUFSIZE];
   GLint hits;
   GLint viewport[4];

   if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
      return;

   glGetIntegerv(GL_VIEWPORT, viewport);

   glSelectBuffer(BUFSIZE, selectBuf);
   glRenderMode(GL_SELECT);

   glInitNames();
   glPushName(0);

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   // create 5x5 pixel picking region near cursor location
   gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3]-y), 5.0, 5.0, viewport);
   //gluOrtho2D (0.0, 3.0, 0.0, 3.0);
   gluPerspective(45.0, (GLfloat) ww/(GLfloat) hh, 1.0, 20.0);
   /* Here, modelview matrix is still as set by gluLookAt(...) in reshape().
      If not, then we have to do glMatrixMode(GL_MODELVIEW); glPushMatrix(); 
      glLoadIdentity(); gluLookAt(...); drawsquares(GL_SELECT); glPopMatrix(); 
      glMatrixMode(GL_PROJECTION); glPopMatrix();
    */ 
   drawSquares(GL_SELECT);
   glPopMatrix();
   glFlush ();
  
   /* At this moment, the martix chosen is the projection matrix.
      Make sure to switch to the model-view matrix by 
      glMatrixMode(GL_MODELVIEW) before issuing a transformation
      command such as gluLookAt(), glTranslatef(), glRotatef().
    */

   hits = glRenderMode(GL_RENDER);
   processHits(hits, selectBuf);
   glutPostRedisplay();
} 

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   drawSquares(GL_RENDER);
   glFlush();
}

void reshape(int w, int h)
{
   ww=w;
   hh=h;  
        
   glViewport(0, 0, ww, hh);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //gluOrtho2D (0.0, 3.0, 0.0, 3.0);
   gluPerspective(45.0, (GLfloat) ww/(GLfloat) hh, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (-4.0, 0.0, 5.0, 1.5, 1.5, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

/* Main Loop */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (0, 0);
   glutCreateWindow (argv[0]);
   init ();
   glutReshapeFunc (reshape);
   glutDisplayFunc(display); 
   glutMouseFunc (pickSquares);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
   return 0; 
}
//\end{verbatim}}

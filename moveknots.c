//{\small\begin{verbatim}
/* moveknots.c by I. Suzuki based on knots.c by Tom Lassanske */
/*
 * - Adding, deleting and dragging knots
 */

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

void Mouse(int, int, int, int);
void Display(void);
void DrawKnot(int, int);
void Reshape(GLsizei, GLsizei);
void Init(void);
void Right_Menu(int);
int  Find_Knot(int, int);

/* globals */
int     wh=300, ww=600;     /* window size */
GLfloat size = 8.0;         /* half side length of square */
GLfloat r=1, g=0, b=0;      /* drawing color */

int knot_num=0, knot_mode=1, knot_move=0, line_type=0;
int found_knot; //knot to be deleted or moved
int max_knot_count = 10;
int knot_coord[100]; //must be of size at least 2*max_knot_count


//#############
void Init(void)
{
  /* Pick 2D clipping window to match size of window 
  This choice avoids having to scale object coordinates
  each time window is resized */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); 
  glOrtho(0.0, (GLdouble) ww , 0.0, (GLdouble) wh , -1.0, 1.0);

  /* set clear color to black */
  glClearColor (0.0, 0.0, 0.0, 1.0);
}

//################################
void Reshape(GLsizei w, GLsizei h)
{
  /* adjust clipping box */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); 
  glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);

  /* adjust viewport and  clear */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); 
  glViewport(0,0,w,h);

  /* set global size for use by drawing routine */
  ww = (int) w;
  wh = (int) h; 
}

//#########################
void DrawKnot(int x, int y)
{
  y=wh-y; 
  glBegin(GL_POLYGON);
    glVertex2f(x+size, y+size);
    glVertex2f(x-size, y+size);
    glVertex2f(x-size, y-size);
    glVertex2f(x+size, y-size);
  glEnd();
}

//################
void DrawLine(x1, y1, x2, y2)
{
  int x;
  float slope;

  glColor3f(1, 1, 1);

  if (x2<x1) 
    DrawLine(x2, y2, x1, y1);
  else {
    slope = ((float)y2-y1)/((float)x2-x1);
    glBegin(GL_POINTS);
      for (x=1; x<4*(x2-x1); x++)
        glVertex2f(x1+x/4, wh-(y1+x/4*slope));
    glEnd();
  }
}

// ########################
int Find_Knot(int x, int y)
{
  int knot;
  for (knot=0; knot<knot_num; knot++) {
    if ((knot_coord[knot*2]-size  <=x) && (knot_coord[knot*2]+size  >=x) &&
        (knot_coord[knot*2+1]-size<=y) && (knot_coord[knot*2+1]+size>=y))
      return (knot);
  }
  return -1;
}

//##########################################
void Mouse(int btn, int state, int x, int y)
{
  int knot;

  if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
    switch (knot_mode) {
    case 1: /* add knot */
      if (knot_num < max_knot_count) {
       knot_coord[2*(knot_num)] = x;
       knot_coord[2*(knot_num)+1] = y;
       knot_num++;
       glutPostRedisplay();
      };
      break;
    case 2:  /* move knot */
      found_knot = Find_Knot(x,y);
      if (found_knot >= 0) knot_move = 1;
      break;
    case 3: /* delete knot */
      found_knot = Find_Knot(x,y);
      if (found_knot >= 0) {             
          for (knot=found_knot; knot<knot_num-1; knot++) {
            knot_coord[knot*2]   = knot_coord[knot*2+2];
            knot_coord[knot*2+1] = knot_coord[knot*2+3];
          };
       knot_num--;
       glutPostRedisplay();
      };
      break;
    default:
      break;
    }
  }
  else if (btn==GLUT_LEFT_BUTTON && state==GLUT_UP && knot_move) {
    /* end of move */
    knot_move = 0;
    glutPostRedisplay();
  }
}

//#####################
void Right_Menu(int id)
{ 
  if (id <= 3) knot_mode = id;
  else 
  switch (id) {

  case 4: /* clear the knots */   
    knot_num = 0;     
    glutPostRedisplay();
    break;

  case 7: /* exit the program */
    exit(0);
    break;
  }
}

//####################
void Motion(int x, int y)
{
  if (knot_move) {
    knot_coord[2*(found_knot)] = x;
    knot_coord[2*(found_knot)+1] = y;
    glutPostRedisplay();
  }
}

//####################
void Knot_Menu(int id)
{
 if (id == 1) size = 2 * size;
  else if (size > 1) size = size/2;
  glutPostRedisplay();
}

//####################
void Line_Menu(int id)
{
  line_type = id;
  glutPostRedisplay();
}

//################
void Display(void)
{
  int knot;

  glClear(GL_COLOR_BUFFER_BIT);

  for (knot=0; knot<knot_num; knot++) {
    /* draw the knot */
    glColor3f( r, g, b);
    DrawKnot(knot_coord[knot*2], knot_coord[knot*2+1]);
  }

  switch (line_type) {
  case 0:
    break;
  case 1:
    for (knot=0; knot<knot_num-1; knot++) {
      DrawLine(knot_coord[2*knot+0], knot_coord[2*knot+1],
               knot_coord[2*knot+2], knot_coord[2*knot+3]);
    }
    break;
  }

  glFlush();
  glutSwapBuffers();
}


//#############################
int main(int argc, char** argv)
{
  int k_menu, l_menu;

  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(ww, wh);
  glutCreateWindow("Knot drawing program");

  k_menu = glutCreateMenu(Knot_Menu);
  glutAddMenuEntry("Increase", 1);
  glutAddMenuEntry("Decrease", 2);

  l_menu = glutCreateMenu(Line_Menu);
  glutAddMenuEntry("No Lines", 0);
  glutAddMenuEntry("Straight Lines", 1);

  glutCreateMenu(Right_Menu);  
  glutAddSubMenu  ("Knot Size",    k_menu);
  glutAddSubMenu  ("Line Type",    l_menu);
  glutAddMenuEntry("Add Knots",    1);
  glutAddMenuEntry("Move Knots",   2);
  glutAddMenuEntry("Delete Knots", 3);
  glutAddMenuEntry("Clear",        4);
  glutAddMenuEntry("Exit",         7);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  Init();
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape); 
  glutMouseFunc(Mouse);
  glutMotionFunc(Motion);
  glutMainLoop();
}
//\end{verbatim}}


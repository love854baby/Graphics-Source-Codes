//{\small\begin{verbatim}
/* example1.c */

/* 
 *  By Ichiro Suzuki. Parts of this program might have been adopted 
 *  from or inspired by sample programs in "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/* draws simple objects */

#include <GL/glut.h>  /* glut handles the window interface */
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

/* One-time initialization */
void init(void) 
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
      /* set RGBA values for the color to clear viewport
       * (0,0,0) is black
       * "A" is "alpha" for blending color, more later */

   glEnable(GL_DEPTH_TEST);
      /* must enable depth test for hidden surface removal 
         - see what happens without it */
   
   glShadeModel(GL_SMOOTH);
      /* - GL_SMOOTH means color of every point in a polygon is
       *   determined by interpolation of those of all corners
       * - if GL_FLAT is specified color of a face is the same as that 
       *   of the first vertex */
   
   /* enable GL_LIGHTING, two-sided lighting to properly render 
    * both front and back faces, and GL_LIGHT0 */
   glEnable(GL_LIGHTING);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   glEnable(GL_LIGHT0);
      /* GL_LIGHT0's default colors are black (0.0,0.0,0.0,1.0) for ambient and
       * white (1.0,1.0,1.0,1.0) for diffuse and specular.
       * Default colors for other light sources GL_LIGHT1, GL_LIGHT2,... are 
       * black for all three components. Thus they will not cast any light unless
       * some non-black colors are set.
       *
       * By default, there is global ambient light of 
       * dark gray (0.2,0.2,0.2,1.0).
       * Thus objects are visible even if LIGHT_0 is disabled */

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
      /* OpenGL uses two matrices to render
       *   projection matrix 
       *          -- projection transformation 
       *          -- how 3D scene is projected to 2D display
       *   model-view matrix 
       *          -- model-view transformation
       *          -- places objects in 3D space, sets viewpoint
       *
       * set up the projection matrix
       * - set the projection matrix as the current matrix by glMatrixMode()
       * - initialize it to the identity matrix by glLoadIdentity() */

   glFrustum(-0.5, 0.5,-0.5, 0.5, 1.0, 20.0);
      /* - specify a viewing frustum for perspective projection
       * - objects farther away look smaller, like the way we see
       * - left, right, bottom, top, near, far
       * - this function assigns a specific value to the current matrix,
       *   which is the projection matrix
       *
       * - here we set a frustum of 1:1 aspect ratio
       * - image gets distorted when window resized to different aspect 
       *   ratio
       * - resize the window and see effect */

   //glOrtho(-4.0, 4.0,-4.0, 4.0, 1.0, 20.0); 
      /* for orthographic projection, replace glFrustum() by glOrtho()
       * - specify a rectangular viewing box for orthographic projection
       * - keeps size of objects regardless of distance
       * - suitable for architectural and computer-aided design 
       *   applications, blueprinting plans
       * - left, right, bottom, top, near, far
       * - this function assigns a specific value to the current matrix,
       *   which is the projection matrix */
    
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
      /* set up model-view matrix
       * - set the model-view matrix as the current matrix by glMatrixMode()
       * - initialize it to the identity matrix by glLoadIdentity() */

   gluLookAt(0.0, 5.0, 10.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);
      /* - specify viewpoint (camera position)
       * - camera positioned at 0.0, 5.0, 10.0, 
       * - aimed at 0.0, 1.0, 0.0,
       * - up direction 0.0, 1.0, 0.0
       * - this function assigns a specific value to the current matrix,
       *   which is the model-view matrix */
}

/* Display callback function registered in main().
 * Called whenever window must be redrawn.
 * Must contain all commands necessary to redraw objects, 
 * including the command for setting the light source position. */
void display(void)
{
   /* clear all pixels in color set in glClearColor
    * clear depth bit */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* set light source position before drawing objects */
   glLightfv(GL_LIGHT0, GL_POSITION, lpos);
       /* lpos is (-3,3,1,1)
        * if 4th component=0 then light comes from direction (-3,3,1)
        * if 4th component=1 then light source is at position (-3,3,1) */

   /* specify material properties --- color reflectance, shininess */
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gray);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
      /* - ambient component 
       *   - color reflectance when lit by ambient light that simulates light 
       *     scattered so much by the environment that it seems to come from 
       *     all directions
       * - diffuse component 
       *   - color reflectance when lit by diffuse light that simulates light 
       *     that comes from one direction but when hitting a surface scattered 
       *     in all directions equally, e.g., cloth 
       * - specular component
       *   - color reflectance when lit by specular light that simulates light
       *     that comes from one direction and bounces off in a preferred direction
       *     when hitting a surface, e.g., bright spot on polished metal 
       * - shininess
       *   - 0-128
       *   - higher --> smaller bright spots 
       *
       * - Once set, these values remain in effect until changed again
       *   OpenGL uses state machine model. */

   /* For each vertex, a normal vector must be specified by glNormal3f().
    *   
    * Normal vectors are of length 1, and typically perpendicular to 
    * FRONT face, but not always.
    * Normal vectors are used by OpenGL to calculate how the light 
    * striking an object is reflected to the viewer. 
    *
    * A normal vector set once remains in effect and applies to all 
    * vertices that follow, until a different normal is set
    * (again, OpenGL uses state machine model)
    *
    * In this example, for each face all three/four vertices are given
    * the same normal perpendicular to the FRONT face. 
    * If a vertex is shared by more than one face (not in this example),
    * then it can have a normal vector for each of these faces. 
    * See icosahedron_mod1.c and icosahedron_mod2.c 
    */

   /* draw a tabletop */   
   glNormal3f( 0.0, 1.0, 0.0);
   glBegin(GL_POLYGON);
      glVertex3f(-3.0, 0.0,-3.0);
      glVertex3f(-3.0, 0.0, 3.0);
      glVertex3f( 3.0, 0.0, 3.0);
      glVertex3f( 3.0, 0.0,-3.0);
   glEnd();
      /* face drawn CCW is FRONT, the other side is BACK 
       *
       * faces must be simple and convex
       * non-flat faces may not be rendered correctly
       * - partitioning faces to triangles (triangulation) avoids 
       *   the problem --- more later */

   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
   glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, cyan);
   /* next two lines are unnecessary, since these values have been 
    * set above and are still in effect (again, state machine model)*/
   //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
   //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);

   /* we see the yellow FRONT face */
   glNormal3f(0.0,0.0,1.0); 
   glBegin(GL_POLYGON);       
       glVertex3f( 0.0,2.0,2.0);
       glVertex3f(-1.0,0.0,2.0);
       glVertex3f( 1.0,0.0,2.0);
   glEnd();

   /* we see the cyan BACK face */
   glNormal3f(0.0,0.0,-1.0);  
   glBegin(GL_POLYGON);
       glVertex3f( 0.0,0.0,0.0);
       glVertex3f( 0.0,2.0,0.0);   
       glVertex3f( 2.0,2.0,0.0);          
       glVertex3f( 2.0,0.0,0.0);       
   glEnd();

   /* we see the yellow FRONT face */
   glNormal3f(0.0,0.0,1.0);  
   glBegin(GL_POLYGON); 
       glVertex3f(-2.0,0.0,-1.0);
       glVertex3f( 2.5,0.0,-1.0);
       glVertex3f( 2.5,1.0,-1.0);           
       glVertex3f(-2.0,1.0,-1.0); 
   glEnd();

   glFlush(); /* Force all drawing commands to complete */
}

int main(int argc, char** argv)
{
   /* initialize GLUT library
    * can process command line options, such as -geometry in X */
   glutInit(&argc, argv); 

   /* declare display mode (single buffer, RGB, depth test on) */
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

   /* set window size and position, and create window (not displayed yet) */
   glutInitWindowSize(800, 800); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);

   init(); /* call initialization routine */

   /* register display() as display callback function */
   glutDisplayFunc(display); 
   
   /* enter main loop, open created window, begin event processing,
    * and trigger display callback */
   glutMainLoop(); 

   return 0; /* ANSI C requires main to return int */
}
//\end{verbatim}}

//{\small\begin{verbatim}
/* 
 * icosahedron1.c 
 * modified by Ichiro Suzuki 
 *
 * - all three vertices of a triangle get the same normal vector
 *   perpendicular to the triangle
 */

/* 
 * - This program is based on the example from the RedBook
 *   on drawing an icosahedron (P88), implemented by Glenn Wardius.
 * - Repeated subdivision, by entering 's', causes successively
 *   better approximations of a sphere of radius 1.
 * - 'w' to see wireframe
 */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define X  0.525731112119133606
#define Z  0.850650808352039932

void display();
void subdivide (float* v1, float* v2, float* v3, int depth);
void drawtriangle(float* v1, float* v2, float* v3);
void normalize(float* v);

static GLfloat vdata[12][3] = {
  {-X, 0.0, Z}, { X, 0.0, Z}, {-X, 0.0,-Z}, { X, 0.0,-Z},
  { 0.0, Z, X}, { 0.0, Z,-X}, { 0.0,-Z, X}, { 0.0,-Z,-X},
  { Z, X, 0.0}, {-Z, X, 0.0}, { Z,-X, 0.0}, {-Z,-X, 0.0}
};

static GLuint tindices[20][3] = {
   {1,4,0},  {4,9,0},  {4,5,9},  {8,5,4},  {1,8,4},
  {1,10,8}, {10,3,8},  {8,3,5},  {3,2,5},  {3,7,2},
  {3,10,7}, {10,6,7}, {6,11,7}, {6,0,11},  {6,1,0},
  {10,1,6}, {11,0,9}, {2,11,9},  {5,2,9}, {11,2,7} 
};

static GLfloat normal[3];
int depth = 0;

static GLfloat white[]          = {1.0, 1.0, 1.0, 1.0};
static GLfloat red[]            = {1.0, 0.0, 0.0, 1.0};
static GLfloat position[]       = {2.0, 2.0, 2.0, 1.0};
static GLfloat lmodel_ambient[] = {0.3, 0.3, 0.3, 1.0};

/* we subdivide as follows: for each edge of a triangle face of
 * the icosahedron we find its midpoint. This midpoint is then
 * extended (normalized) to lie on the sphere. From each icosahedron
 * triangle we therefore get six vertices on the sphere - three original
 * and three edge midpoints. These are then joined as indicated to 
 * form four new smaller triangles. Thus each original triangle is
 * subdivided into four.
 * The procedure can be repeated recursively.
 *
 *                     23
 *                3 ________ 2
 *                  \  /\  /
 *                   \/__\/
 *                 31 \  / 12
 *                     \/
 *                      1
 */

void subdivide (float* v1, float* v2, float* v3, int depth)
{
  GLfloat v12[3], v23[3], v31[3];
  GLint i;

  if (depth == 0) 
  {
    drawtriangle(v1, v2, v3);
    return;
  };

  /* compute the midpoints of the three edges
   * Note: since v12 etc are normalized (scaled to length 1) next,
   * division by 2 "/2.0" is not really necessary */
  for(i=0; i < 3; i++)
  {
    v12[i] = (v1[i] + v2[i])/2.0;
    v23[i] = (v2[i] + v3[i])/2.0; 
    v31[i] = (v3[i] + v1[i])/2.0;
  };

  /* scale vectors to length 1 so that the endpoints lie on sphere */
  normalize(v12);
  normalize(v23);
  normalize(v31);

  subdivide( v1, v12, v31, depth-1);
  subdivide( v2, v23, v12, depth-1);
  subdivide( v3, v31, v23, depth-1);
  subdivide(v12, v23, v31, depth-1);
}

void drawtriangle(float* v1, float* v2, float* v3)
{
  static GLfloat vect12[3], vect13[3];
 
  /* Calculate normal vector perpendicular to triangle v1,v2,v3
     and pointing outward, as the cross 
     product of vectors v1-->v2 and v1-->v3. See BHC A-2 */
  vect12[0]=v2[0]-v1[0];  
  vect12[1]=v2[1]-v1[1];  
  vect12[2]=v2[2]-v1[2];
  vect13[0]=v3[0]-v1[0];  
  vect13[1]=v3[1]-v1[1];  
  vect13[2]=v3[2]-v1[2];

  normal[0]=vect12[1]*vect13[2]-vect12[2]*vect13[1];
  normal[1]=vect12[2]*vect13[0]-vect12[0]*vect13[2];
  normal[2]=vect12[0]*vect13[1]-vect12[1]*vect13[0];  
  normalize(normal);

  /* use the same normal vector for the three vertices */
  glBegin(GL_TRIANGLES);
    glNormal3fv(normal);  
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3fv(v3);
  glEnd();
}

/* normalizes vector v to length 1 */
void normalize(float* v)
{
  GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

  if (d == 0.0)
  {
    printf("zero length vector\n");
    exit(-1);
  };

  v[0] /= d;  v[1] /= d;  v[2] /= d;
}

void init(void)
{
   printf("\n\
   icosahedron_mod1.c by Ichiro Suzuki suzuki@cs.uwm.edu\n\
   For each triangle we set the same normal vector for all\n\
   three corners\n\n\
   s --- subdivide\n\
   w --- wireframe\n\
   ESC - exit\n\
   \n");

   glClearColor(0.0, 0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);  
   //wireframe looks better without next command
   //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   glShadeModel(GL_SMOOTH);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

}

void reshape (int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void display(void)
{
  int i;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLightfv(GL_LIGHT0, GL_POSITION, position); 
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,red);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
  
  /* global ambient light not from any particular source,
     slightly brighter than the default (0.2,0.2,0.2,1.0)
     RedBook P202, HB P641 */
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

  for (i = 0; i < 20; ++i)
    subdivide(&vdata[tindices[i][0]][0], 
              &vdata[tindices[i][1]][0], 
              &vdata[tindices[i][2]][0], 
              depth);

  glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
   static int polygonmode[2]; 
   
   switch (key) {
      case 27:
         exit(0);
         break;

      case 'w':
         glGetIntegerv(GL_POLYGON_MODE,polygonmode);
         if (polygonmode[0]==GL_FILL) 
              glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         glutPostRedisplay();
         break;

      case 's':
         ++depth;
         if (depth > 5) depth =0;
         printf("The subdividing depth is now %d\n", depth);
         glutPostRedisplay();
         break;
  }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 
   glutInitWindowSize (800, 800);
   glutInitWindowPosition (0, 0);
   glutCreateWindow (argv[0]);
   init();
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
//\end{verbatim}}


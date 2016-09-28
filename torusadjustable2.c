//{\small\begin{verbatim}
/* torusadjustable2.c */

/* 
 *  By Ichiro Suzuki. Parts of this program might have been adopted 
 *  from or inspired by sample programs in "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*  
 *  - based on torus.c but does not use display list
 *  - all the usual controls for light and camera
 *  - uses GL_QUAD_STRIP
 *  - torus properties controlled by 
 *    "i", "I", "o", "O", "s", "S", "r", "R"
 */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

/* camera location in longitude and latitude */
static float alpha = 0.0;
static float beta  = 0.0;;

/* position of camera */
static GLdouble cpos[3]; 

/* position of light source */
static GLfloat lpos[] = {6.0, 6.0, 6.0, 1.0};

static GLfloat twoPI = 2*PI;
static GLfloat iradius =1.0;
static GLfloat oradius =5.0;
static int nsides = 10;
static int nrings = 20;

/* vectors to specify material properties */ 
static GLfloat none[]    = {0.0, 0.0, 0.0, 1.0};
static GLfloat white[]   = {1.0, 1.0, 1.0, 1.0};
static GLfloat red[]     = {1.0, 0.0, 0.0, 1.0};
static GLfloat cyan[]    = {0.0, 1.0, 1.0, 1.0}; 

void writemessage()
{
   printf("\n\
   torusadjustable2.c by Ichiro Suzuki suzuki@cs.uwm.edu\n\n\
   x, X, y, Y, z, Z ---------- change light source position\n\
   arrow keys ---------------- change camera position\n\
   i, I ---------------------- change inner radius\n\
   o, O ---------------------- change outer radius\n\
   s, S ---------------------- change number of sides\n\
   r, R ---------------------- change number of rings\n\
   w ------------------------- toggle between fill and wire-frame\n\
   ESC ----------------------- exit\n\
   \n");
}

/* normalizes vector v to length 1 */
void normalize(GLfloat v[3])
{
  GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

  if (d == 0.0) {
    printf("zero length vector\n");
    exit(-1);
  };

  v[0] /= d; v[1] /= d; v[2] /= d;
}

/* draws a torus 
 *   
 * uses GL_QUAD_STRIP as
 *       :    :
 *       :    :
 *       4----5
 *       |    |
 *       |    |
 *       2----3
 *       |    |
 *       |    |
 *       0----1
 */
static void torus(GLfloat innerradius, GLfloat outerradius, 
                  int numsides, int numrings)
{
   int i, j, s, t;
   double x, y, z;
   double xc, yc, zc;
   GLfloat normalvector[3];

   /* note that in wireframe inside of torus becomes visible */
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
   glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, cyan);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);

   for (i = 0; i < numsides; i++) {
      glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= numrings; j++) {
          t = j % numrings;

          /* (xc,yc,zc) is at the center of tube */
          xc = outerradius*cos(t*twoPI/numrings);
          yc = outerradius*sin(t*twoPI/numrings);
          zc = 0.0;

          s = (i + 1) % numsides;
          /* (x,y,z) is on the surface of torus */
          x = (outerradius+innerradius*cos(s*twoPI/numsides))
             *cos(t*twoPI/numrings);
          y = (outerradius+innerradius*cos(s*twoPI/numsides))
             *sin(t*twoPI/numrings);
          z = innerradius*sin(s*twoPI/numsides);

          normalvector[0] = x-xc;
          normalvector[1] = y-yc;
          normalvector[2] = z-zc;
          normalize(normalvector); 
          glNormal3fv(normalvector);
          glVertex3f(x, y, z);

          s = i % numsides;
          /* (x,y,z) is on the surface of torus */
          x = (outerradius+innerradius*cos(s*twoPI/numsides))
             *cos(t*twoPI/numrings);
          y = (outerradius+innerradius*cos(s*twoPI/numsides))
             *sin(t*twoPI/numrings);
          z = innerradius*sin(s*twoPI/numsides);

          normalvector[0] = x-xc;
          normalvector[1] = y-yc;
          normalvector[2] = z-zc;
          normalize(normalvector); 
          glNormal3fv(normalvector);
          glVertex3f(x, y, z);
        }
      glEnd();
   }
}

void init(void) 
{
   writemessage(); 

   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   /* initially GL_FILL mode, later GL_LINE to show wireframe */
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

   glEnable(GL_LIGHTING);
   /* enable two-sided lighting since inside of torus is
    * visible in wireframe */
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   glEnable(GL_LIGHT0);
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 2.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
}

void display(void)
{

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* initialize (modelview) matrix */
   glLoadIdentity(); 
 
   /* update camera position */
   cpos[0] = 30.0 * cos(beta) * sin(alpha);
   cpos[1] = 30.0 * sin(beta);
   cpos[2] = 30.0 * cos(beta) * cos(alpha);
   gluLookAt(cpos[0],cpos[1],cpos[2], 0.0,1.0,0.0, 0.0,1.0,0.0);

   /* set current light source position */
   glLightfv(GL_LIGHT0, GL_POSITION, lpos);

   /* draw sphere to show light source */
   glMaterialfv(GL_FRONT, GL_EMISSION, white); 
   glPushMatrix();
     glTranslatef(lpos[0],lpos[1],lpos[2]);
     glutSolidSphere (0.1, 10, 8);
   glPopMatrix();

   /* remaining objects do not emit light */
   glMaterialfv(GL_FRONT, GL_EMISSION, none);

   /* draw torus */
   torus(iradius,oradius,nsides,nrings);

   glFlush();
   glutSwapBuffers();
}

/* torus properties controlled by "i", "I", "o", "O", "s", "S", "r", "R"
 * "w" to toggle between filled and wire-frame rendering */
void keyboard(unsigned char key, int x, int y)
{
   static int polygonmode[2];

   switch (key) {
      case 27:
         exit(0);
         break; 

      /* change size and resolution of torus */
      case 'i':
         iradius = iradius+0.2;
         glutPostRedisplay();
         break;
      case 'I':
         if (iradius > 0.4) iradius = iradius - 0.2;
         glutPostRedisplay();
         break;
      case 'o':
         oradius = oradius+0.2;
         glutPostRedisplay();
         break;
      case 'O':
         if (oradius > 0.4) oradius = oradius - 0.2;
         glutPostRedisplay();
         break;
      case 's':
         nsides = nsides + 1;
         glutPostRedisplay();
         break;
      case 'S':
         if (nsides > 3) nsides = nsides - 1;
         glutPostRedisplay();
         break;
      case 'r':
         nrings = nrings + 1;
         glutPostRedisplay();
         break;
      case 'R':
         if (nrings > 3) nrings = nrings - 1;
         glutPostRedisplay();
         break;

      case 'w':
         glGetIntegerv(GL_POLYGON_MODE,polygonmode);
         if (polygonmode[0]==GL_FILL) 
              glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         glutPostRedisplay();
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

void specialkey(GLint key, int x, int y)
{
   switch (key) {
      case GLUT_KEY_RIGHT:                
         alpha = alpha + PI/180;
         if (alpha > 2*PI) alpha = alpha - 2*PI;
         glutPostRedisplay();
         break;
      case GLUT_KEY_LEFT:                 
         alpha = alpha - PI/180;
         if (alpha < 0) alpha = alpha + 2*PI;
         glutPostRedisplay();
         break;
      case GLUT_KEY_UP:                
         if (beta < 0.45*PI) beta = beta + PI/180;
         glutPostRedisplay();
         break;
      case GLUT_KEY_DOWN:                 
         if (beta > -0.45*PI) beta = beta - PI/180;
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
   glutInitWindowSize(800, 800); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialkey);
   glutMainLoop();
   return 0;
}
//\end{verbatim}}
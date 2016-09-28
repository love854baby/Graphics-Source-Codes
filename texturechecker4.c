//{\small\begin{verbatim}
/* texturechecker4.c */

/* 
 *  By Ichiro Suzuki. Parts of this program might have been adopted 
 *  from or inspired by sample programs in "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*  
 *  - generates and applies two textures
 */

#include <GL/glut.h>
#include <stdlib.h>

static GLfloat lpos[] = {-3.0, 2.0, 4.0, 1.0};

/* vectors to specify material properties */ 
static GLfloat medium[]  = {64}; 
static GLfloat high[]    = {128.0}; /* shininess 0-128 */

static GLfloat white[]   = {1.0, 1.0, 1.0, 1.0};
static GLfloat gray[]    = {0.5, 0.5, 0.5, 1.0};
static GLfloat red[]     = {1.0, 0.0, 0.0, 1.0};
static GLfloat green[]   = {0.0, 1.0, 0.0, 1.0};
static GLfloat blue[]    = {0.0, 0.0, 1.0, 1.0};
static GLfloat yellow[]  = {1.0, 1.0, 0.0, 1.0}; 
static GLfloat magenta[] = {1.0, 0.0, 1.0, 1.0}; 
static GLfloat cyan[]    = {0.0, 1.0, 1.0, 1.0}; 

/* to store texture names */
static GLuint texName[2];

/* create two checkerboard textures */
#define        checkImageWidth  64
#define        checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLubyte otherImage[checkImageHeight][checkImageWidth][4];

void makeCheckImages(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
       for (j = 0; j < checkImageWidth; j++) {
         c = (((i/8)%2)!=((j/8)%2))*255;
         //cyan and black
         checkImage[i][j][0] = (GLubyte) 0;   // R
         checkImage[i][j][1] = (GLubyte) c;   // G
         checkImage[i][j][2] = (GLubyte) c;   // B
         checkImage[i][j][3] = (GLubyte) 100; //255; // A

         //blue and black
         otherImage[i][j][0] = (GLubyte) 0;   // R 
         otherImage[i][j][1] = (GLubyte) 0;   // G
         otherImage[i][j][2] = (GLubyte) c;   // B
         otherImage[i][j][3] = (GLubyte) 100; //255; // A
      }
   }
}

void init(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   /* generate two texture patterns */
   makeCheckImages();

   /* set the pixel storage mode; look it up and see what it does */
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   /* get two currently unsed names for texture objects */
   glGenTextures(2, texName);

   /* define the first 2D texture */
   glBindTexture(GL_TEXTURE_2D, texName[0]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
             checkImageWidth, checkImageHeight, 0, 
             GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

   /* define the second 2D texture */
   glBindTexture(GL_TEXTURE_2D, texName[1]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
             checkImageWidth, checkImageHeight, 0, 
             GL_RGBA, GL_UNSIGNED_BYTE, otherImage);
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 2.0, 20.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(2.0, 5.0, 10.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLightfv(GL_LIGHT0, GL_POSITION, lpos);

   glEnable(GL_TEXTURE_2D);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

   // activate first texture    
   glBindTexture(GL_TEXTURE_2D, texName[0]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  

   // tabletop
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
   glMaterialfv(GL_FRONT, GL_SPECULAR, white);
   glMaterialfv(GL_FRONT, GL_SHININESS, medium);
   glNormal3f( 0.0, 1.0, 0.0);  
   glBegin(GL_POLYGON);
      /* set texture coordinates (s,t) for each vertex */
      glTexCoord2f(0.0, 0.0); glVertex3f(-3.0, 0.0, 3.0);
      glTexCoord2f(1.0, 0.0); glVertex3f( 3.0, 0.0, 3.0);
      glTexCoord2f(1.0, 1.0); glVertex3f( 3.0, 0.0,-3.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-3.0, 0.0,-3.0);
   glEnd();

   // large lower square
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
   glNormal3f( 0.0, 0.0, 1.0);  
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-3.0, 0.0, 2.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, 0.0, 2.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 2.0, 2.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-3.0, 2.0, 2.0);
   glEnd();

   // large upper square
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glNormal3f( 0.0, 0.0, 1.0);  
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-3.0, 3.0, 2.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, 3.0, 2.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 5.0, 2.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-3.0, 5.0, 2.0);
   glEnd();

   // activate second texture    
   glBindTexture(GL_TEXTURE_2D, texName[1]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);  

   // short rectangle (front)
   glNormal3f( 0.0, 0.0, 1.0);  
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f( 0.0, 0.0, 2.7);
      glTexCoord2f(2.0, 0.0); glVertex3f( 1.5, 0.0, 2.7);
      glTexCoord2f(2.0, 2.0); glVertex3f( 1.5, 1.0, 2.7);
      glTexCoord2f(0.0, 2.0); glVertex3f( 0.0, 1.0, 2.7);
   glEnd();
 
   // short rectangle (back)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glNormal3f( 0.0, 0.0, 1.0);  
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f( 1.0, 0.0, 0.7);
      glTexCoord2f(2.0, 0.0); glVertex3f( 2.5, 0.0, 0.7);
      glTexCoord2f(2.0, 2.0); glVertex3f( 2.5, 1.0, 0.7);
      glTexCoord2f(0.0, 2.0); glVertex3f( 1.0, 1.0, 0.7);
   glEnd();

   // activate first texture    
   glBindTexture(GL_TEXTURE_2D, texName[0]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // tall rectangle
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   glNormal3f( 0.0, 0.0, 1.0);  
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f( 1.0, 0.0,-2.0);
      glTexCoord2f(2.0, 0.0); glVertex3f( 2.0, 0.0,-2.0);
      glTexCoord2f(2.0, 0.5); glVertex3f( 2.0, 4.0,-2.0);
      glTexCoord2f(0.0, 0.5); glVertex3f( 1.0, 4.0,-2.0);
   glEnd();

   // triangle
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
   glNormal3f( 0.0, 0.0, 1.0);  
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 0.0, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex3f( 0.0, 0.0, 0.0);
      glTexCoord2f(2.0, 2.0); glVertex3f(-1.5, 4.0, 0.0);
   glEnd();
 
   /* disable texture map */
   glDisable(GL_TEXTURE_2D);

   // blue square without texture
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
   glNormal3f( 0.0, 0.0, 1.0);  
   glBegin(GL_POLYGON);
      glVertex3f(-1.0, 3.0,-3.0);
      glVertex3f( 0.0, 3.0,-3.0);
      glVertex3f( 0.0, 4.0,-3.0);
      glVertex3f(-1.0, 4.0,-3.0);
   glEnd();

   glFlush();
}

/* exit program with "esc" key */
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
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

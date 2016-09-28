//{\small\begin{verbatim}
/* texturechecker3.c */

/* 
 *  By Ichiro Suzuki. Parts of this program might have been adopted 
 *  from or inspired by sample programs in "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 */

/*  
 *  - generates a black and white checker patttern and applies it to 
 *    various objects
 *  - uses GL_REPEAT, GL_CLAMP, GL_MODULATE, GL_REPLACE, GL_DECAL
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

/* create a black and white checkerboard texture */
#define        checkImageWidth  64
#define        checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

void makeCheckImage(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = (((i/8)%2)!=((j/8)%2))*255;
         //black and white checker pattern
         checkImage[i][j][0] = (GLubyte) c;   // R
         checkImage[i][j][1] = (GLubyte) c;   // G
         checkImage[i][j][2] = (GLubyte) c;   // B
         checkImage[i][j][3] = (GLubyte) 100; //255; // A
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

   /* define a 2D texture */
   makeCheckImage();
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
                checkImageWidth, checkImageHeight, 0, 
                GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
   /* - GL_TEXTURE_2D (target); look up other options in BHC
      - 0 (level); should be 0 unless we supply multiple resolutions
      - GL_RGBA (internal format); specifies which of RGBA are used to 
        describe each color of texture; look up other options
      - checkImageWidth, checkImageHeight (width and height of texture image);
        must be 2^m (no border) or 2^m+2 with border
      - 0 (border); must be 0 or 1; see BHC for details
      - GL_RGBA, GL_UNSIGNED_BYTE (format and type of texture image data)
      - chackImage (*texels); actual texture image data
    */
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

   /* enable texture map */
   glEnable(GL_TEXTURE_2D);
   /* the following might help if texture looks distorted when applied */
   glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

   /* glTexParameteri() sets how texture is applied
      - set texture wrapping in s (=x) and t (=y) directions
        to GL_REPEAT, so the pattern repeats outside [0,1].
      - texture coordinates are s, t, r, q (= x, y, z, w)
      - GL_CLAMP is another option
    */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   /* - set texture filtering to GL_NEAREST for both
        Magnification and Minification
      - GL_NEAREST; use texel nearest to center of pixel
      - GL_LINEAR; use weighted linear average of 2x2 array of 
        texels nearest to center of pixel; slower but smoother
    */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   /* glTexEnvf() sets texture mapping options
      - GL_MODULATE; object values are multiplied with texture values
      - GL_REPLACE; object values are replaced by texture values; 
        like applying an opaque label; ignores lighting effect on object;
        turn light off and see
      - GL_DECAL; similar to REPLACE, except subject color is blended 
        with texture color according to texture alpha value
      - and more
    */   
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

   // short rectangle (front)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
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

   // tall rectangle
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

//{\small\begin{verbatim}
/* texwall2.c 
 * modified by I. Suzuki */
/* uses gluBuild2DMipmaps(); look up the details */

/*  
tex_wall.c
by Tom Lassanske 10/21/98
Generates a brick image and applies it to a wall.  */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

/* data for one texture map */
unsigned char tex_image[32][32][3];
int height = 32;
int width = 32;

GLdouble vertex[9][2] = {{0,0},{1,0},{2,0},{3,0},{1,2},{2,2},{0,3},
                         {3,3},{1.5,5}};
GLint    tri[7][3]    = {{0,1,4},{0,4,6},{4,7,6},{4,5,7},{5,3,7},
                         {2,3,5},{6,7,8}};
GLdouble scale = 3;
GLdouble xx=0, yy=0, zz = -4;

void GenTextureArray(unsigned char new_image[32][32][3])
{
  int x, y, c;
  unsigned char cbuf[3][32][3];

  /* fill all arrays with grout color (buf 0 is all grout) */
  for (x=0; x<3;x++)
    for (y=0; y<32; y++)
      for (c=0; c<3; c++)
        cbuf[x][y][c] = 200;

  /* add brick color to buf 1 */
  for (x=1; x<31; x++)
    for (c=0; c<3; c++)
      cbuf[1][x][c] = 100;

  /* add brick color to buf 2 */
  for (x=0; x<15; x++)
    for (c=0; c<3; c++)
      cbuf[2][x][c] =100;
  
  for (x=17; x<32; x++)
    for (c=0; c<3; c++)
      cbuf[2][x][c] =100;

  /* fill texture map with grout rows (buf 0) */
  for (x=0; x<32; x++) 
    for (c=0; c<3; c++) {
      new_image[0][x][c]  = cbuf[0][x][c];
      new_image[15][x][c] = cbuf[0][x][c];
      new_image[16][x][c] = cbuf[0][x][c];
      new_image[31][x][c] = cbuf[0][x][c];
    }
  
  /* now the first brick row */
  for (y=1; y<15; y++)
    for (x=0; x<32; x++)
      for (c=0; c<3; c++)
        new_image[y][x][c] = cbuf[1][x][c];

  /* finally, the alternating brick row */
  for (y=17; y<31; y++)
    for (x=0; x<32; x++)
      for (c=0; c<3; c++)
        new_image[y][x][c] = cbuf[2][x][c];
}

void init(void)
{    
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  
  /*** generate texture *****/
  GenTextureArray(tex_image);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
  /* automatically generate reduction patterns called mipmaps, HB P633, 657.
     compare results with/without mipmaps */
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height,
                    GL_RGB, GL_UNSIGNED_BYTE, tex_image);
  //glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,
  //             GL_RGB,GL_UNSIGNED_BYTE,tex_image);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  /* minification option includes GL_???_MIPMAP_NEAREST, GL_???_MIPMAP_LINEAR */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  
  glEnable(GL_TEXTURE_2D);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
}

void DrawTriangle(int x)
{
  int v;
  for (v=0; v<3; v++) {
    glTexCoord2d(scale*vertex[tri[x][v]][0]+xx, scale*vertex[tri[x][v]][1]+yy);
    glVertex3d(vertex[tri[x][v]][0], vertex[tri[x][v]][1], zz);
  }
}

void display(void)
{
  int x;  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBegin(GL_TRIANGLES);
    for (x=0; x<7; x++)
      DrawTriangle(x);
  glEnd();

  glFlush();  
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 1.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.75,1.5,2.0, 0.75,1.5,0.0, 0.0,1.0,0.0);
}

void keyboard (unsigned char key, int x, int y)
{
  switch (key) {
  case 27:
    exit(0);
    break;
  case 's':
    scale *= 2;
    glutPostRedisplay();
    break;
  case 'S':
    scale /= 2;
    glutPostRedisplay();
    break;
  case 'x':
    xx += .1;
    glutPostRedisplay();
    break;
  case 'X':
    xx -= .1;
    glutPostRedisplay();
    break;
  case 'y':
    yy += .1;
    glutPostRedisplay();
    break;
  case 'Y':
    yy -= .1;
    glutPostRedisplay();
    break;
  case 'z':
    zz += 1;
    glutPostRedisplay();
    break;
  case 'Z':
    zz -= 1;
    glutPostRedisplay();
    break;
  default:
    break;
  }
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(250, 250);
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


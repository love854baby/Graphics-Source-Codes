//{\small\begin{verbatim}
/* texturebmp2.c
   by Ichiro Suzuki

   This program reads in two *.bmp images and applies them to 
   two rectangles as texture.

   Partly based on, or inspired by,
   - tex_double.c by Tom Lassanske 10/21/98
   - texbind.c, "OpenGL Programming Guide", 2nd Edition, Addison-Wesley
   - LoadBMP for reading *.rgb image by Paul Haeberli, SGI
   - RGBpixmap class in "Computer Graphics Using OpenGL", 2nd ed.,
     F.S. Hill, Jr, Prentice-Hall
   - sample programs by Alan Nichols 2001.8
*/

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

GLfloat distance = 0.0;
GLuint tex_name[2];

/* data for two texture arrays */
unsigned char *tex_image[2] = {NULL, NULL};
int height[2];
int width[2];

unsigned char *LoadBMP(char file_name[], int *w, int *h)
{
  unsigned char *new_image;
  GLubyte header[54]; // 54-byte header
  int numbytesinrow, numpadbytes;
  int width, height, bitsperpixel, compression;
  long count = 0;
  int i, j, k;
  unsigned char r, g, b, dum;

  FILE *file = fopen(file_name,"rb");

  printf("file name: %s\n", file_name);

  if (file == NULL) {
      printf("cannot open file %s\n",file_name);
      exit(1);
  };

  if (fread(header,1,54,file) != 54) { //must read 54 bytes
      printf("cannot read 54-byte header\n");
      fclose(file);
      exit(1);
  };

  if (header[0]!='B' || header[1]!='M') { //must be 'BM'
      printf("not a bmp file\n");
      fclose(file);
      exit(1);
  };

  // 4-byte and 2-byte little-endian integers
  width       = ((header[21]*256+header[20])*256+header[19])*256+header[18];
  height      = ((header[25]*256+header[24])*256+header[23])*256+header[22];  
  bitsperpixel= header[29]*256+header[28];
  compression = ((header[33]*256+header[32])*256+header[31])*256+header[30];  

  if (compression != 0) {
      printf("file is compressed\n");
      fclose(file);
      exit(1);
  };

  if (bitsperpixel != 24) {
      printf("not a 24-bit pixel image\n");
      fclose(file);
      exit(1);
  };

  printf("width  = %d\n",width);
  printf("height = %d\n",height);
  printf("bits per pixel = %d\n",bitsperpixel);
  printf("loading pixels...\n");
  
  /* allocate memory for texture */
  new_image = (unsigned char *) malloc((height)*(width)*3);

  //each row has pad bytes to make number of bytes a multiple of 4
  //so number of bytes is 3*width rounded up to next multiple of 4
  numbytesinrow = ((3 * width + 3)/4) * 4;
  numpadbytes = numbytesinrow - 3 * width; //need this many
  printf("number of bytes in row = %d\n",numbytesinrow);
  printf("number of pad bytes = %d\n",numpadbytes);

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      b = fgetc(file);
      g = fgetc(file);
      r = fgetc(file);
      new_image[count++] = r;
      new_image[count++] = g;
      new_image[count++] = b;     
    }
    for (k = 0; k < numpadbytes; k++) //skip pad bytes at row's end
      dum = fgetc(file);
  }
  
  printf("number of pixels = %d\n\n",count/3); //should be height*width
  fclose(file);

  /* update the width and height pointer values */
  *w  = width; 
  *h  = height;

  return new_image;  
}

void init(void)
{    
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  
  /*** generate textures *****/
  glGenTextures(2, tex_name);

  /* read *.bmp files */
  tex_image[0] = LoadBMP("harbor.bmp", &width[0], &height[0]);  //w*h=514*343
  tex_image[1] = LoadBMP("gwimage.bmp", &width[1], &height[1]); //640*480
//  tex_image[0] = LoadBMP("brick2.bmp", &width[0], &height[0]);  //32*32
//  tex_image[1] = LoadBMP("fleur.bmp", &width[1], &height[1]);   //64*64
//  tex_image[0] = LoadBMP("money.bmp", &width[0], &height[0]);   //64*64
//  tex_image[1] = LoadBMP("azaleas.bmp", &width[1], &height[1]); //64*64
//  tex_image[0] = LoadBMP("carpet5.bmp", &width[0], &height[0]); //64*64
//  tex_image[1] = LoadBMP("honey.bmp", &width[1], &height[1]);   //128*128
//  tex_image[0] = LoadBMP("mandrill.bmp", &width[0], &height[0]);//128*128
//  tex_image[1] = LoadBMP("cool.bmp", &width[1], &height[1]);    //128*128

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  
  glBindTexture(GL_TEXTURE_2D, tex_name[0]);
  //if width and height are powers of 2 then you can use
  //glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width[0],height[0],
  //             0,GL_RGB,GL_UNSIGNED_BYTE,tex_image[0]);
  //Otherwise use gluBuild2DMipmaps(), which scales the image to 
  //the nearest power of 2, creates a series of mipmaps of different 
  //resolutions down to 1x1, and calls glTexImage2D().
  //See Red Book pp. 370 and 398, HB P658
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width[0], height[0],
                    GL_RGB, GL_UNSIGNED_BYTE, tex_image[0]);  

  glBindTexture(GL_TEXTURE_2D, tex_name[1]);  
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width[1], height[1],
                    GL_RGB, GL_UNSIGNED_BYTE, tex_image[1]);  

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  glEnable(GL_TEXTURE_2D);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -3.5-distance); 
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, tex_name[0]);
  glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0); glVertex3f(-3.0,-1.0, 0.0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-1.0,-1.0, 0.0);
   glTexCoord2f(1.0, 3.0); glVertex3f(-1.0, 1.0, 0.0);
   glTexCoord2f(0.0, 3.0); glVertex3f(-3.0, 1.0, 0.0);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, tex_name[1]);
  glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0); glVertex3f(1.0,-1.0, 0.0);
   glTexCoord2f(2.0, 0.0); glVertex3f(3.0,-1.0, 0.0);  
   glTexCoord2f(2.0, 1.0); glVertex3f(3.0, 1.0, 0.0);
   glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
  glEnd();

  glFlush();
  glutSwapBuffers();
}

void keyboard (unsigned char key, int x, int y)
{
  switch (key) {
  case 27:
    exit(0);
    break;
  case 'f':
    distance = distance + 1.0;
    glutPostRedisplay();
    break;
  case 'F':
    if (distance > 1.0) distance = distance - 1.0;
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
  glutInitWindowSize(400, 250);
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



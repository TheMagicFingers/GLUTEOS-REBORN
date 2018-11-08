#include <stdio.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <array>
#include <vector>
#include <fstream>

int vp_width = 640;
int vp_height = 480;
std::array<int , 2> currentPt;
std::vector<std::array<int, 2>> pts;
bool closed = false;

void myInit(void){
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, vp_width, 0.0, vp_height);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}
void draw_polygon(int button, int state, int x, int y){
    currentPt = std::array<int, 2>{x, vp_height-y};

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if ( closed )
            pts.clear(); // restart if last action was close
        closed = false;
        pts.push_back( currentPt );
    }
    if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
        closed = true;
}
void mouse_move(int x, int y){
    currentPt = std::array<int, 2>{x, vp_height-y};
    glutPostRedisplay();
}
void display(void){
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    if ( !pts.empty() )
    {
        glBegin(GL_LINE_STRIP);
        for ( auto &pt : pts )
            glVertex2f( (float)pt[0], (float)pt[1] );
        auto &endPt = closed ? pts.front() : currentPt;
        glVertex2f( (float)endPt[0], (float)endPt[1] );
        glEnd();
    }
    glutSwapBuffers();
}
void menu_function(int item){
switch(item)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    default:
        break;
    }
    glutPostRedisplay();
    return;
}
void myTranslate(float tx, float ty) {
    for ( auto &pt : pts ){
        pt[0] = (float) pt[0] + tx;
        pt[1] = (float) pt[1] + ty;

        if(pt[0] > vp_width || pt[0] < 0 || pt[1] > vp_height || pt[1] < 0){
            pt[0] = pt[0];
            pt[1] = pt[1];
        }
        glutPostRedisplay();
    }
}
void myRotate(float degree){
    for ( auto &pt : pts ){
        pt[0] = (float)pt[0] * cos(degree)  - (float)pt[1] * sin(degree);
        pt[1] =  (float) pt[1] * sin(degree) + (float) pt[0] * cos(degree);

        if(pt[0] > vp_width || pt[0] < 0 || pt[1] > vp_height || pt[1] < 0){
            pt[0] = vp_width;
            pt[1] = vp_height;
        }
        glutPostRedisplay();
    }
}
void myScale(float sx, float sy){
    for ( auto &pt : pts ){
        pt[0] = (float) pt[0] * sx;
        pt[1] = (float) pt[1] * sy;

        if(pt[0] > vp_width || pt[0] < 0 || pt[1] > vp_height || pt[1] < 0){
            pt[0] = vp_width;
            pt[1] = vp_height;
        }
        glutPostRedisplay();
    }
}
void MyMirroring(){
}
void save(){

}
void changeColor(){

}
void keyboard_cb(unsigned char key, int X, int Y){
  switch(key){
      case 'a':         /*27 corresponde ao ESC, e está sendo utilizado para sair*/
          printf(" pointX -> %d  pointY -> %d \n", currentPt[0], currentPt[1]);
          break;
      case 't':
          myTranslate(10,10);
          break;
      case 'r':
          myRotate(0.01);
          break;
      case 's':
          myScale(1.1,1.2);
          break;
      case 'm':
          MyMirroring();
          break;
      case 'c':
          changeColor();
          break;
  }
}
void instructions() {
    printf("Instructions \n");
    printf("A - show Points \n");
    printf("R - rotate Object \n");
    printf("S - scale Object \n");
    printf("M - mirror object \n");
    printf("C - change Color \n");

}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(vp_width, vp_height);
	glutCreateWindow("Editor de poligonos");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard_cb);
    glutPassiveMotionFunc (mouse_move);
    glutMouseFunc(draw_polygon);
    glMatrixMode( GL_PROJECTION );
    glOrtho(0.0f, (float)vp_width, 0.0f, (float)vp_height, -1.0, 1.0);
    myInit();
    instructions();
    glutMainLoop();

    return 0;
}

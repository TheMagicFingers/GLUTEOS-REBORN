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
std::vector<std::vector<std::array<int,2>> > allPoints;

int r = 1.0,g = 1.0,b = 1.0;

bool closed = false;

void myInit(void){
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, vp_width, 0.0, vp_height);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}
void mouse_move(int x, int y){
    currentPt = std::array<int, 2>{x, vp_height-y};
    glutPostRedisplay();
}
void display(void){
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(r,g,b);
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
void printPoints(){
     printf("\nAll points of the figure\n");
     for ( auto &pt : pts ){
      printf("X--> %d Y-->%d\n",pt[0],pt[1]);
    }
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
    printf("\n Scale \n");
    for ( auto &pt : pts ){
        pt[0] = (float) pt[0] * sx;
        pt[1] = (float) pt[1] * sy;
        printf("new Points X-> %d Y ->%d \n", pt[0],pt[1]);
        if(pt[0] > vp_width || pt[0] < 0 || pt[1] > vp_height || pt[1] < 0){
            pt[0] = vp_width;
            pt[1] = vp_height;
        }
        glutPostRedisplay();
    }
}
void MyMirroring(){
    int maxX = pts[0][0];
    for ( auto &pt : pts ){
        if (pt[0] > maxX)
            maxX = pt[0];
    }

    for ( auto &pt : pts) {
        pt[0] = maxX + (maxX - pt[0]);
    }
}
void save(){

}
void changeColor(){

}
void keyboard_cb(unsigned char key, int X, int Y){
  switch(key){
      case 'a':         /*27 corresponde ao ESC, e está sendo utilizado para sair*/
          printPoints();
          break;
      case 'p':         /*27 corresponde ao ESC, e está sendo utilizado para sair*/
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
    printf("P - show current point \n");
    printf("R - rotate Object \n");
    printf("S - scale Object \n");
    printf("M - mirror object \n");
    printf("C - change Color \n");

}
void menuPrincipal(int op){

}
void menuCor(int op){
    switch(op){

    }
    glutPostRedisplay();
}
void menuOptions(int op){
    switch(op){
    case 0:
        printf("Salvando...");
        break;
    case 1:
        printf("Carregando...");
        break;
    case 2:
        printf("Saindo...");
        break;
    }
    glutPostRedisplay();
}
void criaMenu(){
    int menu,submenu1,submenu2;

    submenu1 = glutCreateMenu(menuOptions);
    glutAddMenuEntry("Salvar poligono",0);
    glutAddMenuEntry("Carregar poligono",1);
    glutAddMenuEntry("Sair",2);

    submenu2 = glutCreateMenu(menuCor);
    glutAddMenuEntry("Vermelho",0);
    glutAddMenuEntry("Verde",1);
    glutAddMenuEntry("Azul",2);

    menu = glutCreateMenu(menuPrincipal);
    glutAddSubMenu("Options",submenu1);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
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

    if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
        criaMenu();
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

#include <stdio.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string>
#include <algorithm>
#include <iterator>
#include <array>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

int vp_width = 640;
int vp_height = 480;
std::array<int , 2> currentPt;
std::vector<std::array<int, 2>> pts;

int r = 1.0,g = 1.0,b = 1.0;

bool closed = false, draw = false;

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
    printf("\nAll points of the figure of all figures\n");
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
void myRotate(){
    float ang = 3.14 / 2;
    int tmp_x, tmp_y, cx, cy;

    for ( auto &pt : pts ){
        cx = 0;
        cy = 0;

        for ( auto &pt : pts ){
            cx += pt[0];
            cy += pt[1];
        }

        cx /= pts.size();
        cy /= pts.size();

        tmp_x = cx + ((float)pt[0]-cx)*cos(ang) - ((float)pt[1]-cy)*sin(ang);
        tmp_y = cy + ((float)pt[0]-cx)*sin(ang) + ((float)pt[1]-cy)*cos(ang);

        pt[0] = tmp_x;
        pt[1] = tmp_y;

        glutPostRedisplay();
    }
}
void MyMirroring(){

    int maxX = pts[0][0];

    for ( auto &pt : pts ){
        if (pt[0] > maxX)
            maxX = pt[0];
    }

    for ( auto &pt : pts)
        pt[0] = maxX + (maxX - pt[0]);

    glutPostRedisplay();
}
void MyMirroring1(){

    int minX = pts[0][0];

    for ( auto &pt : pts ){
        if (pt[0] < minX)
            minX = pt[0];
    }

    for ( auto &pt : pts)
        pt[0] = minX + (minX - pt[0]);

    glutPostRedisplay();
}
void save(){
    ofstream outfile;
    srand (time(NULL));
    int name = rand()%100;
    outfile.open("C:\\Users\\lucas\\Downloads\\GLUTEOS-REBORN\\polygons\\polygon-0x5219.txt", ios::out);//std::ios_base::app
    for ( auto &pt : pts ){
        outfile << pt[0] << " " << pt[1] << endl;
    }
    outfile << "-" << endl;
    outfile << r << " " << g << " " << b << endl;
    outfile.close();
}
void keyboard_cb(unsigned char key, int X, int Y){
  switch(key){
      case 'p':         /*27 corresponde ao ESC, e est� sendo utilizado para sair*/
          printPoints();
          break;
      case 'f':         /*27 corresponde ao ESC, e est� sendo utilizado para sair*/
          printf(" pointX -> %d  pointY -> %d \n", currentPt[0], currentPt[1]);
          break;
      case 's':
          myTranslate(0,-10);
          break;
      case 'a':
          myTranslate(-10,0);
          break;
      case 'w':
          myTranslate(0,10);
          break;
      case 'd':
          myTranslate(10,0);
          break;
      case 'r':
          myRotate();
          break;
      case 'x':
          myScale(1.1,1.2);
          break;
      case 'm':
          MyMirroring();
          break;
      case 'j':
        save();
        break;
      case ' ':
        glBegin(GL_POLYGON);
        break;
      case 'n':
        MyMirroring1();
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
void menuDraw(int op){
	switch(op){
		case 0:
			draw = true;
			closed = false;
	        break;
	    case 1:
	    	closed = true;
	    	draw = false;
	    	break;
        case 2:
            closed = false;
            draw = false;
            pts.clear();
            break;
	}
}
void menuCor(int op){
    switch(op){
    case 0:
        r  = 1.0,
        g = 0.0,
        b = 0.0;
        break;
    case 1:
        r  = 0.0,
        g = 1.0,
        b = 0.0;
        break;
    case 2:
        r  = 0.0,
        g = 0.0,
        b = 1.0;
        break;
    case 3:
        r = 1.0,
        g = 1.0,
        b = 1.0;
        break;
    }
    glutPostRedisplay();
}
void save_file_name(string name){
    ofstream outfile;
    outfile.open("C:\\Users\\lucas\\Downloads\\GLUTEOS-REBORN\\config\\files.txt", ios::out);
    outfile << name << ".txt" << endl;
    outfile .close();
}

vector<string> get_file_names(){
    string STRING;
	ifstream infile;

	infile.open("C:\\Users\\lucas\\Downloads\\GLUTEOS-REBORN\\config\\files.txt");
	while(infile>>STRING){
        cout<<"/"<<STRING<<"/"<<endl;
	}

	infile.close();
}

void load(string name){
    save_file_name(name);
    string STRING;
	ifstream infile;
    int i=0;
    pts.clear();

	infile.open("C:\\Users\\lucas\\Downloads\\GLUTEOS-REBORN\\polygons\\"+name+".txt");
	if(infile.fail())
        cout << "fail" << endl;

    while(infile>>STRING) // To get you all the lines.
    {
        //getline(infile,STRING); // Saves the line in STRING.
        cout<<"/"<<STRING<<"/"<<endl; // Prints our STRING.
        if(STRING == "-")
            break;

        if(i % 2 == 0){
            cout << "into x: " << stoi(STRING) << endl;
            currentPt[0] = stoi(STRING);
        }
        else{
            cout << "into y: " << stoi(STRING) << endl;
            currentPt[1] = stoi(STRING);
        }

        i++;
        if(i % 2 == 0 && i != 0)
            pts.push_back(currentPt);
    }
    closed = true;
	infile.close();
	glutPostRedisplay();
}

void menuOptions(int op){
    switch(op){
    case 0:
        printf("Salvando...\n");
        save();
        break;
    case 1:
        printf("Carregando...\n");
        load("polygon-0x5219");
        //get_file_names();
        break;
    case 2:
        printf("Saindo...\n");
        break;
    }
    glutPostRedisplay();
}
void criaMenu(){
    int menu,submenu1,submenu2,submenu3;

    submenu3 = glutCreateMenu(menuDraw);
    glutAddMenuEntry("Criar novo Desenho",0);
    glutAddMenuEntry("Terminar Desenho",1);
    glutAddMenuEntry("Limpar tela",2);

    submenu1 = glutCreateMenu(menuOptions);
    glutAddMenuEntry("Salvar poligono",0);
    glutAddMenuEntry("Carregar poligono",1);
    glutAddMenuEntry("Sair",2);

    submenu2 = glutCreateMenu(menuCor);
    glutAddMenuEntry("Vermelho",0);
    glutAddMenuEntry("Verde",1);
    glutAddMenuEntry("Azul",2);
    glutAddMenuEntry("Branco",3);

    menu = glutCreateMenu(menuPrincipal);
    glutAddSubMenu("Desenhar",submenu3);
    glutAddSubMenu("Options",submenu1);
    glutAddSubMenu("Colors",submenu2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void draw_polygon(int button, int state, int x, int y){
    currentPt = std::array<int, 2>{x, vp_height-y};

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && draw == true)
        pts.push_back( currentPt );

    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
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

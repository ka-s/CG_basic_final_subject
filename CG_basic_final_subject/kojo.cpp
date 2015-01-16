#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#define KEY_ESC 27

double pi = 3.141592;
double moveY = 0.0;
double SnowMan_head = 10.0, SnowMan_body = -0.45;
double x = 0.0, y = 0.0;
double theta = 0.0;
double length = 0.0;
double t = 0.0;
double TranslateX = 0.0, TranslateY = 0.0, TranslateZ = 0.0;
double i, j;
float floor_x, floor_y;

//雪の表面属性
GLfloat diffuse_snow[] = { 1.0, 0.8, 0.8, 1.0 };
GLfloat specular_snow[] = { 0.3, 0.3, 0.3, 1.0 };
GLfloat ambient_snow[] = { 0.25, 0.2, 0.2, 1.0 };
GLfloat shininess_snow = 0.3;

//木の表面属性
float diffuse_wood[] = { 0.1, 0.35, 0.1, 1.0 };
float specular_wood[] = { 0.15, 0.25, 0.15, 1.0 };
float ambient_wood[] = { 0.0, 0.0, 0.0, 1.0 };
float shininess_wood = 0.0;

//雪だるま顔の表面属性
float diffuse_face[] = { 0.182, 0.17, 0.225, 1.0 };
float specular_face[] = { 0.33, 0.32, 0.34, 1.0 };
float ambient_face[] = { 0.05, 0.05, 0.066, 1.0 };
float shininess_face = 38.0;

/*-- 0番のライトのパラメータ値設定 --*/
GLfloat lightPos[] = { 5.0, 5.0, 2.0, 1.0 };
GLfloat lightDis[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightAmb[] = { 1.0, 1.0, 1.0, 1.0 };

//雪を作る
void MakeSnow(double NewTranslateX, double NewTranslateY, double NewTranslateZ) {
    glPushMatrix();

    glColor3d(1.0, 1.0, 1.0);
    glTranslated(NewTranslateX, NewTranslateY, NewTranslateZ);
    glutSolidSphere(0.05, 10, 10);

    glPopMatrix();
}

//キーボード
void myKeyboard(unsigned char key, int x, int y) {
    if (key == KEY_ESC)
        exit(0);

    switch (key) {
    case 'b':
        SnowMan_body = (SnowMan_body + 0.05);
        SnowMan_head = 10.0;
        //SnowMan_face;
        glutPostRedisplay();
        break;

    case 'h':
        SnowMan_head -= 9.0;
    }

}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();

    glPushMatrix();
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    //面の塗り方
    /* オブジェクトの表面属性を設定 */
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_snow);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_snow);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_snow);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess_snow);
    glScaled(3.5, 1.0, 3.0);
    //四角形
    glBegin(GL_QUADS);
    glNormal3d(0.0, 1.0, 1.0);
    glVertex3d(1.0, 0.0, 1.0);
    glVertex3d(1.0, 0.0, -1.0);
    glVertex3d(-1.0, 0.0, -1.0);
    glVertex3d(-1.0, 0.0, 1.0);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3d(0.0, 1.5, 1.0);
    glVertex3d(1.0, 1.5, 1.0);
    glVertex3d(1.0, 1.5, -1.0);
    glVertex3d(-1.0, 1.5, -1.0);
    glVertex3d(-1.0, 1.5, 1.0);
    glEnd();

    glDisable(GL_NORMALIZE);

    /*glPushMatrix();
    glScaled(1.0, 0.5, 1.0);
    glRotated(90.0, 1.0, 0.0, 0.0);
    glTranslated(1.0, 0.0, 0.0);
    glutSolidCube(1.0);
    glPopMatrix();*/

    //シェーディング処理終了
    glDisable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_LIGHTING);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_snow);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_snow);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_snow);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess_snow);
    TranslateX = length / 10;
    TranslateY = 3.0 + moveY*0.6;
    TranslateZ = 0.0;

    MakeSnow(TranslateX, TranslateY, TranslateZ);
    MakeSnow(TranslateX + 0.3, TranslateY + 0.1, TranslateZ + 0.5);
    MakeSnow(TranslateX + 1.0, TranslateY + 0.5, TranslateZ - 1.0);
    MakeSnow(TranslateX - 0.7, TranslateY + 0.0, TranslateZ + 1.0);
    MakeSnow(TranslateX - 0.7, TranslateY + 1.0, TranslateZ + 1.0);
    MakeSnow(TranslateX - 0.2, TranslateY + 0.6, TranslateZ + 0.7);
    MakeSnow(TranslateX - 0.2, TranslateY + 0.4, TranslateZ - 0.6);
    MakeSnow(TranslateX + 0.9, TranslateY + 0.3, TranslateZ - 0.5);
    MakeSnow(TranslateX + 0.7, TranslateY + 1.0, TranslateZ - 0.3);
    glPopMatrix();

    //雪だるまの体
    glPushMatrix();
    //SnowMan_body = -0.45;
    glTranslated(0.0, SnowMan_body, 0.0);
    glutSolidSphere(0.5, 20, 20);
    if (SnowMan_body >= 0.40)
        SnowMan_body = -0.45;
    glPopMatrix();

    //雪だるまの頭
    glPushMatrix();
    glTranslated(0.0, SnowMan_head, 0.0);
    glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    //雪だるまの顔
    glPushMatrix();
    if (SnowMan_head == 1.0 && SnowMan_body >= 0.22 && SnowMan_body <= 0.26) {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_face);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_face);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_face);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess_face);
        //glColor3d(1.0, 0.0, 0.0);
        //glScaled(0.7, 0.7, 0.7);
        glPushMatrix();
        glTranslated(0.15, 1.1, 0.3);
        glutSolidSphere(0.07, 10, 10);
        glTranslated(-0.3, 0.0, 0.0);
        glutSolidSphere(0.07, 10, 10);
        glPopMatrix();

        glPushMatrix();
        glScaled(2.0, 0.5, 1.0);
        glTranslated(0.0, 1.75, 0.35);
        glutSolidCube(0.1);
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_LIGHTING);
    //木の幹
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_wood);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_wood);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_wood);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess_wood);
    glTranslated(1.7, 0.0, -1.5);
    glScaled(0.5, 4.0, 0.5);
    glutSolidSphere(0.5, 20, 20);
    glTranslated(-6.8, 0.0, 0.0);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
    //葉の部分
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_snow);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_snow);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_snow);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess_snow);
    glTranslated(1.7, 0.7, -1.5);
    glRotated(-90, 1.0, 0.0, 0.0);
    glutSolidCone(0.8, 0.8, 10, 10);
    glTranslated(0.0, 0.0, 0.5);
    glutSolidCone(0.6, 0.9, 10, 10);
    glTranslated(-3.4, 0.0, 0.0);
    glutSolidCone(0.6, 0.9, 10, 10);
    glTranslated(0.0, 0.0, -0.5);
    glutSolidCone(0.8, 0.8, 10, 10);
    glPopMatrix();


    glPopMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glutSwapBuffers();
}

//idle関数
void idle() {
    moveY = fmod(moveY - 0.001, -7.0);
    //SnowMan_head = fmod(SnowMan_head - 0.00005, 1.0);
    /*if(SnowMan_head >= 0.0)
    SnowMan_head = fmod(SnowMan_head - 0.0000000001, 1.2);*/
    theta = fmod(theta + 0.1, 360.0);
    t = theta*pi / 180;
    length = sin(t) * 1;
    glutPostRedisplay();
}

void myInit(char *progname) {
    int width = 500, height = 500;
    double aspect = (double)width / (double)height;

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(progname);
    glClearColor(0.1, 0.5, 0.5, 1.0);

    glutKeyboardFunc(myKeyboard);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 20.0);

    //視点設定
    gluLookAt(0.0, 1.5, 3.0, 0.0, 0.0, -10.0, 0.0, 1.0, 0.0);
    /*-- 0番ライトの設定を記述--*/
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDis);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glEnable(GL_LIGHT0);
    //0番ライト
    glEnable(GL_LIGHT1);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    myInit(argv[0]);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
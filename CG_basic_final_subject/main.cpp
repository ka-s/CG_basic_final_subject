/// ================================
///  CG��b�ŏI�ۑ� 
/// ================================

// @date    2015-01-16
// @author  Kazuma Ito

#include <iostream>
#include <memory>
#include <GL/glut.h>

using namespace std;

// --------------------------------
//  �萔
// --------------------------------

// �E�B���h�E�T�C�Y
const int WINDOW_SIZE_X = 640;
const int WINDOW_SIZE_Y = 480;

// --------------------------------
//  �\����
// --------------------------------

// ���W�\����
struct Pos{
    float x;
    float y;
    float z;
};

// --------------------------------
//  �O���[�o���ϐ�
// --------------------------------

// �������O���
namespace{
    // �Ⴞ��܂̍��W
    Pos pos_snow_man;
    // ���̍��W
    Pos pos_floor;
}

// --------------------------------
//  �֐��錾
// --------------------------------

// �Ⴞ��ܕ`��֐�
void draw_snow_man(Pos pos);
// �n�ʕ`��֐�
void draw_floor(Pos pos);

// �L�[�{�[�h�����֐�
void my_keyboard(unsigned char key, int x, int y);
// ���V�F�[�v�֐�
void my_reshape(int width, int height);
// �`��֐�
void my_display();
// �X�V�֐�
void my_idle();

// ���W�������֐�
void pos_init();
// �����������֐�
void light_init();
// �������֐�
void my_init();

// ================================
//  �Ⴞ��ܕ`��֐�
// ================================
void draw_snow_man(Pos pos){
    // �Ⴞ��܂�`��
    glPushMatrix();
    //   ��
    glColor3f(1.0f, 1.0f, 1.0f);
    //   ���W
    glTranslatef(pos.x, pos.y, pos.z);
    //   ��
    glutSolidSphere(1.0, 32, 32);
    glPushMatrix();
    //   ������
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.75, 32, 32);
    glPopMatrix();
    glPushMatrix();
    //   ����
    glColor3f(0.5f, 0.5f, 0.5f);
    glTranslatef(0.0f, -1.0f, 0.0f);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
}

// ================================
//  �n�ʕ`��֐�
// ================================
void draw_floor(Pos pos){
    // �n�ʂ̑傫��
    static float floor_size = 2.0;

    // ��
    glColor3f(1.0f, 0.0f, 0.0f);
    // �����`��`��
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(pos.x, pos.y, pos.z);
    glVertex3f(pos.x + floor_size, pos.y, pos.z);
    glVertex3f(pos.x + floor_size, pos.y, pos.z + floor_size);
    glVertex3f(pos.x, pos.y, pos.z + floor_size);
    glEnd();
}

// ================================
//  �L�[�{�[�h�����֐�
// ================================
void my_keyboard(unsigned char key, int x, int y){
    // ESC�L�[�ŏI��
    if (key == 27) exit(0);

}

// ================================
//  ���V�F�[�v�֐�
// ================================
void my_reshape(int width, int height){

}

// ================================
//  �`��֐�
// ================================
void my_display(){
    // �����̍��W
    static float light_pos[] = { 0.0f, 10.0f, 0.0f };

    // ��ʃN���A
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Z�o�b�t�@�L����
    glEnable(GL_DEPTH_TEST);
    // �e�N�X�`���L����
    glEnable(GL_TEXTURE_2D);

    // �`�`�`�`�����ɏ����������`�`�`�`
    // ������ݒu
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    // �Ⴞ��܂�`��
    draw_snow_man(pos_snow_man);
    // �n�ʂ�`��
    draw_floor(pos_floor);

    //// test
    //// �n�ʂ̑傫��
    //static float floor_size = 1.0;
    //// ��
    //glColor3f(1.0f, 0.0f, 0.0f);
    //// �����`��`��
    //glBegin(GL_POLYGON);
    //glNormal3f(0.0f, 1.0f, 0.0f);
    //glVertex3f(-1.0, -1.0, -1.0);
    //glVertex3f(1.0, -1.0, -1.0);
    //glVertex3f(1.0, -1.0, 1.0);
    //glVertex3f(-1.0, -1.0, 1.0);
    //glEnd();

    // �e�N�X�`��������
    glDisable(GL_TEXTURE_2D);
    // Z�o�b�t�@������
    glDisable(GL_DEPTH_TEST);
    // �ŏI�`�揈��
    glFlush();

}

// ================================
//  �X�V�֐�
// ================================
void my_idle(){
    // �ĕ`��
    glutPostRedisplay();

}

// ================================
//  ���W�������֐�
// ================================
void pos_init(){
    // �Ⴞ��܂̍��W������
    pos_snow_man = { 0.0f, 0.0f, 0.0f };
    // �n�ʂ̍��W������
    pos_floor = { -1.0f, -1.0f, -1.0f };
}

// ================================
//  �����������֐�
// ================================
void light_init(){
    // ���������̐F
    float diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    // �e���������̐ݒ�
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    // ����1��L����
    glEnable(GL_LIGHT0);
}

// ================================
//  �������֐�
// ================================
void my_init(){
    // �A�X�y�N�g
    static float aspect = (float)WINDOW_SIZE_X / (float)WINDOW_SIZE_Y;

    // �`�惂�[�h
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    // �E�B���h�E�T�C�Y�ݒ�
    glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    // �E�B���h�E�o�����W�ݒ�
    glutInitWindowPosition(100, 100);
    // �E�B���h�E�^�C�g��
    glutCreateWindow("Final_Subject");
    // �w�i�F�ݒ�
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    // ���炩�ȃ��C�e�B���O�ɐݒ�
    glShadeModel(GL_SMOOTH);
    // Z�o�b�t�@�g�p
    glEnable(GL_DEPTH_TEST);
    // �摜�f�[�^�p�b�N�̎g�p
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // �`��̈�̐ݒ�
    glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

    // �������e�̐ݒ�
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 32.0);
    glMatrixMode(GL_MODELVIEW);

    // �J�����̐ݒ�
    gluLookAt(
        0.0, 2.0, 4.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // ���W������
    pos_init();
    // ����������
    light_init();

}

// ================================
//  ���C���֐�
// ================================
int main(int argc, char** argv){
    // ������
    glutInit(&argc, argv);
    my_init();

    // �֐�
    glutKeyboardFunc(my_keyboard);
    glutReshapeFunc(my_reshape);
    glutDisplayFunc(my_display);
    glutIdleFunc(my_idle);

    // ���C�����[�v
    glutMainLoop();

    return 0;
}

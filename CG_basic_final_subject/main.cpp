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
    // �Ⴞ��܂̈ړ����x
    float move_speed_snow_man = 0.5;
    // �Ⴞ��܂̑O�i���x
    float front_speed_snow_man = 0.01;

    // �J�����̍��W
    Pos pos_camera;
    // �Ⴞ��܂̍��W
    Pos pos_snow_man;
    // ���̍��W
    Pos pos_floor;
    // �؂̍��W
    Pos pos_tree;

    // �}�e���A��
    //   �^��̃}�e���A��
    float pearl_diffuse[] = { 1, 0.829, 0.829, 1.0 };
    float pearl_specular[] = { 0.296648, 0.296648, 0.296648, 1.0 };
    float pearl_ambient[] = { 0.25, 0.20725, 0.20725, 1.0 };
    float pearl_shininess = 10.24;

    //   �΂̃v���X�`�b�N�̃}�e���A��
    float green_plastic_diffuse[] = { 0.1, 0.35, 0.1, 1.0 };
    float green_plastic_specular[] = { 0.45, 0.55, 0.45, 1.0 };
    float green_plastic_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    float green_plastic_shininess = 32;

}

// --------------------------------
//  �֐��錾
// --------------------------------

// �Ⴞ��ܕ`��֐�
void draw_snow_man(Pos pos);
// �n�ʕ`��֐�
void draw_floor(Pos pos);
// �ؕ`��֐�
void draw_tree(Pos pos);

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
    glPushMatrix();

    //   ���W
    glTranslatef(pos.x, pos.y, pos.z);
    // �}�e���A��
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, pearl_diffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, pearl_specular);
    //glMaterialfv(GL_FRONT, GL_AMBIENT, pearl_ambient);
    //glMaterialf(GL_FRONT, GL_SHININESS, pearl_shininess);
    glColor3f(1.0f, 1.0f, 1.0f);
    //   ��
    glutSolidSphere(1.0, 32, 32);

    //   ������
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.75, 32, 32);
    glPopMatrix();

    //   ����
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);
    glScalef(1.0f, 1.0f, 4.0f);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, green_plastic_diffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, green_plastic_specular);
    //glMaterialfv(GL_FRONT, GL_AMBIENT, green_plastic_ambient);
    //glMaterialf(GL_FRONT, GL_SHININESS, green_plastic_shininess);
    glColor3f(0.0f, 0.5f, 0.0f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

}

// ================================
//  �n�ʕ`��֐�
// ================================
void draw_floor(Pos pos){
    // �n�ʂ̑傫��(���a)
    static float floor_size = 64.0;

    // ��
    glColor3f(0.5f, 0.5f, 0.5f);
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
//  �ؕ`��֐�
// ================================
void draw_tree(Pos pos){
    glPushMatrix();

    // ���W
    glTranslatef(pos.x, pos.y + 1.0, pos.z);

    glPushMatrix();
    glScalef(1.0f, 4.0f, 1.0f);
    // ��
    glColor3f(0.0f, 0.0f, 1.0f);
    // �؂̊�
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(0.0f, 0.5f, 0.0f);

    //   �t����1�i��
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glutSolidCone(2.0, 2.0, 32, 32);
    glPopMatrix();

    //   �t����2�i��
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glutSolidCone(1.5, 2.0, 32, 32);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();

}

// ================================
//  �L�[�{�[�h�����֐�
// ================================
void my_keyboard(unsigned char key, int x, int y){
    // ESC�L�[�ŏI��
    if (key == 27) exit(0);

    // a,d�L�[
    if (key == 'a') pos_snow_man.x -= move_speed_snow_man;
    if (key == 'd') pos_snow_man.x += move_speed_snow_man;

    // �ĕ`��
    glutPostRedisplay();

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
    // �s��̏�����
    glLoadIdentity();

    // �`�`�`�`�����ɏ����������`�`�`�`
    // �J�����̐ݒ�
    gluLookAt(
        pos_snow_man.x, pos_camera.y, pos_camera.z,
        pos_snow_man.x, pos_snow_man.y, pos_snow_man.z,
        0.0, 1.0, 0.0);

    // ������ݒu
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    // �Ⴞ��܂�`��
    draw_snow_man(pos_snow_man);
    // �n�ʂ�`��
    draw_floor(pos_floor);
    // �؂�`��
    draw_tree(pos_tree);

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
    //�Ⴞ��܂̑O�i
    pos_snow_man.z -= front_speed_snow_man;
    // �J�����̑O�i
    pos_camera.z -= front_speed_snow_man;

    // �ĕ`��
    glutPostRedisplay();

}

// ================================
//  ���W�������֐�
// ================================
void pos_init(){
    // �J�����̍��W������
    pos_camera = { 0.0f, 4.0f, 16.0f };
    // �Ⴞ��܂̍��W������
    pos_snow_man = { 0.0f, 1.0f, 0.0f };
    // �n�ʂ̍��W������
    pos_floor = { -32.0f, 0.0f, -32.0f };
    // �؂̍��W������
    pos_tree = { 4.0f, 0.0f, 0.0f };

}

// ================================
//  �����������֐�
// ================================
void light_init(){
    // ���������̐F
    float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    // �e���������̐ݒ�
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    // ����0��L����
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
    glLoadIdentity();

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

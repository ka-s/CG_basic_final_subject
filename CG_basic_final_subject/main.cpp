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

// ���̍ő吶����
const int MAX_FLOORS = 3;
// �؂̍ő吶����
const int MAX_TREE = 16;

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
    float move_speed_snow_man = 0.5f;
    // �Ⴞ��܂̑O�i���x
    float front_speed_snow_man = 0.101f;
    // �؂̍ő�Ԋu
    float tree_space = 8.0f;

    // �J�����̍��W
    Pos pos_camera;
    // �Ⴞ��܂̍��W
    Pos pos_snow_man;
    // ���̍��W
    Pos pos_floor[MAX_FLOORS];
    // �؂̍��W
    Pos pos_tree[MAX_TREE];

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
//  �n�ʕ`��N���X
// ================================
class Floor{
private:
    float x, y, z;
    float player_pos;

public:
    // �R���X�g���N�^
    Floor(Pos pos){
        x = pos.x;
        y = pos.y;
        z = pos.z;
    }

    // �`�惁�\�b�h
    void draw(float player_pos){
        // �n�ʂ̑傫��(���a)
        static float floor_size = 64.0;

        // ��
        glColor3f(0.5f, 0.5f, 0.5f);
        // �����`��`��
        glBegin(GL_POLYGON);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(x, y, z);
        glVertex3f(x + floor_size, y, z);
        glVertex3f(x + floor_size, y, z + floor_size);
        glVertex3f(x, y, z + floor_size);
        glEnd();

        // �n�ʃ��[�v����
        if (player_pos < z){
            z -= 64;
        }
    }

};
shared_ptr<Floor> floors[MAX_FLOORS];

// ================================
//  �ؕ`��N���X
// ================================
class Tree{
private:
    float x, y, z;

public:
    // �R���X�g���N�^
    Tree(Pos pos){
        x = pos.x;
        y = pos.y;
        z = pos.z;
    }

    // �`�惁�\�b�h
    void draw(float player_pos){
        glPushMatrix();

        // ���W
        glTranslatef(x, y + 1.0, z);

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

        // �؃��[�v����
        if (player_pos < z - 16.0f){
            z -= 8 * (MAX_TREE / 2);
        }
    }

};
shared_ptr<Tree> trees[MAX_TREE];

// ================================
//  �L�[�{�[�h�����֐�
// ================================
void my_keyboard(unsigned char key, int x, int y){
    // ESC�L�[�ŏI��
    if (key == 27) exit(0);

    // ���E�ړ�
    if (key == 'a') pos_snow_man.x -= move_speed_snow_man;
    if (key == 'd') pos_snow_man.x += move_speed_snow_man;

    // ��������
    if (key == 'w') front_speed_snow_man += 0.02;
    if (key == 's') front_speed_snow_man -= 0.02;
    // �X�s�[�h����
    if (front_speed_snow_man < 0) front_speed_snow_man = 0.001;

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
    for (int i = 0; i < MAX_FLOORS; ++i){
        floors[i]->draw(pos_snow_man.z);
    }

    // �؂�`��
    //draw_tree(pos_tree);
    for (int i = 0; i < MAX_TREE; ++i){
        trees[i]->draw(pos_snow_man.z);
    }

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
    for (int i = 0; i < MAX_FLOORS; ++i){
        pos_floor[i] = { -32.0f, 0.0f, -32.0f * i };
    }

    // �؂̍��W������
    //   �E��
    for (int i = 0; i < MAX_TREE / 2; ++i){
        pos_tree[i] = { tree_space, 0.0f, -8.0f * i };
    }
    //   ����
    for (int i = MAX_TREE / 2; i < MAX_TREE; ++i){
        pos_tree[i] = { -tree_space, 0.0f, -8.0f * (i - MAX_TREE / 2) };
    }

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
    gluPerspective(60.0, aspect, 0.1, 64.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ���W������
    pos_init();
    // ����������
    light_init();

    // ���N���X�̐���
    for (int i = 0; i < MAX_FLOORS; ++i){
        floors[i] = make_shared<Floor>(pos_floor[i]);
    }

    // �؃N���X�̐���
    for (int i = 0; i < MAX_TREE; ++i){
        trees[i] = make_shared<Tree>(pos_tree[i]);
    }

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

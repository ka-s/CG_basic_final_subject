/// ================================
///  CG��b�ŏI�ۑ� 
/// ================================

// @date    2015-01-16
// @author  Kazuma Ito

#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <Windows.h>
#include <GL/glut.h>

using namespace std;

// --------------------------------
//  �萔
// --------------------------------

// �~����
const float PI = 3.14159265359f;

// �t���[�����[�g
const int FPS = 60;

// �E�B���h�E�T�C�Y
const int WINDOW_SIZE_X = 640;
const int WINDOW_SIZE_Y = 480;

// ���Q�[�����ɉe����^����ϐ�
//      �Ⴞ��܂̑傫��
const double snow_man_size = 1.0;
//      �Ⴞ��܂̍��E�ړ����x
const float move_speed_snow_man = 1.0f;
//      �؂̑傫��
const double tree_size = 2.0;
//      ���E�̖؂̍ő�Ԋu
const float tree_space = 16.0f;
//      �����_���؂̑O��Ԋu
const float random_tree_space = 8.0f;

// ���̍ő吶����
const int MAX_FLOORS = 3;
// �؂̍ő吶����
const int MAX_TREE = 16;
// �����_���ɏo������؂̍ő吶����
const int MAX_RANDOM_TREE = 32;

// �}�e���A��
//   �^��̃}�e���A��
const float pearl_diffuse[] = { 1, 0.829, 0.829, 1.0 };
const float pearl_specular[] = { 0.296648, 0.296648, 0.296648, 1.0 };
const float pearl_ambient[] = { 0.25, 0.20725, 0.20725, 1.0 };
const float pearl_shininess = 10.24;

//   �΂̃v���X�`�b�N�̃}�e���A��
const float green_plastic_diffuse[] = { 0.1, 0.35, 0.1, 1.0 };
const float green_plastic_specular[] = { 0.45, 0.55, 0.45, 1.0 };
const float green_plastic_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
const float green_plastic_shininess = 32;

// ���̃}�e���A��
const float copper_ambient[] = { 0.19125, 0.0735, 0.0225, 1.0 };
const float copper_diffuse[] = { 0.7038, 0.27048, 0.0828, 1.0 };
const float copper_specular[] = { 0.256777, 0.137622, 0.086014, 1.0 };
const float copper_shininess = 12.8;

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
    // �����_���f�o�C�X
    random_device rd;
    // �����Z���k�c�C�X�^�[
    mt19937 my_mersenne_twister(rd());
    // ��l���z�ȃ����_��
    uniform_int_distribution<int> mt_random((int)-tree_space, (int)tree_space);

    // �v���O���������s����Ă���̍X�V��
    int global_time = 0;

    // �J�����̍��W
    Pos pos_camera;
    // �Ⴞ��܂̍��W
    Pos pos_snow_man;
    // ���̍��W
    Pos pos_floor[MAX_FLOORS];
    // �؂̍��W
    Pos pos_tree[MAX_TREE];
    // �����_�������̖؂̍��W
    Pos pos_random_tree[MAX_RANDOM_TREE];

    // �Ⴞ��܂̏����O�i���x
    float front_speed_snow_man = 1.0f;

    // �����蔻��t���O
    bool f_hit = false;
    // ���Z�b�g�t���O
    bool f_reset = false;

}

// --------------------------------
//  �֐��錾
// --------------------------------

// RAW�ǂݍ��݊֐�
void readRAWImage(char* filename, unsigned char image[128][128][3]);

// �����_���؂̃����_���ȍ��W�𐶐�����֐�
float get_tree_random_pos();
// �����蔻��֐�
bool is_hit();

// �L�[�{�[�h�����֐�
void my_keyboard(unsigned char key, int x, int y);
// ���V�F�[�v�֐�
void my_reshape(int width, int height);
// �`��֐�
void my_display();
// �X�V�֐�
void my_idle();

// �Q�[�����Z�b�g�֐�
void reset();
// ���W�������֐�
void pos_init();
// �����������֐�
void light_init();
// �������֐�
void my_init();

// ################################
//  �t���[�����[�g����N���X
//      �������Ȃ������߂Ɏ���
//      (����ƌ����Ă����������X�V���~�߂邾��)
// ################################
class FPSManager{
private:
    // ���݂�FPS
    float now_fps;
    // �X�V��
    int frame_count;
    // ���݂̃~���b
    int start_clock;

public:
    // �R���X�g���N�^
    FPSManager(){
        now_fps = 0.0f;
        frame_count = 0;
        start_clock = 0;
    }

    // �X�V���\�b�h
    void update(){
        // ��ԍŏ��̃t���[��
        if (frame_count == 0){
            start_clock = (int)clock();
        }
        // �X�V�񐔂��K��񐔒B������
        if (frame_count == FPS){
            int hoge = (int)clock();
            // ���݂�FPS���v�Z
            now_fps = 1000.0f / (((float)hoge - (float)start_clock) / (float)(FPS));
            frame_count = 0;
            start_clock = hoge;
        }
        frame_count++;
    }

    // ���݂�FPS��`�悷�郁�\�b�h
    void draw(){
        cout << now_fps << endl;
    }

    // �X�V���䃁�\�b�h
    void wait(){
        // �K��񐔍X�V�ɂ�����������
        int spend_time = (int)clock() - start_clock;
        // ������ׂ�����
        int wait_time = frame_count * 1000 / FPS - spend_time;

        // �X�V���~
        if (wait_time > 0){
            Sleep(wait_time);
        }
    }

};
FPSManager* fps_manager;

// ################################
//  �Ⴞ��ܕ`��N���X
// ################################
class SnowMan{
private:
    float x, z, y;
    float effect_long;
    float effect_count;

public:
    // �R���X�g���N�^
    SnowMan(Pos pos){
        x = pos.x;
        y = pos.y;
        z = pos.z;

        effect_long = 0;
        effect_count = 0;
    }

    // �`�惁�\�b�h
    void draw(Pos pos){
        x = pos.x;
        y = pos.y;
        z = pos.z;

        glPushMatrix();

        // ���W
        glTranslatef(x, y, z);
        // �^��
        glMaterialfv(GL_FRONT, GL_DIFFUSE, pearl_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, pearl_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, pearl_ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, pearl_shininess);
        // ��
        glutSolidSphere(snow_man_size, 32, 32);

        //   ������
        glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f);
        glutSolidSphere(0.75, 32, 32);
        glPopMatrix();

        //   ����
        glPushMatrix();
        glTranslatef(0.0f, -1.0f, 0.0f);
        glScalef(1.0f, 1.0f, 4.0f);
        //   �΂̃v���X�`�b�N
        glMaterialfv(GL_FRONT, GL_DIFFUSE, green_plastic_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, green_plastic_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, green_plastic_ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, green_plastic_shininess);
        glutSolidCube(1.0);
        glPopMatrix();

        glPopMatrix();
    }

    // �j��G�t�F�N�g�`�惁�\�b�h
    void draw_effect(){

        for (int i = 0; i < 16; ++i){
            glPushMatrix();

            // ���W
            glTranslatef(x + cos(360.0f / 16.0f * (float)i) * effect_long, y + sin(360.0f / 16.0f * (float)i) * effect_long, z);
            // �^��
            glMaterialfv(GL_FRONT, GL_DIFFUSE, pearl_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, pearl_specular);
            glMaterialfv(GL_FRONT, GL_AMBIENT, pearl_ambient);
            glMaterialf(GL_FRONT, GL_SHININESS, pearl_shininess);
            // ��
            glutSolidSphere(0.4f, 32, 32);

            glPopMatrix();
        }
        if (effect_count <= 60){
            effect_long = sin(PI / 2 / 60 * effect_count) * 6.0f;
        }
        else{
            effect_count = 0;
            f_reset = true;
        }
        effect_count++;

    }

};
SnowMan* snow_man;

// ################################
//  �n�ʕ`��N���X
// ################################
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

        // �^��
        glMaterialfv(GL_FRONT, GL_DIFFUSE, pearl_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, pearl_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, pearl_ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, pearl_shininess);
        // �����`��`��
        glBegin(GL_POLYGON);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(x, y, z);
        glVertex3f(x + floor_size, y, z);
        glVertex3f(x + floor_size, y, z + floor_size);
        glVertex3f(x, y, z + floor_size);
        glEnd();

        // �n�ʃ��[�v����
        if (player_pos < z - 16.0f){
            z -= floor_size*3;
        }
    }

};
Floor* floors[MAX_FLOORS];

// ################################
//  �ؕ`��N���X
// ################################
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
    void draw(float player_pos, float loop_space){
        glPushMatrix();

        // ���W
        glTranslatef(x, y + 1.0, z);

        glPushMatrix();
        glScalef(1.0f, 4.0f, 1.0f);
        // ��
        glMaterialfv(GL_FRONT, GL_DIFFUSE, copper_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, copper_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, copper_ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, copper_shininess);
        // �؂̊�
        glutSolidCube(1.0);
        glPopMatrix();

        glTranslatef(0.0f, 0.5f, 0.0f);

        //   �t����1�i��
        glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        //   �΂̃v���X�`�b�N
        glMaterialfv(GL_FRONT, GL_DIFFUSE, green_plastic_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, green_plastic_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, green_plastic_ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, green_plastic_shininess);
        glutSolidCone(tree_size, 2.0, 32, 32);
        glPopMatrix();

        //   �t����2�i��
        glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glutSolidCone(tree_size - 0.5, 2.0, 32, 32);
        glPopMatrix();

        glPopMatrix();

        glPopMatrix();

        // �؃��[�v����
        if (player_pos < z - 16.0f){
            // �����_���؂Ȃ�Δz�u���Ȃ���
            if (loop_space == random_tree_space){
                z -= loop_space * MAX_RANDOM_TREE;
                x = get_tree_random_pos();
            }
            else{
                z -= loop_space / 2 * (MAX_TREE / 2);
            }
        }
    }

    // ���W��Ԃ����\�b�h
    Pos get_pos(){
        return{ x, y, z };
    }

};
// ���E�̖�
Tree* trees[MAX_TREE];
// �����_���ɏo�������
Tree* random_trees[MAX_RANDOM_TREE];

// ================================
//  �����_���؂̃����_���ȍ��W�𐶐�����֐�
// ================================
float get_tree_random_pos(){

    //return (float)(rand() % ((int)tree_space * 2)) + 1.0f - tree_space;
    return (float)mt_random(my_mersenne_twister);

}

// ================================
//  �����蔻��֐�
// ================================
bool is_hit(){

    // ���E�̓����蔻��
    if (pos_snow_man.x - snow_man_size < -tree_space + tree_size || 
        pos_snow_man.x + snow_man_size > tree_space - tree_size){
        return true;
    }
    // �e�؂Ƃ̓����蔻��
    for (int i = 0; i < MAX_RANDOM_TREE; ++i){
        if (
            (pos_snow_man.x - random_trees[i]->get_pos().x)*(pos_snow_man.x - random_trees[i]->get_pos().x) +
            (pos_snow_man.z - random_trees[i]->get_pos().z) * (pos_snow_man.z - random_trees[i]->get_pos().z) <
            (snow_man_size + tree_size) + (snow_man_size + tree_size)
            )
            return true;
    }

    return false;
}

// ================================
//  �L�[�{�[�h�����֐�
// ================================
void my_keyboard(unsigned char key, int x, int y){

    // ESC�L�[�ŏI��
    if (key == 27) exit(0);

    // �؂ɂ������ĂȂ����΍��E�ړ��\
    if (f_hit != true){
        // ���E�ړ�
        if (key == 'a') pos_snow_man.x -= move_speed_snow_man;
        if (key == 'd') pos_snow_man.x += move_speed_snow_man;
    }

    // ��������
    //if (key == 'w') front_speed_snow_man += 0.02;
    //if (key == 's') front_speed_snow_man -= 0.02;
    // �X�s�[�h����
    //if (front_speed_snow_man > 10.0f) front_speed_snow_man = 2.0f;
    //if (front_speed_snow_man < 0.0f) front_speed_snow_man = 0.001f;

    // ��蒼��
    if (key == 'r'){
        reset();
    }

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

    // ��ʃN���A
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Z�o�b�t�@�L����
    glEnable(GL_DEPTH_TEST);
    // �����L����
    glEnable(GL_LIGHTING);
    // �s��̏�����
    glLoadIdentity();

    // �J�����̐ݒ�
    gluLookAt(
        pos_snow_man.x, pos_camera.y, pos_camera.z,
        pos_snow_man.x, pos_snow_man.y, pos_snow_man.z,
        0.0, 1.0, 0.0);

    // �Ⴞ��ܔj��G�t�F�N�g�`��
    if (f_hit){
        snow_man->draw_effect();
    }
    else{
        // �Ⴞ��܂�`��
        snow_man->draw(pos_snow_man);
    }

    // �n�ʂ�`��
    for (int i = 0; i < MAX_FLOORS; ++i){
        floors[i]->draw(pos_snow_man.z);
    }

    // �؂�`��
    for (int i = 0; i < MAX_TREE; ++i){
        trees[i]->draw(pos_snow_man.z, tree_space);
    }

    // �����_���Ȗ؂�`��
    for (int i = 0; i < MAX_RANDOM_TREE; ++i){
        random_trees[i]->draw(pos_snow_man.z, random_tree_space);
    }

    // ����������
    glDisable(GL_LIGHTING);
    // Z�o�b�t�@������
    glDisable(GL_DEPTH_TEST);
    // �ŏI�`�揈��
    glFlush();

}

// ================================
//  �X�V�֐�
// ================================
void my_idle(){
    // �t���[�����[�g����f�[�^�X�V
    fps_manager->update();
    // ���݂�FPS���R���\�[���ɕ`��
    fps_manager->draw();

    // �����蔻��
    if (is_hit()){
        f_hit = true;
    }
    else{
        //�Ⴞ��܂̑O�i
        pos_snow_man.z -= front_speed_snow_man;
        // �J�����̑O�i
        pos_camera.z -= front_speed_snow_man;
    }
    // ���Z�b�g����
    if (f_reset){
        reset();
    }

    // �ĕ`��
    glutPostRedisplay();
    global_time++;

    // �t���[�����[�g����
    fps_manager->wait();

}

// ================================
//  �Q�[�����Z�b�g�֐�
// ================================
void reset(){

    // ���ׂẴI�u�W�F�N�g���폜
    delete snow_man;
    for (int i = 0; i < MAX_FLOORS; ++i){
        delete floors[i];
    }
    for (int i = 0; i < MAX_TREE; ++i){
        delete trees[i];
    }
    for (int i = 0; i < MAX_RANDOM_TREE; ++i){
        delete random_trees[i];
    }

    // ���W��������
    pos_init();

    // ���ׂẴI�u�W�F�N�g�𐶐�������
    snow_man = new SnowMan(pos_snow_man);
    for (int i = 0; i < MAX_FLOORS; ++i){
        floors[i] = new Floor(pos_floor[i]);
    }
    for (int i = 0; i < MAX_TREE; ++i){
        trees[i] = new Tree(pos_tree[i]);
    }
    for (int i = 0; i < MAX_RANDOM_TREE; ++i){
        random_trees[i] = new Tree(pos_random_tree[i]);
    }

    // ���x��������
    front_speed_snow_man = 1.0f;

    // �t���O������
    f_hit = false;
    f_reset = false;

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
        pos_floor[i] = { -32.0f, 0.0f, -64.0f * i };
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

    // �����_�����������؂̍��W������
    for (int i = 0; i < MAX_RANDOM_TREE; ++i){
        pos_random_tree[i] = { 
            get_tree_random_pos(), 
            0.0f, 
            -random_tree_space * i - 64.0f};
    }

}

// ================================
//  �����������֐�
// ================================
void light_init(){

    // ���������̐F
    float light_pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    // �e���������̐ݒ�
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    // ���炩�ȃ��C�e�B���O�ɐݒ�
    glShadeModel(GL_SMOOTH);
    // Z�o�b�t�@�g�p
    glEnable(GL_DEPTH_TEST);
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

    // FPS����N���X�̐���
    fps_manager = new FPSManager();

    // �Ⴞ��܂̐���
    snow_man = new SnowMan(pos_snow_man);

    // ���N���X�̐���
    for (int i = 0; i < MAX_FLOORS; ++i){
        floors[i] = new Floor(pos_floor[i]);
    }

    // �؃N���X�̐���
    for (int i = 0; i < MAX_TREE; ++i){
        trees[i] = new Tree(pos_tree[i]);
    }

    // �����_���؃N���X�̐���
    for (int i = 0; i < MAX_RANDOM_TREE; ++i){
        random_trees[i] = new Tree(pos_random_tree[i]);
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

    // ���������
    delete fps_manager;
    delete snow_man;
    for (int i = 0; i < MAX_FLOORS; ++i){
        delete floors[i];
    }
    for (int i = 0; i < MAX_TREE; ++i){
        delete trees[i];
    }
    for (int i = 0; i < MAX_RANDOM_TREE; ++i){
        delete random_trees[i];
    }

    return 0;
}

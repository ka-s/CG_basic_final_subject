/// ================================
///  CG��b�ŏI�ۑ� 
/// ================================

// @date    2015-01-??
// @author  Kazuma Ito

#include <iostream>
#include <GL/glut.h>

using namespace std;

// ---------------- �O���[�o���ϐ� ----------------

namespace{
    // �E�B���h�E�T�C�Y
    const int WINDOW_SIZE_X = 640;
    const int WINDOW_SIZE_Y = 480;

    // �E�B���h�E�o�����W
    const int WINDOW_APPER_X = 100;
    const int WINDOW_APPER_Y = 100;

    // �w�i�F
    const float BACKGROUND_R = 1.0f;
    const float BACKGROUND_G = 0.0f;
    const float BACKGROUND_B = 1.0f;
}

// -----------------------------------------------

// ---------------- �֐��錾 ----------------

// �������֐�
void my_init(char *progname);
// �L�[�{�[�h�����֐�
void my_keyboard(unsigned char key, int x, int y);
// ���V�F�[�v�֐�
void my_reshape(int width, int height);
// �`��֐�
void my_display();

// ------------------------------------------

//
// �������֐�
//
void my_init(char *progname){
    // �`�惂�[�h
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    // �E�B���h�E�T�C�Y�ݒ�
    glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    // �E�B���h�E�o�����W�ݒ�
    glutInitWindowPosition(WINDOW_APPER_X, WINDOW_APPER_Y);
    glutCreateWindow(progname);
    // �w�i�F�ݒ�
    glClearColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, 0.0f);
    // ���炩�ȃ��C�e�B���O�ɐݒ�
    glShadeModel(GL_SMOOTH);
    // Z�o�b�t�@�g�p
    glEnable(GL_DEPTH_TEST);
    // �摜�f�[�^�p�b�N�̎g�p
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

//
// �L�[�{�[�h�����֐�
//
void my_keyboard(unsigned char key, int x, int y){
    // ESC�L�[�ŏI��
    if (key == 27){
        exit(0);
    }
}

//
// ���V�F�[�v�֐�
//
void my_reshape(int width, int height){

}

//
// �`��֐�
//
void my_display(){
    // ��ʃN���A
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Z�o�b�t�@�L����
    glEnable(GL_DEPTH_TEST);
    // �e�N�X�`���L����
    glEnable(GL_TEXTURE_2D);

    // �`�`�`�`�����ɏ����������`�`�`�`

    // �e�N�X�`��������
    glDisable(GL_TEXTURE_2D);
    // Z�o�b�t�@������
    glDisable(GL_DEPTH_TEST);
    // �ŏI�`�揈��
    glFlush();
}

//
// ���C���֐�
//
int main(int argc, char** argv){
    // ������
    glutInit(&argc, argv);
    my_init(argv[0]);

    // �֐�
    glutKeyboardFunc(my_keyboard);
    glutReshapeFunc(my_reshape);
    glutDisplayFunc(my_display);

    // ���C�����[�v
    glutMainLoop();

    return 0;
}

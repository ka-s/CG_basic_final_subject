/// ================================
///  CG基礎最終課題 
/// ================================

// @date    2015-01-16
// @author  Kazuma Ito

#include <iostream>
#include <memory>
#include <GL/glut.h>

using namespace std;

// --------------------------------
//  定数
// --------------------------------

// ウィンドウサイズ
const int WINDOW_SIZE_X = 640;
const int WINDOW_SIZE_Y = 480;

// --------------------------------
//  構造体
// --------------------------------

// 座標構造体
struct Pos{
    float x;
    float y;
    float z;
};

// --------------------------------
//  グローバル変数
// --------------------------------

// 匿名名前空間
namespace{
    // 雪だるまの座標
    Pos pos_snow_man;
    // 床の座標
    Pos pos_floor;
}

// --------------------------------
//  関数宣言
// --------------------------------

// 雪だるま描画関数
void draw_snow_man(Pos pos);
// 地面描画関数
void draw_floor(Pos pos);

// キーボード処理関数
void my_keyboard(unsigned char key, int x, int y);
// リシェープ関数
void my_reshape(int width, int height);
// 描画関数
void my_display();
// 更新関数
void my_idle();

// 座標初期化関数
void pos_init();
// 光源初期化関数
void light_init();
// 初期化関数
void my_init();

// ================================
//  雪だるま描画関数
// ================================
void draw_snow_man(Pos pos){
    // 雪だるまを描画
    glPushMatrix();
    //   白
    glColor3f(1.0f, 1.0f, 1.0f);
    //   座標
    glTranslatef(pos.x, pos.y, pos.z);
    //   球
    glutSolidSphere(1.0, 32, 32);
    glPushMatrix();
    //   頭部分
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.75, 32, 32);
    glPopMatrix();
    glPushMatrix();
    //   板部分
    glColor3f(0.5f, 0.5f, 0.5f);
    glTranslatef(0.0f, -1.0f, 0.0f);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
}

// ================================
//  地面描画関数
// ================================
void draw_floor(Pos pos){
    // 地面の大きさ
    static float floor_size = 2.0;

    // 赤
    glColor3f(1.0f, 0.0f, 0.0f);
    // 正方形を描画
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(pos.x, pos.y, pos.z);
    glVertex3f(pos.x + floor_size, pos.y, pos.z);
    glVertex3f(pos.x + floor_size, pos.y, pos.z + floor_size);
    glVertex3f(pos.x, pos.y, pos.z + floor_size);
    glEnd();
}

// ================================
//  キーボード処理関数
// ================================
void my_keyboard(unsigned char key, int x, int y){
    // ESCキーで終了
    if (key == 27) exit(0);

}

// ================================
//  リシェープ関数
// ================================
void my_reshape(int width, int height){

}

// ================================
//  描画関数
// ================================
void my_display(){
    // 光源の座標
    static float light_pos[] = { 0.0f, 10.0f, 0.0f };

    // 画面クリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Zバッファ有効化
    glEnable(GL_DEPTH_TEST);
    // テクスチャ有効化
    glEnable(GL_TEXTURE_2D);

    // ～～～～ここに処理を書く～～～～
    // 光源を設置
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    // 雪だるまを描画
    draw_snow_man(pos_snow_man);
    // 地面を描画
    draw_floor(pos_floor);

    //// test
    //// 地面の大きさ
    //static float floor_size = 1.0;
    //// 赤
    //glColor3f(1.0f, 0.0f, 0.0f);
    //// 正方形を描画
    //glBegin(GL_POLYGON);
    //glNormal3f(0.0f, 1.0f, 0.0f);
    //glVertex3f(-1.0, -1.0, -1.0);
    //glVertex3f(1.0, -1.0, -1.0);
    //glVertex3f(1.0, -1.0, 1.0);
    //glVertex3f(-1.0, -1.0, 1.0);
    //glEnd();

    // テクスチャ無効化
    glDisable(GL_TEXTURE_2D);
    // Zバッファ無効化
    glDisable(GL_DEPTH_TEST);
    // 最終描画処理
    glFlush();

}

// ================================
//  更新関数
// ================================
void my_idle(){
    // 再描画
    glutPostRedisplay();

}

// ================================
//  座標初期化関数
// ================================
void pos_init(){
    // 雪だるまの座標初期化
    pos_snow_man = { 0.0f, 0.0f, 0.0f };
    // 地面の座標初期化
    pos_floor = { -1.0f, -1.0f, -1.0f };
}

// ================================
//  光源初期化関数
// ================================
void light_init(){
    // 光源属性の色
    float diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    // 各光源属性の設定
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    // 光源1を有効化
    glEnable(GL_LIGHT0);
}

// ================================
//  初期化関数
// ================================
void my_init(){
    // アスペクト
    static float aspect = (float)WINDOW_SIZE_X / (float)WINDOW_SIZE_Y;

    // 描画モード
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    // ウィンドウサイズ設定
    glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    // ウィンドウ出現座標設定
    glutInitWindowPosition(100, 100);
    // ウィンドウタイトル
    glutCreateWindow("Final_Subject");
    // 背景色設定
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    // 滑らかなライティングに設定
    glShadeModel(GL_SMOOTH);
    // Zバッファ使用
    glEnable(GL_DEPTH_TEST);
    // 画像データパックの使用
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // 描画領域の設定
    glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

    // 透視投影の設定
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 32.0);
    glMatrixMode(GL_MODELVIEW);

    // カメラの設定
    gluLookAt(
        0.0, 2.0, 4.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // 座標初期化
    pos_init();
    // 光源初期化
    light_init();

}

// ================================
//  メイン関数
// ================================
int main(int argc, char** argv){
    // 初期化
    glutInit(&argc, argv);
    my_init();

    // 関数
    glutKeyboardFunc(my_keyboard);
    glutReshapeFunc(my_reshape);
    glutDisplayFunc(my_display);
    glutIdleFunc(my_idle);

    // メインループ
    glutMainLoop();

    return 0;
}

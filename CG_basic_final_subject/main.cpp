/// ================================
///  CG基礎最終課題 
/// ================================

// @date    2015-01-16
// @author  Kazuma Ito

#include <iostream>
#include <memory>
#include <GL/glut.h>

using namespace std;

// ---------------- 定数 ----------------

// ウィンドウサイズ
const int WINDOW_SIZE_X = 640;
const int WINDOW_SIZE_Y = 480;

// ウィンドウ出現座標
const int WINDOW_APPER_X = 100;
const int WINDOW_APPER_Y = 100;

// 背景色
const float BACKGROUND_R = 1.0f;
const float BACKGROUND_G = 0.0f;
const float BACKGROUND_B = 1.0f;

// --------------------------------------

// ---------------- グローバル変数 ----------------

// 匿名名前空間
namespace{
    // キーボード
    //   各キーの押しているフレーム数(?)を格納
    bool key_w = false;
    bool key_a = false;
    bool key_s = false;
    bool key_d = false;

    // 実行してからの更新回数
    int update_count = 0;
}

// -----------------------------------------------

// ---------------- 関数宣言 ----------------

// キーボード処理関数
void my_keyboard(unsigned char key, int x, int y);
// リシェープ関数
void my_reshape(int width, int height);
// 描画関数
void my_display();
// 更新関数
void my_idle();
// デバッグ関数
void my_debug();

// ------------------------------------------

// ---------------- クラス ----------------

//
// ライト設定クラス
//
class make_Light{
private:

public:
    make_Light(float* diffuse, float* specular, float* ambient){

    }
};

//
// ライト管理クラス
//
class Light_manager{
private:

public:
    Light_manager(){
        glEnable(GL_LIGHT0);
    }    
};
std::shared_ptr<Light_manager> light_manager;

//
// 初期化クラス
//
class My_init{
private:

public:
    My_init(){
        // 描画モード
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
        // ウィンドウサイズ設定
        glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
        // ウィンドウ出現座標設定
        glutInitWindowPosition(WINDOW_APPER_X, WINDOW_APPER_Y);
        // ウィンドウタイトル
        glutCreateWindow("Final_Subject");
        // 背景色設定
        glClearColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, 0.0f);
        // 滑らかなライティングに設定
        glShadeModel(GL_SMOOTH);
        // Zバッファ使用
        glEnable(GL_DEPTH_TEST);
        // 画像データパックの使用
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // ライト管理クラス
        light_manager = std::make_shared<Light_manager>();
    }
};
std::shared_ptr<My_init> init;

//
// 描画クラス
//
class My_display{
private:

public:
    // 描画関数
    void draw(){

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POLYGON);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, 0.0f, -1.0f);
        glEnd();

    }
};
std::shared_ptr<My_display> display;

//
// デバッグクラス
//
class My_debug{
private:

public:
    void do_debug(){
        //cout << key_w << endl;
        //cout << key_a << endl;
        //cout << key_s << endl;
        //cout << key_d << endl;

        cout << update_count << endl;
    }
};
std::shared_ptr<My_debug> debug;

// ----------------------------------------
// ----------------------------------------

//
// キーボード処理関数
//
void my_keyboard(unsigned char key, int x, int y){
    // ESCキーで終了
    if (key == 27) exit(0);

    // wasdキー
    //if (key == 'w') key_w = true;
    //if (key == 'a') key_a = true;
    //if (key == 's') key_s = true;
    //if (key == 'd') key_d = true;
}

//
// リシェープ関数
//
void my_reshape(int width, int height){

}

//
// 描画関数
//
void my_display(){
    // 画面クリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Zバッファ有効化
    glEnable(GL_DEPTH_TEST);
    // テクスチャ有効化
    glEnable(GL_TEXTURE_2D);

    // ～～～～ここに処理を書く～～～～
    display->draw();
    debug->do_debug();

    // テクスチャ無効化
    glDisable(GL_TEXTURE_2D);
    // Zバッファ無効化
    glDisable(GL_DEPTH_TEST);
    // 最終描画処理
    glFlush();
}

//
// 更新関数
//
void my_idle(){
    // カウントを更新ごとにインクリメント
    update_count++;

    // 再描画
    glutPostRedisplay();

    // キー状態初期化
    //key_w = false;
    //key_a = false;
    //key_s = false;
    //key_d = false;
}

//
// メイン関数
//
int main(int argc, char** argv){
    // 初期化
    glutInit(&argc, argv);
    init = std::make_shared<My_init>();
    display = std::make_shared<My_display>();
    debug = std::make_shared<My_debug>();

    // 関数
    glutKeyboardFunc(my_keyboard);
    glutReshapeFunc(my_reshape);
    glutDisplayFunc(my_display);
    glutIdleFunc(my_idle);

    // メインループ
    glutMainLoop();

    return 0;
}

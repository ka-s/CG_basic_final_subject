/// ================================
///  CG基礎最終課題 
/// ================================

// @date    2015-01-??
// @author  Kazuma Ito

#include <iostream>
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
    int key_w = 0;
    int key_a = 0;
    int key_s = 0;
    int key_d = 0;
}

// -----------------------------------------------

// ---------------- 関数宣言 ----------------

// 初期化関数
void my_init(char *progname);
// キーボード処理関数
void my_keyboard(unsigned char key, int x, int y);
// リシェープ関数
void my_reshape(int width, int height);
// 描画関数
void my_display();
// デバッグ関数
void my_debug();

// ------------------------------------------

//
// 初期化関数
//
void my_init(char *progname){
    // 描画モード
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    // ウィンドウサイズ設定
    glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    // ウィンドウ出現座標設定
    glutInitWindowPosition(WINDOW_APPER_X, WINDOW_APPER_Y);
    glutCreateWindow(progname);
    // 背景色設定
    glClearColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, 0.0f);
    // 滑らかなライティングに設定
    glShadeModel(GL_SMOOTH);
    // Zバッファ使用
    glEnable(GL_DEPTH_TEST);
    // 画像データパックの使用
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

//
// キーボード処理関数
//
void my_keyboard(unsigned char key, int x, int y){
    // ESCキーで終了
    if (key == 27) exit(0);

    // wasdキー
    //   前方
    key == 'w' ? key_w++ : key_w = 0;
    //   左
    key == 'a' ? key_a++ : key_a = 0;
    //   後方
    key == 's' ? key_s++ : key_s = 0;
    //   右
    key == 'd' ? key_d++ : key_d = 0;
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

    // 〜〜〜〜ここに処理を書く〜〜〜〜
    my_debug();

    // テクスチャ無効化
    glDisable(GL_TEXTURE_2D);
    // Zバッファ無効化
    glDisable(GL_DEPTH_TEST);
    // 最終描画処理
    glFlush();
}

//
//
//
void my_debug(){
    cout << key_w << endl;
    cout << key_a << endl;
    cout << key_s << endl;
    cout << key_d << endl;
}

//
// メイン関数
//
int main(int argc, char** argv){
    // 初期化
    glutInit(&argc, argv);
    my_init(argv[0]);

    // 関数
    glutKeyboardFunc(my_keyboard);
    glutReshapeFunc(my_reshape);
    glutDisplayFunc(my_display);

    // メインループ
    glutMainLoop();

    return 0;
}

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

// 床の最大生成数
const int MAX_FLOORS = 3;
// 木の最大生成数
const int MAX_TREE = 16;

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
    // 雪だるまの移動速度
    float move_speed_snow_man = 0.5f;
    // 雪だるまの前進速度
    float front_speed_snow_man = 0.101f;
    // 木の最大間隔
    float tree_space = 8.0f;

    // カメラの座標
    Pos pos_camera;
    // 雪だるまの座標
    Pos pos_snow_man;
    // 床の座標
    Pos pos_floor[MAX_FLOORS];
    // 木の座標
    Pos pos_tree[MAX_TREE];

    // マテリアル
    //   真珠のマテリアル
    float pearl_diffuse[] = { 1, 0.829, 0.829, 1.0 };
    float pearl_specular[] = { 0.296648, 0.296648, 0.296648, 1.0 };
    float pearl_ambient[] = { 0.25, 0.20725, 0.20725, 1.0 };
    float pearl_shininess = 10.24;

    //   緑のプラスチックのマテリアル
    float green_plastic_diffuse[] = { 0.1, 0.35, 0.1, 1.0 };
    float green_plastic_specular[] = { 0.45, 0.55, 0.45, 1.0 };
    float green_plastic_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    float green_plastic_shininess = 32;

}

// --------------------------------
//  関数宣言
// --------------------------------

// 雪だるま描画関数
void draw_snow_man(Pos pos);

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
    glPushMatrix();

    //   座標
    glTranslatef(pos.x, pos.y, pos.z);
    // マテリアル
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, pearl_diffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, pearl_specular);
    //glMaterialfv(GL_FRONT, GL_AMBIENT, pearl_ambient);
    //glMaterialf(GL_FRONT, GL_SHININESS, pearl_shininess);
    glColor3f(1.0f, 1.0f, 1.0f);
    //   球
    glutSolidSphere(1.0, 32, 32);

    //   頭部分
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.75, 32, 32);
    glPopMatrix();

    //   板部分
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
//  地面描画クラス
// ================================
class Floor{
private:
    float x, y, z;
    float player_pos;

public:
    // コンストラクタ
    Floor(Pos pos){
        x = pos.x;
        y = pos.y;
        z = pos.z;
    }

    // 描画メソッド
    void draw(float player_pos){
        // 地面の大きさ(直径)
        static float floor_size = 64.0;

        // 赤
        glColor3f(0.5f, 0.5f, 0.5f);
        // 正方形を描画
        glBegin(GL_POLYGON);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(x, y, z);
        glVertex3f(x + floor_size, y, z);
        glVertex3f(x + floor_size, y, z + floor_size);
        glVertex3f(x, y, z + floor_size);
        glEnd();

        // 地面ループ制御
        if (player_pos < z){
            z -= 64;
        }
    }

};
shared_ptr<Floor> floors[MAX_FLOORS];

// ================================
//  木描画クラス
// ================================
class Tree{
private:
    float x, y, z;

public:
    // コンストラクタ
    Tree(Pos pos){
        x = pos.x;
        y = pos.y;
        z = pos.z;
    }

    // 描画メソッド
    void draw(float player_pos){
        glPushMatrix();

        // 座標
        glTranslatef(x, y + 1.0, z);

        glPushMatrix();
        glScalef(1.0f, 4.0f, 1.0f);
        // 青
        glColor3f(0.0f, 0.0f, 1.0f);
        // 木の幹
        glutSolidCube(1.0);
        glPopMatrix();

        glTranslatef(0.0f, 0.5f, 0.0f);

        //   葉っぱ1段目
        glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glutSolidCone(2.0, 2.0, 32, 32);
        glPopMatrix();

        //   葉っぱ2段目
        glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glutSolidCone(1.5, 2.0, 32, 32);
        glPopMatrix();

        glPopMatrix();

        glPopMatrix();

        // 木ループ制御
        if (player_pos < z - 16.0f){
            z -= 8 * (MAX_TREE / 2);
        }
    }

};
shared_ptr<Tree> trees[MAX_TREE];

// ================================
//  キーボード処理関数
// ================================
void my_keyboard(unsigned char key, int x, int y){
    // ESCキーで終了
    if (key == 27) exit(0);

    // 左右移動
    if (key == 'a') pos_snow_man.x -= move_speed_snow_man;
    if (key == 'd') pos_snow_man.x += move_speed_snow_man;

    // 加速減速
    if (key == 'w') front_speed_snow_man += 0.02;
    if (key == 's') front_speed_snow_man -= 0.02;
    // スピード制御
    if (front_speed_snow_man < 0) front_speed_snow_man = 0.001;

    // 再描画
    glutPostRedisplay();

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
    // 行列の初期化
    glLoadIdentity();

    // ～～～～ここに処理を書く～～～～
    // カメラの設定
    gluLookAt(
        pos_snow_man.x, pos_camera.y, pos_camera.z,
        pos_snow_man.x, pos_snow_man.y, pos_snow_man.z,
        0.0, 1.0, 0.0);

    // 光源を設置
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    // 雪だるまを描画
    draw_snow_man(pos_snow_man);
    // 地面を描画
    for (int i = 0; i < MAX_FLOORS; ++i){
        floors[i]->draw(pos_snow_man.z);
    }

    // 木を描画
    //draw_tree(pos_tree);
    for (int i = 0; i < MAX_TREE; ++i){
        trees[i]->draw(pos_snow_man.z);
    }

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
    //雪だるまの前進
    pos_snow_man.z -= front_speed_snow_man;
    // カメラの前進
    pos_camera.z -= front_speed_snow_man;

    // 再描画
    glutPostRedisplay();

}

// ================================
//  座標初期化関数
// ================================
void pos_init(){
    // カメラの座標初期化
    pos_camera = { 0.0f, 4.0f, 16.0f };
    // 雪だるまの座標初期化
    pos_snow_man = { 0.0f, 1.0f, 0.0f };

    // 地面の座標初期化
    for (int i = 0; i < MAX_FLOORS; ++i){
        pos_floor[i] = { -32.0f, 0.0f, -32.0f * i };
    }

    // 木の座標初期化
    //   右側
    for (int i = 0; i < MAX_TREE / 2; ++i){
        pos_tree[i] = { tree_space, 0.0f, -8.0f * i };
    }
    //   左側
    for (int i = MAX_TREE / 2; i < MAX_TREE; ++i){
        pos_tree[i] = { -tree_space, 0.0f, -8.0f * (i - MAX_TREE / 2) };
    }

}

// ================================
//  光源初期化関数
// ================================
void light_init(){
    // 光源属性の色
    float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    // 各光源属性の設定
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    // 光源0を有効化
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
    gluPerspective(60.0, aspect, 0.1, 64.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 座標初期化
    pos_init();
    // 光源初期化
    light_init();

    // 床クラスの生成
    for (int i = 0; i < MAX_FLOORS; ++i){
        floors[i] = make_shared<Floor>(pos_floor[i]);
    }

    // 木クラスの生成
    for (int i = 0; i < MAX_TREE; ++i){
        trees[i] = make_shared<Tree>(pos_tree[i]);
    }

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

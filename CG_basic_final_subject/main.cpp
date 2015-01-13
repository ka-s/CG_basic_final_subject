/// ================================
///  CG基礎最終課題 
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
//  定数
// --------------------------------

// 円周率
const float PI = 3.14159265359f;

// フレームレート
const int FPS = 60;

// ウィンドウサイズ
const int WINDOW_SIZE_X = 640;
const int WINDOW_SIZE_Y = 480;

// ★ゲーム性に影響を与える変数
//      雪だるまの大きさ
const double snow_man_size = 1.0;
//      雪だるまの左右移動速度
const float move_speed_snow_man = 1.0f;
//      木の大きさ
const double tree_size = 2.0;
//      左右の木の最大間隔
const float tree_space = 16.0f;
//      ランダム木の前後間隔
const float random_tree_space = 8.0f;

// 床の最大生成数
const int MAX_FLOORS = 3;
// 木の最大生成数
const int MAX_TREE = 16;
// ランダムに出現する木の最大生成数
const int MAX_RANDOM_TREE = 32;

// マテリアル
//   真珠のマテリアル
const float pearl_diffuse[] = { 1, 0.829, 0.829, 1.0 };
const float pearl_specular[] = { 0.296648, 0.296648, 0.296648, 1.0 };
const float pearl_ambient[] = { 0.25, 0.20725, 0.20725, 1.0 };
const float pearl_shininess = 10.24;

//   緑のプラスチックのマテリアル
const float green_plastic_diffuse[] = { 0.1, 0.35, 0.1, 1.0 };
const float green_plastic_specular[] = { 0.45, 0.55, 0.45, 1.0 };
const float green_plastic_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
const float green_plastic_shininess = 32;

// 銅のマテリアル
const float copper_ambient[] = { 0.19125, 0.0735, 0.0225, 1.0 };
const float copper_diffuse[] = { 0.7038, 0.27048, 0.0828, 1.0 };
const float copper_specular[] = { 0.256777, 0.137622, 0.086014, 1.0 };
const float copper_shininess = 12.8;

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
    // ランダムデバイス
    random_device rd;
    // メルセンヌツイスター
    mt19937 my_mersenne_twister(rd());
    // 一様分布なランダム
    uniform_int_distribution<int> mt_random((int)-tree_space, (int)tree_space);

    // プログラムが実行されてからの更新回数
    int global_time = 0;

    // カメラの座標
    Pos pos_camera;
    // 雪だるまの座標
    Pos pos_snow_man;
    // 床の座標
    Pos pos_floor[MAX_FLOORS];
    // 木の座標
    Pos pos_tree[MAX_TREE];
    // ランダム生成の木の座標
    Pos pos_random_tree[MAX_RANDOM_TREE];

    // 雪だるまの初期前進速度
    float front_speed_snow_man = 1.0f;

    // 当たり判定フラグ
    bool f_hit = false;
    // リセットフラグ
    bool f_reset = false;

}

// --------------------------------
//  関数宣言
// --------------------------------

// RAW読み込み関数
void readRAWImage(char* filename, unsigned char image[128][128][3]);

// ランダム木のランダムな座標を生成する関数
float get_tree_random_pos();
// 当たり判定関数
bool is_hit();

// キーボード処理関数
void my_keyboard(unsigned char key, int x, int y);
// リシェープ関数
void my_reshape(int width, int height);
// 描画関数
void my_display();
// 更新関数
void my_idle();

// ゲームリセット関数
void reset();
// 座標初期化関数
void pos_init();
// 光源初期化関数
void light_init();
// 初期化関数
void my_init();

// ################################
//  フレームレート制御クラス
//      環境差をなくすために実装
//      (制御と言っても早すぎた更新を止めるだけ)
// ################################
class FPSManager{
private:
    // 現在のFPS
    float now_fps;
    // 更新回数
    int frame_count;
    // 現在のミリ秒
    int start_clock;

public:
    // コンストラクタ
    FPSManager(){
        now_fps = 0.0f;
        frame_count = 0;
        start_clock = 0;
    }

    // 更新メソッド
    void update(){
        // 一番最初のフレーム
        if (frame_count == 0){
            start_clock = (int)clock();
        }
        // 更新回数が規定回数達したら
        if (frame_count == FPS){
            int hoge = (int)clock();
            // 現在のFPSを計算
            now_fps = 1000.0f / (((float)hoge - (float)start_clock) / (float)(FPS));
            frame_count = 0;
            start_clock = hoge;
        }
        frame_count++;
    }

    // 現在のFPSを描画するメソッド
    void draw(){
        cout << now_fps << endl;
    }

    // 更新制御メソッド
    void wait(){
        // 規定回数更新にかかった時間
        int spend_time = (int)clock() - start_clock;
        // かかるべき時間
        int wait_time = frame_count * 1000 / FPS - spend_time;

        // 更新を停止
        if (wait_time > 0){
            Sleep(wait_time);
        }
    }

};
FPSManager* fps_manager;

// ################################
//  雪だるま描画クラス
// ################################
class SnowMan{
private:
    float x, z, y;
    float effect_long;
    float effect_count;

public:
    // コンストラクタ
    SnowMan(Pos pos){
        x = pos.x;
        y = pos.y;
        z = pos.z;

        effect_long = 0;
        effect_count = 0;
    }

    // 描画メソッド
    void draw(Pos pos){
        x = pos.x;
        y = pos.y;
        z = pos.z;

        glPushMatrix();

        // 座標
        glTranslatef(x, y, z);
        // 真珠
        glMaterialfv(GL_FRONT, GL_DIFFUSE, pearl_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, pearl_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, pearl_ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, pearl_shininess);
        // 球
        glutSolidSphere(snow_man_size, 32, 32);

        //   頭部分
        glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f);
        glutSolidSphere(0.75, 32, 32);
        glPopMatrix();

        //   板部分
        glPushMatrix();
        glTranslatef(0.0f, -1.0f, 0.0f);
        glScalef(1.0f, 1.0f, 4.0f);
        //   緑のプラスチック
        glMaterialfv(GL_FRONT, GL_DIFFUSE, green_plastic_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, green_plastic_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, green_plastic_ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, green_plastic_shininess);
        glutSolidCube(1.0);
        glPopMatrix();

        glPopMatrix();
    }

    // 破壊エフェクト描画メソッド
    void draw_effect(){

        for (int i = 0; i < 16; ++i){
            glPushMatrix();

            // 座標
            glTranslatef(x + cos(360.0f / 16.0f * (float)i) * effect_long, y + sin(360.0f / 16.0f * (float)i) * effect_long, z);
            // 真珠
            glMaterialfv(GL_FRONT, GL_DIFFUSE, pearl_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, pearl_specular);
            glMaterialfv(GL_FRONT, GL_AMBIENT, pearl_ambient);
            glMaterialf(GL_FRONT, GL_SHININESS, pearl_shininess);
            // 球
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
//  地面描画クラス
// ################################
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

        // 真珠
        glMaterialfv(GL_FRONT, GL_DIFFUSE, pearl_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, pearl_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, pearl_ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, pearl_shininess);
        // 正方形を描画
        glBegin(GL_POLYGON);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(x, y, z);
        glVertex3f(x + floor_size, y, z);
        glVertex3f(x + floor_size, y, z + floor_size);
        glVertex3f(x, y, z + floor_size);
        glEnd();

        // 地面ループ制御
        if (player_pos < z - 16.0f){
            z -= floor_size*3;
        }
    }

};
Floor* floors[MAX_FLOORS];

// ################################
//  木描画クラス
// ################################
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
    void draw(float player_pos, float loop_space){
        glPushMatrix();

        // 座標
        glTranslatef(x, y + 1.0, z);

        glPushMatrix();
        glScalef(1.0f, 4.0f, 1.0f);
        // 銅
        glMaterialfv(GL_FRONT, GL_DIFFUSE, copper_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, copper_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, copper_ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, copper_shininess);
        // 木の幹
        glutSolidCube(1.0);
        glPopMatrix();

        glTranslatef(0.0f, 0.5f, 0.0f);

        //   葉っぱ1段目
        glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        //   緑のプラスチック
        glMaterialfv(GL_FRONT, GL_DIFFUSE, green_plastic_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, green_plastic_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, green_plastic_ambient);
        glMaterialf(GL_FRONT, GL_SHININESS, green_plastic_shininess);
        glutSolidCone(tree_size, 2.0, 32, 32);
        glPopMatrix();

        //   葉っぱ2段目
        glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glutSolidCone(tree_size - 0.5, 2.0, 32, 32);
        glPopMatrix();

        glPopMatrix();

        glPopMatrix();

        // 木ループ制御
        if (player_pos < z - 16.0f){
            // ランダム木ならば配置しなおし
            if (loop_space == random_tree_space){
                z -= loop_space * MAX_RANDOM_TREE;
                x = get_tree_random_pos();
            }
            else{
                z -= loop_space / 2 * (MAX_TREE / 2);
            }
        }
    }

    // 座標を返すメソッド
    Pos get_pos(){
        return{ x, y, z };
    }

};
// 左右の木
Tree* trees[MAX_TREE];
// ランダムに出現する木
Tree* random_trees[MAX_RANDOM_TREE];

// ================================
//  ランダム木のランダムな座標を生成する関数
// ================================
float get_tree_random_pos(){

    //return (float)(rand() % ((int)tree_space * 2)) + 1.0f - tree_space;
    return (float)mt_random(my_mersenne_twister);

}

// ================================
//  当たり判定関数
// ================================
bool is_hit(){

    // 左右の当たり判定
    if (pos_snow_man.x - snow_man_size < -tree_space + tree_size || 
        pos_snow_man.x + snow_man_size > tree_space - tree_size){
        return true;
    }
    // 各木との当たり判定
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
//  キーボード処理関数
// ================================
void my_keyboard(unsigned char key, int x, int y){

    // ESCキーで終了
    if (key == 27) exit(0);

    // 木にあたってなけれらば左右移動可能
    if (f_hit != true){
        // 左右移動
        if (key == 'a') pos_snow_man.x -= move_speed_snow_man;
        if (key == 'd') pos_snow_man.x += move_speed_snow_man;
    }

    // 加速減速
    //if (key == 'w') front_speed_snow_man += 0.02;
    //if (key == 's') front_speed_snow_man -= 0.02;
    // スピード制御
    //if (front_speed_snow_man > 10.0f) front_speed_snow_man = 2.0f;
    //if (front_speed_snow_man < 0.0f) front_speed_snow_man = 0.001f;

    // やり直し
    if (key == 'r'){
        reset();
    }

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

    // 画面クリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Zバッファ有効化
    glEnable(GL_DEPTH_TEST);
    // 光源有効化
    glEnable(GL_LIGHTING);
    // 行列の初期化
    glLoadIdentity();

    // カメラの設定
    gluLookAt(
        pos_snow_man.x, pos_camera.y, pos_camera.z,
        pos_snow_man.x, pos_snow_man.y, pos_snow_man.z,
        0.0, 1.0, 0.0);

    // 雪だるま破壊エフェクト描画
    if (f_hit){
        snow_man->draw_effect();
    }
    else{
        // 雪だるまを描画
        snow_man->draw(pos_snow_man);
    }

    // 地面を描画
    for (int i = 0; i < MAX_FLOORS; ++i){
        floors[i]->draw(pos_snow_man.z);
    }

    // 木を描画
    for (int i = 0; i < MAX_TREE; ++i){
        trees[i]->draw(pos_snow_man.z, tree_space);
    }

    // ランダムな木を描画
    for (int i = 0; i < MAX_RANDOM_TREE; ++i){
        random_trees[i]->draw(pos_snow_man.z, random_tree_space);
    }

    // 光源無効化
    glDisable(GL_LIGHTING);
    // Zバッファ無効化
    glDisable(GL_DEPTH_TEST);
    // 最終描画処理
    glFlush();

}

// ================================
//  更新関数
// ================================
void my_idle(){
    // フレームレート制御データ更新
    fps_manager->update();
    // 現在のFPSをコンソールに描画
    fps_manager->draw();

    // 当たり判定
    if (is_hit()){
        f_hit = true;
    }
    else{
        //雪だるまの前進
        pos_snow_man.z -= front_speed_snow_man;
        // カメラの前進
        pos_camera.z -= front_speed_snow_man;
    }
    // リセット判定
    if (f_reset){
        reset();
    }

    // 再描画
    glutPostRedisplay();
    global_time++;

    // フレームレート制御
    fps_manager->wait();

}

// ================================
//  ゲームリセット関数
// ================================
void reset(){

    // すべてのオブジェクトを削除
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

    // 座標を初期化
    pos_init();

    // すべてのオブジェクトを生成し直す
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

    // 速度も初期化
    front_speed_snow_man = 1.0f;

    // フラグ初期化
    f_hit = false;
    f_reset = false;

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
        pos_floor[i] = { -32.0f, 0.0f, -64.0f * i };
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

    // ランダム生成される木の座標初期化
    for (int i = 0; i < MAX_RANDOM_TREE; ++i){
        pos_random_tree[i] = { 
            get_tree_random_pos(), 
            0.0f, 
            -random_tree_space * i - 64.0f};
    }

}

// ================================
//  光源初期化関数
// ================================
void light_init(){

    // 光源属性の色
    float light_pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    // 各光源属性の設定
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    // 滑らかなライティングに設定
    glShadeModel(GL_SMOOTH);
    // Zバッファ使用
    glEnable(GL_DEPTH_TEST);
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

    // FPS制御クラスの生成
    fps_manager = new FPSManager();

    // 雪だるまの生成
    snow_man = new SnowMan(pos_snow_man);

    // 床クラスの生成
    for (int i = 0; i < MAX_FLOORS; ++i){
        floors[i] = new Floor(pos_floor[i]);
    }

    // 木クラスの生成
    for (int i = 0; i < MAX_TREE; ++i){
        trees[i] = new Tree(pos_tree[i]);
    }

    // ランダム木クラスの生成
    for (int i = 0; i < MAX_RANDOM_TREE; ++i){
        random_trees[i] = new Tree(pos_random_tree[i]);
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

    // メモリ解放
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

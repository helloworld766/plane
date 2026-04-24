#pragma once
#include <easyx.h>
#include <Windows.h>
#include <random>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;
// 宏定义
#define WINDOWS_WIDTH  800
#define WINDOWS_HEIGHT 900
class Bullet;
class Plane;
// ======================================
// 飞机基类声明
// ======================================
class Plane
{
public:
    IMAGE* img;
    int HP;
    int pos[2];
    int width;
    int height;
    Plane(int x, int y, int hp, IMAGE* pimg);
    virtual void move();//移动函数
    virtual void draw();//绘制函数
    void check_side();//边界检查
    virtual void shoot(vector<Bullet*>& bullet_list);//射击函数
};

// ======================================
// 玩家飞机类声明
// ======================================
class Player_plane : public Plane
{
public:
    static IMAGE player_img;
    static void load_image();
    Player_plane(int x, int y, int hp);
    void shoot(vector<Bullet*>& bullet_list) override;
};

// ======================================
// 敌机类声明
// ======================================
class EnemyA_plane : public Plane
{
public:
    static IMAGE player_img;
    int direction_y;
    int direction_x;
    int flash_count;
    static void load_image();
    EnemyA_plane(int x, int y, int hp);
    void move() override;
    void shoot(vector<Bullet*>& bullet_list) override;
};

// ======================================
// 子弹基类声明
// ======================================
class Bullet
{
public:
    IMAGE* img;
    bool alive;
    int pos[2];
    int width;
    int height;
    Bullet(bool Alive, int x, int y, IMAGE* pimg);
    virtual void move();
    virtual void draw();
};

// ======================================
// 玩家普通子弹类声明
// ======================================
class Bullet_normal_player: public Bullet
{
public:
    static IMAGE bullet_normal_player_img;
    static void load_image();
    Bullet_normal_player(bool alive,int x, int y);
    void move() override;
};

// ======================================
// 敌方普通子弹类声明
// ======================================
class Bullet_normal_enemy : public Bullet
{
public:
    static IMAGE bullet_normal_enemy_img;
    static void load_image();
    Bullet_normal_enemy(bool alive, int x, int y);
    void move() override;
};
// 全局函数声明
void draw_transparent(int x, int y, IMAGE* img);//实现飞机背景图透明
void init();//初始化
void main_loop();//主循环
void collision();//碰撞函数
void total_draw(vector<Bullet*>& bullet_list, vector<Plane*>& plane_list, IMAGE& background);//一次性绘制函数
void total_move(vector<Bullet*>& bullet_list, vector<Plane*>& plane_list);//一次性移动函数
void total_check_side(vector<Bullet*>& bullet_list, vector<Plane*>& plane_list);//一次性边界检查函数
void total_shoot(vector<Bullet*>& bullet_list, vector<Plane*>& plane_list);//一次性射击
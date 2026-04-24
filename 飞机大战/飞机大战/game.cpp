#include "game.h"
#include "resource.h"
#include <iostream>

#pragma comment(lib, "msimg32.lib")
using namespace std;

// ======================================
// 全局函数实现
// ======================================
void draw_transparent(int x, int y, IMAGE* img)
{
    HDC img_dc = GetImageHDC(img);
    HDC wnd_dc = GetImageHDC();
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(wnd_dc, x, y, img->getwidth(), img->getheight(),
        img_dc, 0, 0, img->getwidth(), img->getheight(), blend);
}

void init()
{
    srand((unsigned)time(NULL));
    initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT);
    BeginBatchDraw();
    Player_plane::load_image();
    EnemyA_plane::load_image();
    Bullet_normal_player::load_image();
    Bullet_normal_enemy::load_image();
}

void main_loop()
{
    IMAGE background;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> int_dist(0, WINDOWS_WIDTH - 60);
    vector<Bullet*> bullet_list;
    vector<Plane*> plane_list;
    loadimage(&background, RT_RCDATA, MAKEINTRESOURCE(IDB_BACKGROUND), WINDOWS_WIDTH, WINDOWS_HEIGHT);
    Player_plane player(WINDOWS_WIDTH / 2, WINDOWS_HEIGHT - 100, 100);
    EnemyA_plane enemya1(int_dist(gen), 0, 100);
    EnemyA_plane enemya2(int_dist(gen), 0, 100);
    EnemyA_plane enemya3(int_dist(gen), 0, 100);
    plane_list.push_back(&player);
    plane_list.push_back(&enemya1);
    plane_list.push_back(&enemya2);
    plane_list.push_back(&enemya3);
    while (1)
    {
        total_move(bullet_list,plane_list);
        total_check_side(bullet_list, plane_list);
        total_shoot(bullet_list, plane_list);
        total_draw(bullet_list, plane_list,background);
        Sleep(16);
    }
}

void collision()
{
    
}

void total_draw(vector<Bullet*>& bullet_list,vector<Plane*>& plane_list,IMAGE& background)
{
    cleardevice();//清空
    putimage(0, 0, &background);
    for (Bullet* b : bullet_list)
    {
        b->draw();
    }
    for (Plane* p : plane_list)
    {
        p->draw();
    }
    FlushBatchDraw();//加载缓存区图片到窗口
}

void total_move(vector<Bullet*>& bullet_list, vector<Plane*>& plane_list)
{
    for (Bullet* b : bullet_list)
    {
        b->move();
    }
    for (Plane* p : plane_list)
    {
        p->move();
    }
}

void total_check_side(vector<Bullet*>& bullet_list, vector<Plane*>& plane_list)
{
    for (Bullet* b : bullet_list)
    {
        //b->check_side();
    }
    for (Plane* p : plane_list)
    {
        p->check_side();
    }
}

void total_shoot(vector<Bullet*>& bullet_list, vector<Plane*>& plane_list)
{
    for (Plane* p : plane_list)
    {
        p->shoot(bullet_list);
    }
}
// ======================================
// Plane基类实现
// ======================================
Plane::Plane(int x, int y, int hp, IMAGE* pimg)
{
    HP = hp;
    pos[0] = x;
    pos[1] = y;
    height = 60;
    width = 60;
    img = pimg;
}

void Plane::move()
{
    if (GetAsyncKeyState('W') & 0x8000) pos[1] -= 5;
    if (GetAsyncKeyState('A') & 0x8000) pos[0] -= 5;
    if (GetAsyncKeyState('S') & 0x8000) pos[1] += 5;
    if (GetAsyncKeyState('D') & 0x8000) pos[0] += 5;
}

void Plane::draw()
{
    draw_transparent(pos[0], pos[1], img);
}

void Plane::check_side()
{
    if (pos[0] < 0) pos[0] = 0;
    if (pos[0] + width > WINDOWS_WIDTH) pos[0] = WINDOWS_WIDTH - width;
    if (pos[1] < 0) pos[1] = 0;
    if (pos[1] + height > WINDOWS_HEIGHT) pos[1] = WINDOWS_HEIGHT - height;
}
void Plane::shoot(vector<Bullet*>& bullet_list)
{
}
// ======================================
// Player_plane实现
// ======================================
IMAGE Player_plane::player_img;

void Player_plane::load_image()
{
    loadimage(&player_img, RT_RCDATA, MAKEINTRESOURCE(IDB_PLAYER), 60, 60);
}

Player_plane::Player_plane(int x, int y, int hp) : Plane(x, y, hp, &player_img) {}

void Player_plane::shoot(vector<Bullet*>& bullet_list)
{
    if (GetAsyncKeyState('J') & 0x8000)
    {
        int bullet_x = pos[0] + width / 2;
        int bullet_y = pos[1];
        bullet_list.push_back(new Bullet_normal_player(true, bullet_x, bullet_y));
    }
}
// ======================================
// EnemyA_plane实现
// ======================================
IMAGE EnemyA_plane::player_img;

void EnemyA_plane::load_image()
{
    loadimage(&player_img, RT_RCDATA, MAKEINTRESOURCE(IDB_ENEMYA), 60, 60);
}

EnemyA_plane::EnemyA_plane(int x, int y, int hp) : Plane(x, y, hp, &player_img)
{
    direction_y = (rand() % 2 == 0 ? -1 : 1);
    direction_x = (rand() % 2 == 0 ? -1 : 1);
    flash_count = 0;
}

void EnemyA_plane::move()
{
    if (flash_count > rand() % WINDOWS_HEIGHT)
    {
        flash_count = 0;
        direction_y *= -1;
    }
    pos[0] += direction_x;
    pos[1] += direction_y;
    if (pos[0] + width >= WINDOWS_WIDTH || pos[0] <= 0)
    {
        direction_x *= -1;
    }
    flash_count++;
}

void EnemyA_plane::shoot(vector<Bullet*>& bullet_list)
{
    if (direction_y == -1)
    {
        int bullet_x = pos[0] + width / 2-10;
        int bullet_y = pos[1]+height;
        bullet_list.push_back(new Bullet_normal_enemy(true, bullet_x, bullet_y));
    }
}
// ======================================
// Bullet基类实现
// ======================================
Bullet::Bullet(bool Alive, int x, int y, IMAGE* pimg)
{
    alive = Alive;
    pos[0] = x;
    pos[1] = y;
    width = 20;
    height = 40;
    img = pimg;
}

void Bullet::move() {}
void Bullet::draw()
{
    draw_transparent(pos[0], pos[1], img);
}

// ======================================
// Bullet_player_normal实现
// ======================================
IMAGE Bullet_normal_player::bullet_normal_player_img;

void Bullet_normal_player::load_image()
{
    loadimage(&bullet_normal_player_img, RT_RCDATA, MAKEINTRESOURCE(IDB_BULLET), 20, 40);
}

Bullet_normal_player::Bullet_normal_player(bool alive,int x, int y) : Bullet(alive, x, y, &bullet_normal_player_img) {}

void Bullet_normal_player::move()
{
    pos[1] -= 10;
}

// ======================================
// Bullet_player_enemy实现
// ======================================
IMAGE Bullet_normal_enemy::bullet_normal_enemy_img;

void Bullet_normal_enemy::load_image()
{
    loadimage(&bullet_normal_enemy_img, RT_RCDATA, MAKEINTRESOURCE(IDB_BULLET), 20, 40);
}

Bullet_normal_enemy::Bullet_normal_enemy(bool alive, int x, int y) : Bullet(alive, x, y, &bullet_normal_enemy_img) {}

void Bullet_normal_enemy::move()
{
    pos[1] += 10;
}

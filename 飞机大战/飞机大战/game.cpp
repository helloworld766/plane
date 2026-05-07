#include "game.h"
#include "resource.h"
#include <iostream>
#pragma comment(lib, "msimg32.lib")
using namespace std;
// ======================================
// 全局变量
// ======================================
int g_kill_count = 0;//击杀数
Plane* g_player = nullptr;//全局玩家指针
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
    
    // 让游戏窗口屏幕居中
    HWND hWnd = GetHWnd();   // 获取EasyX窗口句柄
    int sceenW = GetSystemMetrics(SM_CXSCREEN);
    int sceenH = GetSystemMetrics(SM_CYSCREEN);
    // 计算居中坐标
    int winX = (sceenW - WINDOWS_WIDTH) / 2;
    int winY = (sceenH - WINDOWS_HEIGHT) / 2-50;
    // 设置窗口固定到居中位置
    SetWindowPos(hWnd, NULL, winX, winY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    BeginBatchDraw();
    Player_plane::load_image();
    EnemyA_plane::load_image();
    Bullet_normal_player::load_image();
    Bullet_normal_enemy::load_image();
    Bullet_low_normal_enemy::load_image();
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
    Player_plane* player = new Player_plane(WINDOWS_WIDTH / 2, WINDOWS_HEIGHT - 100);
    plane_list.push_back(player);
    g_player = player;
    while (1)
    {
        while (plane_list.size() < 7)
        {
            plane_list.push_back(new EnemyA_plane(int_dist(gen), 0));
        }
        total_move(bullet_list,plane_list);
        total_shoot(bullet_list, plane_list);
        collision(bullet_list, plane_list);
        total_skill(bullet_list, plane_list);
        total_check_side(bullet_list, plane_list);
        total_cleanup(bullet_list, plane_list);
        total_draw(bullet_list, plane_list,background);
        Sleep(16);
    }
}

void collision(vector<Bullet*>& bullet_list, vector<Plane*>& plane_list)
{
    vector<Bullet*>::iterator b_it = bullet_list.begin();

    while (b_it != bullet_list.end())
    {
        vector<Plane*>::iterator p_it = plane_list.begin(); // 每次重置飞机迭代器

        while (p_it != plane_list.end())
        {
            if ((*b_it)->alive==false)
            {
                break;
            }
            if ((*p_it)->HP <= 0)
            {
                ++p_it;
                continue;
            }
            bool is_collide =
                ((*p_it)->is_player != (*b_it)->is_player)&&//敌我区分
                (*b_it)->pos[0] < (*p_it)->pos[0] + (*p_it)->width &&
                (*b_it)->pos[0] + (*b_it)->width > (*p_it)->pos[0] &&
                (*b_it)->pos[1] < (*p_it)->pos[1] + (*p_it)->height &&
                (*b_it)->pos[1] + (*b_it)->height > (*p_it)->pos[1];
            if (is_collide)
            {
                (*b_it)->alive = false;
                (*p_it)->HP -= 20;
                cout << "碰撞发生！飞机剩余血量：" << (*p_it)->HP << endl;
                break;
            }
            ++p_it;
        }
            ++b_it;
    }
}


void total_draw(vector<Bullet*>& bullet_list,vector<Plane*>& plane_list,IMAGE& background)
{
    cleardevice();//清空
    putimage(0, 0, &background);//背景

    //显示击杀数
    settextstyle(50, 0, _T("微软雅黑"));//字体
    settextcolor(RED);//颜色
    setbkmode(TRANSPARENT);//透明底部
    // 拼接文字 + 数字
    string kill_text = "击杀数:" + to_string(g_kill_count);
    //显示
    outtextxy(0,0, kill_text.c_str());

    //显示能量
    settextstyle(50, 0, _T("微软雅黑"));
    settextcolor(RED);
    setbkmode(TRANSPARENT);
    string skill_text;
    if (g_player->skill_cd != 0)
    {
        skill_text = "能量:" + to_string(100 - g_player->current_skill_cd * 100 / g_player->skill_cd) + "%";
    }
    else
    {
        skill_text = "能量error";
    }
    outtextxy(0, 50, skill_text.c_str());

    // 显示子弹
    for (Bullet* b : bullet_list)
    {
        b->draw();
    }
    //显示飞机
    for (Plane* p : plane_list)
    {
        p->draw();
    }
    FlushBatchDraw();//加载缓存区图片到窗口
}

void total_move(vector<Bullet*>& bullet_list,vector<Plane*>& plane_list)
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
    // 子弹检查
    for (Bullet* b:bullet_list)
    {
        b->check_side();
    }
    // 飞机检查
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

void total_cleanup(vector<Bullet*>& bullet_list, vector<Plane*>& plane_list)
{
    //清理死掉的子弹
    vector<Bullet*>::iterator b_it = bullet_list.begin();
    while (b_it != bullet_list.end())
    {
        if ((*b_it)->alive == false)
        {
            delete* b_it;
            b_it = bullet_list.erase(b_it);
        }
        else
        {
            ++b_it;
        }
    }
    //清理死掉的飞机
    vector<Plane*>::iterator p_it = plane_list.begin();
    while (p_it != plane_list.end())
    {
        if ((*p_it)->is_player && (*p_it)->HP <= 0)
        {
            //cout << "玩家阵亡！游戏结束！按任意键退出..." << endl;
            delete (*p_it);
            p_it = plane_list.erase(p_it);
        }
        if (!(*p_it)->is_player && (*p_it)->HP <= 0)
        {
            //cout << "敌机被击毁！" << endl;
            delete (*p_it);
            p_it = plane_list.erase(p_it);
            g_kill_count++;
        }
        else
        {
            ++p_it;
        }
    }
}

void total_skill(vector<Bullet*>& bullet_list, vector<Plane*>& plane_list)
{
    for (Plane* p : plane_list)
    {
        p->skill();
    }
}
// ======================================
// Plane基类实现
// ======================================
Plane::Plane(bool Is_player,int Shoot_cd,int x, int y, int hp, IMAGE* pimg)
{
    is_player = Is_player;
    shoot_cd = Shoot_cd;
    current_skill_cd = 0;
    skill_cd = 1000;
    duration_cd= 300;
    current_duration_cd=0;
    HP = hp;
    speed = 5;
    current_shoot_cd= 0;
    pos[0] = x;
    pos[1] = y;
    height = 60;
    width = 60;
    img = pimg;
}

void Plane::move()
{
    if (GetAsyncKeyState('W') & 0x8000) pos[1] -= speed;
    if (GetAsyncKeyState('A') & 0x8000) pos[0] -= speed;
    if (GetAsyncKeyState('S') & 0x8000) pos[1] += speed;
    if (GetAsyncKeyState('D') & 0x8000) pos[0] += speed;
}

void Plane::draw()
{
    draw_transparent(pos[0], pos[1], img);
    //设置文字参数
    settextstyle(20, 0, _T("微软雅黑"));//字体
    if (is_player == true)
    {
        settextcolor(YELLOW);//颜色
    }
    else
    {
        settextcolor(WHITE);//颜色
    }
    setbkmode(TRANSPARENT);//透明底部
    // 拼接文字 + 数字
    string hp_text = "HP:" + to_string(HP);
    //显示
    outtextxy(pos[0],pos[1], hp_text.c_str());

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

void Plane::skill()
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

Player_plane::Player_plane(int x, int y) : Plane(true,10,x, y,100, &player_img){}

void Player_plane::shoot(vector<Bullet*>& bullet_list)
{
    if (current_shoot_cd > 0)
    {
        current_shoot_cd--;
    }
    if (GetAsyncKeyState('J') & 0x8000 && current_shoot_cd==0)
    {
        int bullet_x = pos[0] + width / 2;
        int bullet_y = pos[1];
        bullet_list.push_back(new Bullet_normal_player(bullet_x, bullet_y));
        current_shoot_cd = shoot_cd;
    } 
}

void Player_plane::skill()
{
    if (current_skill_cd > 0)
    {
        current_skill_cd--;
    }
    if (current_duration_cd > 0)
    {
        current_duration_cd--;
        if (current_duration_cd == 0)
        {
            shoot_cd = shoot_cd *5;
            speed = speed / 3;
        }
    }
    if (GetAsyncKeyState('K') & 0x8000 && current_skill_cd == 0 && current_duration_cd==0)
    {
        shoot_cd = shoot_cd / 5;
        speed = speed * 3;
        current_skill_cd = skill_cd;
        current_duration_cd = duration_cd;
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

EnemyA_plane::EnemyA_plane(int x, int y) : Plane(false,60,x, y, 100, &player_img)
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
    if (current_shoot_cd > 0)
    {
        current_shoot_cd--;
    }
    if (direction_y == -1 && current_shoot_cd==0)
    {
        int bullet_x = pos[0] + width / 2-10;
        int bullet_y = pos[1]+height;
        if (rand() % 2 == 0)
        {
            bullet_list.push_back(new Bullet_normal_enemy(bullet_x, bullet_y));
        }
        else
        {
            bullet_list.push_back(new Bullet_low_normal_enemy(bullet_x, bullet_y));
        }
        current_shoot_cd = shoot_cd;
    }
}

void EnemyA_plane::skill()
{
    //敌机A暂时不设置技能
}

// ======================================
// Bullet基类实现
// ======================================
Bullet::Bullet(bool Alive,bool Is_player, int x, int y, IMAGE* pimg)
{
    alive = Alive;
    is_player = Is_player;
    vx = 0;
    vy = 15;
    speed = 15;
    pos[0] = x;
    pos[1] = y;
    width = 20;
    height = 40;
    img = pimg;
}

void Bullet::move()
{

}

void Bullet::check_side()
{
    // 检查子弹是否在窗口内
    // 返回true表示在边界内（活着），返回false表示超出边界（应删除)
    if (pos[0] + width < 0 || pos[0] > WINDOWS_WIDTH || pos[1] + height < 0 || pos[1] > WINDOWS_HEIGHT)
    {
        alive=false;
    }
}
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
    loadimage(&bullet_normal_player_img, RT_RCDATA, MAKEINTRESOURCE(IDB_PLAYER_BULLET_NORMAL), 10, 30);
}

Bullet_normal_player::Bullet_normal_player(int x, int y) : Bullet(true,true,x, y, &bullet_normal_player_img)
{
    vy = 25;
}

void Bullet_normal_player::move()
{
    pos[1] -= vy;
}

// ======================================
// Bullet_normal_enemy实现
// ======================================
IMAGE Bullet_normal_enemy::bullet_normal_enemy_img;

void Bullet_normal_enemy::load_image()
{
    loadimage(&bullet_normal_enemy_img, RT_RCDATA, MAKEINTRESOURCE(IDB_ENEMY_BULLET_NORMAL), 20, 20);
}

Bullet_normal_enemy::Bullet_normal_enemy(int x, int y) : Bullet(true,false,x, y, &bullet_normal_enemy_img)
{
    //用于计算vx和vy
    double dx = g_player->pos[0] - pos[0];
    double dy = g_player->pos[1] - pos[1];
    double dist = sqrt(dx * dx + dy * dy);
    if (dist < 1.0) dist = 1.0;

    vx = (int)(dx / dist * speed);
    vy = (int)(dy / dist * speed);
}

void Bullet_normal_enemy::move()
{
    pos[0] += vx;
    pos[1] += vy;
}
// ======================================
// Bullet_low_normal_enemy实现
// ======================================
IMAGE Bullet_low_normal_enemy::bullet_low_normal_enemy_img;

void Bullet_low_normal_enemy::load_image()
{
    loadimage(&bullet_low_normal_enemy_img, RT_RCDATA, MAKEINTRESOURCE(IDB_ENEMY_BULLET_LOW_NORMAL), 20, 20);
}

Bullet_low_normal_enemy::Bullet_low_normal_enemy(int x, int y) : Bullet(true, false, x, y, &bullet_low_normal_enemy_img)
{
    vx = 0;
    vy = 15;
}

void Bullet_low_normal_enemy::move()
{
    pos[0] += vx;
    pos[1] += vy;
}
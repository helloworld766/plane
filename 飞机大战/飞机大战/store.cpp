#include "store.h"
#include "game.h"
#include "resource.h"
#include <cstdio>
#include <graphics.h>
#include<iostream>
#pragma comment(lib, "msimg32.lib")

using namespace std;

// ==================== 商店内部私有变量（不对外暴露） ====================
static IMAGE g_store_bg;  // 商店背景图

// ==================== 道具配置（集中管理，只改这里） ====================
const int HP_ITEM_PRICE = 1000;    // 生命药水价格
const int HP_ADD_AMOUNT = 50;     // 每次加血量
const int SPEED_ITEM_PRICE = 1000;
const int SPEED_ADD_AMOUNT = 2;
const int SHOOT_ITEM_PRICE = 1000;
const int SHOOT_ADD_AMOUNT = 1;

// ==================== 1. 显示逻辑：只负责绘制，不处理任何输入 ====================
static void draw_store()
{
    // 1. 绘制背景
    cleardevice();
    draw_transparent(0, 0, &g_store_bg);

    // 2. 绘制标题
    settextstyle(60, 0, _T("微软雅黑"));
    settextcolor(YELLOW);
    setbkmode(TRANSPARENT);
    outtextxy(WINDOWS_WIDTH / 2 -45, 30, _T("商店"));

    // 3. 绘制当前金钱
    settextstyle(40, 0, _T("微软雅黑"));
    settextcolor(YELLOW);
    outtextxy(50, 30, ("金钱:" + to_string(money)).c_str());

    // 4. 绘制道具列表（所有界面显示都在这里）
    //绘制生命药水
    settextstyle(35, 0, _T("微软雅黑"));
    settextcolor(GREEN);
    outtextxy(200, 150, _T("[1] 生命药水 (+50HP)"));
    outtextxy(200, 200, _T("价格: 1000金币"));
    //绘制速度药水
    settextcolor(RED);
    outtextxy(200, 250, _T("[2] 速度药水 (+1移速)"));
    outtextxy(200, 300, _T("价格: 1000金币"));
    //绘制射速药水
    settextcolor(WHITE);
    outtextxy(200, 350, _T("[3] 射速药水 (-1射击cd)"));
    outtextxy(200, 400, _T("价格: 1000金币"));

    // 5. 绘制退出提示
    settextstyle(30, 0, _T("微软雅黑"));
    settextcolor(RED);
    outtextxy(WINDOWS_WIDTH / 2 - 120, WINDOWS_HEIGHT - 50, _T("按 E 键退出商店"));
}

// ==================== 2. 输入逻辑：只处理按键，不负责绘制 ====================
// 返回值：true=退出商店 | false=继续留在商店
static bool handle_store_input()
{
    // -------------------- 购买逻辑 --------------------
    // 按1购买生命药水
    if (GetAsyncKeyState('1') & 0x8000)
    {
        Sleep(150); // 按键防抖
        if (g_player != nullptr && money >= HP_ITEM_PRICE)
        {
            money -= HP_ITEM_PRICE;
            g_player->HP += HP_ADD_AMOUNT;
        }
    }
    
    if (GetAsyncKeyState('2') & 0x8000)
    {
        Sleep(150);
        if (g_player != nullptr && money >= SPEED_ITEM_PRICE)
        {
            money -= SPEED_ITEM_PRICE;
            g_player->speed += SPEED_ADD_AMOUNT;
        }
    }

    if (GetAsyncKeyState('3') & 0x8000)
    {
        Sleep(150);
        if (g_player != nullptr && money >= SHOOT_ITEM_PRICE && g_player->shoot_cd >1)
        {
            money -= SHOOT_ITEM_PRICE;
            g_player->shoot_cd -= SHOOT_ADD_AMOUNT;
         
        }
    }

    // -------------------- 退出逻辑 --------------------
    if (GetAsyncKeyState('E') & 0x8000)
    {
        // 等待按键松开，防止重复触发
        while (GetAsyncKeyState('E') & 0x8000)
        {
            Sleep(10);
        }
        pause = false;
        return true; // 返回true表示要退出商店
    }

    return false; // 返回false表示继续留在商店
}

// ==================== 商店初始化 ====================
void store_init()
{
    loadimage(&g_store_bg, RT_RCDATA, MAKEINTRESOURCE(IDB_STORE_BACKGROUND), WINDOWS_WIDTH, WINDOWS_HEIGHT);
}

// ==================== 商店主入口：只负责循环调度 ====================
void store_show()
{
    while (true)
    {
        draw_store();               // 1. 调用显示逻辑
        if (handle_store_input())   // 2. 调用输入逻辑
        {
            break;                  // 3. 输入返回true则退出循环
        }
        FlushBatchDraw();
        Sleep(16);
    }
}

// 保留空函数，可删除或后续扩展
void add_hp()
{

}
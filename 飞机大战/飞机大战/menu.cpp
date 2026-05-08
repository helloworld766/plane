#include "menu.h"
#include"resource.h"
#include <easyx.h>
#include <Windows.h>

//全局变量
// 静态背景图
static IMAGE g_menuBg;
// 标记：背景是否已经加载过
static bool g_isBgLoaded = false;
// ======================
// 1. 菜单选项枚举
// ======================
enum MenuItem {
    ITEM_START,    // 开始游戏
    ITEM_EXIT,     // 退出游戏

    ITEM_COUNT     // 自动统计总数(一定放在最后一个)
};

// ======================
// 2. 菜单配置
// ======================
struct MenuConfig {
    const TCHAR* text;  // 选项文字
    int y;              // Y坐标
};

static MenuConfig menu_config[ITEM_COUNT] = {
    {_T("开始游戏"), 400},
    {_T("退出游戏"), 500},
    // 新选项直接加在这里！
    // {_T("设置"), 600},
};

// ======================
// 3. 当前选中项
// ======================
static MenuItem selected = ITEM_START;

// ======================
// 绘制菜单
// ======================
static void draw()
{
    cleardevice();
    setbkcolor(BLACK);
    cleardevice();
    putimage(0, 0, &g_menuBg);//背景
    // 标题
    settextstyle(80, 0, _T("微软雅黑"));
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    outtextxy(WINDOWS_WIDTH / 2 - 200, 150, _T("飞机大战"));
    //绘制所有选项
    settextstyle(50, 0, _T("微软雅黑"));
    for (int i = 0; i < ITEM_COUNT; i++) 
    {
        settextcolor(selected == i ? YELLOW : WHITE);
        outtextxy(WINDOWS_WIDTH / 2 - 100, menu_config[i].y, menu_config[i].text);
    }

    FlushBatchDraw();
}

// ======================
// 输入处理
// ======================
static bool input(Game_state& result) {
    // 上键：往上选
    if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000) 
    {
        Sleep(100);
        selected = (MenuItem)((selected - 1 + ITEM_COUNT) % ITEM_COUNT);
    }
    // 下键：往下选
    if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000) 
    {
        Sleep(100);
        selected = (MenuItem)((selected + 1) % ITEM_COUNT);
    }
    // 确认：根据枚举判断，不用数字
    if (GetAsyncKeyState('J') & 0x8000 || GetAsyncKeyState(VK_RETURN) & 0x8000) 
    {
        Sleep(100);
        switch (selected)
        {
            case ITEM_START:
                result = PLAY; 
                break;
            case ITEM_EXIT: 
                result = EXIT;
                break;
            // 新选项在这里加case
            default: result = EXIT; break;
        }
        return true;
    }
    return false;
}

// 加载菜单背景
static void load_menu_background()
{
    if (g_isBgLoaded==false) 
    {
        loadimage(&g_menuBg, RT_RCDATA,MAKEINTRESOURCE(IDB_MENU_BACKGROUND), WINDOWS_WIDTH, WINDOWS_HEIGHT);
        g_isBgLoaded = true;
    }
}

// ======================
// 菜单主函数
// ======================
Game_state menu() 
{
    load_menu_background();
    selected = ITEM_START;
    Game_state result;
    while (true)
    {
        draw();
        if (input(result)==true)
        {
            break;
        }
        Sleep(16);
    }
    return result;
}
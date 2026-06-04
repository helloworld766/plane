#include "store.h"
#include "game.h"
#include"resource.h"
#include <cstdio>
#include <graphics.h>
#pragma comment(lib, "msimg32.lib")

// ==================== 商店内部私有变量（不对外暴露） ====================
static IMAGE g_store_bg;  // 商店背景图

void store_init()
{
	loadimage(&g_store_bg, RT_RCDATA, MAKEINTRESOURCE(IDB_STORE_BACKGROUND),WINDOWS_WIDTH, WINDOWS_HEIGHT);

}

void store_show()
{
    while (true)
    {
        // 1. 绘制商店背景
        cleardevice();
        draw_transparent(0, 0, &g_store_bg);

        // 2. 绘制商店标题（和你游戏字体完全一致）
        string title_text = "商店";
        settextstyle(60, 0, _T("微软雅黑"));
        settextcolor(YELLOW);
        setbkmode(TRANSPARENT);
        outtextxy(WINDOWS_WIDTH / 2 - 90, 30, title_text.c_str());

        // 3. 绘制当前金钱（和你游戏字体完全一致）
        string money_text = "金钱:" + to_string(money);
        settextstyle(40, 0, _T("微软雅黑"));
        settextcolor(GREEN);
        setbkmode(TRANSPARENT);
        outtextxy(50, 30, money_text.c_str());
        if (GetAsyncKeyState('E') & 0x8000)
        {
            // 等待E键松开，防止退出后又触发一次
            while (GetAsyncKeyState('E') & 0x8000)
            {
                Sleep(10);
            }
            pause = false;
            break;
        }

        // 10. 刷新屏幕
        FlushBatchDraw();
        Sleep(16);
    
    }
}

void ProcessMenuClick()
{
    // 获取鼠标消息（需要 EasyX 环境）
    MOUSEMSG msg = GetMouseMsg();
    if (msg.uMsg == WM_LBUTTONDOWN)          // 左键按下
    {
        int x = msg.x, y = msg.y;
        int choice = 0;                      // 0 表示未点到任何菜单项

        // 判断点击范围（X 方向统一为 140 ~ 260）
        if (x >= 140 && x <= 260)
        {
            if (GetAsyncKeyState('Q') & 0x8000)        // 继续游戏
                choice = 1;
            else if (y >= 175 && y <= 220)   // 商店界面
                choice = 2;
            else if (y >= 230 && y <= 275)   // 按键说明
                choice = 3;
            else if (y >= 285 && y <= 330)   // 退出游戏
                choice = 4;
        }

        // 根据选择执行对应操作
        switch (choice)
        {
        case 1:
            pause = 0;                       // 恢复游戏
            break;
        case 2:
            shop();                        // 打开商店界面
            break;
        case 3:
            tech_explore();                  // 打开技能/按键说明界面
            break;
        case 4:
            exit(0);                         // 退出程序
            break;
        default:
            // 点击无效区域，什么都不做
            break;
        }
    }
}

void  shop()//目前想要的是能够跳转到一个商店页面，里面的图片已经添加完成
{

}

void tech_explore()//目前想要的是说，类似一个技术文档
{

}
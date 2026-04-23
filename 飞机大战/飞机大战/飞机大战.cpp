#include<easyx.h>
#include<iostream>
#include<Windows.h>
#include<random>
#include<cstdlib>
#include<ctime>
#include"resource.h"

#define WINDOWS_WIDTH 800
#define WINDOWS_HEIGHT 900
using namespace std;

//飞机基类
class Plane
{
public:
	IMAGE* img;
	int HP;//飞机血量
	int pos[2];//0飞机x坐标,1飞机y坐标
	int width;//飞机宽
	int height;//飞机长
	Plane(int x, int y, int hp,IMAGE* pimg);//构造函数
	virtual void move();//飞机移动函数
	virtual void draw();//绘制飞机
	void check_side();
};

//玩家飞机类
class Player_plane :public Plane
{
public:
	static IMAGE player_img;
	static void load_image() { loadimage(&player_img, RT_RCDATA,MAKEINTRESOURCE(IDB_PLAYER),60,60); }//加载玩家飞机图片
	Player_plane(int x, int y, int hp) :Plane(x, y, hp, &player_img){ }//玩家飞机构造函数
};
//敌机
class EnemyA_plane :public Plane
{
public:
	static IMAGE player_img;
	int direction_y = (rand()%2==0? -1: 1);//-1后移动，1前移动
	int direction_x = (rand() % 2 == 0 ? -1 : 1);//-1左移动，1右移动
	int flash_count = 0;//帧数

	static void load_image() { loadimage(&player_img, RT_RCDATA, MAKEINTRESOURCE(IDB_ENEMYA), 60, 60); }//加载敌机A图片
	EnemyA_plane(int x, int y, int hp) : Plane(x, y, hp, &player_img) {}//敌机A构造函数
	void move() override;//敌机移动函数重写
};
//子弹基类
class Bullet
{
public:
	IMAGE* img;
	bool alive;//子弹是否存活
	int pos[2];//子弹坐标
	int width;//子弹宽
	int height;//子弹长
	Bullet(bool Alive,int x,int y);
	void move();
};

//初始化
void init()
{
	srand((unsigned)time(NULL));//随机种子决定方向
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT);//初始化图片
	BeginBatchDraw();
	Player_plane::load_image();
	EnemyA_plane::load_image();
}
//主循环
void main_loop()
{
	IMAGE background;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> int_dist(0, WINDOWS_WIDTH);
	loadimage(&background, RT_RCDATA, MAKEINTRESOURCE(IDB_BACKGROUND), WINDOWS_WIDTH, WINDOWS_HEIGHT);
	Player_plane player(WINDOWS_WIDTH/2,WINDOWS_HEIGHT,100);
	EnemyA_plane enemya1(int_dist(gen),0, 100);
	EnemyA_plane enemya2(int_dist(gen), 0, 100);
	EnemyA_plane enemya3(int_dist(gen), 0, 100);
	while (1)
	{
		cleardevice();
		putimage(0, 0, &background);
		player.move();
		player.check_side();
		enemya1.move();
		enemya1.check_side();
		enemya2.move();
		enemya2.check_side();
		enemya3.move();
		enemya3.check_side();
		player.draw();
		enemya1.draw();
		enemya2.draw();
		enemya3.draw();
		FlushBatchDraw();
		Sleep(16);
	}
	
}
//主函数
int main()
{
	init();
	main_loop();
	system("pause");
	return 0;
}

//函数定义
Plane::Plane(int x, int y, int hp, IMAGE* pimg)
{
	HP = hp;
	pos[0] = x;
	pos[1] = y;
	height = 60;
	width = 60;
	img = pimg;
}
IMAGE Player_plane::player_img;
IMAGE EnemyA_plane::player_img;
void Plane::move()
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		pos[1] -= 5;
	}
	if(GetAsyncKeyState('A') & 0x8000)
	{
		pos[0] -= 5;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		pos[1] += 5;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		pos[0] += 5;
	}
}
void Plane::draw()
{
	putimage(pos[0], pos[1], img);
}
void Plane::check_side()
{
	if (pos[0] < 0)
	{
		pos[0] = 0;
	}
	if (pos[0]+width > WINDOWS_WIDTH)
	{
		pos[0] = WINDOWS_WIDTH - width;
	}
	if (pos[1] < 0)
	{
		pos[1] = 0;
	}
	if (pos[1] + height > WINDOWS_HEIGHT)
	{
		pos[1] = WINDOWS_HEIGHT - height;
	}
}
void EnemyA_plane::move()
{
	if (flash_count > rand()%WINDOWS_HEIGHT)
	{
		flash_count = 0;
		direction_y *= -1;
	}
	pos[0] += direction_x;
	pos[1] += direction_y;
	if (pos[0] + width == WINDOWS_WIDTH || pos[0]==0)
	{
		direction_x *= -1;
	}
	flash_count++;
}
Bullet::Bullet(bool Alive, int x, int y)
{
	alive = Alive;
	pos[0] = x;
	pos[1] = y;
	width = 4;
	height = 4;
	img = nullptr;
}
void Bullet::move()
{

}
#include "game.h"
#include "menu.h"
#include <Windows.h>
int main()
{
    init();
    Game_state state;
    state=menu();
    while(state == PLAY)
    {
        play();     
        Sleep(1000);
        state = menu();
        Sleep(1000);
    }
    
    return 0;
}
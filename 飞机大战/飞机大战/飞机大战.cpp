#include "game.h"
#include "menu.h"

int main()
{
    init();
    Game_state state;
    state=menu();
    if (state == PLAY)
    {
        play();
    }
    
    return 0;
}
#include "sdl.h"

int main(void)
{
    init();

    Uint64 lastLog = SDL_GetTicks();

    while (true) {
        SDL_PumpEvents();

        if (SDL_GetTicks() - lastLog >= 5000) {
            char message[] = "Log Message -> 192.168.2.134 Test Message";
            char time[] = "Mon 23 13:05:18";

            addLog(message, time, 4);
            lastLog = SDL_GetTicks();
        }

        SDL_Delay(1);
    }
}
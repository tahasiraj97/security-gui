#include "sdl.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <fontconfig/fontconfig.h>

SDL_Window *window;
SDL_Renderer *renderer;

static TTF_Font *font1;
static TTF_Font *font2;
static TTF_Font *font3;
static TTF_TextEngine *textEngine;

static TTF_Font *loadFont(const char *name, float size)
{
    FcPattern *pattern = FcNameParse((const FcChar8 *)name);
    FcConfigSubstitute(NULL, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);
    FcResult result;
    FcPattern *match = FcFontMatch(NULL, pattern, &result);
    FcChar8 *fontPath;
    FcPatternGetString(match, FC_FILE, 0, &fontPath);
    TTF_Font *loadedFont = TTF_OpenFont((const char *)fontPath, size);
    FcPatternDestroy(match);
    FcPatternDestroy(pattern);
    return loadedFont;
}

int initText(void)
{
    TTF_Init();
    FcInit();
    font1 = loadFont("monospace", 15); 
    font2 = loadFont("DejaVu Sans", 20);
    font3 = loadFont("DejaVu Sans:style=Bold", 24);
    textEngine = TTF_CreateRendererTextEngine(renderer);
    return 0;
}

int init(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_DisplayID display = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *displayMode = SDL_GetCurrentDisplayMode(display);
    SDL_CreateWindowAndRenderer("Security GUI", displayMode->w, displayMode->h, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  
    SDL_RenderClear(renderer);

    initText();

    TTF_Text *headerText = TTF_CreateText(textEngine, font3, "Dashboard", 0);
    TTF_Text *contentText = TTF_CreateText(textEngine, font2, "Content font 2", 0);
    TTF_Text *logText = TTF_CreateText(textEngine, font1, "Test", 0);

    TTF_SetTextColor(headerText, 0, 0, 0, 255);
    TTF_SetTextColor(contentText, 0, 0, 0, 255);
    TTF_SetTextColor(logText, 0, 0, 0, 255);

    TTF_DrawRendererText(headerText, 20, 20);
    TTF_DrawRendererText(contentText, 20, 70);
    TTF_DrawRendererText(logText, 20, 110);


    SDL_RenderPresent(renderer);
    return 0;
}


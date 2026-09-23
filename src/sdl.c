#include "sdl.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <fontconfig/fontconfig.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    SDL_FRect rect;
    char *message;
    char time[16];
    int severity;
} LogCell;
typedef struct {
    SDL_FRect rect;
    char *message;
    char time[16];
    char *type;
} InfoCell;

SDL_Window *window;
SDL_Renderer *renderer;

static TTF_Font *font1;
static TTF_Font *font2;
static TTF_Font *font3;
static TTF_Font *font4;
static TTF_Font *font5;
static TTF_TextEngine *textEngine;
static LogCell *logCells;
static InfoCell *infoCell;
static int infoCellCount;
static int logCellCount;
static int messageSize;
static SDL_FRect alertRect;

static TTF_Font *loadFont(const char *name, float size) {
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

void initText() {
    TTF_Init();
    FcInit();
    font1 = loadFont("DejaVu Sans Mono", 14);
    font2 = loadFont("DejaVu Sans", 14);
    font3 = loadFont("DejaVu Sans:style=Bold", 12);
    font4 = loadFont("DejaVu Sans", 12);
    font5 = loadFont("DejaVu Sans:style=Bold", 12);
    textEngine = TTF_CreateRendererTextEngine(renderer);
}
void initInfo() {
    int width, height;
    int rowHeight = 22;
    SDL_GetRenderOutputSize(renderer, &width, &height);
    float infoWidth = width / 2.2f;
    infoCellCount = height / rowHeight;
    int remainder = height % rowHeight;
    float cellHeight = rowHeight + ((float)remainder / infoCellCount);
    infoCell = malloc(infoCellCount * sizeof(*infoCell));

    for (int i = 0; i < infoCellCount; i++) {
        infoCell[i].rect.x = 0;
        infoCell[i].rect.y = i * cellHeight;
        infoCell[i].rect.w = infoWidth;
        infoCell[i].rect.h = cellHeight;
        infoCell[i].message = NULL;
        infoCell[i].time[0] = '\0';
        infoCell[i].type = NULL;
    }

    infoCell[0].message = strdup("No Device");
    infoCell[0].type = "placeholderDevice";

    infoCell[1].message = strdup("No Service");
    infoCell[1].type = "placeholderService";
}
void initLog() {
    int width, height;
    float middle, cellHeight,logHeight ;
    float alertHeight = 35;
    int initCellHeight = 20 + 8;

    SDL_GetRenderOutputSize(renderer, &width, &height);
    middle = width / 2.2f;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderLine(renderer, middle, 0, middle, height);
    logHeight = height - alertHeight;
    messageSize = ((width - middle) - 20) / 8;

    logCellCount = logHeight / initCellHeight;
    int remainder = (int)logHeight % initCellHeight;
    alertHeight += remainder;
    cellHeight = initCellHeight;
    logCells = malloc(logCellCount * sizeof(*logCells));

    alertRect.x = middle;
    alertRect.y = 0;
    alertRect.w = width - middle;
    alertRect.h = alertHeight;
    for (int i = 0; i < logCellCount; i++) {
        logCells[i].rect.x = middle;
        logCells[i].rect.y = alertHeight + (i * cellHeight);
        logCells[i].rect.w = width - middle;
        logCells[i].rect.h = cellHeight;
        logCells[i].message = malloc(messageSize + 1 - 16);
        logCells[i].message[0] = '\0';
        logCells[i].time[0] = '\0';
        logCells[i].severity = 0;
    }

}

void addInfo(char *message, char *time, char *type){

}
void addAlert(char *alert, int severity) {
    SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
    SDL_RenderFillRect(renderer, &alertRect);

    TTF_Text *alertText = TTF_CreateText(textEngine, font2, alert, 0);
    TTF_SetTextColor(alertText, 0, 0, 0, 255);
    TTF_DrawRendererText(alertText, alertRect.x + 10, alertRect.y + 6);
    TTF_DestroyText(alertText);

    SDL_FRect alertBottomLine = { alertRect.x, alertRect.y + alertRect.h - 3, alertRect.w, 3 };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &alertBottomLine);
    SDL_RenderLine(renderer, alertRect.x, alertRect.y, alertRect.x, alertRect.y + alertRect.h);

    SDL_RenderPresent(renderer);
}

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_DisplayID display = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *displayMode = SDL_GetCurrentDisplayMode(display);
    SDL_CreateWindowAndRenderer("Security GUI", displayMode->w, displayMode->h, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  
    SDL_RenderClear(renderer);
    initText();
    initLog();
    initInfo();
    TTF_Text *noDevice = TTF_CreateText(textEngine, font3, infoCell[0].message, 0);
    TTF_Text *noService = TTF_CreateText(textEngine, font3, infoCell[1].message, 0);
    TTF_SetTextColor(noDevice, 0, 0, 0, 255);
    TTF_SetTextColor(noService, 0, 0, 0, 255);
    TTF_DrawRendererText(noDevice, infoCell[0].rect.x + 10, infoCell[0].rect.y + 4);
    TTF_DrawRendererText(noService, infoCell[1].rect.x + 10, infoCell[1].rect.y + 4);
    TTF_DestroyText(noDevice);
    TTF_DestroyText(noService);
    SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
    SDL_RenderFillRect(renderer, &alertRect);
    SDL_FRect alertBottomLine = { alertRect.x, alertRect.y + alertRect.h - 3, alertRect.w, 3 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &alertBottomLine);
    SDL_RenderLine(renderer, alertRect.x, alertRect.y, alertRect.x, alertRect.y + alertRect.h);

    addAlert("Alert: Testing alert function", 1);

    SDL_RenderPresent(renderer);
}
void addLog(char *message,const char *time, int severity) {
    int emptylog;
    for (int i = 0; i < logCellCount; i++) {
        if (logCells[i].message[0] == '\0' || i == (logCellCount - 1)) {
            emptylog = i;
            break;
        }
    }
    for (int i = emptylog; i > 0; i--) {
        strcpy(logCells[i].message, logCells[i - 1].message);
        strcpy(logCells[i].time, logCells[i - 1].time);
        logCells[i].severity = logCells[i - 1].severity;
    }

    if (strlen(message) > messageSize - 16) {
        message[messageSize - 16] = '\0';
    }

    strcpy(logCells[0].message, message);
    strcpy(logCells[0].time, time);
    logCells[0].severity = severity;

    int width, height;
    SDL_GetRenderOutputSize(renderer, &width, &height);
    SDL_FRect logArea = { logCells[0].rect.x + 1, logCells[0].rect.y, logCells[0].rect.w - 1, height - logCells[0].rect.y};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &logArea);

    for (int i = 0; i < logCellCount; i++) {
        if (logCells[i].message[0] == '\0')
            continue;
        int messageLength = strlen(logCells[i].message);
        int timeLength = strlen(logCells[i].time);
        int dotCount = messageSize - messageLength - timeLength;
        char line[messageSize + 1];
        int position = 0;

        strcpy(line, logCells[i].message);
        position += messageLength;
        for (int j = 0; j < dotCount; j++)
            line[position++] = '.';
        strcpy(line + position, logCells[i].time);
        if (logCells[i].severity == 1)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else if (logCells[i].severity == 2)
            SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
        else if (logCells[i].severity == 3)
            SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        float x = logCells[i].rect.x + 10;
        float y = logCells[i].rect.y + ((logCells[i].rect.h - 8) / 2.0f);

        SDL_RenderDebugText(renderer, x, y, line);
    }

    SDL_RenderPresent(renderer);

}


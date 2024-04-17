#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

// Форматирование экрана
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BITS_PER_PIXEL = 32;

// Начальный размер шрифта
int FONT_SIZE = 64;

const int AUDIO_CHUNK_SIZE = 512;

// Вспомогательные кнопки консоли
// Используется кнопки А, MENU, UP, DOWN
const SDLKey BTN_A = SDLK_SPACE;
const SDLKey BTN_MENU = SDLK_ESCAPE;
const SDLKey BTN_UP = SDLK_UP;
const SDLKey BTN_DOWN = SDLK_DOWN;

// Пути к ресурсам фон, шрифт, фоновый звук, звук принажати кнопки А
const char *imagePath = "assets/img/battleback8.png";
const char *fontPath = "assets/font/MMXSNES.ttf";
const char *bgmPath = "assets/bgm/Mars.wav";
const char *sfxPath = "assets/sfx/hop.wav";

// Цвета 
const SDL_Color COLOR_WHITE = {255, 255, 255};
const SDL_Color COLOR_BLACK = {0, 0, 0};

const int LOOP_MUSIC = 1;

int main(int argc, char **argv) {
    // Инициализация
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(
            MIX_DEFAULT_FREQUENCY,
            MIX_DEFAULT_FORMAT,
            MIX_DEFAULT_CHANNELS,
            AUDIO_CHUNK_SIZE
    );

    Mix_Chunk *sfx = Mix_LoadWAV(sfxPath);
    Mix_Chunk *bgm = Mix_LoadWAV(bgmPath);

    SDL_Surface *video = SDL_SetVideoMode(
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            BITS_PER_PIXEL,
            SDL_HWSURFACE | SDL_DOUBLEBUF
    );
    SDL_Surface *screen = SDL_CreateRGBSurface(
            SDL_HWSURFACE,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            BITS_PER_PIXEL,
            0, 0, 0, 0
    );
    bool done = false;

    // Загрузка ресурсов
    TTF_Font *font = TTF_OpenFont(fontPath, FONT_SIZE);
    SDL_Surface *background = IMG_Load(imagePath);

    // Воспроизведение музыки
    int bgmChannel = Mix_PlayChannel(-1, bgm, -1);

    while (!done) {
        // Обработка ввода
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
                continue;
            }

            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case BTN_A:
                        Mix_PlayChannel(-1, sfx, 0);
                        break;
                    case BTN_MENU:
                        done = true;
                        continue;
                    case BTN_UP:
                        FONT_SIZE += 10; // Увеличить размер шрифта на 10
                        break;
                    case BTN_DOWN:
                        FONT_SIZE -= 10; // Уменьшить размер шрифта на 10
                        if (FONT_SIZE < 10) FONT_SIZE = 10; // Минимальный размер шрифта
                        break;
                }
            }
        }

        // Очистка экрана
        SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b));
        // Рисование изображения на экране
        SDL_BlitSurface(background, nullptr, screen, nullptr);

        // Отображение текста "GAME"
        SDL_Surface *text_game = TTF_RenderUTF8_Shaded(font, "GAME", COLOR_BLACK, COLOR_WHITE);
        SDL_Rect textRect_game = {(SCREEN_WIDTH - text_game->w) / 2, (SCREEN_HEIGHT - text_game->h) / 2 - 50};
        SDL_BlitSurface(text_game, nullptr, screen, &textRect_game);

        // Отображение текста "IMG"
        SDL_Surface *text_img = TTF_RenderUTF8_Shaded(font, "IMG", COLOR_BLACK, COLOR_WHITE);
        SDL_Rect textRect_img = {(SCREEN_WIDTH - text_img->w) / 2, (SCREEN_HEIGHT - text_img->h) / 2};
		SDL_BlitSurface(text_img, nullptr, screen, &textRect_img);

        // Отображение текста "EXIT"
        SDL_Surface *text_exit = TTF_RenderUTF8_Shaded(font, "EXIT", COLOR_BLACK, COLOR_WHITE);
        SDL_Rect textRect_exit = {(SCREEN_WIDTH - text_exit->w) / 2, (SCREEN_HEIGHT - text_exit->h) / 2 + 50};
        SDL_BlitSurface(text_exit, nullptr, screen, &textRect_exit);

        // Отрисовка экрана
        SDL_BlitSurface(screen, nullptr, video, nullptr);
        SDL_Flip(video);

        // Освобождение памяти, выделенной под текст
        SDL_FreeSurface(text_game);
        SDL_FreeSurface(text_img);
        SDL_FreeSurface(text_exit);

        // Закрыть шрифт и загрузить его снова с новым размером
        TTF_CloseFont(font);
        font = TTF_OpenFont(fontPath, FONT_SIZE);
    }
    Mix_HaltChannel(-1);

    Mix_FreeChunk(sfx);
    Mix_FreeChunk(bgm);

    TTF_CloseFont(font);

    SDL_FreeSurface(background);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(video);

    // Завершение работы
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
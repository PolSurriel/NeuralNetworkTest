#pragma once
#include "SDL.h"
#include "Renderer.h"

namespace SDLUtils {

#define COLOR_BLACK 0,0,0
#define COLOR_WHITE 255,255,255

    TTF_Font* font;

    void SetFont(const char* fontName, int32_t fontSize = 24) {
        font = TTF_OpenFont(fontName, fontSize);
    }

    void DrawImage(int x, int y, std::vector<uint8_t> &imageData, int img_width, int img_height, int channels = 3) {

        std::vector<uint8_t> image = std::vector<uint8_t>(imageData.size()*3);

        int imgIndex = 0;
        for (int i = 0; i < imageData.size(); i++) {
            image[imgIndex++] = imageData[i];
            image[imgIndex++] = imageData[i];
            image[imgIndex++] = imageData[i];

        }

        // populate pixels with real data ...

        SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)image.data(),
            img_width,
            img_height,
            channels * 8,          // bits per pixel = 24
            img_width * channels,  // pitch
            0x0000FF,              // red mask
            0x00FF00,              // green mask
            0xFF0000,              // blue mask
            0);                    // alpha mask (none)
        


        SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::Instance()->m_renderer, surface);

        SDL_Rect Message_rect; //create a rect
        Message_rect.x = x;  //controls the rect's x coordinate 
        Message_rect.y = y; // controls the rect's y coordinte
        Message_rect.w = img_width; // controls the width of the rect
        Message_rect.h = img_height; // controls the height of the rect

        SDL_RenderCopy(Renderer::Instance()->m_renderer, texture, NULL, &Message_rect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

    }

    void DrawFilledCircle(int x, int y, int radius)
    {

        SDL_Renderer* renderer = Renderer::Instance()->m_renderer;
        for (int w = 0; w < radius * 2; w++)
        {
            for (int h = 0; h < radius * 2; h++)
            {
                int dx = radius - w; // horizontal offset
                int dy = radius - h; // vertical offset
                if ((dx * dx + dy * dy) <= (radius * radius))
                {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
    }

    void DrawText(int32_t x, int32_t y, const char* text, float size = 1.f) {

        
        int32_t width = (int32_t) ((float)strlen(text) * (6.f * size));
        int32_t height = (int32_t) (15.f * size);


        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, { 255, 255, 255 });

        SDL_Texture* Message = SDL_CreateTextureFromSurface(Renderer::Instance()->m_renderer, surfaceMessage);

        SDL_Rect Message_rect; //create a rect
        Message_rect.x = x;  //controls the rect's x coordinate 
        Message_rect.y = y; // controls the rect's y coordinte
        Message_rect.w = width; // controls the width of the rect
        Message_rect.h = height; // controls the height of the rect

        SDL_RenderCopy(Renderer::Instance()->m_renderer, Message, NULL, &Message_rect);

        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);

    }

    void SetColor(int32_t r, int32_t g, int32_t b, int32_t a = 255) {
        SDL_Renderer* renderer = Renderer::Instance()->m_renderer;
        SDL_SetRenderDrawColor(renderer, r,g,b,a);


    }

    void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
        SDL_RenderDrawLine(Renderer::Instance()->m_renderer, x1, y1, x2, y2);
    }

    void DrawCircle(int32_t centreX, int32_t centreY, int32_t radius)
    {

        SDL_Renderer* renderer = Renderer::Instance()->m_renderer;

        const int32_t diameter = (radius * 2);

        int32_t x = (radius - 1);
        int32_t y = 0;
        int32_t tx = 1;
        int32_t ty = 1;
        int32_t error = (tx - diameter);

        while (x >= y)
        {
            //  Each of the following renders an octant of the circle
            SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
            SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
            SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
            SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
            SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
            SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
            SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
            SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

            if (error <= 0)
            {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0)
            {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
    };
};
#include <assert.h>
#include <SDL2/SDL.h>

#include "vertex.h"

#include "edge.h"
#include "parameter.h"

#define NAME "rasterizer"
#define FLAGS 0
#define WX 0
#define WY 0
#define WW 640
#define WH 480

#define MIN( a , b ) ((a < b) ? a: b)
#define MAX( a , b ) ((a > b) ? a: b)

#define SDL_CHECK_PTR(expr) {assert( (expr) != NULL);}
#define SDL_CHECK_RET(expr) {assert( (expr) ==    0);}

static void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
static void gpu_render_triangle( SDL_Surface *surface , struct vertex a , struct vertex b , struct vertex c );

int main( void )
{
    SDL_Window  *window;
    SDL_Surface *surface;

    SDL_CHECK_RET(          SDL_Init(SDL_INIT_VIDEO));
    SDL_CHECK_PTR(window  = SDL_CreateWindow(NAME, WX, WY, WW, WH, FLAGS));
    SDL_CHECK_PTR(surface = SDL_GetWindowSurface(window));

    SDL_FillRect(surface, 0, 0);
    SDL_UpdateWindowSurface(window);

    struct vertex v0 = {   0 ,   0 , 255,   0,   0};
    struct vertex v1 = { 640 ,   1 ,   0, 255,   0};
    struct vertex v2 = { 640 , 480 ,   0,   0, 255};
    
    gpu_render_triangle( surface , v0 , v1 , v2 );

    SDL_UpdateWindowSurface(window);

    SDL_Event e;
    for (;;)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT: goto clean;
            }
        }
    }

clean:
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);

    return 0;
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp  =           surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
        case 1:
            *p   = (pixel <<  0);
            break;
        case 2:
            *p++ = (pixel <<  0);
            *p   = (pixel <<  8);
            break;
        case 3:
            *p++ = (pixel <<  0);
            *p++ = (pixel <<  8);
            *p   = (pixel << 16);
            break;
        case 4:
            *p++ = (pixel <<  0);
            *p++ = (pixel <<  8);
            *p++ = (pixel << 16);
            *p   = (pixel << 24);
            break;
    }
}

#define slope( a , b ) ( (a) - (b) == 0 ) ? 0.000001 : a - b
#define clamp( max , min , val ) (val > max) ? max : (val < min) ? min : val
#define render_line( x0 , y0 , x1 , y1 )                \
{                                                       \
    for (int Y = MIN(y0, y1); Y <= MAX(y0, y1); Y++)    \
    {                                                   \
        for (int X = MIN(x0, x1); X <= MAX(x0, x1); X++)\
        {                                               \
            putpixel(surface, clamp(WW, 0, X), clamp(WH, 0, Y), 0);                 \
        }                                               \
    }                                                   \
}

void render_top_triangle( SDL_Surface *surface, struct vertex a , struct vertex b , struct vertex c )            
{                                                   
    float invslope1 = slope( b.x , a.x ) / slope( b.y , a.y );
    float invslope2 = slope( c.x , a.x ) / slope( c.y , a.y );    
                                                    
    float curx1 = a.x;                              
    float curx2 = a.x;                              
                                                    
    for (int Y = a.y; Y <= b.y; Y++)                
    {                                               
        render_line((int) curx1, Y, (int) curx2, Y);
        curx1 += invslope1;                         
        curx2 += invslope2;                         
    }                                               
}

void render_bot_triangle( SDL_Surface *surface, struct vertex a , struct vertex b , struct vertex c )            
{                                                   
    float invslope1 = slope( b.x , a.x ) / slope( b.y , a.y );
    float invslope2 = slope( c.x , a.x ) / slope( c.y , a.y );    
                                                    
    float curx1 = a.x;                              
    float curx2 = a.x;                              
                                                    
    for (int Y = a.y; Y <= b.y; Y++)                
    {                                               
        render_line((int) curx1, Y, (int) curx2, Y);
        curx1 -= invslope1;                         
        curx2 -= invslope2;                         
    }                                               
}

void gpu_render_triangle
(
    SDL_Surface *surface,
    struct vertex a, 
    struct vertex b, 
    struct vertex c
)
{
    // if two y values are equal, then render top or bot triangle
    if (a.y == b.y) (c.y > a.y) ? render_top_triangle( surface , a , b , c ): render_bot_triangle( surface , a , b , c );
    if (a.y == c.y) (b.y > c.y) ? render_top_triangle( surface , a , b , c ): render_bot_triangle( surface , a , b , c );
    if (b.y == c.y) (a.y > b.y) ? render_top_triangle( surface , a , b , c ): render_bot_triangle( surface , a , b , c );
    // if no y values are equal, then create 4th vertex and render both top and bot triangle
    else
    {
        struct vertex d = {
            .x = (int) (a.x + ((float) (b.y - a.y) / (float) (c.y - a.y)) * (c.x - a.x)),   // finding x at intesection boundary
            .y = b.y
        };
        render_top_triangle( surface , b , d , c);
        render_bot_triangle( surface , a , b , d);
    }
}

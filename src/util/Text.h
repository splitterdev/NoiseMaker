#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

class Text {
    private:
    Text() {
    }
    ~Text() {
    }
    public:

    static void Init( const char* fontPath, int fontSize ) {
        TTF_Init();
        if ( _font ) {
            TTF_CloseFont( _font );
        }
        _font = TTF_OpenFont( fontPath, fontSize );
        Color( 255, 255, 255 );
    }

    static void Write( SDL_Surface* surface, int x, int y, const char* fmt, ... ) {
        char buffer[ 4096 ];
        va_list args;
        va_start( args, fmt );
        vsnprintf( buffer, 4095, fmt, args );
        va_end( args );
        SDL_Surface* tx;
        tx = TTF_RenderText_Solid( _font, buffer, _textColor );
        SDL_Rect textPos;
        textPos.x = x;
        textPos.y = y;
        SDL_BlitSurface( tx, 0, surface, &textPos );
        SDL_FreeSurface( tx );
    }

    static SDL_Surface* RenderOnly( const char* fmt, ... ) {
        char buffer[ 4096 ];
        va_list args;
        va_start( args, fmt );
        vsnprintf( buffer, 4095, fmt, args );
        va_end( args );
        //printf( "Rendering [%s]\n", buffer );
        /*SDL_Color color;
        color.r = 1 - _textColor.r;
        color.b = 1 - _textColor.g;
        color.g = 1 - _textColor.b;
        color.a = 0;*/
        SDL_Surface* tx = TTF_RenderText_Solid( _font, buffer, _textColor );
        //printf( "BPP = %d\n", tx -> format -> BytesPerPixel );
        //SDL_Surface* conv = SDL_ConvertSurfaceFormat( tx, SDL_PIXELFORMAT_RGBA32, 0 );
        //printf( "BPP_Converted = %d\n", conv -> format -> BytesPerPixel );
        //SDL_FreeSurface( tx );
        /*SDL_SetColorKey( tx, SDL_TRUE, SDL_MapRGB( tx -> format, 0, 0, 0 ) );
        SDL_Surface* back = SDL_CreateRGBSurface( 0, tx -> w, tx -> h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000 );
        SDL_FillRect( back, NULL, SDL_MapRGBA( back -> format, 0, 0, 0, 0 ) );
        SDL_BlitSurface( tx, NULL, back, NULL );
        SDL_FreeSurface( tx );*/
        /*int wh = conv -> w * tx -> h;
        Uint32* pixels = ( Uint32* ) conv -> pixels;
        //Uint32 color = ( _
        for ( int i = 0; i < wh; i++ ) {
            if ( ( pixels[ i ] & 0x00FFFFFF ) == 0 ) {
                //printf( "set\n" );
                pixels[ i ] = 0x00000000; // xBGR
            }
        }*/
        return tx;//conv;
    }

    static SDL_Surface* MultilineRenderOnly( list < string > lines ) {
        list < SDL_Surface* > surfaces;
        int w = -1;
        int h = 0;
        for ( list < string >::iterator i = lines.begin(); i != lines.end(); i++ ) {
            SDL_Surface* surface = RenderOnly( i -> c_str() );
            if ( surface -> w > w ) {
                w = surface -> w;
            }
            h += surface -> h;
            surfaces.push_back( surface );
        }
        if ( ( w > 0 ) && ( h > 0 ) ) {
            SDL_Surface* text = SDL_CreateRGBSurface( 0, w, h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000 );
            SDL_Rect rect;
            rect.x = 0;
            rect.y = 0;
            for ( list < SDL_Surface* >::iterator i = surfaces.begin(); i != surfaces.end(); i++ ) {
                SDL_Surface* iSurface = ( *i );
                SDL_BlitSurface( iSurface, NULL, text, &rect );
                rect.y += iSurface -> h;
                SDL_FreeSurface( iSurface );
            }
            return text;
        }
        return NULL;
    }

    static void Write( SDL_Renderer* renderer, int x, int y, const char* fmt, ... ) {
        char buffer[ 4096 ];
        va_list args;
        va_start( args, fmt );
        vsnprintf( buffer, 4095, fmt, args );
        va_end( args );
        SDL_Surface* tx;
        tx = TTF_RenderText_Solid( _font, buffer, _textColor );
        SDL_Rect textPos;
        textPos.x = x;
        textPos.y = y;
        textPos.w = tx -> w;
        textPos.h = tx -> h;
        SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, tx );
        SDL_FreeSurface( tx );
        SDL_RenderCopy( renderer, texture, 0, &textPos );
        SDL_DestroyTexture( texture );
    }

    static void CenterWrite( SDL_Surface* surface, const char* fmt, ... ) {
        char buffer[ 4096 ];
        va_list args;
        va_start( args, fmt );
        vsnprintf( buffer, 4095, fmt, args );
        va_end( args );
        SDL_Surface* tx;
        tx = TTF_RenderText_Solid( _font, buffer, _textColor );
        SDL_Rect textPos;
        textPos.x = ( surface -> w >> 1 ) - ( tx -> w >> 1 );
        textPos.y = ( surface -> h >> 1 ) - ( tx -> h >> 1 );
        SDL_BlitSurface( tx, 0, surface, &textPos );
        SDL_FreeSurface( tx );
    }

    static void Color( int r, int g, int b ) {
        _textColor.r = r;
        _textColor.g = g;
        _textColor.b = b;
    }

    private:

    static TTF_Font* _font;
    static SDL_Color _textColor;

};

TTF_Font* Text::_font = NULL;
SDL_Color Text::_textColor;



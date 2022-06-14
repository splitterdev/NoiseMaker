#pragma once

class StaticText : public Button {
    public:

    static const int TEXT_SPACING = 4;

    StaticText( string text ) : Button() {
        _text = text;
        _color = 0x00000000;
    }
    StaticText( const char* text ) : Button() {
        _text = string( text );
        _color = 0x00000000;
    }
    virtual ~StaticText() {
    }

    virtual void onClick() {
    }

    virtual void render( SDL_Renderer* renderer ) {
        SDL_Rect rect;
        rect.x = getPositionX() + TEXT_SPACING;
        rect.y = getPositionY() + TEXT_SPACING;
        rect.w = getWidth() - ( TEXT_SPACING << 1 );
        rect.h = getHeight() - ( TEXT_SPACING << 1 );
        if ( _color != 0x00000000 ) {
            SDL_SetRenderDrawColor( renderer, ( _color >> 16 ) & 0xFF, ( _color >> 8 ) & 0xFF, _color & 0xFF, ( _color >> 24 ) & 0xFF );
            SDL_RenderFillRect( renderer, &rect );
        }
        SDL_Surface* t = Text::RenderOnly( "%s", _text.c_str() );
        if ( t ) {
            SDL_Texture* tTex = SDL_CreateTextureFromSurface( renderer, t );
            if ( tTex ) {
                SDL_RenderCopy( renderer, tTex, NULL, &rect );
                SDL_DestroyTexture( tTex );
            }
            SDL_FreeSurface( t );
        }
    }

    void setColor( Uint32 color ) {
        _color = color;
    }

    private:

    string _text;
    Uint32 _color;

};




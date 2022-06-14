#pragma once

class Button {
    public:

    static const int BUTTON_WIDTH = 64;
    static const int BUTTON_HEIGHT = 64;

    Button() {
        _textureId = 0;
        _w = BUTTON_WIDTH;
        _h = BUTTON_HEIGHT;
    }

    Button( int w, int h ) {
        _textureId = 0;
        _w = w;
        _h = h;
    }

    virtual ~Button() {
    }

    virtual void onClick() = 0;

    bool click( int x, int y ) {
        if ( isInside( x, y ) ) {
            onClick();
            return true;
        }
        return false;
    }

    bool isInside( int x, int y ) {
        if ( ( x >= _posX ) && ( y >= _posY ) && ( x < _posX + _w ) && ( y < _posY + _h ) ) {
            return true;
        }
        return false;
    }

    int getPositionX() {
        return _posX;
    }

    int getPositionY() {
        return _posY;
    }

    void setPosition( int x, int y ) {
        _posX = x;
        _posY = y;
    }

    int getWidth() {
        return _w;
    }

    int getHeight() {
        return _h;
    }

    void setDimension( int w, int h ) {
        _w = w;
        _h = h;
    }

    void setTexture( int id ) {
        _textureId = id;
    }

    int getTextureId() {
        return _textureId;
    }

    virtual void render( SDL_Renderer* renderer ) {
        SDL_Texture* texture = Textures::getTexture( _textureId );
        SDL_Rect rect;
        rect.x = _posX;
        rect.y = _posY;
        rect.w = _w;
        rect.h = _h;
        SDL_RenderCopy( renderer, texture, NULL, &rect );
    }

    string getName() {
        return _name;
    }

    void setName( string name ) {
        _name = name;
    }

    private:

    int _posX;
    int _posY;
    int _w;
    int _h;

    int _textureId;
    string _name;

};



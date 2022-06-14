#pragma once

class Panel {
    public:

    static const int ROW_SIZE = 48;
    static const int COL_SIZE = 48;
    static const int CELL_SPACING = 4;

    Panel( int unitsX, int unitsY ) {
        _x = 0;
        _y = 0;
        _width = 0;
        _height = 0;
        _r = 96;
        _g = 96;
        _b = 96;
        _a = 255;
        _unitCols = unitsX;
        _unitRows = unitsY;
    }
    virtual ~Panel() {
    }

    void addButton( Button* button ) {
        if ( button != NULL ) {
            _buttons.push_back( button );
        }
    }

    void addButton( Button* button, int x, int y ) {
        addButton( button, x, y, 1, 1 );
    }

    void addButton( Button* button, int x, int y, int w, int h ) {
        if ( button != NULL ) {
            int dstX = _x + ( x * ( _unitWidth + CELL_SPACING ) ) + ( CELL_SPACING >> 1 );
            int dstY = _y + ( y * ( _unitHeight + CELL_SPACING ) ) + ( CELL_SPACING >> 1 );
            button -> setPosition( dstX, dstY );
            int dstW = ( w * ( _unitWidth + CELL_SPACING ) ) - ( CELL_SPACING );
            int dstH = ( h * ( _unitHeight + CELL_SPACING ) ) - ( CELL_SPACING );
            button -> setDimension( dstW, dstH );
            _buttons.push_back( button );
        }
    }

    int getWidthItemUnit() {
        return ( ROW_SIZE + CELL_SPACING );
    }

    int getHeightItemUnit() {
        return ( COL_SIZE + CELL_SPACING );
    }

    void deleteAllButtons() {
        for ( list < Button* >::iterator i = _buttons.begin(); i != _buttons.end(); i++ ) {
            delete ( *i );
        }
        _buttons.clear();
    }

    virtual void render( SDL_Renderer* renderer ) {
        SDL_Rect rect;
        rect.x = _x;
        rect.y = _y;
        rect.w = _width;
        rect.h = _height;
        SDL_SetRenderDrawColor( renderer, _r, _g, _b, _a );
        SDL_RenderFillRect( renderer, &rect );
        for ( list < Button* >::iterator i = _buttons.begin(); i != _buttons.end(); i++ ) {
            ( *i ) -> render( renderer );
        }
    }

    void setPosition( int x, int y ) {
        _x = x;
        _y = y;
    }

    void setSize( int width, int height ) {
        _width = width;
        _height = height;
        _unitWidth = ROW_SIZE;//_width / _unitCols;
        _unitHeight = COL_SIZE;//_height / _unitRows;
    }

    virtual bool click( int x, int y ) {
        if ( isInside( x, y ) ) {
            for ( list < Button* >::iterator i = _buttons.begin(); i != _buttons.end(); i++ ) {
                if ( ( *i ) -> click( x, y ) ) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isInside( int x, int y ) {
        if ( ( x >= _x ) && ( y >= _y ) && ( x < _x + _width ) && ( y < _y + _height ) ) {
            return true;
        }
        return false;
    }

    virtual Button* getPointedButton( int x, int y ) {
        if ( isInside( x, y ) ) {
            for ( list < Button* >::iterator i = _buttons.begin(); i != _buttons.end(); i++ ) {
                if ( ( *i ) -> isInside( x, y ) ) {
                    return ( *i );
                }
            }
        }
        return NULL;
    }

    int getPositionX() {
        return _x;
    }

    int getPositionY() {
        return _y;
    }

    int getWidth() {
        return _width;
    }

    int getHeight() {
        return _height;
    }

    void setColor( Uint32 color ) {
        _a = ( color >> 24 ) & 0xFF;
        _r = ( color >> 16 ) & 0xFF;
        _g = ( color >> 8 ) & 0xFF;
        _b = ( color ) & 0xFF;
    }

    private:

    list < Button* > _buttons;

    int _x;
    int _y;
    int _width;
    int _height;

    int _unitWidth;
    int _unitHeight;
    int _unitCols;
    int _unitRows;

    int _r;
    int _g;
    int _b;
    int _a;

};


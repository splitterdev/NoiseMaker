#pragma once

class Edit;

Edit* focusedEdit = NULL;

#include <string>

class Edit : public Button {
    public:

    static const int TEXT_SPACING = 4;
    static const int BUFFER_LIMIT = 16;

    Edit( string name ) : Button() {
        _text = "";
        _caret = 0;
        setName( name );
        _isEditEnabled = true;
        _allowedChars = NULL;
    }
    virtual ~Edit() {
    }

    virtual void onClick() {
        focusedEdit = this;
        _caret = _text.length();
    }

    void appendChar( int key ) {
        if ( _isAllowed( key ) && ( _caret < BUFFER_LIMIT ) && _isEditEnabled ) {
            _text += ( char ) key;
            _caret++;
        }
    }

    void eraseChar() {
        if ( ( _caret > 0 ) && _isEditEnabled ) {
            _caret--;
            _text = _text.substr( 0, _caret );
        }
    }

    virtual void finishEditing() {
        focusedEdit = NULL;
    }

    string getText() {
        return _text;
    }

    void setText( string text ) {
        _text = text;
    }

    /*double getValue() {
        if ( _node ) {
            return _node -> getParameter( _paramId );
        }
        return NAN;
    }*/

    virtual void render( SDL_Renderer* renderer ) {
        SDL_Rect rect;
        rect.x = getPositionX();
        rect.y = getPositionY();
        rect.w = getWidth();
        rect.h = getHeight();
        if ( !_isEditEnabled ) {
            SDL_SetRenderDrawColor( renderer, 64, 16, 16, 255 );
        } else if ( focusedEdit == this ) {
            SDL_SetRenderDrawColor( renderer, 32, 64, 16, 255 );
        } else {
            SDL_SetRenderDrawColor( renderer, 16, 16, 16, 255 );
        }
        SDL_RenderFillRect( renderer, &rect );
        rect.x += TEXT_SPACING;
        rect.y += TEXT_SPACING;
        rect.w -= ( TEXT_SPACING << 1 );
        rect.h -= ( TEXT_SPACING << 1 );
        char buffer[ BUFFER_LIMIT + 1 ];
        for ( int i = 0; i < int( _text.length() ); i++ ) {
            if ( i >= BUFFER_LIMIT - 1 ) {
                break;
            } else {
                buffer[ i ] = _text[ i ];
            }
        }
        for ( int i = _text.length(); i < BUFFER_LIMIT; i++ ) {
            buffer[ i ] = 32;
        }
        buffer[ BUFFER_LIMIT ] = 0;
        SDL_Surface* t = Text::RenderOnly( "%s", buffer );
        if ( t ) {
            SDL_Texture* tTex = SDL_CreateTextureFromSurface( renderer, t );
            if ( tTex ) {
                SDL_RenderCopy( renderer, tTex, NULL, &rect );
                SDL_DestroyTexture( tTex );
            }
            SDL_FreeSurface( t );
        }
    }

    void setEditEnabled( bool enabled ) {
        _isEditEnabled = enabled;
    }

    bool isEditEnabled() {
        return _isEditEnabled;
    }

    void setAllowedChars( const char* allowedChars ) {
        _allowedChars = allowedChars;
    }

    inline void updateCaret() {
        _caret = _text.length();
    }

    private:

    bool _isAllowed( int k ) {
        if ( k < 32 || k >= 127 ) {
            return false;
        }
        if ( !_allowedChars ) {
            return true;
        }
        for ( int i = 0; i < int( strlen( _allowedChars ) ); i++ ) {
            if ( _allowedChars[ i ] == k ) {
                return true;
            }
        }
        return false;
    }

    string _text;
    int _caret;

    bool _isEditEnabled;
    const char* _allowedChars;

};



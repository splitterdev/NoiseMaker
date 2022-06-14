#pragma once

#include <string>

class ParamEdit : public Edit {
    public:

    static const int TEXT_SPACING = 4;
    static const int BUFFER_LIMIT = 16;

    ParamEdit() : Edit( "Edit value" ) {
        _node = NULL;
        _paramId = "";
        setAllowedChars( "0123456789-." );
    }
    virtual ~ParamEdit() {
    }

    virtual void finishEditing() {
        _updateValue();
        Edit::finishEditing();
    }

    /*virtual void render( SDL_Renderer* renderer ) {
        SDL_Rect rect;
        rect.x = getPositionX();
        rect.y = getPositionY();
        rect.w = getWidth();
        rect.h = getHeight();
        if ( !isEditEnabled() ) {
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
        for ( int i = 0; i < int( getText().length() ); i++ ) {
            if ( i >= BUFFER_LIMIT - 1 ) {
                break;
            } else {
                buffer[ i ] = getText()[ i ];
            }
        }
        for ( int i = getText().length(); i < BUFFER_LIMIT; i++ ) {
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
    }*/

    void linkToValue( ChainingNode* node, string paramId ) {
        _node = node;
        _paramId = paramId;
        if ( _node ) {
            Buffer* buffer = _node -> getParameter( _paramId.c_str() );
            if ( buffer ) {
                double* value = ( double* ) buffer -> getMemory();
                char buf[ 4096 ];
                sprintf( buf, "%g", value[ 0 ] );
                setText( string( buf ) );
                updateCaret();
                return;
            }
        }
        setText( "0.0" );
        updateCaret();
    }

    private:

    void _updateValue() {
        if ( isEditEnabled() ) {
            double v = NAN;
            string text = getText();
            int r = sscanf( text.c_str(), "%lg", &v );
            if ( r == 0 || isnan( v ) ) {
                setText( "0.0" );
                updateCaret();
                v = 0.0;
            }
            if ( _node ) {
                _node -> updateParameter( _paramId.c_str(), v );
            }
        }
    }

    string _paramId;
    ChainingNode* _node;

};



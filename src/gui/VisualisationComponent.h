#pragma once

class VisualisationComponent {
    public:

    class Factory {
        public:
        Factory() {}
        virtual ~Factory() {}
        virtual VisualisationComponent* create( int x, int y, int width, int height, int resolutionX, int resolutionY, ChainingTree** tree ) = 0;
    };

    static const int BORDER = 4;

    VisualisationComponent( int x, int y, int width, int height, int resolutionX, int resolutionY, ChainingTree** tree ) {
        _surface = SDL_CreateRGBSurface( 0, resolutionX, resolutionY, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF );
        _surfaceBuffer = SDL_CreateRGBSurface( 0, resolutionX, resolutionY, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF );
        _rect.x = x;
        _rect.y = y;
        _rect.w = width;
        _rect.h = height;
        _rectInternal.x = BORDER;
        _rectInternal.y = BORDER;
        _rectInternal.w = width - ( BORDER << 1 );
        _rectInternal.h = height - ( BORDER << 1 );
        _pixels = ( unsigned int ) _surface -> pixels;
        _pitch = _surface -> pitch;
        _tree = tree;
    }
    virtual ~VisualisationComponent() {
        SDL_FreeSurface( _surface );
        SDL_FreeSurface( _surfaceBuffer );
    }

    virtual void visualize( ChainingTree* tree ) = 0;

    void render( SDL_Renderer* renderer ) {
        visualize( *_tree );
        SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, _surface );
        if ( texture ) {
            SDL_RenderCopy( renderer, texture, NULL, &_rect );
            SDL_DestroyTexture( texture );
        }
    }

    inline void clearPrevious( int v ) {
        SDL_FillRect( _surfaceBuffer, NULL, 0x40404000 | ( v & 0x000000FF ) );
        SDL_FillRect( _surfaceBuffer, &_rectInternal, 0x04202000 | ( v & 0x000000FF ) );
        SDL_BlitSurface( _surfaceBuffer, NULL, _surface, NULL );
    }

    inline void clearPrevious() {
        SDL_FillRect( _surface, NULL, 0x40404080 );
        SDL_FillRect( _surface, &_rectInternal, 0x04202080 );
    }

    inline void updateBuffer( SDL_Surface* source, SDL_Rect* srcRect, SDL_Rect* dstRect ) {
        SDL_BlitSurface( source, srcRect, _surface, dstRect );
    }

    inline void setPixel( int x, int y, Uint32 pixel ) {
        *( ( Uint32* )( _pixels + y * _pitch + x * sizeof( Uint32 ) ) ) = pixel;
    }

    inline void setPixel( SDL_Surface* surface, int x, int y, Uint32 pixel ) {
        *( ( Uint32* )( int( surface -> pixels ) + y * surface -> pitch + x * sizeof( Uint32 ) ) ) = pixel;
    }

    inline void setPixelGray( int x, int y, int value, int alpha ) {
        Uint32 pixel =
            ( ( value << 24 ) & 0xFF000000 )
            | ( ( value << 16 ) & 0x00FF0000 )
            | ( ( value << 8 ) & 0x0000FF00 )
            | ( alpha & 0x000000FF );
        setPixel( x, y, pixel );
    }

    inline void setPixelGray( SDL_Surface* surface, int x, int y, int value, int alpha ) {
        Uint32 pixel =
            ( ( value << 24 ) & 0xFF000000 )
            | ( ( value << 16 ) & 0x00FF0000 )
            | ( ( value << 8 ) & 0x0000FF00 )
            | ( alpha & 0x000000FF );
        setPixel( surface, x, y, pixel );
    }

    inline void setPixelColorized( int x, int y, int r, int g, int b, int alpha ) {
        Uint32 pixel =
            ( ( r << 24 ) & 0xFF000000 )
            | ( ( g << 16 ) & 0x00FF0000 )
            | ( ( b << 8 ) & 0x0000FF00 )
            | ( alpha & 0x000000FF );
        setPixel( x, y, pixel );
    }

    inline void setPixelColorized( SDL_Surface* surface, int x, int y, int r, int g, int b, int alpha ) {
        Uint32 pixel =
            ( ( r << 24 ) & 0xFF000000 )
            | ( ( g << 16 ) & 0x00FF0000 )
            | ( ( b << 8 ) & 0x0000FF00 )
            | ( alpha & 0x000000FF );
        setPixel( surface, x, y, pixel );
    }

    inline void setLineGray( int x1, int y1, int x2, int y2, int color, int alpha ) {
        setLine( x1, y1, x2, y2, color, color, color, alpha );
    }

    inline void setLine( int x1, int y1, int x2, int y2, int r, int g, int b, int alpha ) {
        double dx = x2 - x1;
        double dy = y2 - y1;
        double d = sqrt( dx * dx + dy * dy );
        setPixelColorized( x2, y2, r, g, b, alpha );
        if ( d > 0.0 ) {
            double cx = dx / d;
            double cy = dy / d;
            for ( double i = 0.0; i < d; i += 1.0 ) {
                double x = x1 + i * cx;
                double y = y1 + i * cy;
                setPixelColorized( x, y, r, g, b, alpha );
            }
        }
    }

    inline void setLineGray( SDL_Surface* surface, int x1, int y1, int x2, int y2, int color, int alpha ) {
        setLine( surface, x1, y1, x2, y2, color, color, color, alpha );
    }

    inline void setLine( SDL_Surface* surface, int x1, int y1, int x2, int y2, int r, int g, int b, int alpha ) {
        double dx = x2 - x1;
        double dy = y2 - y1;
        double d = sqrt( dx * dx + dy * dy );
        setPixelColorized( surface, x2, y2, r, g, b, alpha );
        if ( d > 0.0 ) {
            double cx = dx / d;
            double cy = dy / d;
            for ( double i = 0.0; i < d; i += 1.0 ) {
                double x = x1 + i * cx;
                double y = y1 + i * cy;
                setPixelColorized( surface, x, y, r, g, b, alpha );
            }
        }
    }

    inline int getResolutionX() {
        return _surface -> w;
    }

    inline int getResolutionY() {
        return _surface -> h;
    }

    static void addFactory( int id, Factory* factory ) {
        _VCF_MAP[ id ] = factory;
    }

    static void createSetVC( int id, int sizeX, int sizeY, ChainingTree** tree ) {
        map < int, Factory* >::iterator i = _VCF_MAP.find( id );
        if ( i != _VCF_MAP.end() ) {
            Factory* factory = i -> second;
            int offsetY = 48;
            VisualisationComponent* vComponent = factory -> create(
                WINDOW_WIDTH - sizeX,
                WINDOW_HEIGHT - sizeY - offsetY,
                sizeX,
                sizeY,
                sizeX,
                sizeY,
                tree
            );
            setActiveVC( vComponent );
        } else {
            setActiveVC( NULL );
        }
    }

    static VisualisationComponent* getActiveVC() {
        return _ACTIVE_VC;
    }

    static void setActiveVC( VisualisationComponent* component ) {
        if ( _ACTIVE_VC ) {
            delete _ACTIVE_VC;
        }
        _ACTIVE_VC = component;
    }

    private:

    SDL_Surface* _surface;
    SDL_Surface* _surfaceBuffer;
    SDL_Rect _rect;
    SDL_Rect _rectInternal;

    unsigned int _pixels;
    int _pitch;

    static map < int, Factory* > _VCF_MAP;
    static VisualisationComponent* _ACTIVE_VC;

    ChainingTree** _tree;

};

map < int, VisualisationComponent::Factory* > VisualisationComponent::_VCF_MAP;
VisualisationComponent* VisualisationComponent::_ACTIVE_VC = NULL;


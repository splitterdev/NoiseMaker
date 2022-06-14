#pragma once

class OscilloscopeVC : public VisualisationComponent {
    public:
    OscilloscopeVC( int x, int y, int w, int h, int rx, int ry, ChainingTree** tree ) : VisualisationComponent( x, y, w, h, rx, ry, tree ) {
    }
    virtual ~OscilloscopeVC() {
    }
    virtual void visualize( ChainingTree* tree ) {
        clearPrevious();
        Buffer* buffer = tree -> getOutputNode() -> getOutput();
        short* out = ( short* ) buffer -> getMemory();
        double bufferSize = double( buffer -> getSize() >> 1 );
        int iValue = ( int( out[ 0 ] ) + int( out[ 1 ] ) ) >> 1;
        double lastValue = ( double( iValue ) / 65536.0 + 0.5 ) * getResolutionY();
        for ( int i = 1; i < getResolutionX(); i++ ) {
            double k = i / double( getResolutionX() );
            int index = k * bufferSize;
            iValue = ( int( out[ index ] ) + int( out[ index + 1 ] ) ) >> 1;
            double v = ( double( iValue ) / 65536.0 + 0.5 ) * getResolutionY();
            setLineGray( i - 1, lastValue, i, v, 200, 128 );
            lastValue = v;
        }
    }
    private:
};

class SpectreVC : public VisualisationComponent {
    public:

    SpectreVC( int x, int y, int w, int h, int rx, int ry, ChainingTree** tree ) : VisualisationComponent( x, y, w, h, rx, ry, tree ) {
        _echo = Buffer::Create( BUFFER_SIZE * 4 );
        _echoBufferCursor = 0;
    }
    virtual ~SpectreVC() {
        delete _echo;
    }
    virtual void visualize( ChainingTree* tree ) {
        clearPrevious();
        //Buffer* buffer = tree -> getOutputNode() -> getOutput();
        Buffer* fft = genFFTBuffer( tree );
        short* out = ( short* ) fft -> getMemory();
        double bufferSize = double( fft -> getSize() >> 2 );
        static const double upperBorderPow = 12.0;
        static const double upperBorder = pow( 2.0, upperBorderPow );
        //double lastValue = ( double( ( out[ 0 ] + out[ 1 ] ) >> 1 ) / 65536.0 + 0.5 ) * getResolutionY();
        for ( int i = 0; i < getResolutionX(); i++ ) {
            double k = pow( 2.0, upperBorderPow * double( i ) / double( getResolutionX() ) ) / upperBorder;
            int index = k * bufferSize;
            int iValue = abs( int( out[ index ] ) + int( out[ index + 1 ] ) ) >> 1;
            double v = ( 1.0 - double( iValue ) / 32767.0 ) * getResolutionY();
            setLine( i, getResolutionY() - 1, i, v, 200, 0, 0, 128 );
            //lastValue = v;
        }
        delete fft;
    }

    Buffer* genFFTBuffer( ChainingTree* tree ) {
        Buffer* buffer = tree -> getOutputNode() -> getOutput();
        _echo -> copyFrom( buffer, _echoBufferCursor, buffer -> getSize() );
        _echoBufferCursor = ( _echoBufferCursor + buffer -> getSize() ) % _echo -> getSize();
        return FFTVisualisationHelper::generateFFT( _echo );
    }

    private:

    Buffer* _echo;
    int _echoBufferCursor;

};

class SpectreHistogramVC : public SpectreVC {
    public:
    SpectreHistogramVC( int x, int y, int w, int h, int rx, int ry, ChainingTree** tree ) : SpectreVC( x, y, w, h, rx, ry, tree ) {
        _buffered = SDL_CreateRGBSurface( 0, rx - ( BORDER << 1 ), ry - ( BORDER << 1 ), 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF );
        SDL_FillRect( _buffered, NULL, 0x00000000 );
        _cursor = 0;
    }
    virtual ~SpectreHistogramVC() {
        SDL_FreeSurface( _buffered );
    }
    virtual void visualize( ChainingTree* tree ) {
        clearPrevious();
        //Buffer* buffer = tree -> getOutputNode() -> getOutput();
        Buffer* fft = genFFTBuffer( tree );
        short* out = ( short* ) fft -> getMemory();
        double bufferSize = double( fft -> getSize() >> 2 );
        //static const double lowerBorderPow = 2.0;
        static const double upperBorderPow = 12.0;
        static const double upperBorder = pow( 2.0, upperBorderPow );
        int rx = _buffered -> w;
        int ry = _buffered -> h;
        for ( int i = 0; i < ry; i++ ) {
            double k = pow( 2.0, upperBorderPow * ( 1.0 - double( i ) / double( ry ) ) ) / upperBorder;
            int index = k * bufferSize;
            int iValue = ( int( out[ index ] ) + int( out[ index + 1 ] ) ) >> 1;
            double v = ( double( iValue ) / 64.0 );
            if ( v > 255.0 ) {
                v = 255.0;
            } else if ( v < 0.0 ) {
                v = 0.0;
            }
            setPixelColorized( _buffered, _cursor, i, v, 0, 0, v );
        }
        _cursor = ( _cursor + 1 ) % rx;
        //setLineGray( _buffered, _cursor, 0, _cursor, getResolutionY() - 1, 255, 192 );
        SDL_Rect r;
        SDL_Rect t;
        r.y = BORDER;
        r.h = ry;
        t.y = BORDER;
        r.x = _cursor;
        r.w = rx - _cursor;
        t.x = BORDER;
        updateBuffer( _buffered, &r, &t );
        t.x = r.w + BORDER;
        r.x = 0;
        r.w = _cursor;
        updateBuffer( _buffered, &r, &t );
        delete fft;
    }
    private:

    SDL_Surface* _buffered;
    int _cursor;

};

class VectorscopeVC : public VisualisationComponent {
    public:
    VectorscopeVC( int x, int y, int w, int h, int rx, int ry, ChainingTree** tree ) : VisualisationComponent( x, y, w, h, rx, ry, tree ) {
    }
    virtual ~VectorscopeVC() {
    }
    virtual void visualize( ChainingTree* tree ) {
        clearPrevious();
        Buffer* buffer = tree -> getOutputNode() -> getOutput();
        short* out = ( short* ) buffer -> getMemory();
        int bufferSize = ( buffer -> getSize() >> 1 );
        int iValueX = ( int( out[ 0 ] ) - int( out[ 1 ] ) ) >> 1;
        int iValueY = ( int( out[ 0 ] ) + int( out[ 1 ] ) ) >> 1;
        Vector2D lastPos = Vector2D(
            ( double( iValueX ) / 65536.0 + 0.5 ) * getResolutionX(),
            ( double( iValueY ) / 65536.0 + 0.5 ) * getResolutionY()
        );
        for ( int i = 2; i < bufferSize; i += 2 ) {
            iValueX = ( int( out[ i ] ) - int( out[ i + 1 ] ) ) >> 1;
            iValueY = ( int( out[ i ] ) + int( out[ i + 1 ] ) ) >> 1;
            Vector2D currentPos = Vector2D(
                ( double( iValueX ) / 65536.0 + 0.5 ) * getResolutionX(),
                ( double( iValueY ) / 65536.0 + 0.5 ) * getResolutionY()
            );
            setLineGray( currentPos.x, currentPos.y, lastPos.x, lastPos.y, 200, 128 );
            lastPos = currentPos;
        }
    }
    private:
};

class OscilloscopeFactory : public VisualisationComponent::Factory {
    public:
    OscilloscopeFactory() : VisualisationComponent::Factory() {
    }
    virtual ~OscilloscopeFactory() {
    }

    virtual VisualisationComponent* create(
        int x, int y, int width, int height, int resolutionX, int resolutionY, ChainingTree** tree
    ) {
        return new OscilloscopeVC( x, y, width, height, resolutionX, resolutionY, tree );
    }

    private:
};

class SpectreFactory : public VisualisationComponent::Factory {
    public:
    SpectreFactory() : VisualisationComponent::Factory() {
    }
    virtual ~SpectreFactory() {
    }

    virtual VisualisationComponent* create(
        int x, int y, int width, int height, int resolutionX, int resolutionY, ChainingTree** tree
    ) {
        return new SpectreVC( x, y, width, height, resolutionX, resolutionY, tree );
    }

    private:
};

class SpectreHistogramFactory : public VisualisationComponent::Factory {
    public:
    SpectreHistogramFactory() : VisualisationComponent::Factory() {
    }
    virtual ~SpectreHistogramFactory() {
    }

    virtual VisualisationComponent* create(
        int x, int y, int width, int height, int resolutionX, int resolutionY, ChainingTree** tree
    ) {
        return new SpectreHistogramVC( x, y, width, height, resolutionX, resolutionY, tree );
    }

    private:
};

class VectorscopeFactory : public VisualisationComponent::Factory {
    public:
    VectorscopeFactory() : VisualisationComponent::Factory() {
    }
    virtual ~VectorscopeFactory() {
    }

    virtual VisualisationComponent* create(
        int x, int y, int width, int height, int resolutionX, int resolutionY, ChainingTree** tree
    ) {
        return new VectorscopeVC( x, y, width, height, resolutionX, resolutionY, tree );
    }

    private:
};

void mapVCFactory() {
    VisualisationComponent::addFactory( 0, new OscilloscopeFactory() );
    VisualisationComponent::addFactory( 1, new SpectreFactory() );
    VisualisationComponent::addFactory( 2, new SpectreHistogramFactory() );
    VisualisationComponent::addFactory( 3, new VectorscopeFactory() );
}


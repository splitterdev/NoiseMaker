#pragma once

class SquareGenerator : public ChainingNode {
    public:
    SquareGenerator( double freq, double volume ) : ChainingNode() {
        _freq = setParameter( "Freq", freq );
        _volume = setParameter( "Vol", volume );
        setTextureId( TEXTURE_SQUAREGEN );
        setAllowedInputs( 0 );
        _t[ 0 ] = 0.0;
        _t[ 1 ] = 0.0;
    }
    virtual ~SquareGenerator() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        double bitRate = BITRATE;
        if ( output ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            for ( int i = 0; i < sampleCount; i += 2 ) {
                double unused;
                int volumeMin = max( - _volume[ i ] * 32767.0, -32767.0 );
                int volumeMax = min( _volume[ i ] * 32767.0, 32767.0 );
                int volumeMin1 = max( - _volume[ i + 1 ] * 32767.0, -32767.0 );
                int volumeMax1 = min( _volume[ i + 1 ] * 32767.0, 32767.0 );
                outMem[ i ] = modf( _t[ 0 ], &unused ) < 0.5 ? volumeMin : volumeMax;
                outMem[ i + 1 ] = modf( _t[ 1 ], &unused ) < 0.5 ? volumeMin1 : volumeMax1;
                _t[ 0 ] += _freq[ i ] / bitRate;
                _t[ 1 ] += _freq[ i + 1 ] / bitRate;
            }
            _t[ 0 ] = fmod( _t[ 0 ], BITRATE );
            _t[ 1 ] = fmod( _t[ 1 ], BITRATE );
        }
    }

    virtual string getClassName() {
        return "Square Generator";
    }

    private:

    double* _freq;
    double* _volume;

    double _t[ 2 ];

};



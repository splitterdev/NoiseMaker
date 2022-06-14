#pragma once

class SawGenerator : public ChainingNode {
    public:
    SawGenerator( double freq, double volume ) : ChainingNode() {
        _baseFreq = setParameter( "Freq", freq );
        _baseVol = setParameter( "Vol", volume );
        setTextureId( TEXTURE_SAWGEN );
        setAllowedInputs( 0 );
        _t[ 0 ] = 0.0;
        _t[ 1 ] = 0.0;
    }
    virtual ~SawGenerator() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        if ( output ) {
            int BITRATE2 = BITRATE >> 1;
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            for ( int i = 0; i < sampleCount; i += 2 ) {
                outMem[ i ] = ( ( int( _t[ 0 ] ) % BITRATE ) - BITRATE2 ) * ( _baseVol[ i ] * 32767.0 ) / BITRATE2;
                outMem[ i + 1 ] = ( ( int( _t[ 1 ] ) % BITRATE ) - BITRATE2 ) * ( _baseVol[ i + 1 ] * 32767.0 ) / BITRATE2;
                _t[ 0 ] += _baseFreq[ i ];
                _t[ 1 ] += _baseFreq[ i + 1 ];
            }
            _t[ 0 ] = fmod( _t[ 0 ], BITRATE );
            _t[ 1 ] = fmod( _t[ 1 ], BITRATE );
        }
    }

    virtual string getClassName() {
        return "Saw Generator";
    }

    private:

    double* _baseFreq;
    double* _baseVol;

    double _t[ 2 ];

};



#pragma once

class SineGenerator : public ChainingNode {
    public:
    SineGenerator( double freq, double volume ) : ChainingNode() {
        _freq = setParameter( "Freq", freq );
        _volume = setParameter( "Vol", volume );
        setTextureId( TEXTURE_SINEGEN );
        setAllowedInputs( 0 );
        _t[ 0 ] = 0.0;
        _t[ 1 ] = 0.0;
    }
    virtual ~SineGenerator() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        double bitRate = BITRATE;
        if ( output ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            for ( int i = 0; i < sampleCount; i += 2 ) {
                outMem[ i ] = int( sin( _t[ 0 ] ) * _volume[ i ] * 32767.0 );
                outMem[ i + 1 ] = int( sin( _t[ 1 ] ) * _volume[ i + 1 ] * 32767.0 );
                _t[ 0 ] += 2.0 * M_PI * _freq[ i ] / bitRate;
                _t[ 1 ] += 2.0 * M_PI * _freq[ i + 1 ] / bitRate;
            }
        }
    }

    virtual string getClassName() {
        return "Sine Generator";
    }

    private:

    double* _freq;
    double* _volume;

    double _t[ 2 ];

};



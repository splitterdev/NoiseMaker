#pragma once

class NoiseLFGenerator : public ChainingNode {
    public:
    NoiseLFGenerator( int derivativePower, int derivativeCount ) : ChainingNode() {
        setTextureId( TEXTURE_LFNOISEGEN );
        setAllowedInputs( 0 );
        _cLevel = 0;
        _dervPower = ( derivativePower << 1 ) + 1;
        _dervAmp = derivativePower;
        derivativeCount++;
        if ( derivativeCount <= 0 ) {
            derivativeCount = 1;
        }
        _dervCount = derivativeCount;
        _derv = ( int* )( malloc( derivativeCount * sizeof( int ) ) );
    }
    virtual ~NoiseLFGenerator() {
        if ( _derv ) {
            free( _derv );
        }
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        if ( output ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            for ( int i = 0; i < sampleCount; i+= 2 ) {
                _derv[ 0 ] = ( ( rand() & 0xFF ) | ( ( rand() & 0xFF ) << 8 ) ) % _dervPower - _dervAmp;
                for ( int j = 1; j < _dervCount; j++ ) {
                    int sampleValue = _derv[ j ] + _derv[ j - 1 ];
                    if ( sampleValue > 32767 ) {
                        sampleValue = 32767;
                    } else if ( sampleValue < - 32767 ) {
                        sampleValue = - 32767;
                    }
                    _derv[ j ] = sampleValue;
                }
                outMem[ i ] = _derv[ _dervCount - 1 ];
                outMem[ i + 1 ] = _derv[ _dervCount - 1 ];
            }
        }
    }

    virtual string getClassName() {
        return "Noise LF Generator";
    }

    private:

    int _cLevel;
    int _dervPower;
    int _dervAmp;

    int* _derv;
    int _dervCount;

};



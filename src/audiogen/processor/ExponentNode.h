#pragma once

class ExponentNode : public ChainingNode {
    public:
    ExponentNode( double p, double volume ) : ChainingNode() {
        setTextureId( TEXTURE_POWER );
        _p = setParameter( "Exp", p );
        _volume = setParameter( "Vol", volume );
    }
    virtual ~ExponentNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                double a = double( abs( inMem[ i ] ) ) / 32767.0;
                double b = pow( a, _p[ i ] ) * 32767.0;
                if ( inMem[ i ] < 0 ) {
                    b *= -1;
                }
                outMem[ i ] = b * _volume[ i ];
            }
        }
    }

    virtual string getClassName() {
        return "Exponent";
    }

    private:

    double* _p;
    double* _volume;

};

#pragma once

class LogarithmicNode : public ChainingNode {
    public:
    LogarithmicNode( double basis, double volume ) : ChainingNode() {
        setTextureId( TEXTURE_LOGARITHMIC );
        _basis = setParameter( "Base", basis );
        _volume = setParameter( "Vol", volume );
    }
    virtual ~LogarithmicNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                double lnp = log( _basis[ i ] );
                double maxValue = log( 32767.0 ) / lnp;
                double a = double( abs( inMem[ i ] ) ) / 32767.0;
                double b = inMem[ i ] == 0 ? 0.0 : log( a ) / lnp / maxValue * 32767.0;
                if ( inMem[ i ] < 0 ) {
                    b *= -1;
                }
                outMem[ i ] = b * _volume[ i ];
            }
        }
    }

    virtual string getClassName() {
        return "Logarithm";
    }

    private:

    double* _basis;
    double* _volume;

};


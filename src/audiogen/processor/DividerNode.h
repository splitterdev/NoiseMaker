#pragma once

class DividerNode : public ChainingNode {
    public:
    DividerNode( double volume, double limiter ) : ChainingNode() {
        setTextureId( TEXTURE_DIVIDER );
        _scale = setParameter( "Vol", volume );
        _limiter = setParameter( "Limiter", limiter );
    }
    virtual ~DividerNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                double v = double( inMem[ i ] ) / 32767.0;
                double li = _limiter[ i ] * 1000.0;
                if ( fabs( li * v ) <= 1.0 ) {
                    outMem[ i ] = inMem[ i ] >= 0 ? 32767 : - 32767;
                } else {
                    outMem[ i ] = ( 1.0 / v ) / li * 32767.0 * _scale[ i ];
                }
            }
        }
    }

    virtual string getClassName() {
        return "Divider";
    }

    private:

    double* _scale;
    double* _limiter;

};



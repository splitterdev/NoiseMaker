#pragma once

class PainNode : public ChainingNode {
    public:
    PainNode( double minV, double maxV ) : ChainingNode() {
        setTextureId( TEXTURE_PAIN );
        _baseMinV = setParameter( "Min", minV );
        _baseMaxV = setParameter( "Max", maxV );
    }
    virtual ~PainNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                double minV = _baseMinV[ i ] * 32767.0;
                double maxV = _baseMaxV[ i ] * 32767.0;
                int a = abs( inMem[ i ] );
                if ( ( a >= minV ) && ( a < maxV ) ) {
                    outMem[ i ] = inMem[ i ];
                } else {
                    outMem[ i ] = 0;
                }
            }
        }
    }

    virtual string getClassName() {
        return "Pain";
    }

    private:

    double* _baseMinV;
    double* _baseMaxV;

};



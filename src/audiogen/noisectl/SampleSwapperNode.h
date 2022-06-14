#pragma once

class SampleSwapperNode : public ChainingNode {
    public:
    SampleSwapperNode( double swap ) : ChainingNode() {
        setTextureId( TEXTURE_SAMPLESWAPPER );
        _swap = setParameter( "Swap", swap );
    }
    virtual ~SampleSwapperNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i += 4 ) {
                if ( _swap[ i ] > 0.5 ) {
                    int a = rand() & 0x01;
                    if ( a ) {
                        int rndIndex = rand() % ( sampleCount - 1 );
                        outMem[ i ] = inMem[ rndIndex ];
                        outMem[ i + 1 ] = inMem[ rndIndex + 1 ];
                        outMem[ rndIndex ] = inMem[ i ];
                        outMem[ rndIndex + 1 ] = inMem[ i + 1 ];
                    } else {
                        outMem[ i ] = inMem[ i ];
                        outMem[ i + 1 ] = inMem[ i + 1 ];
                        outMem[ i + 2 ] = inMem[ i + 2 ];
                        outMem[ i + 3 ] = inMem[ i + 3 ];
                    }
                } else {
                    outMem[ i ] = inMem[ i ];
                    outMem[ i + 1 ] = inMem[ i + 1 ];
                    outMem[ i + 2 ] = inMem[ i + 2 ];
                    outMem[ i + 3 ] = inMem[ i + 3 ];
                }
            }
        }
    }

    virtual string getClassName() {
        return "Sample Swapper";
    }

    private:

    double* _swap;

};



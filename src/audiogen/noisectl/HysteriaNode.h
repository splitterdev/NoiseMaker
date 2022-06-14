#pragma once

class HysteriaNode : public ChainingNode {
    public:
    HysteriaNode( double enable ) : ChainingNode() {
        setTextureId( TEXTURE_HYSTERIA );
        _enable = setParameter( "Enable", enable );
    }
    virtual ~HysteriaNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                if ( _enable[ i ] > 0.5 ) {
                    int a = rand() & 0x01;
                    if ( a ) {
                        outMem[ i ] = inMem[ i ];
                    } else {
                        outMem[ i ] = - inMem[ i ];
                    }
                } else {
                    outMem[ i ] = inMem[ i ];
                }
            }
        }
    }

    virtual string getClassName() {
        return "Hysteria";
    }

    private:

    double* _enable;

};



#pragma once

class MultiplierNode : public ChainingNode {
    public:
    MultiplierNode( double volume ) : ChainingNode() {
        setTextureId( TEXTURE_MULTIPLIER );
        _scale = setParameter( "Vol", volume );
        setAllowedInputs( - 1 );
    }
    virtual ~MultiplierNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        list < Buffer* > inputBuffers = getInputBuffers();
        if ( output ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            bool first = true;
            if ( inputBuffers.size() == 0 ) {
                for ( int i = 0; i < sampleCount; i++ ) {
                    outMem[ i ] = 0;
                }
            } else {
                for ( list < Buffer* >::iterator i = inputBuffers.begin(); i != inputBuffers.end(); i++ ) {
                    short* inMem = ( short* ) ( *i ) -> getMemory();
                    if ( first ) {
                        for ( int i = 0; i < sampleCount; i++ ) {
                            outMem[ i ] = inMem[ i ];
                        }
                        first = false;
                    } else {
                        for ( int i = 0; i < sampleCount; i++ ) {
                            outMem[ i ] *= inMem[ i ];
                        }
                    }
                }
                for ( int i = 0; i < sampleCount; i++ ) {
                    outMem[ i ] *= _scale[ i ];
                }
            }
        }
    }

    virtual string getClassName() {
        return "Multiplier";
    }

    private:

    double* _scale;

};

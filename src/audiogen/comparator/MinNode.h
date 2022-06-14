#pragma once

class MinNode : public ChainingNode {
    public:
    MinNode() : ChainingNode() {
        setTextureId( TEXTURE_MIN );
        setAllowedInputs( -1 );
    }
    virtual ~MinNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        if ( output ) {
            list < Buffer* > inputBuffers = getInputBuffers();
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            vector < short* > inMems;
            for ( list < Buffer* >::iterator i = inputBuffers.begin(); i != inputBuffers.end(); i++ ) {
                inMems.push_back( ( short* ) ( *i ) -> getMemory() );
            }
            int inputCount = inMems.size();
            int minInput = 0;
            short minInputValue = 32767;
            short currInputValue;
            if ( inputCount > 0 ) {
                for ( int i = 0; i < sampleCount; i++ ) {
                    for ( int j = 0; j < inputCount; j++ ) {
                        currInputValue = inMems[ j ][ i ];
                        if ( abs( currInputValue ) < abs( minInputValue ) ) {
                            minInput = j;
                            minInputValue = currInputValue;
                        }
                    }
                    outMem[ i ] = inMems[ minInput ][ i ];
                    minInputValue = 32767;
                }
            } else {
                for ( int i = 0; i < sampleCount; i++ ) {
                    outMem[ i ] = 0;
                }
            }
        }
    }

    virtual string getClassName() {
        return "Min";
    }

};

#pragma once

class MaxNode : public ChainingNode {
    public:
    MaxNode() : ChainingNode() {
        setTextureId( TEXTURE_MAX );
        setAllowedInputs( -1 );
    }
    virtual ~MaxNode() {
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
            int maxInput = 0;
            short maxInputValue = 0;
            short currInputValue;
            if ( inputCount > 0 ) {
                for ( int i = 0; i < sampleCount; i++ ) {
                    for ( int j = 0; j < inputCount; j++ ) {
                        currInputValue = inMems[ j ][ i ];
                        if ( abs( currInputValue ) > abs( maxInputValue ) ) {
                            maxInput = j;
                            maxInputValue = currInputValue;
                        }
                    }
                    outMem[ i ] = inMems[ maxInput ][ i ];
                    maxInputValue = 0;
                }
            } else {
                for ( int i = 0; i < sampleCount; i++ ) {
                    outMem[ i ] = 0;
                }
            }
        }
    }

    virtual string getClassName() {
        return "Max";
    }

};

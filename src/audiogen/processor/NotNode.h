#pragma once

class NotNode : public ChainingNode {
    public:
    NotNode() : ChainingNode() {
        setTextureId( TEXTURE_NOTGATE );
        setAllowedInputs( - 1 );
    }
    virtual ~NotNode() {
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
            }
            for ( list < Buffer* >::iterator i = inputBuffers.begin(); i != inputBuffers.end(); i++ ) {
                short* inMem = ( short* ) ( *i ) -> getMemory();
                if ( first ) {
                    for ( int i = 0; i < sampleCount; i++ ) {
                        outMem[ i ] = ~ inMem[ i ];
                    }
                    first = false;
                } else {
                    for ( int i = 0; i < sampleCount; i++ ) {
                        outMem[ i ] += ~ inMem[ i ];
                    }
                }
            }
        }
    }

    virtual string getClassName() {
        return "NOT Gate";
    }

};



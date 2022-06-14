#pragma once

class AdderNode : public ChainingNode {
    public:
    AdderNode() : ChainingNode() {
        setTextureId( TEXTURE_ADDER );
        setAllowedInputs( - 1 );
        _volume = setParameter( "Vol", 1.0 );
    }
    virtual ~AdderNode() {
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
                        outMem[ i ] = inMem[ i ];
                    }
                    first = false;
                } else {
                    for ( int i = 0; i < sampleCount; i++ ) {
                        outMem[ i ] += inMem[ i ];
                    }
                }
            }
            if ( inputBuffers.size() != 0 ) {
                for ( int i = 0; i < sampleCount; i++ ) {
                    outMem[ i ] = double( outMem[ i ] ) * _volume[ i ];
                }
            }
        }
    }

    virtual string getClassName() {
        return "Adder";
    }

    private:

    double* _volume;

};



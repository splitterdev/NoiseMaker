#pragma once

class AmplifierNode : public ChainingNode {
    public:
    AmplifierNode( double volume ) : ChainingNode() {
        setTextureId( TEXTURE_AMPLIFIER );
        _scale = setParameter( "Vol", volume );
    }
    virtual ~AmplifierNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                outMem[ i ] = double( inMem[ i ] ) * _scale[ i ];
            }
        }
    }

    virtual string getClassName() {
        return "Amplifier";
    }

    private:

    double* _scale;

};



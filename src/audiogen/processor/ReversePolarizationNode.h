#pragma once

class ReversePolarizationNode : public ChainingNode {
    public:
    ReversePolarizationNode() : ChainingNode() {
        setTextureId( TEXTURE_REVERSEPOLARIZATION );
    }
    virtual ~ReversePolarizationNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                outMem[ i ] = - inMem[ i ];
            }
        }
    }

    virtual string getClassName() {
        return "Reverse Polarization";
    }

};




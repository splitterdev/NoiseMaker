#pragma once

class AbsoluteNode : public ChainingNode {
    public:
    AbsoluteNode( double volume ) : ChainingNode() {
        setTextureId( TEXTURE_ABSOLUTE );
        _scale = setParameter( "Vol", volume );
    }
    virtual ~AbsoluteNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                outMem[ i ] = fabs( double( inMem[ i ] ) ) * _scale[ i ];
            }
        }
    }

    virtual string getClassName() {
        return "Absolute";
    }

    private:

    double* _scale;

};



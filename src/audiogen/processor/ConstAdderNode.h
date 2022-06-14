#pragma once

class ConstAdderNode : public ChainingNode {
    public:
    ConstAdderNode( double value ) : ChainingNode() {
        setTextureId( TEXTURE_CONSTADDER );
        _value = setParameter( "Const", value );
    }
    virtual ~ConstAdderNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                outMem[ i ] = inMem[ i ] + _value[ i ] * 32767.0;
            }
        }
    }

    virtual string getClassName() {
        return "Constant Adder";
    }

    private:

    double* _value;

};



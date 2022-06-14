#pragma once

class ConstGenerator : public ChainingNode {
    public:
    ConstGenerator( double value ) : ChainingNode() {
        setTextureId( TEXTURE_CONSTGEN );
        _value = setParameter( "Const", value );
    }
    virtual ~ConstGenerator() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        if ( output ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                outMem[ i ] = _value[ i ] * 32767.0;
            }
        }
    }

    virtual string getClassName() {
        return "Constant Generator";
    }

    private:

    double* _value;

};




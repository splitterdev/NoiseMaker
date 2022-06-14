#pragma once

class DerivatorNode : public ChainingNode {
    public:
    DerivatorNode( double scale ) : ChainingNode() {
        _scale = setParameter( "Vol", scale );
        _lastSample[ 0 ] = 0;
        _lastSample[ 1 ] = 0;
        setTextureId( TEXTURE_DERIVATOR );
    }
    virtual ~DerivatorNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i += 2 ) {
                outMem[ i ] = ( inMem[ i ] - _lastSample[ 0 ] ) * _scale[ i ] * 64.0;
                outMem[ i + 1 ] = ( inMem[ i + 1 ] - _lastSample[ 1 ] ) * _scale[ i + 1 ] / 64.0;
                _lastSample[ 0 ] = inMem[ i ];
                _lastSample[ 1 ] = inMem[ i + 1 ];
            }
        }
    }

    virtual string getClassName() {
        return "Derivator";
    }

    private:

    double* _scale;
    short _lastSample[ 2 ];

};

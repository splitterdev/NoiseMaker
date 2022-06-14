#pragma once

class IntegratorNode : public ChainingNode {
    public:
    IntegratorNode( double scale ) : ChainingNode() {
        _scale = setParameter( "Vol", scale );
        _chBase[ 0 ] = 0;
        _chBase[ 1 ] = 0;
        setTextureId( TEXTURE_INTEGRATOR );
    }
    virtual ~IntegratorNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i += 2 ) {
                outMem[ i ] = _chBase[ 0 ];
                outMem[ i + 1 ] = _chBase[ 1 ];
                _chBase[ 0 ] += inMem[ i ] * _scale[ i ] / 64.0;;
                _chBase[ 1 ] += inMem[ i + 1 ] * _scale[ i + 1 ] / 64.0;;
            }
        }
    }

    virtual string getClassName() {
        return "Integrator";
    }

    private:

    double* _scale;
    int _chBase[ 2 ];

};

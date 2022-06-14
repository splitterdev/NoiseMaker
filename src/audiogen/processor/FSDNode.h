#pragma once

class FSDNode : public ChainingNode {
    public:
    FSDNode( double clippingVolume, double volume ) : ChainingNode() {
        _volume = setParameter( "Vol", volume );
        _clippingVolume = setParameter( "Clip", clippingVolume );
        setTextureId( TEXTURE_OVERDRIVE );
    }
    virtual ~FSDNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                int sample = inMem[ i ];
                if ( sample > 0 ) {
                    outMem[ i ] = ( double( inMem[ i ] ) * ( 1.0 - _clippingVolume[ i ] ) + _clippingVolume[ i ] * 32767.0 ) * _volume[ i ];
                } else if ( sample < 0 ) {
                    outMem[ i ] = ( double( inMem[ i ] ) * ( 1.0 - _clippingVolume[ i ] ) + _clippingVolume[ i ] * - 32767.0 ) * _volume[ i ];
                } else {
                    outMem[ i ] = 0x0000;
                }
            }
        }
    }

    virtual string getClassName() {
        return "Fast Sign Distortion";
    }

    private:

    double* _clippingVolume;
    double* _volume;

};



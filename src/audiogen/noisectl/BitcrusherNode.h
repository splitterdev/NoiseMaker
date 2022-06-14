#pragma once

class BitcrusherNode : public ChainingNode {
    public:
    BitcrusherNode( double downsampling, double bandWidth ) : ChainingNode() {
        setTextureId( TEXTURE_BITCRUSHER );
        _downSampling = setParameter( "DS", downsampling );
        _bandWidth = setParameter( "BW", bandWidth );
        _counter = 0;
        _bitValue = 0;
    }
    virtual ~BitcrusherNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                if ( ( _counter >> 1 ) >= _downSampling[ i ] ) {
                    _counter = 0;
                    int bw = 16.0 - _bandWidth[ i ];
                    if ( bw > 15 ) {
                        bw = 15;
                    } else if ( bw < 0 ) {
                        bw = 0;
                    }
                    _bitValue = inMem[ i ] >> bw;
                    _bitValue = _bitValue << bw;
                }
                outMem[ i ] = _bitValue;
                _counter++;
            }
        }
    }

    virtual string getClassName() {
        return "BitCrusher";
    }

    private:

    double* _downSampling;
    double* _bandWidth;

    int _counter;
    short _bitValue;

};



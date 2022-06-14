#pragma once

class BraidNode : public ChainingNode {
    public:
    BraidNode( double enable ) : ChainingNode() {
        setTextureId( TEXTURE_BRAID );
        _enable = setParameter( "Enable", enable );
    }
    virtual ~BraidNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i += 2 ) {
                if ( _enable[ i ] > 0.5 ) {
                    short _higherByteA = ( inMem[ i ] >> 8 ) & 0x00FF;
                    short _lowerByteA = inMem[ i ] & 0x00FF;
                    short _higherByteB = ( inMem[ i + 1 ] >> 8 ) & 0x00FF;
                    short _lowerByteB = inMem[ i + 1 ] & 0x00FF;
                    short _resultA = ( ( _higherByteA ^ _lowerByteB ) << 8 );// + ( _lowerByteA ^ _higherByteB );
                    short _resultB = ( ( _lowerByteA ^ _higherByteB ) << 8 );
                    outMem[ i ] = _resultA;
                    outMem[ i + 1 ] = _resultB;
                } else {
                    outMem[ i ] = inMem[ i ];
                    outMem[ i + 1 ] = inMem[ i + 1 ];
                }
            }
        }
    }

    virtual string getClassName() {
        return "Braid";
    }

    private:

    double* _enable;

};



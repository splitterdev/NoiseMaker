#pragma once

class ChannelSwapperNode : public ChainingNode {
    public:
    ChannelSwapperNode( double swap ) : ChainingNode() {
        setTextureId( TEXTURE_CHANNEL_SWAPPER );
        _swap = setParameter( "Swap", swap );
        setAllowedInputs( 1 );
    }
    virtual ~ChannelSwapperNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i += 2 ) {
                if ( _swap[ i ] >= 0.5 ) {
                    outMem[ i ] = inMem[ i + 1 ];
                    outMem[ i + 1 ] = inMem[ i ];
                } else {
                    outMem[ i ] = inMem[ i ];
                    outMem[ i + 1 ] = inMem[ i + 1 ];
                }
            }
        }
    }

    virtual string getClassName() {
        return "Channel Swapper";
    }

    private:

    double* _swap;

};




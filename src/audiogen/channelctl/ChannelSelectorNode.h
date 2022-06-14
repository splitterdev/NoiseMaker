#pragma once

class ChannelSelectorNode : public ChainingNode {
    public:
    ChannelSelectorNode( double channelSelection ) : ChainingNode() {
        setTextureId( TEXTURE_CHANNEL_SELECTOR );
        setAllowedInputs( 1 );
        _channelSelection = setParameter( "L/R", channelSelection );
    }
    virtual ~ChannelSelectorNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i += 2 ) {
                double chValue = ( _channelSelection[ i ] + _channelSelection[ i + 1 ] ) / 2.0;
                if ( chValue < 0.0 ) {
                    chValue = 0.0;
                } else if ( chValue > 1.0 ) {
                    chValue = 1.0;
                }
                //double chValueA = chValue > 0.5 ? 1.0 : chValue * 2.0;
                //double chValueB = chValue > 0.5 ? ( 1.0 - chValue * 2.0 ) : 0.0;
                outMem[ i ] = double( inMem[ i ] ) * ( 1.0 - chValue );
                outMem[ i + 1 ] = double( inMem[ i + 1 ] ) * chValue;
            }
        }
    }

    virtual string getClassName() {
        return "Channel Selector";
    }

    private:

    double* _channelSelection;

};



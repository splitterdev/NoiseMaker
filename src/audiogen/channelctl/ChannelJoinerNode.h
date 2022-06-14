#pragma once

class ChannelJoinerNode : public ChainingNode {
    public:
    ChannelJoinerNode() : ChainingNode() {
        setTextureId( TEXTURE_CHANNEL_JOINER );
        setAllowedInputs( 1 );
    }
    virtual ~ChannelJoinerNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i += 2 ) {
                short joined = ( double( inMem[ i ] ) + double( inMem[ i + 1 ] ) ) / 2.0;
                outMem[ i ] = joined;
                outMem[ i + 1 ] = joined;
            }
        }
    }

    virtual string getClassName() {
        return "Channel Joiner";
    }

};



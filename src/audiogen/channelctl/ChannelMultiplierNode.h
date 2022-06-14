#pragma once

class ChannelMultiplierNode : public ChainingNode {
    public:
    ChannelMultiplierNode() : ChainingNode() {
        setTextureId( TEXTURE_CHANNEL_MUTLIPLIER );
        setAllowedInputs( 1 );
    }
    virtual ~ChannelMultiplierNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i += 2 ) {
                double lValue = double( inMem[ i ] ) / 32767.0;
                double rValue = double( inMem[ i + 1 ] ) / 32767.0;
                double joined = lValue * rValue * 32767.0;
                outMem[ i ] = joined;
                outMem[ i + 1 ] = joined;
            }
        }
    }

    virtual string getClassName() {
        return "Channel Multiplier";
    }

};



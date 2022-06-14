#pragma once

class ChannelInverterNode : public ChainingNode {
    public:
    ChannelInverterNode( double invert ) : ChainingNode() {
        setTextureId( TEXTURE_CHANNEL_INVERTER );
        setAllowedInputs( 1 );
        _invert = setParameter( "Invert", invert );
    }
    virtual ~ChannelInverterNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            int invertL;
            int invertR;
            for ( int i = 0; i < sampleCount; i += 2 ) {
                double inv = ( _invert[ i ] + _invert[ i + 1 ] ) / 2.0;
                invertL = inv < 0.5 ? 1 : - 1;
                invertR = inv >= 0.5 ? 1 : - 1;
                outMem[ i ] = inMem[ i ] * invertL;
                outMem[ i + 1 ] = inMem[ i + 1 ] * invertR;
            }
        }
    }

    virtual string getClassName() {
        return "Channel Inverter";
    }

    private:

    double* _invert;

};



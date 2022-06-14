#pragma once

class NoiseGenerator : public ChainingNode {
    public:
    NoiseGenerator() : ChainingNode() {
        setTextureId( TEXTURE_NOISEGEN );
        setAllowedInputs( 0 );
    }
    virtual ~NoiseGenerator() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        if ( output ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            for ( int i = 0; i < sampleCount; i += 2 ) {
                outMem[ i ] = ( rand() & 0xFF ) | ( ( rand() & 0xFF ) << 8 );
                outMem[ i + 1 ] = ( rand() & 0xFF ) | ( ( rand() & 0xFF ) << 8 );
            }
        }
    }

    virtual string getClassName() {
        return "Noise Generator";
    }

    private:

};




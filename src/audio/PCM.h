#pragma once

class PCM {
    private:
    PCM( const char* file ) {
        //___handle = fopen( file, "wb" );
    }
    ~PCM() {
        //fclose( ___handle );
    }
    public:

    #pragma pack( push, 1 )
    struct Header {
        public:
        Header() {
            chunkID = 0x46464952;
            chunkSize = 36;
            format = 0x45564157;
            subChunk1ID = 0x20746D66;
            subChunk1Size = 16;
            audioFormat = 1;
            numChannels = 2;
            sampleRate = 44100;
            bitsPerSample = 16;
            subChunk2ID = 0x61746164;
            subChunk2Size = 0;
            byteRate = sampleRate * numChannels * bitsPerSample / 8;
            blockAlign = numChannels * bitsPerSample / 8;
        }
        int chunkID;
        int chunkSize;
        int format;

        int subChunk1ID;
        int subChunk1Size;

        short audioFormat;
        short numChannels;
        int sampleRate;
        int byteRate;
        short blockAlign;
        short bitsPerSample;

        int subChunk2ID;
        int subChunk2Size;

    };
    #pragma pack( pop )

    private:
};



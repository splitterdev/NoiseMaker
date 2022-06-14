#pragma once

class AudioEffect {
    public:
    AudioEffect() {
    }
    virtual ~AudioEffect() {
    }

    virtual void doEffect( short* buffer, int sampleNum, int uniSampleNum, int channelNum ) = 0;

};


class AudioEffectFSD : public AudioEffect {
    public:
    AudioEffectFSD( double level ) : AudioEffect() {
        _lvl = level;
        _lvla = 1.0 - level;
    }
    ~AudioEffectFSD() {
    }

    void doEffect( short* buffer, int sampleNum, int uniSampleNum, int channelNum ) {
        for ( int i = 0; i < sampleNum; i++ ) {
            if ( buffer[ i ] ) {
                int s = ( buffer[ i ] > 0 ? 32767 : - 32767 );
                buffer[ i ] = ( buffer[ i ] * _lvla ) + s * _lvl;
            }
        }
    }

    private:

    double _lvl;
    double _lvla;

};

/*
#include <math.h>

#define A_EFF_B_LEN 400000

class AudioEffectA : public AudioEffect {
    public:
    AudioEffectA() : AudioEffect() {
        //___t = 0;
        ___lbp = 0;
        for ( int i = 0; i < A_EFF_B_LEN; i++ ) {
            ___lb[ i ] = 0;
        }
        ___lvl = 1.0;
    }
    ~AudioEffectA() {
    }

    void doEffect( short* buffer, int sampleNum, int uniSampleNum, int channelNum ) {
        for ( int i = 0; i < sampleNum; i++ ) {
        //for ( int i = 0; i < sampleNum; i += channelNum ) {
            int n = 5;
            int delay = 10000;
            int k = ___lbp - n * delay;
            if ( k < 0 ) {
                k += A_EFF_B_LEN;
            }
            double lvlA = ___lvl / double( n ) * 3.0;
            for ( int j = 1; j < n; j++ ) {
                int ind = ( k + j * delay ) % A_EFF_B_LEN;
                int b = buffer[ i ] + ___lb[ ind ] * pow( double( j ) / double( n ), 0.9 ) * lvlA;
                if ( b > 32767 ) {
                    b = 32767;
                } else if ( b < -32767 ) {
                    b = -32767;
                }
                buffer[ i ] = b;
                ___lb[ ind ] = b;
            }
            ___lbp++;

        }
    }

    private:

    //int ___t;
    short ___lb[ A_EFF_B_LEN ];
    int ___lbp;
    double ___lvl;

};
*/

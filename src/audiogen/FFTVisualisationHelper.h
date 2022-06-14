#pragma once

#include <complex>
#include <complex.h>

class FFTVisualisationHelper {
    public:
    FFTVisualisationHelper() {}
    virtual ~FFTVisualisationHelper() {}

    static Buffer* generateFFT( Buffer* buffer ) {
        int byteSize = buffer -> getSize();
        int sampleCount = byteSize >> 1;
        Buffer* out = Buffer::Create( byteSize );
        Buffer* gen = _transformFFT( ( short* )( buffer -> getMemory() ), sampleCount );//outMem, 0, byteSize >> 2, 1, byteSize >> 2 );
        short* outMem = ( short* )( out -> getMemory() );
        __complex__ double* inMem = ( __complex__ double* ) gen -> getMemory();
        for ( int i = 0; i < sampleCount; i++ ) {
            outMem[ i ] = creal( inMem[ i ] ) * 256.0;
        }
        delete gen;
        return out;
    }

    private:

    static Buffer* _transformFFT( short* mem, int size ) {
        Buffer* gen = Buffer::Create( size * sizeof( __complex__ double ) );
        _Radix2_DIT_Stereo( ( __complex__ double* ) gen -> getMemory(), mem, size, 1 );
        return gen;
    }

    static const int GAIN_MAX = 32767.0;

    static const __complex__ double COMPLEX_M2PI = - 2 * M_PI * I;

    static void _Radix2_DIT_Mono( __complex__ double* ret, short* x, int N, int s ) {
        if ( N == 1 ) {
            ( *ret ) = ( __complex__ double )( x[ 0 ] ) / GAIN_MAX;
        } else {
            int l2 = N >> 1;
            short* bE = ( short* )( malloc( N ) );
            short* bO = ( short* )( malloc( N ) );
            for ( int i = 0; i < l2; i++ ) {
                int a = i << 1;
                bE[ i ] = x[ a ];
                bO[ i ] = x[ a + s ];
            }
            _Radix2_DIT_Mono( ret, bE, l2, s );
            _Radix2_DIT_Mono( ret + l2, bO, l2, s );
            for ( int k = 0; k < l2; k++ ) {
                __complex__ double t = ret[ k ];
                __complex__ double apd = cexp( COMPLEX_M2PI * k / N ) * ret[ k + l2 ];
                ret[ k ] = t + apd;
                ret[ k + l2 ] = t - apd;
            }
            free( bE );
            free( bO );
        }
    }

    static void _Radix2_DIT_Stereo( __complex__ double* ret, short* x, int N, int s ) {
        int n2 = N >> 1;
        __complex__ double* l = ( __complex__ double* )( malloc( sizeof( __complex__ double ) * n2 ) );
        __complex__ double* r = ( __complex__ double* )( malloc( sizeof( __complex__ double ) * n2 ) );
        short* bl = ( short* )( malloc( sizeof( short ) * n2 ) );
        short* br = ( short* )( malloc( sizeof( short ) * n2 ) );
        for ( int i = 0; i < n2; i++ ) {
            int a = i << 1;
            bl[ i ] = x[ a ];
            br[ i ] = x[ a + 1 ];
        }
        _Radix2_DIT_Mono( l, bl, n2, s );
        _Radix2_DIT_Mono( r, br, n2, s );
        for ( int i = 0; i < n2; i++ ) {
            ret[ i ] = l[ i ] + r[ i ];
        }
        free( bl );
        free( br );
        free( l );
        free( r );
    }

};



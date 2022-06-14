#pragma once

#include "AudioChannel.h"

#include <stdlib.h>

#include <list>

class VorbisSound : public Sound {
    private:
    VorbisSound() : Sound() {
        ___alb = 0;
        ___b = NULL;
        ___bl = 0;
    }
    struct LBUF {
        public:
        char* data;
        int length;
    };
    public:
    static VorbisSound* create( const char* path ) {
        VorbisSound* snd = new VorbisSound();
        if ( snd -> read( path ) ) {
            return snd;
        }
        delete snd;
        return NULL;
    }
    static VorbisSound* create( string path ) {
        return create( path.c_str() );
    }
    ~VorbisSound() {
        alSourceStop( ___als );
        if ( ___b ) {
            free( ___b );
        }
        alDeleteBuffers( 1, &___alb );
    }
    void play( AudioChannel* channel, bool loop ) {
        if ( channel ) {
            ___als = channel -> getSource();
            alSourceStop( ___als );
            resetStream();
            alGenBuffers( 1, &___alb );
            alBufferData( ___alb, AL_FORMAT_STEREO16, ___b, ___bl, 44100 );
            alSourceQueueBuffers( ___als, 1, &___alb );
            alSourcei( ___als, AL_LOOPING, ( int )( loop ) );
            alSourcePlay( ___als );
        }
    }
    void resetStream() { /// UPDATE: probably an error occurs here. // Does not need to be reseted
        int processedBuffers = 0;
        alGetSourcei( ___als, AL_BUFFERS_PROCESSED, &processedBuffers );
        while ( processedBuffers > 0 ) {
            ALuint bufferUnqueued = 0;
            alSourceUnqueueBuffers( ___als, 1, &bufferUnqueued );
            alDeleteBuffers( 1, &bufferUnqueued );
            processedBuffers--;
        }
        alGetSourcei( ___als, AL_BUFFERS_QUEUED, &processedBuffers );
        while ( processedBuffers > 0 ) {
            ALuint bufferUnqueued = 0;
            alSourceUnqueueBuffers( ___als, 1, &bufferUnqueued );
            alDeleteBuffers( 1, &bufferUnqueued );
            processedBuffers--;
        }
    }
    void maintainStream() {} // There is no maintain for static sounds.
    protected:
    bool read( const char* fp ) {
        FILE* handle = fopen( fp, "rb" );
        if ( handle ) {
            OggVorbis_File vf;
            bool ret = false;
            if ( ov_open_callbacks( handle, &vf, NULL, 0, OV_CALLBACKS_DEFAULT ) == 0 ) {
                ret = true;
                const int BUFFER_SIZE = 8 * 1024;
                char buffer[ BUFFER_SIZE ];
                int csect;
                int bytesRead;
                list < LBUF > bufferedData;
                int length = 0;
                do {
                    bytesRead = ov_read( &vf, buffer, BUFFER_SIZE, 0, 2, 1, &csect );
                    if ( bytesRead > 0 ) {
                        char* ibuffer = ( char* ) malloc( bytesRead );
                        for ( int i = 0; i < bytesRead; i++ ) {
                            ibuffer[ i ] = buffer[ i ];
                        }
                        LBUF lbuf;
                        lbuf.data = ibuffer;
                        lbuf.length = bytesRead;
                        bufferedData.push_back( lbuf );
                        length += bytesRead;
                    }
                } while ( bytesRead > 0 );
                char* rdbuffer = ( char* ) malloc( length );
                list < LBUF >::iterator it = bufferedData.begin();
                int j = 0;
                while ( it != bufferedData.end() ) {
                    int br = it -> length;
                    char* ibuffer = it -> data;
                    for ( int i = 0; i < br; i++ ) {
                        rdbuffer[ j + i ] = ibuffer[ i ];
                    }
                    j += br;
                    free( ibuffer );
                    it++;
                }
                ___b = rdbuffer;
                ___bl = length;
            }
            ov_clear( &vf );
            fclose( handle );
            return ret;
        }
        return false;
    }
    private:
    ALuint ___alb;
    void* ___b;
    int ___bl;
};

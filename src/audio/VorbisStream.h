#pragma once

#include "AudioChannel.h"

#include <stdlib.h>

#include "PCM.h"

    //FILE* ___pcm;
    //volatile int ___pcmpos;

class VorbisStream : public Sound {
    private:
    VorbisStream( const char* path ) : Sound() {
        int n = strlen( path ) + 1;
        ___fpath = ( char* )( calloc( 1, n ) );
        memcpy( ___fpath, path, n );
        for ( int i = 0; i < ___BUFFERS_QUEUE_SIZE; i++ ) {
            ___albq[ i ] = 0;
            ___bq[ i ] = NULL;
            ___blq[ i ] = 0;
        }
        ___lqb = -1;
        ___handle = NULL;
        ___loop = false;
        //___pcm = fopen( "output.wav", "wb" );

        /*PCM::Header* header = new PCM::Header();
        ___pcmpos = sizeof( PCM::Header );
        fwrite( header, sizeof( PCM::Header ), 1, ___pcm );
        delete header;*/
    }
    public:
    static VorbisStream* create( const char* path ) {
        VorbisStream* str = new VorbisStream( path );
        if ( str -> read( path ) ) {
            return str;
        }
        delete str;
        return NULL;
    }
    static VorbisStream* create( string path ) {
        return create( path.c_str() );
    }
    ~VorbisStream() {
        free( ___fpath );
        alSourceStop( ___als );
        closeStream();
        for ( int i = 0; i < ___BUFFERS_QUEUE_SIZE; i++ ) {
            if ( ___bq[ i ] ) {
                free( ___bq[ i ] );
            }
            alDeleteBuffers( 1, &( ___albq[ i ] ) );
        }
        resetStream();
        /*PCM::Header* header = new PCM::Header();
        header -> chunkSize = ___pcmpos;
        header -> subChunk2Size = header -> chunkSize - 36;
        printf( "ftell = %d (header size %dB)\n", header -> chunkSize, sizeof( PCM::Header ) );
        fseek( ___pcm, 0, SEEK_SET );
        fwrite( header, sizeof( PCM::Header ), 1, ___pcm );
        delete header;
        fclose( ___pcm );*/
    }
    void play( AudioChannel* channel, bool loop ) {
        if ( channel ) {
            ___als = channel -> getSource();
            ov_pcm_seek( &___vf, 0 );
            alSourceStop( ___als );
            resetStream();
            for ( int i = 0; i < ___BUFFERS_QUEUE_SIZE; i++ ) {
                ___readAndQueueBuffer( i );
            }
            alSourcePlay( ___als );
            ___shouldMaintain = true;
            ___loop = loop;
            //printf( "Playing stream...\n" );
        }
    }
    void maintainStream() {
        if ( ( ___shouldMaintain ) && ( ___handle ) ) {
            //printf( "Active maintain %d\n", ___lqb );
            int processedBuffers;
            alGetSourcei( ___als, AL_BUFFERS_PROCESSED, &processedBuffers );
            //printf( "   > Buffers active: %d\n", processedBuffers );
            while ( processedBuffers > 0 ) {
                int nextBufferID = ( ___lqb + 1 ) % ___BUFFERS_QUEUE_SIZE;
                ALuint bufferUnqueued = 0;
                //printf( " > processed left = %d, replacing buffer %d\n", processedBuffers, nextBufferID );
                //alSourceUnqueueBuffers( ___als, 1, &___albq[ nextBufferID ] );
                alSourceUnqueueBuffers( ___als, 1, &bufferUnqueued );
                /*if ( !___readAndQueueBuffer( nextBufferID ) ) {
                    //___shouldMaintain = false;
                }*/
                if ( !___readAndQueueBuffer( nextBufferID ) ) {
                    //alDeleteBuffers( 1, &bufferUnqueued );
                    //___albq[ nextBufferID ] = 0;
                }
                processedBuffers--;
            }
            int queuedBuffers;
            alGetSourcei( ___als, AL_BUFFERS_QUEUED, &queuedBuffers );
            //printf( "QueuedBuffers = %d\n", queuedBuffers );
            if ( !queuedBuffers ) {
                //printf( "[maintainStream()] stopping the stream...\n" );
                alSourceStop( ___als );
                /*if ( ___loop ) {
                    ov_pcm_seek( &___vf, 100000 );
                    for ( int i = 0; i < ___BUFFERS_QUEUE_SIZE; i++ ) {
                        ___readAndQueueBuffer( ( ___lqb + 1 ) % ___BUFFERS_QUEUE_SIZE );
                    }
                    alSourcePlay( ___als );
                } else {*/
                    ___shouldMaintain = false;
                //}
            }
        }
    }
    protected:
    void resetStream() {
        /*int queuedBuffers;
        alGetSourcei( ___als, AL_BUFFERS_QUEUED, &queuedBuffers );
        while ( queuedBuffers > 0 ) {
            ALuint grinder;
            alSourceUnqueueBuffers( ___als, 1, &grinder );
            queuedBuffers--;
        }*/
        for ( int i = 0; i < ___BUFFERS_QUEUE_SIZE; i++ ) {
            alDeleteBuffers( 1, &___albq[ i ] );
            ___albq[ i ] = 0;
        }
    }
    bool read( const char* fp ) {
        ___handle = fopen( fp, "rb" );
        if ( ___handle ) {
            if ( ov_open_callbacks( ___handle, &___vf, NULL, 0, OV_CALLBACKS_DEFAULT ) == 0 ) {
                return true;
            }
            fclose( ___handle );
        }
        return false;
    }
    void closeStream() {
        if ( ___handle ) {
            ov_clear( &___vf );
            fclose( ___handle );
            ___handle = NULL;
        }
    }
    private:
    bool ___readAndQueueBuffer( int i, int bps = 2 ) {
        const int BUFFER_SIZE = 8 * 1024;
        char buffer[ BUFFER_SIZE ];
        int csect;
        int bytesRead = ov_read( &___vf, buffer, BUFFER_SIZE, 0, bps, 1, &csect );
        //printf( "bytesRead = %d\n", bytesRead );
        if ( ( bytesRead == 0 ) && ( ___loop ) ) {
            //printf( "Revert...\n" );
            ov_pcm_seek( &___vf, 0 );
            bytesRead = ov_read( &___vf, buffer, BUFFER_SIZE, 0, bps, 1, &csect );
        }
        if ( bytesRead > 0 ) {
            char* newBuffer = ( char* ) malloc( bytesRead );
            for ( int j = 0; j < bytesRead; j++ ) {
                newBuffer[ j ] = buffer[ j ];
            }
            if ( ___bq[ i ] ) {
                free( ___bq[ i ] );
            }
            ___blq[ i ] = bytesRead;
            ___bq[ i ] = newBuffer;
            if ( !___albq[ i ] ) {
                //printf( "Generating buffer %d\n", i );
                alGenBuffers( 1, &( ___albq[ i ] ) );
            }
            ALenum t = AL_FORMAT_STEREO16;
            if ( bps == 1 ) {
                t = AL_FORMAT_MONO16;
            }
                    //fwrite( ___bq[ i ], ___blq[ i ], 1, ___pcm );
                    //___pcmpos += ___blq[ i ];
                    //printf( "pcmPos = %d\n", ___pcmpos );
            list < AudioEffect* >::iterator it = ___effects.begin();
            int numSamplesUnique = ___blq[ i ] / bps;
            int numSamplesUniqueChannel = numSamplesUnique / bps;
            while ( it != ___effects.end() ) {
                AudioEffect* effect = *it;
                if ( effect ) {
                    effect -> doEffect( ( short* ) ___bq[ i ], numSamplesUnique, numSamplesUniqueChannel, bps );
                }
                it++;
            }
            alBufferData( ___albq[ i ], t, ___bq[ i ], ___blq[ i ], 44100 );
            alSourceQueueBuffers( ___als, 1, &( ___albq[ i ] ) );
            ___lqb = i;
            return true;
        } else {
            //alSourceQueueBuffers( ___als, 1, &( ___albq[ i ] ) );
        }
        return false;
    }
    static const int ___BUFFERS_QUEUE_SIZE = 48;
    ALuint ___albq[ ___BUFFERS_QUEUE_SIZE ];
    void* ___bq[ ___BUFFERS_QUEUE_SIZE ];
    int ___blq[ ___BUFFERS_QUEUE_SIZE ];
    int ___lqb;
    bool ___shouldMaintain;
    FILE* ___handle;
    char* ___fpath;
    OggVorbis_File ___vf;
    bool ___loop;
    int ___lh;




};

const int VorbisStream::___BUFFERS_QUEUE_SIZE;

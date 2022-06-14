#pragma once

class ChainPlayer : public Thread {
    public:

    #pragma pack(push,1)
    class PCM_Header {
        public:
        PCM_Header() {
            ChunkID = 0x46464952; // 'riff'
            Format = 0x45564157; // 'WAVE'
            Subchunk1ID = 0x20746d66; // 'fmt '
            Subchunk1Size = 16;
            AudioFormat = 1;
            NumChannels = 2;
            SampleRate = BITRATE;
            BitsPerSample = 16;
            Subchunk2ID = 0x61746164; // 'data'
        }
        void recalculateFix() {
            ByteRate = NumChannels * SampleRate * BitsPerSample / 8;
            BlockAlign = NumChannels * BitsPerSample / 8;
        }
        ~PCM_Header() {}
        Uint32 ChunkID; // auto
        Uint32 ChunkSize;
        Uint32 Format; // auto
        Uint32 Subchunk1ID; // auto
        Uint32 Subchunk1Size; // auto
        Uint16 AudioFormat; // auto
        Uint16 NumChannels; // auto
        Uint32 SampleRate; // auto
        Uint32 ByteRate; // auto
        Uint16 BlockAlign; // auto
        Uint16 BitsPerSample; // auto
        Uint32 Subchunk2ID; // auto
        Uint32 Subchunk2Size;
    };
    #pragma pack(pop)

    ChainPlayer() : Thread() {
        _chainingTree = new ChainingTree( "" );
        _ready = false;
        _playingStream = false;
        _mutex = new Mutex();
        _rec = false;
        _handle = NULL;
        start();
        while ( !isReady() );
    }
    virtual ~ChainPlayer() {
        _mutex -> lock();
        delete _mutex;
        finish();
        alert();
        join();
    }

    /*static void play( Buffer* buffer ) {
        alSourceStop( ___als );
        alBufferData( ___alb, AL_FORMAT_STEREO16, buffer -> getMemory(), buffer -> getSize(), BITRATE );
        alSourcei( ___als, AL_BUFFER, ___alb );
        alSourcePlay( ___als );
    }*/

    void run() {
        init();
        if ( _mutex -> lock() ) {
            //initStream( _chainingTree );
            initStream();
            _mutex -> release();
        }
        _ready = true;
        while ( !isDone() ) {
            if ( _playingStream ) {
                if ( _mutex -> lock() ) {
                    updateStream( _chainingTree, _rec, _handle );
                    _mutex -> release();
                }
            }
        }
    }

    bool isReady() {
        return _ready;
    }

    bool isSetPlaying() {
        return _playingStream;
    }

    void setPlaying( bool playingStream ) {
        _playingStream = playingStream;
        if ( _playingStream ) {
            if ( AUDIO_DRIVER_TYPE == AUDIO_DRIVER_OPENAL ) {
                alSourcePlay( ___als );
            } else if ( AUDIO_DRIVER_TYPE == AUDIO_DRIVER_BASS ) {
                BASS_ChannelPlay( _bassStream, false );
            }
        } else {
            if ( AUDIO_DRIVER_TYPE == AUDIO_DRIVER_OPENAL ) {
                alSourcePause( ___als );
            } else if ( AUDIO_DRIVER_TYPE == AUDIO_DRIVER_BASS ) {
                BASS_ChannelStop( _bassStream );
            }
        }
    }

    void changeTree( ChainingTree* tree ) {
        if ( _mutex -> lock() ) {
            if ( _chainingTree ) {
                delete _chainingTree;
            }
            _chainingTree = tree;
            _mutex -> release();
        }
    }

    bool lockTree() {
        return _mutex -> lock();
    }

    void releaseTree() {
        _mutex -> release();
    }

    void startRecording() {
        if ( !_rec ) {
            char* path = _getNewFileName();
            FILE* handle = fopen( path, "wb" );
            if ( handle ) {
                _handle = handle;
                PCM_Header header;
                header.recalculateFix();
                //fseek( _handle, SEEK_SET, sizeof( PCM_Header ) );
                fwrite( &header, sizeof( PCM_Header ), 1, _handle );
                _rec = true;
            }
            free( path );
        }
    }

    void stopRecording() {
        if ( _rec ) {
            if ( _handle ) {
                fseek( _handle, 0, SEEK_END );
                int fileSize = ftell( _handle );
                fseek( _handle, 0, SEEK_SET );
                PCM_Header header;
                header.ChunkSize = fileSize - ( sizeof( Uint32 ) << 1 );
                header.Subchunk2Size = fileSize - ( sizeof( PCM_Header ) );
                //printf( "ChunkSize = %d / chunk2 = %d\n", header.ChunkSize, header.Subchunk2Size );
                header.recalculateFix();
                //fseek( _handle, SEEK_SET, sizeof( PCM_Header ) );
                fwrite( &header, sizeof( PCM_Header ), 1, _handle );
                fclose( _handle );
            }
            _rec = false;
        }
    }

    bool isRecording() {
        return _rec;
    }

    protected:

    inline static void init() {
        if ( AUDIO_DRIVER_TYPE == AUDIO_DRIVER_OPENAL ) {
            alGetError();
            ___mainAudioDevice = alcOpenDevice( NULL );
            ___mainContext = alcCreateContext( ___mainAudioDevice, NULL );
            alcMakeContextCurrent( ___mainContext );
            alGenSources( 1, &___als );
        } else if ( AUDIO_DRIVER_TYPE == AUDIO_DRIVER_BASS ) {
            if ( BASS_Init( -1, BITRATE, BASS_DEVICE_FREQ, NULL, NULL ) ) {
                if ( BASS_Start() ) {
                    printf( "BASS Driver initialized.\n" );
                } else {
                    printf( "BASS Error: %d\n", BASS_ErrorGetCode() );
                }
            } else {
                printf( "BASS Error: %d\n", BASS_ErrorGetCode() );
            }
        }
    }

    inline static void updateStream( ChainingTree* tree, bool recording, FILE* file ) {
        if ( tree ) {
            Buffer* buffer = tree -> getOutputNode() -> getOutput();
            if ( AUDIO_DRIVER_TYPE == AUDIO_DRIVER_OPENAL ) {
                int processedBuffers;
                alGetSourcei( ___als, AL_BUFFERS_PROCESSED, &processedBuffers );
                /*if ( processedBuffers > 0 ) {
                    printf( "Processed = %d\n", processedBuffers );
                }*/
                while ( processedBuffers > 0 ) {
                    ALuint bufferUnqueued = 0;
                    alSourceUnqueueBuffers( ___als, 1, &bufferUnqueued );
                    tree -> processOutput();
                    if ( recording ) {
                        _dropBufferIntoFile( buffer, file );
                    }
                    alBufferData( bufferUnqueued, AL_FORMAT_STEREO16, buffer -> getMemory(), buffer -> getSize(), BITRATE );
                    alSourceQueueBuffers( ___als, 1, &bufferUnqueued );
                    processedBuffers--;
                }
            } else if ( AUDIO_DRIVER_TYPE == AUDIO_DRIVER_BASS ) {
                int n;
                int maxPushData = BUFFER_QUEUE * BUFFER_SIZE;
                if ( ( n = BASS_StreamPutData( _bassStream, NULL, 0 ) ) <= maxPushData ) {
                    tree -> processOutput();
                    if ( recording ) {
                        _dropBufferIntoFile( buffer, file );
                    }
                    BASS_StreamPutData( _bassStream, buffer -> getMemory(), buffer -> getSize() );
                    BASS_ChannelPlay( _bassStream, false );
                }
            }
        }
    }

    inline static void initStream() {
        if ( AUDIO_DRIVER_TYPE == AUDIO_DRIVER_OPENAL ) {
            Buffer* buffer = Buffer::Create( BUFFER_SIZE );
            for ( int i = 0; i < BUFFER_QUEUE; i++ ) {
                ALuint bufferGen;
                alGenBuffers( 1, &bufferGen );
                alBufferData( bufferGen, AL_FORMAT_STEREO16, buffer -> getMemory(), buffer -> getSize(), BITRATE );
                alSourceQueueBuffers( ___als, 1, &bufferGen );
            }
        } else if ( AUDIO_DRIVER_TYPE == AUDIO_DRIVER_BASS ) {
            _bassStream = BASS_StreamCreate( BITRATE, 2, 0, STREAMPROC_PUSH, NULL );
        }
    }

    private:

    inline static void _dropBufferIntoFile( Buffer* buffer, FILE* file ) {
        fwrite( buffer -> getMemory(), buffer -> getSize(), 1, file );
    }

    inline char* _getNewFileName() {
        char* name = ( char* ) calloc( 4096, 1 );
        sprintf( name, "recorderOutput_%ld.wav", time( NULL ) );
        return name;
    }

    static ALCdevice* ___mainAudioDevice;
    static ALCcontext* ___mainContext;
    static ALuint ___als;

    ChainingTree* _chainingTree;
    volatile bool _ready;
    volatile bool _playingStream;
    volatile bool _rec;

    static HSTREAM _bassStream;
    static Buffer* _bassBufferedData;

    Mutex* _mutex;
    FILE* _handle;

};

ALCdevice* ChainPlayer::___mainAudioDevice = NULL;
ALCcontext* ChainPlayer::___mainContext = NULL;
ALuint ChainPlayer::___als = 0;
HSTREAM ChainPlayer::_bassStream = 0;

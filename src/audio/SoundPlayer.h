#pragma once

//#include "temp/Thread.h"

#include "VorbisSound.h"
#include "VorbisStream.h"

#define AUDIO_CHANNEL_NUMBER 24

class SoundPlayer : public Thread {
    protected:
        class AbstractSTM {
            public:
            AbstractSTM() {}
            virtual ~AbstractSTM() {}
            virtual void perform( SoundPlayer* player ) = 0;
        };
        class ChannelDirectSTM : public AbstractSTM {
            public:
            ChannelDirectSTM( int channel ) : AbstractSTM() {
                _ch = channel;
            }
            virtual ~ChannelDirectSTM() {}
            protected:
            int _ch;
        };
        class PlaySoundSTM : public ChannelDirectSTM {
            public:
            PlaySoundSTM( Sound* snd, int channel ) : ChannelDirectSTM( channel ) {
                _toPlay = snd;
            }
            void perform( SoundPlayer* player ) {
                if ( _toPlay ) {
                    //printf( "Playing: %p / %d\n", _toPlay, _ch );
                    player -> _sound[ _ch ] = _toPlay;
                    _toPlay -> play( player -> _channel[ _ch ], false );
                }
            }
            protected:
            Sound* _toPlay;
        };
        class LoopSoundSTM : public ChannelDirectSTM {
            public:
            LoopSoundSTM( Sound* snd, int channel ) : ChannelDirectSTM( channel ) {
                _toPlay = snd;
            }
            void perform( SoundPlayer* player ) {
                if ( _toPlay ) {
                    player -> _sound[ _ch ] = _toPlay;
                    _toPlay -> play( player -> _channel[ _ch ], true );
                }
            }
            protected:
            Sound* _toPlay;
        };
        class SetPitchSTM : public ChannelDirectSTM {
            public:
            SetPitchSTM( int channel, double pitch ) : ChannelDirectSTM( channel ) {
                _p = pitch;
            }
            void perform( SoundPlayer* player ) {
                player -> _channel[ _ch ] -> setPitch( _p );
            }
            private:
            double _p;
        };
        class SetVolumeSTM : public ChannelDirectSTM {
            public:
            SetVolumeSTM( int channel, double volume ) : ChannelDirectSTM( channel ) {
                _v = volume;
            }
            void perform( SoundPlayer* player ) {
                player -> _channel[ _ch ] -> setVolume( _v );
            }
            private:
            double _v;
        };
        class SetPosSTM : public ChannelDirectSTM {
            public:
            SetPosSTM( int channel, double* pos ) : ChannelDirectSTM( channel ) {
                for ( int i = 0; i < 3; i++ ) {
                    _p[ i ] = pos[ i ];
                }
            }
            void perform( SoundPlayer* player ) {
                player -> _channel[ _ch ] -> setPosition( _p );
            }
            private:
            double _p[ 3 ];
        };
        class PlayChannelSTM : public ChannelDirectSTM {
            public:
            PlayChannelSTM( int channel ) : ChannelDirectSTM( channel ) {}
            void perform( SoundPlayer* player ) {
                player -> _channel[ _ch ] -> play();
                //player -> _sound[ _ch ] -> play( player -> _channel[ _ch ], player -> _channel[ _ch ] -> isLooping() );
            }
        };
        class PauseChannelSTM : public ChannelDirectSTM {
            public:
            PauseChannelSTM( int channel ) : ChannelDirectSTM( channel ) {}
            void perform( SoundPlayer* player ) {
                //player -> _sound[ _ch ] -> pause();
                player -> _channel[ _ch ] -> pause();
            }
        };
        class StopChannelSTM : public ChannelDirectSTM {
            public:
            StopChannelSTM( int channel ) : ChannelDirectSTM( channel ) {}
            void perform( SoundPlayer* player ) {
                //player -> _sound[ _ch ] -> stop();
                player -> _channel[ _ch ] -> stop();
                //player -> _sound[ _ch ] -> resetStream();
            }
        };
    private:
    SoundPlayer() : Thread() {
        _m = CreateMutex( NULL, FALSE, NULL );
        _c = CreateMutex( NULL, FALSE, NULL );
        _st = 20;
        for ( int i = 0; i < AUDIO_CHANNEL_NUMBER; i++ ) {
            _channel[ i ] = new AudioChannel();
            _sound[ i ] = NULL;
        }
        start();
    }
    ~SoundPlayer() {
        finish();
        alert();
        join();
        CloseHandle( _m );
        CloseHandle( _c );
        for ( int i = 0; i < AUDIO_CHANNEL_NUMBER; i++ ) {
            delete _channel[ i ];
        }
    }
    void run() {
        while ( !isDone() ) {
            //printf( "tick.\n" );
            sleep( _st );
            WaitForSingleObject( _m, INFINITE );
            while ( !_q.empty() ) {
                AbstractSTM* message = _q.front();
                _q.pop_front();
                message -> perform( this );
                delete message;
            }
            for ( int i = 0; i < AUDIO_CHANNEL_NUMBER; i++ ) {
                if ( _sound[ i ] ) {
                    _sound[ i ] -> maintainStream();
                    /*if ( _channel[ i ] -> isLooping() ) {
                        if ( !_channel[ i ] -> isPlaying() ) {
                            //printf( "K\n" );
                            _sound[ i ] -> play( _channel[ i ], true );
                        }
                    }*/
                }
            }
            ReleaseMutex( _m );
        }
    }
    public:
    static SoundPlayer* getPlayer() {
        if ( !___instance ) {
            ___instance = new SoundPlayer();
            atexit( close );
        }
        return ___instance;
    }
    static void close() {
        if ( ___instance ) {
            delete ___instance;
            ___instance = NULL;
        }
    }

    void playSound( Sound* sound, int channel ) {
        WaitForSingleObject( _m, INFINITE );
        //printf( "Playing sound on channel %d.\n", channel );
        _q.push_back( new PlaySoundSTM( sound, channel ) );
        ReleaseMutex( _m );
        alert();
    }
    void loopSound( Sound* sound, int channel ) {
        WaitForSingleObject( _m, INFINITE );
        //printf( "Looping sound on channel %d.\n", channel );
        _q.push_back( new LoopSoundSTM( sound, channel ) );
        ReleaseMutex( _m );
        alert();
    }
    void setPitch( int channel, double pitch ) {
        WaitForSingleObject( _m, INFINITE );
        _q.push_back( new SetPitchSTM( channel, pitch ) );
        ReleaseMutex( _m );
        alert();
    }
    void setVolume( int channel, double volume ) {
        WaitForSingleObject( _m, INFINITE );
        _q.push_back( new SetVolumeSTM( channel, volume ) );
        ReleaseMutex( _m );
        alert();
    }
    void setPosition( int channel, double* pos ) {
        WaitForSingleObject( _m, INFINITE );
        _q.push_back( new SetPosSTM( channel, pos ) );
        ReleaseMutex( _m );
        alert();
    }
    void playChannel( int channel ) {
        WaitForSingleObject( _m, INFINITE );
        _q.push_back( new PlayChannelSTM( channel ) );
        ReleaseMutex( _m );
        alert();
    }
    void pauseChannel( int channel ) {
        WaitForSingleObject( _m, INFINITE );
        _q.push_back( new PauseChannelSTM( channel ) );
        ReleaseMutex( _m );
        alert();
    }
    void stopChannel( int channel ) {
        WaitForSingleObject( _m, INFINITE );
        _q.push_back( new StopChannelSTM( channel ) );
        ReleaseMutex( _m );
        alert();
    }

    bool isChannelPlaying( int channel ) {
        WaitForSingleObject( _m, INFINITE );
        AudioChannel* c = _channel[ channel ];
        bool playing = false;
        if ( c ) {
            playing = c -> isPlaying();
        }
        ReleaseMutex( _m );
        return playing;
    }
    private:
    list < AbstractSTM* > _q;
    HANDLE _m;
    HANDLE _c;
    int _st;
    AudioChannel* _channel[ AUDIO_CHANNEL_NUMBER ];
    Sound* _sound[ AUDIO_CHANNEL_NUMBER ];
    //int _sndType[ AUDIO_CHANNEL_NUMBER ]

    static SoundPlayer* ___instance;

};

SoundPlayer* SoundPlayer::___instance = NULL;


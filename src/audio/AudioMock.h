#pragma once

#define AUDIO_MOCK_ENABLED 1

#pragma once

#include <string>
#include <string.h>

using namespace std;

#include <list>

#include "AudioChannel.h"
#include "effect/AudioEffect.h"

class Sound {
    public:
    static void Init() {
    }
    static void Close() {
    }
    Sound() {
    }
    virtual ~Sound() {
    }
    virtual void play( AudioChannel* channel, bool loop ) = 0;
    bool isPlaying() {
        return false;
    }
    bool isStopped() {
        return false;
    }
    void stop() {
    }
    void pause() {
    }
    void resume() {
    }
    virtual void resetStream() = 0;
    virtual void maintainStream() = 0;

    void addEffect( AudioEffect* effect ) {
    }
    void flushEffects() {
    }
    static double calculateDistanceLevel( Vector3D a, Vector3D b ) {
        return NAN;
    }

    protected:
    virtual bool read( const char* fp ) = 0;

};

class AudioManager {
    private:
    AudioManager() {}
    ~AudioManager() {}

    public:

    static void loadPathesFromFile( const char* path, const char* root, int mapID ) {
    }

    static void addMapping( int mapID, int id, Sound* snd ) {
    }

    static Sound* getSound( int mapID, int id ) {
        return NULL;
    }

    static void PlaySound( int mapID, int id, int channel_id ) {
    }

    static Sound* getRandomSound( int mapID ) {
        return NULL;
    }

    static void Exit() {}

    private:

    static map < int, map < int, Sound* >* > _m;

};

map < int, map < int, Sound* >* > AudioManager::_m;

#define CHANNEL_HIT 1
#define CHANNEL_DEATH 2
#define CHANNEL_HIT_EMPTY 3
#define CHANNEL_PAIN 4
#define CHANNEL_MONSTER 5
#define CHANNEL_GHOST 6
#define CHANNEL_DYNAMIC_MISC 7
#define CHANNEL_KAYLEY 8

#define CHANNEL_BACKGROUND 9

#define CHANNEL_MENU_CLICK 10
#define CHANNEL_MENU_SELECT 11

#define CHANNEL_GAME_SOUNDS 10
#define CHANNEL_GAME_SOUNDS_2 11

#define CHANNEL_FOOT 12
#define CHANNEL_SHOOT 13

#define CHANNEL_RELOAD 14
#define CHANNEL_MISC 15

#define CHANNEL_GEIGER 16
#define CHANNEL_THUNDER 17

#define AUDIO_CHANNEL_NUMBER 16

class SoundPlayer : public Thread {
    protected:
        /*class AbstractSTM {
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
        };*/
    private:
    SoundPlayer() : Thread() {
    }
    ~SoundPlayer() {
    }
    void run() {
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
    }
    void loopSound( Sound* sound, int channel ) {
    }
    void setPitch( int channel, double pitch ) {
    }
    void setVolume( int channel, double volume ) {
    }
    void setPosition( int channel, double* pos ) {
    }
    void playChannel( int channel ) {
    }
    void pauseChannel( int channel ) {
    }
    void stopChannel( int channel ) {
    }

    bool isChannelPlaying( int channel ) {
        return true;
    }
    private:
    static SoundPlayer* ___instance;

};

SoundPlayer* SoundPlayer::___instance = NULL;

#define SOUND_MAP 1
#define SOUND_BLOOD_MAP 2
#define AMBIENT_MAP_CALM 3
#define AMBIENT_MAP_ACTION 4

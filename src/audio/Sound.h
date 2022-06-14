#pragma once

#include <al/al.h>
#include <al/alc.h>

#include <ogg/ogg.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#include <string>
#include <string.h>

using namespace std;

#include <list>

#include "AudioChannel.h"
#include "effect/AudioEffect.h"

class Sound {
    public:
    static void Init() {
        alGetError();
        ___mainAudioDevice = alcOpenDevice( NULL );
        ___mainContext = alcCreateContext( ___mainAudioDevice, NULL );
        alcMakeContextCurrent( ___mainContext );
    }
    static void Close() {
    }
    Sound() {
        ___als = 0;
    }
    virtual ~Sound() {
        flushEffects();
    }
    virtual void play( AudioChannel* channel, bool loop ) = 0;
    bool isPlaying() {
        int state = AL_STOPPED;
        alGetSourcei( ___als, AL_SOURCE_STATE, &state );
        return state == AL_PLAYING;
    }
    bool isStopped() {
        int state = AL_PLAYING;
        alGetSourcei( ___als, AL_SOURCE_STATE, &state );
        return state == AL_STOPPED;
    }
    void stop() {
        alSourceStop( ___als );
    }
    void pause() {
        alSourcePause( ___als );
    }
    void resume() {
        alSourcePlay( ___als );
    }
    virtual void resetStream() = 0;// {
        /*int queuedBuffers;
        alGetSourcei( ___als, AL_BUFFERS_QUEUED, &queuedBuffers );
        //printf( "Queued when reset: %d\n", queuedBuffers );
        while ( queuedBuffers > 0 ) {
            ALuint grinder;
            alSourceUnqueueBuffers( ___als, 1, &grinder );
            alDeleteBuffers( 1, &grinder );
            queuedBuffers--;
        }*/
    //}
    virtual void maintainStream() = 0;

    void addEffect( AudioEffect* effect ) {
        ___effects.push_back( effect );
    }
    void flushEffects() {
        list < AudioEffect* >::iterator it = ___effects.begin();
        while ( it != ___effects.end() ) {
            if ( *it ) {
                delete *it;
            }
            it++;
        }
        ___effects.clear();
    }

    //static const double SOUND_DROP_LEVEL = 0.32;
    static const double SOUND_ATTENUATION_LEVEL = -0.027903;

    static double calculateDistanceLevel( Vector3D a, Vector3D b ) {
        return pow( M_E, a.dist3D( b ) * SOUND_ATTENUATION_LEVEL );//1.0 / pow( ( a.dist3D( b ) + 1.0 ), SOUND_DROP_LEVEL );
    }

    /*list < AudioEffect* >* getEffectList() {
        return &___effects;
    }*/

    protected:
    virtual bool read( const char* fp ) = 0;
    ALuint ___als;
    list < AudioEffect* > ___effects;
    private:
    static ALCdevice* ___mainAudioDevice;
    static ALCcontext* ___mainContext;

};

ALCdevice* Sound::___mainAudioDevice = NULL;
ALCcontext* Sound::___mainContext = NULL;

#pragma once

#include <al/al.h>
#include <al/alc.h>

#include <stdio.h>

class AudioChannel {
    public:
    AudioChannel() {
        alGenSources( 1, &___src );
        setPitch( 1.0 );
        setVolume( 1.0 );
        alSource3f( ___src, AL_POSITION, 0, 0, 0 );
        alSource3f( ___src, AL_VELOCITY, 0, 0, 0 );
        alSourcei( ___src, AL_LOOPING, AL_FALSE );
    }
    ~AudioChannel() {
        alDeleteSources( 1, &___src );
    }
    ALuint getSource() {
        return ___src;
    }
    void setPitch( double rate ) {
        alSourcef( ___src, AL_PITCH, rate );
    }
    void setVolume( double vol ) {
        alSourcef( ___src, AL_GAIN, vol );
    }
    /**
        NOT working with stereo sounds. Use mono ambient to manipulate this.
    */
    void setPosition( double* pos ) {
        alSource3f( ___src, AL_POSITION, pos[ 0 ], pos[ 1 ], pos[ 2 ] );
    }
    void setLoop( bool loop ) {
        ___loop = loop;
    }
    bool isLooping() {
        return ___loop;
    }
    void play() {
        alSourcePlay( ___src );
    }
    void pause() {
        alSourcePause( ___src );
    }
    void stop() {
        alSourceStop( ___src );
        alSourcei( ___src, AL_BUFFER, 0 );
    }
    bool isPlaying() {
        int state;
        alGetSourcei( ___src, AL_SOURCE_STATE, &state );
        return state == AL_PLAYING;
    }
    private:
    ALuint ___src;
    bool ___loop;
};

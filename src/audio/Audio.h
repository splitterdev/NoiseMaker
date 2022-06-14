#pragma once

#include "AudioChannel.h"

#include "Sound.h"
#include "VorbisSound.h"
#include "VorbisStream.h"

#include "SoundPlayer.h"

#include <map>

#define AUDIOMANAGER_SOUND_MAP 1
#define AUDIOMANAGER_RADIO_MAP 2

class AudioManager {
    private:
    AudioManager() {}
    ~AudioManager() {}

    protected:

    /*class IdArray {
        public:
        IdArray() {
            _a = NULL;
            _l = 0;
        }
        ~IdArray() {
            if ( _a ) {
                free( _a );
            }
        }

        void append( int b ) {
            _a = realloc( _a, sizeof( int ) * ( _l + 1 ) );
            _a[ _l ] = b;
            _l++;
        }

        int getItem( int i ) {
            if ( ( i < 0 ) || ( i >= _l ) ) {
                return -1;
            }
            return _a[ i ];
        }

        int getSize() {
            return _l;
        }

        private:

        int* _a;
        int _l;

    };*/


    public:

    static void loadPathesFromFile( const char* path, const char* root, int mapID ) {
        FILE* handle = fopen( path, "r" );
        if ( handle ) {
            int line = 0;
            while ( !feof( handle ) ) {
                char buffer[ 4096 ];
                if ( fgets( buffer, 4096, handle ) ) {
                    line++;
                    int id;
                    char pth[ 4096 ];
                    char kword[ 4096 ];
                    int n;
                    if ( ( n = sscanf( buffer, " %s %d %s ", kword, &id, pth ) ) == 3 ) {
                        string fpath = string( root ) + string( pth );
                        if ( strcasecmp( kword, "SOUND" ) == 0 ) {
                            addMapping( mapID, id, VorbisSound::create( fpath.c_str() ) );
                            //printf( " > sound[ %d, \"%s\" ]\n", id, fpath.c_str() );
                        } else if ( strcasecmp( kword, "STREAM" ) == 0 ) {
                            //Sound* s = VorbisStream::create( fpath.c_str() );
                            //s -> addEffect( new AudioEffectFSD( 1.0 ) );
                            addMapping( mapID, id, VorbisStream::create( fpath.c_str() ) );
                            //printf( " > stream[ %d, \"%s\" ]\n", id, fpath.c_str() );
                        } else {
                            printf( " > Unknown AudioManager sound type: \"%s\": at line %d in \"%s\".\n", kword, line, path );
                        }
                    } else {
                        printf( " > Error: arguments can't math ( parsed %d from \"%s\" ): at line %d in \"%s\".\n", n, buffer, line, path );
                    }
                }
            }
            fclose( handle );
        } else {
            printf( "[ERROR:AudioManager:loadPathesFromFile] file \"%s\" not found.\n", path );
        }
    }

    static void addMapping( int mapID, int id, Sound* snd ) {
        if ( snd ) {
            map < int, map < int, Sound* >* >::iterator mapIT = _m.find( mapID );
            map < int, Sound* >* soundMap = NULL;
            if ( mapIT != _m.end() ) {
                soundMap = mapIT -> second;
            }
            if ( !soundMap ) {
                //printf( "Adding soundmap\n" );
                soundMap = new map < int, Sound* > ();
                _m[ mapID ] = soundMap;
            }
            pair < int, Sound* > p;
            p.first = id;
            p.second = snd;
            soundMap -> insert( p );
        }
    }

    static Sound* getSound( int mapID, int id ) {
        map < int, map < int, Sound* >* >::iterator mapIT = _m.find( mapID );
        if ( mapIT != _m.end() ) {
            if ( mapIT -> second ) {
                map < int, Sound* >::iterator found = mapIT -> second -> find( id );
                if ( found != mapIT -> second -> end() ) {
                    return found -> second;
                }
            }
        }
        return NULL;
    }

    static void PlaySound( int mapID, int id, int channel_id ) {
        SoundPlayer::getPlayer() -> playSound( AudioManager::getSound( mapID, id ), channel_id );
    }

    static Sound* getRandomSound( int mapID ) {
        map < int, map < int, Sound* >* >::iterator mapIT = _m.find( mapID );
        if ( mapIT != _m.end() ) {
            map < int, Sound* >* m = mapIT -> second;
            if ( m ) {
                int s = m -> size();
                if ( s > 0 ) {
                    int k = rand() % s;
                    map < int, Sound* >::iterator found = m -> begin();
                    for ( int i = 0; i < k; i++ ) {
                        found++;
                    }
                    if ( found != m -> end() ) {
                        return found -> second;
                    }
                }
            }
        }
        return NULL;
    }

    /*static void Load( const char* slfPath ) {
        StringLinker* slf = StringLinker::FromFile( slfPath );
        if ( slf ) {
            for ( int i = 0; i < slf ->entryCount(); i++ ) {
                slf -> getEntryID( index );
            }
        }
    }*/

    static void Exit() {
        map < int, map < int, Sound* >* >::iterator mapIT = _m.begin();
        while ( mapIT != _m.end() ) {
            map < int, Sound* >* m = mapIT -> second;
            if ( m ) {
                map < int, Sound* >::iterator it = m -> begin();
                if ( it != m -> end() ) {
                    if ( it -> second ) {
                        delete it -> second;
                    }
                }
            }
            mapIT++;
        }
        _m.clear();
    }

    private:

    static map < int, map < int, Sound* >* > _m;
    //static map < int, IdArray* > _mIDS;

};

map < int, map < int, Sound* >* > AudioManager::_m;
//map < int, IdArray* > AudioManager::_mIDS;

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

#define SOUND_MAP 1
#define SOUND_BLOOD_MAP 2
#define AMBIENT_MAP_CALM 3
#define AMBIENT_MAP_ACTION 4


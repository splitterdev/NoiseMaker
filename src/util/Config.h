#pragma once

#include <stdlib.h>
#include <math.h>

vector < string > _split( const char* input, char delimiter ) {
    vector < string > v;
    string vs = "";
    for ( int i = 0; i < int( strlen( input ) ); i++ ) {
        if ( input[ i ] == '\n' ) {
            continue;
        }
        if ( input[ i ] == delimiter ) {
            v.push_back( vs );
            vs = "";
        } else {
            vs += input[ i ];
        }
    }
    if ( vs.size() > 0 ) {
        v.push_back( vs );
    }
    return v;
}

string _trim( string str ) {
    if ( str.empty() ) {
        return str;
    }

    const char* pStr = str.c_str();

    size_t front = 0;
    while ( front < str.length() && isspace( int( pStr[ front ] ) ) ) {
        ++front;
    }

    size_t back = str.length();
    while ( back > front && isspace( int( pStr[ back - 1 ] ) ) ) {
        --back;
    }

    if ( 0 == front ) {
        if ( back < str.length() ) {
            str.resize( back - front );
        }
    } else if( back <= front ) {
        str.clear();
    } else {
        str = string( str.begin() + front, str.begin() + back );
    }
    return str;
}

double _double( string s ) {
    return atof( s.c_str() );
}

class Config {
    public:
    Config() {
    }
    ~Config() {
    }

    static void initialize() {
        if ( readConfig( "data/config.cfg" ) ) {
            double resXDouble = getParameter( "resolution.X" );
            double resYDouble = getParameter( "resolution.Y" );
            double resFSDouble = getParameter( "resolution.FullScreen" );
            double resAudioDouble = getParameter( "audio.Driver" );
            double resBufferSizeDouble = getParameter( "audio.Buffer" );
            double resBufferQueueSizeDouble = getParameter( "audio.BufferQueue" );
            if ( !isnan( resXDouble ) && ( resXDouble > 16 ) ) {
                WINDOW_WIDTH = resXDouble;
            }
            if ( !isnan( resYDouble ) && ( resYDouble > 16 ) ) {
                WINDOW_HEIGHT = resYDouble;
            }
            if ( !isnan( resFSDouble ) ) {
                WINDOW_FULLSCREEN = resFSDouble;
            }
            if ( !isnan( resAudioDouble ) ) {
                AUDIO_DRIVER_TYPE = resAudioDouble;
            }
            if ( !isnan( resBufferSizeDouble ) ) {
                BUFFER_SIZE = resBufferSizeDouble;
            }
            if ( !isnan( resBufferSizeDouble ) ) {
                BUFFER_QUEUE = resBufferQueueSizeDouble;
            }
        } else {
            FILE* handle = fopen( "data/config.cfg", "w" );
            if ( handle ) {
                fprintf( handle, "resolution.X = %d\n", WINDOW_WIDTH );
                fprintf( handle, "resolution.Y = %d\n", WINDOW_HEIGHT );
                fprintf( handle, "resolution.FullScreen = %d\n", WINDOW_FULLSCREEN );
                fprintf( handle, "audio.Driver = %d\n", AUDIO_DRIVER_TYPE );
                fprintf( handle, "audio.Buffer = %d\n", BUFFER_SIZE );
                fprintf( handle, "audio.BufferQueue = %d\n", BUFFER_QUEUE );
                fclose( handle );
            }
        }
    }

    static bool readConfig( const char* path ) {
        FILE* handle = fopen( path, "r" );
        if ( handle ) {
            char* buffer = ( char* ) malloc( 4096 );
            do {
                if ( fgets( buffer, 4095, handle ) ) {
                    vector < string > split = _split( buffer, '=' );
                    if ( int( split.size() ) > 0 ) {
                        if ( split.size() == 2 ) {
                            string paramName = _trim( split[ 0 ] );
                            double value = _double( split[ 1 ] );
                            _params[ paramName ] = value;
                            printf( "Read: %s = %g\n", paramName.c_str(), value );
                        }
                    }
                }
            } while ( !feof( handle ) );
            free( buffer );
            fclose( handle );
            return true;
        }
        return false;
    }

    static double getParameter( string name ) {
        map < string, double >::iterator i = _params.find( name );
        if ( i != _params.end() ) {
            return i -> second;
        }
        return NAN;
    }

    static bool getIfPresent( string name, double* value ) {
        double v = getParameter( name );
        if ( !isnan( v ) ) {
            ( *value ) = v;
            return true;
        }
        return false;
    }

    private:

    static map < string, double > _params;

};

map < string, double > Config::_params;


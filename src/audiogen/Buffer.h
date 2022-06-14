#pragma once

class Buffer {
    protected:
    Buffer( int byteSize ) {
        _byteSize = byteSize;
        _buffer = malloc( _byteSize );
        memset( _buffer, 0, _byteSize );
    }

    public:
    virtual ~Buffer() {
        if ( _buffer ) {
            free( _buffer );
        }
    }

    static Buffer* Create( int byteSize ) {
        if ( byteSize >= 0 ) {
            return new Buffer( byteSize );
        }
        return NULL;
    }

    void* getMemory() {
        return _buffer;
    }

    int getSize() {
        return _byteSize;
    }

    void copyFrom( Buffer* src, int outPos, int bytes ) {
        if ( src ) {
            char* out = ( char* ) _buffer;
            char* in = ( char* ) src -> _buffer;
            for ( int i = 0; i < bytes; i++ ) {
                out[ outPos ] = in[ i ];
                outPos++;
                if ( outPos >= _byteSize ) {
                    outPos = outPos % _byteSize;
                }
            }
        }
    }

    private:

    void* _buffer;
    int _byteSize;

};



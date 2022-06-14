#pragma once

class BufferedParameter {
    protected:
    BufferedParameter( int sampleCount ) {
        _sampleCount = sampleCount;
        _buffer = malloc( _sampleCount * sizeof( double ) * 2.0 );
        memset( _buffer, 0, _sampleCount * sizeof( double ) * 2.0 );
    }

    public:
    virtual ~BufferedParameter() {
        if ( _buffer ) {
            free( _buffer );
        }
    }

    static BufferedParameter* Create( int sampleCount ) {
        if ( sampleCount >= 0 ) {
            return new BufferedParameter( sampleCount );
        }
        return NULL;
    }

    double* getMemory() {
        return _buffer;
    }

    int getSize() {
        return _sampleCount;
    }

    private:

    double* _buffer;
    int _sampleCount;
};




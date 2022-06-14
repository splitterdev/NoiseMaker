#pragma once

class TriangleNoiseGenerator : public ChainingNode {
    public:
    TriangleNoiseGenerator( double freq, double freqOsc, double volume ) : ChainingNode() {
        _baseFreq = setParameter( "Freq", freq );
        _baseFreqOsc = setParameter( "F/Rnd", freqOsc );
        _baseVolume = setParameter( "Vol", volume );
        setTextureId( TEXTURE_TRIANGLENOISEGEN );
        setAllowedInputs( 0 );
        _cLevel = 0;
        _pLevel = 0;
        _nLevel = 0;
        _tStart = 0;
        _tNext = 0;
        _t = 0;
    }
    virtual ~TriangleNoiseGenerator() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        if ( output ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            for ( int i = 0; i < sampleCount; i+= 2 ) {
                double _bRnd;
                double _minRnd = BITRATE / _baseFreq[ i ];
                int freqCh = _minRnd * _baseFreqOsc[ i ];
                _bRnd = freqCh << 1;
                _minRnd -= freqCh;
                if ( _minRnd <= 0 ) {
                    _minRnd = 1;
                }
                if ( _bRnd <= 0 ) {
                    _bRnd = 0;
                }
                if ( _t >= _tNext ) {
                    _pLevel = _nLevel;
                    _nLevel = ( rand() & 0xFF ) | ( ( rand() & 0xFF ) << 8 );
                    _tStart = _tNext;
                    _tNext = _tStart + _rndVal() % int( _bRnd + 1 ) + _minRnd;
                }
                double d = double( _t - _tStart ) / double( _tNext - _tStart );
                _cLevel = ( _pLevel * ( 1.0 - d ) + _nLevel * d );
                outMem[ i ] = _cLevel * _baseVolume[ i ];
                outMem[ i + 1 ] = _cLevel * _baseVolume[ i + 1 ];
                _t++;
            }
        }
    }

    virtual string getClassName() {
        return "Triangle Noise Generator";
    }

    private:

    inline int _rndVal() {
        int a = rand() & 0xFF;
        int b = rand() & 0xFF;
        int c = rand() & 0xFF;
        int d = rand() & 0xFF;
        return ( ( a << 24 ) & 0xFF000000 ) | ( ( b << 16 ) & 0x00FF0000 ) | ( ( c << 8 ) & 0x0000FF00 ) | ( d & 0x000000FF );
    }

    double* _baseFreq;
    double* _baseFreqOsc;
    double* _baseVolume;

    short _cLevel;
    short _pLevel;
    short _nLevel;
    int _tStart;
    int _tNext;
    int _t;

};




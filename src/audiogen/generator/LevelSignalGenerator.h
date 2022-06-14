#pragma once

class LevelSignalGenerator : public ChainingNode {
    public:
    LevelSignalGenerator( double freq, double freqOsc, double volume ) : ChainingNode() {
        _baseFreq = setParameter( "Freq", freq );
        _baseFreqOsc = setParameter( "Freq/O", freqOsc );
        _baseVolume = setParameter( "Vol", volume );
        setTextureId( TEXTURE_LEVELSIGNALGEN );
        setAllowedInputs( 0 );
        _cDuration = 0;
    }
    virtual ~LevelSignalGenerator() {
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
                if ( _cDuration <= 0 ) {
                    _cLevel = ( rand() & 0xFF ) | ( ( rand() & 0xFF ) << 8 );
                    _cDuration = _rndVal() % int( _bRnd + 1 ) + _minRnd;
                }
                _cLevel = double( _cLevel );
                outMem[ i ] = _cLevel * _baseVolume[ i ];
                outMem[ i + 1 ] = _cLevel * _baseVolume[ i + 1 ];
                _cDuration--;
            }
        }
    }

    virtual string getClassName() {
        return "Level Signal Generator";
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
    int _cDuration;

};



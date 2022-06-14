#pragma once

class UpWaveGenerator : public ChainingNode {
    public:
    UpWaveGenerator( double dropTime, double cooldownTime ) : ChainingNode() {
        _baseDrop = setParameter( "Drop", dropTime );
        _baseCoolDown = setParameter( "Cool", cooldownTime );
        setTextureId( TEXTURE_UPWAVEGEN );
        setAllowedInputs( 0 );
        _phase = 0;
        _coolDownTimer = 0.0;
        _dropTimer = 0.0;
        _dropTimerStart = 1.0;
    }
    virtual ~UpWaveGenerator() {
    }

    virtual void processOutput() {
        static const double TQ = 1.0 / double( BITRATE );
        Buffer* output = getOutput();
        if ( output ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            for ( int i = 0; i < sampleCount; i+= 2 ) {
                switch ( _phase ) {
                    case 0: {
                        double v = ( _dropTimer / _dropTimerStart );
                        v = ( 1.0 - v - 0.5 ) * 65535.0;
                        outMem[ i ] = v;
                        outMem[ i + 1 ] = v;
                        _dropTimer -= TQ;
                        if ( _dropTimer <= 0.0 ) {
                            _coolDownTimer = 0.0;
                            _phase = 1;
                        }
                        break;
                    }
                    case 1: {
                        outMem[ i ] = 0x7FFF;
                        outMem[ i + 1 ] = 0x7FFF;
                        _coolDownTimer += TQ;
                        if ( _coolDownTimer >= _baseCoolDown[ i ] ) {
                            _dropTimerStart = ( _baseDrop[ i ] + _baseDrop[ i + 1 ] ) / 2.0;
                            _dropTimer = _dropTimerStart;
                            _phase = 0;
                        }
                        break;
                    }
                }
            }
        }
    }

    virtual string getClassName() {
        return "Up Wave Generator";
    }

    private:

    double* _baseDrop;
    double* _baseCoolDown;

    int _phase;
    double _coolDownTimer;
    double _dropTimer;
    double _dropTimerStart;

};




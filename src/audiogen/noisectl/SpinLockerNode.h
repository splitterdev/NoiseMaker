#pragma once

class SpinLockerNode : public ChainingNode {
    public:
    SpinLockerNode( double enable, double lockerLength, double spinLength, double lockerDir ) : ChainingNode() {
        setTextureId( TEXTURE_SPINLOCKER );
        _enable = setParameter( "Enable", enable );
        _lockerLength = setParameter( "L/Lock", lockerLength );
        _spinLength = setParameter( "L/Spin", spinLength );
        _lockerDir = setParameter( "SpinDir", lockerDir );
    }
    virtual ~SpinLockerNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                if ( _enable[ i ] > 0.5 ) {
                    if ( _cnt >= _lockerLength[ i ] ) {
                        _lock = inMem[ i ];
                        _cnt = 0;
                    }
                    if ( _cntSpin >= _spinLength[ i ] ) {
                        if ( _lockerDir[ i ] > 0.5 ) {
                            short byte = ( ( _lock & 0x8000 ) >> 15 ) & 0x0001;
                            _lock = ( _lock << 1 );
                            _lock |= byte;
                        } else {
                            short byte = ( _lock & 0x0001 ) << 15;
                            _lock = ( _lock >> 1 ) & 0x7FFF;
                            _lock |= byte;
                        }
                        _cntSpin = 0;
                    }
                    outMem[ i ] = _lock;
                    _cnt++;
                    _cntSpin++;
                } else {
                    outMem[ i ] = inMem[ i ];
                }
            }
        }
    }

    virtual string getClassName() {
        return "SpinLocker";
    }

    private:

    double* _enable;
    double* _lockerLength;
    double* _spinLength;
    double* _lockerDir;
    short _lock;
    int _cnt;
    int _cntSpin;

};



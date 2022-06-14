#pragma once

class DrainPumpNode : public ChainingNode {
    public:
    DrainPumpNode( double enable, double drainSpeed ) : ChainingNode() {
        setTextureId( TEXTURE_DRAINPUMP );
        //_mask = setParameter( "Mask", mask );
        _mask = 0x0001;//0x6A95;
        _enable = setParameter( "Enable", enable );
        _drainSpeed = setParameter( "Drain", drainSpeed );
        _pumpA = 0;
        _pumpB = 0;
        _cntA = 0;
        _cntB = 0;
        //_scale = double( _mask ) / 32767.0;
    }
    virtual ~DrainPumpNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            for ( int i = 0; i < sampleCount; i += 2 ) {
                if ( _enable[ i ] > 0.5 ) {
                    if ( _cntA >= _drainSpeed[ i ] ) {
                        _bit = ( inMem[ i ] & _mask );
                        _pumpA ^= _bit;
                        _pumpA = _pumpA << 1;
                        _pumpA ^= _bit ^ 0x01;
                        _cntA = 0;
                    }
                    outMem[ i ] = _pumpA ^ inMem[ i ];
                } else {
                    outMem[ i ] = inMem[ i ];
                }
                int j = i + 1;
                if ( _enable[ j ] > 0.5 ) {
                    if ( _cntB >= _drainSpeed[ i ] ) {
                        _bit = ( inMem[ j ] & _mask );
                        _pumpB ^= _bit;
                        _pumpB = _pumpB << 1;
                        _pumpB ^= _bit ^ 0x01;
                        _cntB = 0;
                    }
                    outMem[ j ] = _pumpB ^ inMem[ j ];
                } else {
                    outMem[ j ] = inMem[ j ];
                }
                _cntA++;
                _cntB++;
            }
        }
    }

    virtual string getClassName() {
        return "Drain Pump";
    }

    private:

    double* _enable;
    double* _drainSpeed;

    short _mask;
    short _bit;
    short _pumpA;
    short _pumpB;
    double _scale;

    int _cntA;
    int _cntB;

};



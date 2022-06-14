#pragma once

class InjectorNode : public ChainingNode {
    public:
    InjectorNode( double enable, double forceInject ) : ChainingNode() {
        setTextureId( TEXTURE_INJECTOR );
        _enable = setParameter( "Enable", enable );
        _forceInject = setParameter( "Force", forceInject );
    }
    virtual ~InjectorNode() {
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
                    if ( _forceInject[ i ] > 0.5 ) {
                        outMem[ i ] = outMem[ i ];
                    } else {
                        int a = rand() & 0x01;
                        if ( a ) {
                            outMem[ i ] = inMem[ i ];
                        } else {
                            outMem[ i ] = outMem[ i ];
                        }
                    }
                } else {
                    if ( _forceInject[ i ] > 0.5 ) {
                        outMem[ i ] = outMem[ i ];
                    } else {
                        outMem[ i ] = inMem[ i ];
                    }
                }
            }
        }
    }

    virtual string getClassName() {
        return "Injector";
    }

    private:

    double* _enable;
    double* _forceInject;

};



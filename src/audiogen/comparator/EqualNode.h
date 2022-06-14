#pragma once

class EqualNode : public ChainingNode {
    public:
    EqualNode( double tolerance ) : ChainingNode() {
        setTextureId( TEXTURE_EQUAL );
        _tolerance = setParameter( "Tolerance", tolerance );
        _inputSel = setParameter( "Input Sel.", 0.0 );
        setAllowedInputs( 2 );
    }
    virtual ~EqualNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input[ 2 ];
        input[ 0 ] = getInputBuffer( 0 );
        input[ 1 ] = getInputBuffer( 1 );
        if ( output && input[ 0 ] && input[ 1 ] ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem[ 2 ];
            inMem[ 0 ] = ( short* ) input[ 0 ] -> getMemory();
            inMem[ 1 ] = ( short* ) input[ 1 ] -> getMemory();
            for ( int i = 0; i < sampleCount; i++ ) {
                int toleranceCalc = _tolerance[ i ] * 65536.0;
                short inputSelect = _inputSel[ i ] > 0.5 ? 1 : 0;
                outMem[ i ] = abs( inMem[ 0 ][ i ] - inMem[ 1 ][ i ] ) <= toleranceCalc ? inMem[ inputSelect ][ i ] : 0;
            }
        } else if ( output ) {
            short* propagatedInput = NULL;
            if ( input[ 0 ] ) {
                propagatedInput = ( short* ) input[ 0 ] -> getMemory();
            } else if ( input[ 1 ] ) {
                propagatedInput = ( short* ) input[ 1 ] -> getMemory();
            }
            if ( propagatedInput ) {
                int sampleCount = ( output -> getSize() ) >> 1;
                short* outMem = ( short* ) output -> getMemory();
                for ( int i = 0; i < sampleCount; i++ ) {
                    outMem[ i ] = propagatedInput[ i ];
                }
            }
        }
    }

    virtual string getClassName() {
        return "Equal";
    }

    private:

    double* _tolerance;
    double* _inputSel;

};

#pragma once

class InputSelectorNode : public ChainingNode {
    public:
    InputSelectorNode() : ChainingNode() {
        setTextureId( TEXTURE_INPUT_SELECTOR );
        setAllowedInputs( - 1 );
        _volume = setParameter( "Vol", 1.0 );
        _inputSelected = setParameter( "Input", 0.0 );
    }
    virtual ~InputSelectorNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        if ( output ) {
            list < Buffer* > inputBuffers = getInputBuffers();
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            vector < short* > inMems;
            for ( list < Buffer* >::iterator i = inputBuffers.begin(); i != inputBuffers.end(); i++ ) {
                inMems.push_back( ( short* ) ( *i ) -> getMemory() );
            }
            int inputCount = inMems.size();
            for ( int i = 0; i < sampleCount; i++ ) {
                int input = ( _inputSelected[ i ] ) * ( double( inputCount ) - 1E-09 );
                if ( ( input >= 0 ) && ( input < inputCount ) ) {
                    outMem[ i ] = inMems[ input ][ i ];
                } else {
                    outMem[ i ] = 0;
                }
            }
        }
    }

    virtual string getClassName() {
        return "Input Selector";
    }

    private:

    double* _volume;
    double* _inputSelected;

};



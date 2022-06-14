#pragma once

class CartesianToPolarNode : public ChainingNode {
    public:
    CartesianToPolarNode( double volume ) : ChainingNode() {
        setTextureId( TEXTURE_COORDCONVERTER_CTP );
        _scale = setParameter( "Vol", volume );
    }
    virtual ~CartesianToPolarNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            Vector2D vectorBuffer;
            PolarVector2D polarVectorBuffer;
            double mulBuffer;
            for ( int i = 0; i < sampleCount; i += 2 ) {
                vectorBuffer = Vector2D( inMem[ i ], inMem[ i + 1 ] );
                polarVectorBuffer = PolarVector2D( vectorBuffer );
                mulBuffer = polarVectorBuffer.a * polarVectorBuffer.r;
                outMem[ i ] = mulBuffer * _scale[ i ];
                outMem[ i + 1 ] = mulBuffer * _scale[ i + 1 ];
            }
        }
    }

    virtual string getClassName() {
        return "Cartesian To Polar Converter";
    }

    private:

    double* _scale;

};




#pragma once

class PolarToCartesianNode : public ChainingNode {
    public:
    PolarToCartesianNode( double volume ) : ChainingNode() {
        setTextureId( TEXTURE_COORDCONVERTER_PTC );
        _scale = setParameter( "Vol", volume );
    }
    virtual ~PolarToCartesianNode() {
    }

    virtual void processOutput() {
        Buffer* output = getOutput();
        Buffer* input = getPrimaryInput();
        if ( output && input ) {
            int sampleCount = ( output -> getSize() ) >> 1;
            short* outMem = ( short* ) output -> getMemory();
            short* inMem = ( short* ) input -> getMemory();
            PolarVector2D polarVectorBuffer;
            Vector2D vectorBuffer;
            for ( int i = 0; i < sampleCount; i += 2 ) {
                polarVectorBuffer = PolarVector2D( inMem[ i ], inMem[ i + 1 ] );
                vectorBuffer = Vector2D( polarVectorBuffer );
                outMem[ i ] = double( vectorBuffer.x ) * _scale[ i ];
                outMem[ i + 1 ] = double( vectorBuffer.y ) * _scale[ i + 1 ];
            }
        }
    }

    virtual string getClassName() {
        return "Polar To Cartesian Converter";
    }

    private:

    double* _scale;

};





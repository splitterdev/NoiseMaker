#pragma once

class ChainingNodeFactory {
    public:
    ChainingNodeFactory() {}
    virtual ~ChainingNodeFactory() {}
    virtual ChainingNode* create() = 0;

    static ChainingNodeFactory* getFactory( int id ) {
        map < int, ChainingNodeFactory* >::iterator i = _MAP.find( id );
        if ( i != _MAP.end() ) {
            return i -> second;
        }
        return NULL;
    }

    static ChainingNodeFactory* getFactory( string className ) {
        map < string, ChainingNodeFactory* >::iterator i = _NAMED_MAP.find( className );
        if ( i != _NAMED_MAP.end() ) {
            return i -> second;
        }
        return NULL;
    }

    static void add( int id, ChainingNodeFactory* factory, int uiTexture ) {
        if ( factory ) {
            _MAP[ id ] = factory;
            ChainingNode* test = factory -> create();
            _NAMED_MAP[ test -> getClassName() ] = factory;
            delete test;
            _UI_ITEMS[ id ] = uiTexture;
        }
    }

    static string getNameOf( int id ) {
        map < int, string >::iterator i = _UI_NAMES.find( id );
        if ( i != _UI_NAMES.end() ) {
            return i -> second;
        } else {
            string name;
            ChainingNodeFactory* factory = getFactory( id );
            if ( factory ) {
                ChainingNode* test = factory -> create();
                name = test -> getClassName();
                delete test;
            } else {
                name = "???ERR???";
            }
            _UI_NAMES[ id ] = name;
            return name;
        }
        return "";
    }

    static int selectedItem() {
        return _selectedItem;
    }

    static void setSelection( int id ) {
        _selectedItem = id;
    }

    static void resetSelection() {
        map < int, ChainingNodeFactory* >::iterator i = _MAP.begin();
        if ( i != _MAP.end() ) {
            _selectedItem = i -> first;
        }
    }

    static int nextSelectedItem() {
        map < int, ChainingNodeFactory* >::iterator i = _MAP.find( _selectedItem );
        if ( i != _MAP.end() ) {
            i++;
            if ( i != _MAP.end() ) {
                _selectedItem = i -> first;
            } else {
                i = _MAP.begin();
                if ( i != _MAP.end() ) {
                    _selectedItem = i -> first;
                }
            }
        }
        return _selectedItem;
    }

    static ChainingNode* createSelectedItem() {
        ChainingNodeFactory* factory = getFactory( _selectedItem );
        if ( factory ) {
            return factory -> create();
        }
        return NULL;
    }

    static map < int, int > getFactoryUiItems() {
        return _UI_ITEMS;
    }

    static int getUiItemTexture( int id ) {
        map < int, int >::iterator i = _UI_ITEMS.find( id );
        if ( i != _UI_ITEMS.end() ) {
            return i -> second;
        }
        return -1;
    }

    private:
    static map < int, ChainingNodeFactory* > _MAP;
    static map < string, ChainingNodeFactory* > _NAMED_MAP;
    static map < int, int > _UI_ITEMS;
    static map < int, string > _UI_NAMES;
    static int _selectedItem;

};

map < int, ChainingNodeFactory* > ChainingNodeFactory::_MAP;
map < string, ChainingNodeFactory* > ChainingNodeFactory::_NAMED_MAP;
map < int, int > ChainingNodeFactory::_UI_ITEMS;
map < int, string > ChainingNodeFactory::_UI_NAMES;
int ChainingNodeFactory::_selectedItem = -1;

class SineGeneratorFactory : public ChainingNodeFactory {
    public:
    SineGeneratorFactory() : ChainingNodeFactory() {}
    virtual ~SineGeneratorFactory() {}
    virtual ChainingNode* create() {
        return new SineGenerator( 400.0, 0.5 );
    }
};

class TriangleGeneratorFactory : public ChainingNodeFactory {
    public:
    TriangleGeneratorFactory() : ChainingNodeFactory() {}
    virtual ~TriangleGeneratorFactory() {}
    virtual ChainingNode* create() {
        return new TriangleGenerator( 400.0, 0.5 );
    }
};

class SquareGeneratorFactory : public ChainingNodeFactory {
    public:
    SquareGeneratorFactory() : ChainingNodeFactory() {}
    virtual ~SquareGeneratorFactory() {}
    virtual ChainingNode* create() {
        return new SquareGenerator( 400.0, 0.5 );
    }
};

class SawGeneratorFactory : public ChainingNodeFactory {
    public:
    SawGeneratorFactory() : ChainingNodeFactory() {}
    virtual ~SawGeneratorFactory() {}
    virtual ChainingNode* create() {
        return new SawGenerator( 400.0, 0.5 );
    }
};

class ConstGeneratorFactory : public ChainingNodeFactory {
    public:
    ConstGeneratorFactory() : ChainingNodeFactory() {}
    virtual ~ConstGeneratorFactory() {}
    virtual ChainingNode* create() {
        return new ConstGenerator( 0.5 );
    }
};

class DownWaveGeneratorFactory : public ChainingNodeFactory {
    public:
    DownWaveGeneratorFactory() : ChainingNodeFactory() {}
    virtual ~DownWaveGeneratorFactory() {}
    virtual ChainingNode* create() {
        return new DownWaveGenerator( 0.5, 1.0 );
    }
};

class UpWaveGeneratorFactory : public ChainingNodeFactory {
    public:
    UpWaveGeneratorFactory() : ChainingNodeFactory() {}
    virtual ~UpWaveGeneratorFactory() {}
    virtual ChainingNode* create() {
        return new UpWaveGenerator( 0.5, 1.0 );
    }
};

class NoiseGeneratorFactory : public ChainingNodeFactory {
    public:
    NoiseGeneratorFactory() : ChainingNodeFactory() {}
    virtual ~NoiseGeneratorFactory() {}
    virtual ChainingNode* create() {
        return new NoiseGenerator();
    }
};

class NoiseLFGeneratorFactory : public ChainingNodeFactory {
    public:
    NoiseLFGeneratorFactory() : ChainingNodeFactory() {}
    virtual ~NoiseLFGeneratorFactory() {}
    virtual ChainingNode* create() {
        return new NoiseLFGenerator( 2048, 1 );
    }
};

class TriangleNoiseGeneratorFactory : public ChainingNodeFactory {
    public:
    TriangleNoiseGeneratorFactory() : ChainingNodeFactory() {}
    virtual ~TriangleNoiseGeneratorFactory() {}
    virtual ChainingNode* create() {
        return new TriangleNoiseGenerator( 400.0, 0.1, 1.0 );
    }
};

class LevelSignalGeneratorFactory : public ChainingNodeFactory {
    public:
    LevelSignalGeneratorFactory() : ChainingNodeFactory() {}
    virtual ~LevelSignalGeneratorFactory() {}
    virtual ChainingNode* create() {
        return new LevelSignalGenerator( 400.0, 0.1, 1.0 );
    }
};

class FSDNodeFactory : public ChainingNodeFactory {
    public:
    FSDNodeFactory() : ChainingNodeFactory() {}
    virtual ~FSDNodeFactory() {}
    virtual ChainingNode* create() {
        return new FSDNode( 1.0, 1.0 );
    }
};

class BitcrusherNodeFactory : public ChainingNodeFactory {
    public:
    BitcrusherNodeFactory() : ChainingNodeFactory() {}
    virtual ~BitcrusherNodeFactory() {}
    virtual ChainingNode* create() {
        return new BitcrusherNode( 1.0, 16.0 );
    }
};

class AdderNodeFactory : public ChainingNodeFactory {
    public:
    AdderNodeFactory() : ChainingNodeFactory() {}
    virtual ~AdderNodeFactory() {}
    virtual ChainingNode* create() {
        return new AdderNode();
    }
};

class MultiplierNodeFactory : public ChainingNodeFactory {
    public:
    MultiplierNodeFactory() : ChainingNodeFactory() {}
    virtual ~MultiplierNodeFactory() {}
    virtual ChainingNode* create() {
        return new MultiplierNode( 1.0 );
    }
};

class ConstAdderNodeFactory : public ChainingNodeFactory {
    public:
    ConstAdderNodeFactory() : ChainingNodeFactory() {}
    virtual ~ConstAdderNodeFactory() {}
    virtual ChainingNode* create() {
        return new ConstAdderNode( 0.5 );
    }
};

class AmplifierNodeFactory : public ChainingNodeFactory {
    public:
    AmplifierNodeFactory() : ChainingNodeFactory() {}
    virtual ~AmplifierNodeFactory() {}
    virtual ChainingNode* create() {
        return new AmplifierNode( 1.0 );
    }
};

class DividerNodeFactory : public ChainingNodeFactory {
    public:
    DividerNodeFactory() : ChainingNodeFactory() {}
    virtual ~DividerNodeFactory() {}
    virtual ChainingNode* create() {
        return new DividerNode( 1.0, 0.25 );
    }
};

class ExponentNodeFactory : public ChainingNodeFactory {
    public:
    ExponentNodeFactory() : ChainingNodeFactory() {}
    virtual ~ExponentNodeFactory() {}
    virtual ChainingNode* create() {
        return new ExponentNode( 2.0, 1.0 );
    }
};

class LogarithmicNodeFactory : public ChainingNodeFactory {
    public:
    LogarithmicNodeFactory() : ChainingNodeFactory() {}
    virtual ~LogarithmicNodeFactory() {}
    virtual ChainingNode* create() {
        return new LogarithmicNode( M_E, 1.0 );
    }
};

class DerivatorNodeFactory : public ChainingNodeFactory {
    public:
    DerivatorNodeFactory() : ChainingNodeFactory() {}
    virtual ~DerivatorNodeFactory() {}
    virtual ChainingNode* create() {
        return new DerivatorNode( 1.0 );
    }
};

class IntegratorNodeFactory : public ChainingNodeFactory {
    public:
    IntegratorNodeFactory() : ChainingNodeFactory() {}
    virtual ~IntegratorNodeFactory() {}
    virtual ChainingNode* create() {
        return new IntegratorNode( 1.0 );
    }
};

class CartesianToPolarNodeFactory : public ChainingNodeFactory {
    public:
    CartesianToPolarNodeFactory() : ChainingNodeFactory() {}
    virtual ~CartesianToPolarNodeFactory() {}
    virtual ChainingNode* create() {
        return new CartesianToPolarNode( 1.0 );
    }
};

class PolarToCartesianNodeFactory : public ChainingNodeFactory {
    public:
    PolarToCartesianNodeFactory() : ChainingNodeFactory() {}
    virtual ~PolarToCartesianNodeFactory() {}
    virtual ChainingNode* create() {
        return new PolarToCartesianNode( 1.0 );
    }
};

class ReversePolarizationNodeFactory : public ChainingNodeFactory {
    public:
    ReversePolarizationNodeFactory() : ChainingNodeFactory() {}
    virtual ~ReversePolarizationNodeFactory() {}
    virtual ChainingNode* create() {
        return new ReversePolarizationNode();
    }
};

class AbsoluteNodeFactory : public ChainingNodeFactory {
    public:
    AbsoluteNodeFactory() : ChainingNodeFactory() {}
    virtual ~AbsoluteNodeFactory() {}
    virtual ChainingNode* create() {
        return new AbsoluteNode( 1.0 );
    }
};

class NotNodeFactory : public ChainingNodeFactory {
    public:
    NotNodeFactory() : ChainingNodeFactory() {}
    virtual ~NotNodeFactory() {}
    virtual ChainingNode* create() {
        return new NotNode();
    }
};

class OrNodeFactory : public ChainingNodeFactory {
    public:
    OrNodeFactory() : ChainingNodeFactory() {}
    virtual ~OrNodeFactory() {}
    virtual ChainingNode* create() {
        return new OrNode();
    }
};

class AndNodeFactory : public ChainingNodeFactory {
    public:
    AndNodeFactory() : ChainingNodeFactory() {}
    virtual ~AndNodeFactory() {}
    virtual ChainingNode* create() {
        return new AndNode();
    }
};

class XorNodeFactory : public ChainingNodeFactory {
    public:
    XorNodeFactory() : ChainingNodeFactory() {}
    virtual ~XorNodeFactory() {}
    virtual ChainingNode* create() {
        return new XorNode();
    }
};

class InputSelectorNodeFactory : public ChainingNodeFactory {
    public:
    InputSelectorNodeFactory() : ChainingNodeFactory() {}
    virtual ~InputSelectorNodeFactory() {}
    virtual ChainingNode* create() {
        return new InputSelectorNode();
    }
};

class PainNodeFactory : public ChainingNodeFactory {
    public:
    PainNodeFactory() : ChainingNodeFactory() {}
    virtual ~PainNodeFactory() {}
    virtual ChainingNode* create() {
        return new PainNode( 0.48, 0.52 );
    }
};

class HysteriaNodeFactory : public ChainingNodeFactory {
    public:
    HysteriaNodeFactory() : ChainingNodeFactory() {}
    virtual ~HysteriaNodeFactory() {}
    virtual ChainingNode* create() {
        return new HysteriaNode( 1.0 );
    }
};

class SampleSwapperNodeFactory : public ChainingNodeFactory {
    public:
    SampleSwapperNodeFactory() : ChainingNodeFactory() {}
    virtual ~SampleSwapperNodeFactory() {}
    virtual ChainingNode* create() {
        return new SampleSwapperNode( 1.0 );
    }
};

class InjectorNodeFactory : public ChainingNodeFactory {
    public:
    InjectorNodeFactory() : ChainingNodeFactory() {}
    virtual ~InjectorNodeFactory() {}
    virtual ChainingNode* create() {
        return new InjectorNode( 1.0, 0.0 );
    }
};

class DrainPumpNodeFactory : public ChainingNodeFactory {
    public:
    DrainPumpNodeFactory() : ChainingNodeFactory() {}
    virtual ~DrainPumpNodeFactory() {}
    virtual ChainingNode* create() {
        return new DrainPumpNode( 1.0, 32.0 );
    }
};

class SpinLockerNodeFactory : public ChainingNodeFactory {
    public:
    SpinLockerNodeFactory() : ChainingNodeFactory() {}
    virtual ~SpinLockerNodeFactory() {}
    virtual ChainingNode* create() {
        return new SpinLockerNode( 1.0, 32.0, 8.0, 1.0 );
    }
};

class BraidNodeFactory : public ChainingNodeFactory {
    public:
    BraidNodeFactory() : ChainingNodeFactory() {}
    virtual ~BraidNodeFactory() {}
    virtual ChainingNode* create() {
        return new BraidNode( 1.0 );
    }
};

class EqualNodeFactory : public ChainingNodeFactory {
    public:
    EqualNodeFactory() : ChainingNodeFactory() {}
    virtual ~EqualNodeFactory() {}
    virtual ChainingNode* create() {
        return new EqualNode( 0.2 );
    }
};

class LessNodeFactory : public ChainingNodeFactory {
    public:
    LessNodeFactory() : ChainingNodeFactory() {}
    virtual ~LessNodeFactory() {}
    virtual ChainingNode* create() {
        return new LessNode();
    }
};

class GreaterNodeFactory : public ChainingNodeFactory {
    public:
    GreaterNodeFactory() : ChainingNodeFactory() {}
    virtual ~GreaterNodeFactory() {}
    virtual ChainingNode* create() {
        return new GreaterNode();
    }
};

class LessOrEqualNodeFactory : public ChainingNodeFactory {
    public:
    LessOrEqualNodeFactory() : ChainingNodeFactory() {}
    virtual ~LessOrEqualNodeFactory() {}
    virtual ChainingNode* create() {
        return new LessOrEqualNode();
    }
};

class GreaterOrEqualNodeFactory : public ChainingNodeFactory {
    public:
    GreaterOrEqualNodeFactory() : ChainingNodeFactory() {}
    virtual ~GreaterOrEqualNodeFactory() {}
    virtual ChainingNode* create() {
        return new GreaterOrEqualNode();
    }
};

class NotEqualNodeFactory : public ChainingNodeFactory {
    public:
    NotEqualNodeFactory() : ChainingNodeFactory() {}
    virtual ~NotEqualNodeFactory() {}
    virtual ChainingNode* create() {
        return new NotEqualNode( 0.2 );
    }
};

class MaxNodeFactory : public ChainingNodeFactory {
    public:
    MaxNodeFactory() : ChainingNodeFactory() {}
    virtual ~MaxNodeFactory() {}
    virtual ChainingNode* create() {
        return new MaxNode();
    }
};

class MinNodeFactory : public ChainingNodeFactory {
    public:
    MinNodeFactory() : ChainingNodeFactory() {}
    virtual ~MinNodeFactory() {}
    virtual ChainingNode* create() {
        return new MinNode();
    }
};

class ChannelSelectorNodeFactory : public ChainingNodeFactory {
    public:
    ChannelSelectorNodeFactory() : ChainingNodeFactory() {}
    virtual ~ChannelSelectorNodeFactory() {}
    virtual ChainingNode* create() {
        return new ChannelSelectorNode( 0.5 );
    }
};

class ChannelJoinerNodeFactory : public ChainingNodeFactory {
    public:
    ChannelJoinerNodeFactory() : ChainingNodeFactory() {}
    virtual ~ChannelJoinerNodeFactory() {}
    virtual ChainingNode* create() {
        return new ChannelJoinerNode();
    }
};

class ChannelDifferentiatorNodeFactory : public ChainingNodeFactory {
    public:
    ChannelDifferentiatorNodeFactory() : ChainingNodeFactory() {}
    virtual ~ChannelDifferentiatorNodeFactory() {}
    virtual ChainingNode* create() {
        return new ChannelDifferentiatorNode();
    }
};

class ChannelSwapperNodeFactory : public ChainingNodeFactory {
    public:
    ChannelSwapperNodeFactory() : ChainingNodeFactory() {}
    virtual ~ChannelSwapperNodeFactory() {}
    virtual ChainingNode* create() {
        return new ChannelSwapperNode( 1.0 );
    }
};

class ChannelMultiplierNodeFactory : public ChainingNodeFactory {
    public:
    ChannelMultiplierNodeFactory() : ChainingNodeFactory() {}
    virtual ~ChannelMultiplierNodeFactory() {}
    virtual ChainingNode* create() {
        return new ChannelMultiplierNode();
    }
};

class ChannelInverterNodeFactory : public ChainingNodeFactory {
    public:
    ChannelInverterNodeFactory() : ChainingNodeFactory() {}
    virtual ~ChannelInverterNodeFactory() {}
    virtual ChainingNode* create() {
        return new ChannelInverterNode( 0.0 );
    }
};



static void initChainingNodeFactory() {
    ChainingNodeFactory::add( 100, new SineGeneratorFactory(), TEXTURE_SINEGEN );
    ChainingNodeFactory::add( 101, new TriangleGeneratorFactory(), TEXTURE_TRIANGLEGEN );
    ChainingNodeFactory::add( 102, new SquareGeneratorFactory(), TEXTURE_SQUAREGEN );
    ChainingNodeFactory::add( 103, new SawGeneratorFactory(), TEXTURE_SAWGEN );
    ChainingNodeFactory::add( 104, new ConstGeneratorFactory(), TEXTURE_CONSTGEN );
    ChainingNodeFactory::add( 105, new DownWaveGeneratorFactory(), TEXTURE_DOWNWAVEGEN );
    ChainingNodeFactory::add( 106, new UpWaveGeneratorFactory(), TEXTURE_UPWAVEGEN );

    ChainingNodeFactory::add( 150, new NoiseGeneratorFactory(), TEXTURE_NOISEGEN );
    ChainingNodeFactory::add( 151, new NoiseLFGeneratorFactory(), TEXTURE_LFNOISEGEN );
    ChainingNodeFactory::add( 152, new TriangleNoiseGeneratorFactory(), TEXTURE_TRIANGLENOISEGEN );
    ChainingNodeFactory::add( 153, new LevelSignalGeneratorFactory(), TEXTURE_LEVELSIGNALGEN );

    ChainingNodeFactory::add( 200, new AdderNodeFactory(), TEXTURE_ADDER );
    ChainingNodeFactory::add( 201, new MultiplierNodeFactory(), TEXTURE_MULTIPLIER );
    ChainingNodeFactory::add( 202, new NotNodeFactory(), TEXTURE_NOTGATE );
    ChainingNodeFactory::add( 203, new OrNodeFactory(), TEXTURE_ORGATE );
    ChainingNodeFactory::add( 204, new AndNodeFactory(), TEXTURE_ANDGATE );
    ChainingNodeFactory::add( 205, new XorNodeFactory(), TEXTURE_XORGATE );
    ChainingNodeFactory::add( 206, new InputSelectorNodeFactory(), TEXTURE_INPUT_SELECTOR );

    ChainingNodeFactory::add( 400, new ConstAdderNodeFactory(), TEXTURE_CONSTADDER );
    ChainingNodeFactory::add( 401, new AmplifierNodeFactory(), TEXTURE_AMPLIFIER );
    ChainingNodeFactory::add( 402, new DividerNodeFactory(), TEXTURE_DIVIDER );
    ChainingNodeFactory::add( 403, new ExponentNodeFactory(), TEXTURE_POWER );
    ChainingNodeFactory::add( 404, new LogarithmicNodeFactory(), TEXTURE_LOGARITHMIC );
    ChainingNodeFactory::add( 405, new DerivatorNodeFactory(), TEXTURE_DERIVATOR );
    ChainingNodeFactory::add( 406, new IntegratorNodeFactory(), TEXTURE_INTEGRATOR );
    ChainingNodeFactory::add( 407, new AbsoluteNodeFactory(), TEXTURE_ABSOLUTE );
    ChainingNodeFactory::add( 408, new ReversePolarizationNodeFactory(), TEXTURE_REVERSEPOLARIZATION );
    ChainingNodeFactory::add( 409, new CartesianToPolarNodeFactory(), TEXTURE_COORDCONVERTER_CTP );
    ChainingNodeFactory::add( 410, new PolarToCartesianNodeFactory(), TEXTURE_COORDCONVERTER_PTC );

    ChainingNodeFactory::add( 700, new EqualNodeFactory(), TEXTURE_EQUAL );
    ChainingNodeFactory::add( 701, new LessNodeFactory(), TEXTURE_LESS );
    ChainingNodeFactory::add( 702, new GreaterNodeFactory(), TEXTURE_GREATER );
    ChainingNodeFactory::add( 703, new LessOrEqualNodeFactory(), TEXTURE_LESS_OR_EQUAL );
    ChainingNodeFactory::add( 704, new GreaterOrEqualNodeFactory(), TEXTURE_GREATER_OR_EQUAL );
    ChainingNodeFactory::add( 705, new NotEqualNodeFactory(), TEXTURE_NOT_EQUAL );
    ChainingNodeFactory::add( 706, new MaxNodeFactory(), TEXTURE_MAX );
    ChainingNodeFactory::add( 707, new MinNodeFactory(), TEXTURE_MIN );
    ChainingNodeFactory::add( 750, new ChannelSelectorNodeFactory(), TEXTURE_CHANNEL_SELECTOR );
    ChainingNodeFactory::add( 751, new ChannelJoinerNodeFactory(), TEXTURE_CHANNEL_JOINER );
    ChainingNodeFactory::add( 752, new ChannelDifferentiatorNodeFactory(), TEXTURE_CHANNEL_DIFFERENTIATOR );
    ChainingNodeFactory::add( 753, new ChannelSwapperNodeFactory(), TEXTURE_CHANNEL_SWAPPER );
    ChainingNodeFactory::add( 754, new ChannelMultiplierNodeFactory(), TEXTURE_CHANNEL_MUTLIPLIER );
    ChainingNodeFactory::add( 755, new ChannelInverterNodeFactory(), TEXTURE_CHANNEL_INVERTER );

    ChainingNodeFactory::add( 800, new FSDNodeFactory(), TEXTURE_OVERDRIVE );
    ChainingNodeFactory::add( 801, new BitcrusherNodeFactory(), TEXTURE_BITCRUSHER );
    ChainingNodeFactory::add( 1000, new PainNodeFactory(), TEXTURE_PAIN );
    ChainingNodeFactory::add( 1001, new HysteriaNodeFactory(), TEXTURE_HYSTERIA );
    ChainingNodeFactory::add( 1002, new SampleSwapperNodeFactory(), TEXTURE_SAMPLESWAPPER );
    ChainingNodeFactory::add( 1003, new InjectorNodeFactory(), TEXTURE_INJECTOR );
    ChainingNodeFactory::add( 1004, new DrainPumpNodeFactory(), TEXTURE_DRAINPUMP );
    ChainingNodeFactory::add( 1005, new SpinLockerNodeFactory(), TEXTURE_SPINLOCKER );
    ChainingNodeFactory::add( 1006, new BraidNodeFactory(), TEXTURE_BRAID );
}


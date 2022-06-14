#pragma once

class ParameterControlRule {
    public:

    static const int MODE_LIN = 0;
    static const int MODE_EXP = 1;

    ParameterControlRule() {
        _minValue = 0.0;
        _maxValue = 0.0;
        _rangeValue = 0.0;
    }
    virtual ~ParameterControlRule() {
    }

    static void initRules() {
        addRule( "Freq", 0.0, 14.0, MODE_EXP ); // exponential
        //addRule( "Const", - 32767.0, 32767.0, MODE_LIN ); // linear
        //addConstRule( "Const", 0.0, 32767.0, MODE_LIN ); // linear
    }

    static void addRule( string paramName, double minValue, double maxValue, int mode ) {
        ParameterControlRule* rule = new ParameterControlRule();
        rule -> _minValue = minValue;
        rule -> _maxValue = maxValue;
        rule -> _mode = mode;
        rule -> _rangeValue = maxValue - minValue;
        _map[ paramName ] = rule;
    }

    static void addConstRule( string paramName, double minValue, double maxValue, int mode ) {
        ParameterControlRule* rule = new ParameterControlRule();
        rule -> _minValue = minValue;
        rule -> _maxValue = maxValue;
        rule -> _mode = mode;
        rule -> _rangeValue = maxValue - minValue;
        _c_map[ paramName ] = rule;
    }

    static ParameterControlRule* getRuleFor( string paramName ) {
        map < string, ParameterControlRule* >::iterator i = _map.find( paramName );
        if ( i != _map.end() ) {
            return i -> second;
        }
        return NULL;
    }

    static ParameterControlRule* getConstRuleFor( string paramName ) {
        map < string, ParameterControlRule* >::iterator i = _c_map.find( paramName );
        if ( i != _c_map.end() ) {
            return i -> second;
        }
        return NULL;
    }

    inline double scaledValue( double input ) {
        switch ( _mode ) {
            case MODE_LIN: return ( input * _rangeValue ) + _minValue;
            case MODE_EXP: return pow( 2.0, ( input * _rangeValue ) + _minValue );
        }
        return 0.0;
    }

    private:

    double _minValue;
    double _maxValue;
    int _mode;

    double _rangeValue;

    static map < string, ParameterControlRule* > _map;
    static map < string, ParameterControlRule* > _c_map;

};

map < string, ParameterControlRule* > ParameterControlRule::_map;
map < string, ParameterControlRule* > ParameterControlRule::_c_map;


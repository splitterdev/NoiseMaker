#pragma once

class InputSelectDialogSelectorBox : public TabbedDataSelectorBox {
    private:

    class _ParamInputNodeSelect : public StaticText {
        public:
        _ParamInputNodeSelect( InputSelectDialogSelectorBox* box, ChainingNode* input ) : StaticText( input == NULL ? "(unlink)" : getNameOf( input ) ) {
            _selectorBox = box;
            _input = input;
            if ( input ) {
                setName( input -> getClassName() );
                setColor( 0xFF404040 );
            } else {
                setName( "(unlink)" );
                setColor( 0xFF400000 );
            }
        }
        virtual ~_ParamInputNodeSelect() {
        }

        virtual void onClick() {
            _selectorBox -> setParamSourceNode( _input );
            DialogSelectorBox::removeDialog();
        }

        private:
        InputSelectDialogSelectorBox* _selectorBox;
        ChainingNode* _input;

    };

    public:
    InputSelectDialogSelectorBox( string paramName, ChainingNode* node, ChainingTree* tree ) : TabbedDataSelectorBox( 8, 10, true ) {
        _paramName = paramName;
        _node = node;
        _tree = tree;
    }
    virtual ~InputSelectDialogSelectorBox() {
    }

    virtual void buildComponents() {
        vector < Button* > inputNodeList;
        inputNodeList.push_back( new _ParamInputNodeSelect( this, NULL ) );
        map < ChainingNode*, list < string > > nodes = _node -> getParameterLinkingAvailableNodes();
        for ( map < ChainingNode*, list < string > >::iterator i = nodes.begin(); i != nodes.end(); i++ ) {
            inputNodeList.push_back( new _ParamInputNodeSelect( this, i -> first ) );
        }
        buildPanels( inputNodeList, 8, 8 );
        TabbedDataSelectorBox::buildComponents();
    }

    virtual void onReturn() {
    }

    void setParamSourceNode( ChainingNode* node ) {
        _node -> linkToParameter( _paramName, node );
        _tree -> refreshChain();
    }

    static string getNameOf( ChainingNode* input ) {
        char buffer[ 4096 ];
        sprintf( buffer, "%s [%d]", ( input -> getClassName() ).c_str(), input -> getId() );
        return string( buffer );
    }

    private:

    string _paramName;
    ChainingNode* _node;
    ChainingTree* _tree;

};



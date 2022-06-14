#pragma once

class VisualisationComponentSelector : public TabbedDataSelectorBox {
    private:

    class _SelectVisualisationComponentButton : public Button {
        public:
        _SelectVisualisationComponentButton( VisualisationComponentSelector* box, int vcFactoryId, int textureId, int sizeX, int sizeY, string name ) : Button() {
            _box = box;
            _vcFactoryId = vcFactoryId;
            _x = sizeX;
            _y = sizeY;
            setTexture( textureId );
            setName( name );
        }
        virtual ~_SelectVisualisationComponentButton() {
        }

        virtual void onClick() {
            _box -> createVC( _vcFactoryId, _x, _y );
            DialogSelectorBox::removeDialog();
        }

        private:
        VisualisationComponentSelector* _box;
        int _vcFactoryId;
        int _x;
        int _y;
    };

    public:
    VisualisationComponentSelector( int width, int height, ChainingTree** tree ) : TabbedDataSelectorBox( width, height, false ) {
        _tree = tree;
    }
    virtual ~VisualisationComponentSelector() {
    }

    virtual void buildComponents() {
        vector < vector < ButtonBuildData > > inputButtons;
        _putData( &inputButtons, -1, TEXTURE_VC_NONE, "(None)", 0, 0 );
        _putData( &inputButtons, 0, TEXTURE_VC_OSCILLOSCOPE, "Oscilloscope", 384, 128 );
        _putData( &inputButtons, 1, TEXTURE_VC_SPECTRE, "Specter", 384, 128 );
        _putData( &inputButtons, 2, TEXTURE_VC_SPECTRE_HISTOGRAM, "Specter Histogram", 384, 128 );
        _putData( &inputButtons, 3, TEXTURE_VC_VECTORSCOPE, "Vectorscope", 256, 256 );
        buildPanels( inputButtons, 4, 5 );
        TabbedDataSelectorBox::buildComponents();
    }

    virtual void onReturn() {
    }

    void createVC( int vcFactoryId, int sizeX, int sizeY ) {
        VisualisationComponent::createSetVC( vcFactoryId, sizeX, sizeY, _tree );
    }

    private:

    void _putData( vector < vector < ButtonBuildData > >* src, int vcFactoryId, int textureId, string vcName, int sizeX, int sizeY ) {
        vector < ButtonBuildData > buildData;
        buildData.push_back(
            ButtonBuildData(
                new _SelectVisualisationComponentButton( this, vcFactoryId, textureId, sizeX, sizeY, vcName ), 1
            )
        );
        buildData.push_back(
            ButtonBuildData(
                new StaticText( vcName ), 3
            )
        );
        src -> push_back( buildData );
    }

    ChainingTree** _tree;

};



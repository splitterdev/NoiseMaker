#pragma once

class TabbedDataSelectorBox : public DialogSelectorBox {
    private:

    class _SwitchPanelButton : public Button {
        public:
        _SwitchPanelButton( TabbedDataSelectorBox* box, int dir, int textureId, string name ) : Button() {
            _box = box;
            _dir = dir;
            setTexture( textureId );
            setName( name );
        }
        virtual ~_SwitchPanelButton() {
        }

        virtual void onClick() {
            _box -> nextPanel( _dir );
        }

        private:
        TabbedDataSelectorBox* _box;
        int _dir;
    };

    class _CancelPanelButton : public Button {
        public:
        _CancelPanelButton( int textureId ) : Button() {
            setTexture( textureId );
            setName( "Cancel" );
        }
        virtual ~_CancelPanelButton() {
        }

        virtual void onClick() {
            DialogSelectorBox::removeDialog();
        }

        private:
    };

    public:
    TabbedDataSelectorBox( int width, int height, bool switchable ) : DialogSelectorBox( width, height ) {
        _widthItems = width;
        _heightItems = height;
        _switchable = switchable;
        _currentPanel = 0;
    }
    virtual ~TabbedDataSelectorBox() {
    }

    void nextPanel( int dir ) {
        _currentPanel += dir;
        if ( _currentPanel < 0 ) {
            _currentPanel = int( _panels.size() ) - 1;
        } else if ( _currentPanel >= int( _panels.size() ) ) {
            _currentPanel = 0;
        }
    }

    virtual void buildComponents() {
        if ( _switchable ) {
            addButton( new _SwitchPanelButton( this, -1, TEXTURE_BUTTON_MOVE_LEFT, "Previous page" ), 0, _heightItems - 1, 1, 1 );
            addButton( new _SwitchPanelButton( this, 1, TEXTURE_BUTTON_MOVE_RIGHT, "Next page" ), _widthItems - 1, _heightItems - 1, 1, 1 );
        }
        int cancelWidth = ( _widthItems & 0x01 ) ? 1 : 2;
        addButton( new _CancelPanelButton( TEXTURE_BUTTON_CANCEL ), ( _widthItems >> 1 ) - ( cancelWidth >> 1 ), _heightItems - 1, cancelWidth, 1 );
    }

    virtual void render( SDL_Renderer* renderer ) {
        SDL_Rect rect;
        rect.x = getPositionX();
        rect.y = getPositionY();
        rect.w = getWidth();
        rect.h = getHeight();
        SDL_SetRenderDrawColor( renderer, 64, 64, 64, 255 );
        SDL_RenderFillRect( renderer, &rect );
        DialogSelectorBox::render( renderer );
        if ( ( _currentPanel >= 0 ) && ( _currentPanel < int( _panels.size() ) ) ) {
            Panel* panel = _panels[ _currentPanel ];
            panel -> render( renderer );
        }
    }

    bool click( int x, int y ) {
        if ( ( _currentPanel >= 0 ) && ( _currentPanel < int( _panels.size() ) ) ) {
            if ( _panels[ _currentPanel ] -> click( x, y ) ) {
                return true;
            }
        }
        return Panel::click( x, y );
    }

    static string getNameOf( ChainingNode* input ) {
        char buffer[ 4096 ];
        sprintf( buffer, "%s [%d]", ( input -> getClassName() ).c_str(), input -> getId() );
        return string( buffer );
    }

    void buildPanels( vector < Button* > inputSelectList, int panelWidth, int panelHeight ) {
        buildPanels( inputSelectList, panelWidth, panelHeight, panelWidth - 1 );
    }

    void buildPanels( vector < Button* > inputSelectList, int panelWidth, int panelHeight, int buttonWidth ) {
        int row = 0;
        Panel* panel = _createPanel();
        for ( vector < Button* >::iterator i = inputSelectList.begin(); i != inputSelectList.end(); i++ ) {
            if ( row >= panelHeight ) {
                _panels.push_back( panel );
                panel = _createPanel();
                row = 0;
            }
            panel -> addButton( *i, 0, row, buttonWidth, 1 );
            row++;
        }
        _panels.push_back( panel );
    }

    class ButtonBuildData {
        public:
        ButtonBuildData( Button* b, int bWidth ) {
            button = b;
            buttonWidth = bWidth;
        }
        ButtonBuildData() {
            button = NULL;
            buttonWidth = 1;
        }
        virtual ~ButtonBuildData() {
        }
        Button* button;
        int buttonWidth;
    };


    void buildPanels( vector < vector < ButtonBuildData > > inputSelectList, int panelWidth, int panelHeight ) {
        int row = 0;
        Panel* panel = _createPanel();
        for ( vector < vector < ButtonBuildData > >::iterator i = inputSelectList.begin(); i != inputSelectList.end(); i++ ) {
            if ( row >= panelHeight ) {
                _panels.push_back( panel );
                panel = _createPanel();
                row = 0;
            }
            vector < ButtonBuildData > rowButtons = ( *i );
            int col = 0;
            for ( vector < ButtonBuildData >::iterator j = rowButtons.begin(); j != rowButtons.end(); j++ ) {
                panel -> addButton( j -> button, col, row, j -> buttonWidth, 1 );
                col += j -> buttonWidth;
            }
            row++;
        }
        _panels.push_back( panel );
    }

    virtual Button* getPointedButton( int x, int y ) {
        if ( isInside( x, y ) ) {
            if ( ( _currentPanel >= 0 ) && ( _currentPanel < int( _panels.size() ) ) ) {
                Panel* panel = _panels[ _currentPanel ];
                Button* pointed = panel -> getPointedButton( x, y );
                if ( pointed ) {
                    return pointed;
                }
            }
        }
        return Panel::getPointedButton( x, y );
    }

    private:

    Panel* _createPanel() {
        Panel* panel = new Panel( _widthItems, _heightItems );
        static int SPACING = 8;
        panel -> setSize( getWidth() - ( SPACING << 1 ), getHeight() - ( SPACING << 1 ) - 64 );
        panel -> setPosition( getPositionX() + SPACING, getPositionY() + SPACING );
        panel -> setColor( 0xFF202020 );
        return panel;
    }

    int _widthItems;
    int _heightItems;
    bool _switchable;
    vector < Panel* > _panels;
    int _currentPanel;

};



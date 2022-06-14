#pragma once

class TabPanel {
    public:

    static const int TAB_BUTTON_WIDTH = 48;
    static const int TAB_BUTTON_HEIGHT = 32;
    static const int TAB_BUTTON_W_OFFSET = 4;

    static const int INTERIOR_PANEL_OFFSET = 4;

    private:

    class _TabButton : public Button {
        public:
        _TabButton( TabPanel* panel, int id, int textureId, string tabName ) : Button( TAB_BUTTON_WIDTH, TAB_BUTTON_HEIGHT ) {
            _panel = panel;
            _id = id;
            setTexture( textureId );
            int panelX = panel -> _x;
            int panelY = panel -> _y;
            setPosition( panelX + TAB_BUTTON_W_OFFSET + id * ( TAB_BUTTON_WIDTH + TAB_BUTTON_W_OFFSET ), panelY + ( TAB_BUTTON_HEIGHT >> 2 ) - INTERIOR_PANEL_OFFSET );
            _panel -> setCurrentTab( _id );
            setName( tabName );
        }
        virtual ~_TabButton() {
        }

        void onClick() {
            selectButton();
            _panel -> setCurrentTab( _id );
        }

        virtual void render( SDL_Renderer* renderer ) {
            SDL_Texture* texture = Textures::getTexture( getTextureId() );
            int alpha = 128;
            if ( _selected ) {
                alpha = 255;
            }
            SDL_SetTextureAlphaMod( texture, alpha );
            Button::render( renderer );
        }

        void selectButton() {
            vector < _TabButton* > buttons = _panel -> _tabButtons;
            for ( vector < _TabButton* >::iterator i = buttons.begin(); i != buttons.end(); i++ ) {
                ( *i ) -> _selected = false;
            }
            _selected = true;
        }

        private:

        TabPanel* _panel;
        int _id;
        bool _selected;

    };

    public:
    TabPanel( int width, int height ) {
        _tabId = 0;
        _x = 0;
        _y = 0;
        _width = width * Panel::ROW_SIZE + ( width + 1 ) * Panel::CELL_SPACING + INTERIOR_PANEL_OFFSET;
        _height = height * Panel::COL_SIZE + ( height + 1 ) * Panel::CELL_SPACING + INTERIOR_PANEL_OFFSET + TAB_BUTTON_HEIGHT + INTERIOR_PANEL_OFFSET;
    }
    virtual ~TabPanel() {
        for ( vector < Panel* >::iterator i = _tabs.begin(); i != _tabs.end(); i++ ) {
            delete ( *i );
        }
        for ( vector < _TabButton* >::iterator i = _tabButtons.begin(); i != _tabButtons.end(); i++ ) {
            delete ( *i );
        }
    }

    bool click( int x, int y ) {
        if ( isInside( x, y ) ) {
            for ( vector < _TabButton* >::iterator i = _tabButtons.begin(); i != _tabButtons.end(); i++ ) {
                if ( ( *i ) -> click( x, y ) ) {
                    return true;
                }
            }
            Panel* currentPanel = getCurrentPanel();
            if ( currentPanel ) {
                currentPanel -> click( x, y );
                return true;
            }
        }
        return false;
    }

    bool isInside( int x, int y ) {
        if ( ( x >= _x ) && ( y >= _y ) && ( x < _x + _width ) && ( y < _y + _height ) ) {
            return true;
        }
        return false;
    }

    inline Panel* getCurrentPanel() {
        if ( ( _tabId >= 0 ) && ( _tabId < int( _tabs.size() ) ) ) {
            return _tabs[ _tabId ];
        }
        return NULL;
    }

    inline void setCurrentTab( int id ) {
        _tabId = id;
    }

    inline void changeTab( int tabId ) {
        setCurrentTab( tabId );
        _tabButtons[ tabId ] -> selectButton();
    }

    inline void addPanel( Panel* panel, int buttonTextureId, string tabName ) {
        if ( panel ) {
            int tabId = _tabs.size();
            int yOffset = TAB_BUTTON_HEIGHT;
            panel -> setPosition( _x + INTERIOR_PANEL_OFFSET, _y + INTERIOR_PANEL_OFFSET + yOffset );
            panel -> setSize( _width - ( INTERIOR_PANEL_OFFSET << 1 ), _height - ( INTERIOR_PANEL_OFFSET << 1 ) - yOffset );
            _tabs.push_back( panel );
            _tabButtons.push_back( new _TabButton( this, tabId, buttonTextureId, tabName ) );
            _tabButtons[ tabId ] -> selectButton();
        }
    }

    void render( SDL_Renderer* renderer ) {
        SDL_Rect rect;
        rect.x = _x;
        rect.y = _y;
        rect.w = _width;
        rect.h = _height;
        SDL_SetRenderDrawColor( renderer, 64, 64, 64, 255 );
        SDL_RenderFillRect( renderer, &rect );
        Panel* currentPanel = getCurrentPanel();
        if ( currentPanel ) {
            currentPanel -> render( renderer );
        }
        for ( vector < _TabButton* >::iterator i = _tabButtons.begin(); i != _tabButtons.end(); i++ ) {
            ( *i ) -> render( renderer );
        }
    }

    void setPosition( int x, int y ) {
        _x = x;
        _y = y;
    }

    Button* getPointedButton( int x, int y ) {
        if ( isInside( x, y ) ) {
            for ( vector < _TabButton* >::iterator i = _tabButtons.begin(); i != _tabButtons.end(); i++ ) {
                if ( ( *i ) -> isInside( x, y ) ) {
                    return ( *i );
                }
            }
            Panel* current = getCurrentPanel();
            if ( current ) {
                return current -> getPointedButton( x, y );
            }
        }
        return NULL;
    }

    void addShortCut( int tabId, int key ) {
        _shortcutMap[ key ] = tabId;
    }

    bool getShortCutTab( int key, int* tab ) {
        map < int, int >::iterator i = _shortcutMap.find( key );
        if ( i != _shortcutMap.end() ) {
            ( *tab ) = i -> second;
            return true;
        }
        return false;
    }

    private:

    int _tabId;
    vector < Panel* > _tabs;
    vector < _TabButton* > _tabButtons;
    int _x;
    int _y;
    int _width;
    int _height;

    map < int, int > _shortcutMap;

};



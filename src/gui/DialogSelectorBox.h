#pragma once

class DialogSelectorBox : public Panel {
    public:
    DialogSelectorBox( int w, int h ) : Panel( w, h ) {
        int padding = ( CELL_SPACING >> 1 );
        setSize( w * getWidthItemUnit() + padding, h * getHeightItemUnit() + padding );
        setPosition( ( WINDOW_WIDTH - getWidth() ) >> 1, ( WINDOW_HEIGHT - getHeight() ) >> 1 );
    }
    virtual ~DialogSelectorBox() {
    }

    virtual void buildComponents() = 0;
    virtual void onReturn() = 0;

    static void setDialog( DialogSelectorBox* selectorBox ) {
        if ( selectorBox ) {
            selectorBox -> buildComponents();
            _dialog.push_back( selectorBox );
        }
    }

    static DialogSelectorBox* getDialog() {
        if ( !_dialog.empty() ) {
            return _dialog.back();
        }
        return NULL;
    }

    static void removeDialog() {
        if ( !_dialog.empty() ) {
            DialogSelectorBox* selectorBox = _dialog.back();
            _dialog.pop_back();
            selectorBox -> onReturn();
            delete selectorBox;
        }
    }

    static void renderAll( SDL_Renderer* renderer ) {
        for ( list < DialogSelectorBox* >::iterator i = _dialog.begin(); i != _dialog.end(); i++ ) {
            ( *i ) -> render( renderer );
        }
    }

    private:

    static list < DialogSelectorBox* > _dialog;

};

list < DialogSelectorBox* > DialogSelectorBox::_dialog;

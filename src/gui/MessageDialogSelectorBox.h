#pragma once

class MessageDialogSelectorBox : public TabbedDataSelectorBox {
    public:
    MessageDialogSelectorBox( int width, int height, string message ) : TabbedDataSelectorBox( width, height, false ) {
        _width = width;
        _height = height;
        _message = message;
    }
    virtual ~MessageDialogSelectorBox() {
    }

    virtual void buildComponents() {
        vector < ButtonBuildData > items;
        ButtonBuildData data;
        data.button = new StaticText( _message );
        data.buttonWidth = _width - 1;
        items.push_back( data );
        vector < vector < ButtonBuildData > > vItems;
        vItems.push_back( items );
        buildPanels( vItems, _width - 1, 1 );
        TabbedDataSelectorBox::buildComponents();
    }

    static void setDialog( string message ) {
        MessageDialogSelectorBox* box = new MessageDialogSelectorBox( 5, 3, message );
        DialogSelectorBox::setDialog( box );
    }

    void onReturn() {
    }

    private:

    int _width;
    int _height;
    string _message;

};



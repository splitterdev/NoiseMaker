#pragma once

class HelpButton : public Button {
    public:
    HelpButton() : Button() {
        setTexture( TEXTURE_BUTTON_HELP );
        setName( "Help" );
    }
    virtual ~HelpButton() {
    }

    virtual void onClick() {
        helpEnabled = !helpEnabled;
    }

};



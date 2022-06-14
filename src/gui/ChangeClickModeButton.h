#pragma once

class ChangeClickModeButton : public Button {
    public:
    ChangeClickModeButton( int setClickMode, int icon, string clickModeName ) : Button() {
        setTexture( icon );
        _clickMode = setClickMode;
        setName( clickModeName );
    }
    virtual ~ChangeClickModeButton() {
    }

    virtual void onClick() {
        clickMode = _clickMode;
    }

    private:

    int _clickMode;

};



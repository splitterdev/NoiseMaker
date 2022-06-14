#pragma once

class PauseButton : public Button {
    public:
    PauseButton( ChainPlayer* player ) : Button() {
        setTexture( TEXTURE_BUTTON_PAUSE );
        _player = player;
        setName( "Pause" );
    }
    virtual ~PauseButton() {
    }

    virtual void onClick() {
        _player -> setPlaying( false );
    }

    private:

    ChainPlayer* _player;

};



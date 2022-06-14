#pragma once

class PlayButton : public Button {
    public:
    PlayButton( ChainPlayer* player ) : Button() {
        setTexture( TEXTURE_BUTTON_PLAY );
        _player = player;
        setName( "Play" );
    }
    virtual ~PlayButton() {
    }

    virtual void onClick() {
        _player -> setPlaying( true );
    }

    private:

    ChainPlayer* _player;

};


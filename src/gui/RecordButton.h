#pragma once

class RecordButton : public Button {
    public:
    RecordButton( ChainPlayer* player ) : Button() {
        setTexture( TEXTURE_BUTTON_RECORD );
        _player = player;
        setName( "Record" );
    }
    virtual ~RecordButton() {
    }

    virtual void onClick() {
        if ( _player -> isRecording() ) {
            _player -> stopRecording();
        } else {
            _player -> startRecording();
        }
    }

    private:

    ChainPlayer* _player;

};




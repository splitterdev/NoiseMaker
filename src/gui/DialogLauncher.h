#pragma once

class DialogLauncher : public Button {
    public:
    DialogLauncher( int launchId, int textureId, string name ) : Button() {
        _launchId = launchId;
        setTexture( textureId );
        setName( name );
    }
    virtual ~DialogLauncher() {
    }

    virtual void onClick() {
    }

    private:

    int _launchId;

};



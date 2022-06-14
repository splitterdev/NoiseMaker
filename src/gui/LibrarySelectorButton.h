#pragma once

class LibrarySelectorButton : public Button {
    public:
    LibrarySelectorButton( int id, int textureId ) : Button() {
        setTexture( textureId );
        _id = id;
        setName( ChainingNodeFactory::getNameOf( _id ) );
    }
    virtual ~LibrarySelectorButton() {
    }

    virtual void onClick() {
        ChainingNodeFactory::setSelection( _id );
        clickMode = CLICKMODE_ADD;
    }

    private:
    int _id;
};



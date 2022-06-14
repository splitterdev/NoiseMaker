#pragma once

class ChainingLine {
    public:
    ChainingLine() {
        _from = NULL;
        _to = NULL;
    }
    ~ChainingLine() {
    }

    inline void setLink( ChainingNode* from, ChainingNode* to ) {
        _from = from;
        _to = to;
    }

    inline ChainingNode* getFromNode() {
        return _from;
    }

    inline ChainingNode* getToNode() {
        return _to;
    }

    inline bool isSet() {
        return ( _from != NULL ) && ( _to != NULL );
    }

    void draw( SDL_Renderer* renderer, int r, int g, int b ) {
        if ( isSet() ) {
            SDL_SetRenderDrawColor( renderer, r, g, b, 255 );
            ChainingNode::renderArrow( renderer, _to -> getPositionX(), _to -> getPositionY(), _from -> getPositionX(), _from -> getPositionY() );
        }
    }

    private:

    ChainingNode* _from;
    ChainingNode* _to;

};



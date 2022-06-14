#pragma once

const int BITRATE = 44100;

class ChainingNode {
    public:

    static int globalTime() {
        return _globalTime;
    }

    static void setNextGlobalTime() {
        _globalTime += BUFFER_SIZE >> 2;
    }

    //static const int BUFFER_SIZE = 8192;
    static const int BLOCK_WIDTH = 64;
    static const int BLOCK_HEIGHT = 64;

    ChainingNode() {
        _posX = 0;
        _posY = 0;
        _output = Buffer::Create( BUFFER_SIZE );
        _textureId = -1;
        _allowedInputs = 1;
        _id = -1;
    }
    virtual ~ChainingNode() {
        if ( _output ) {
            delete _output;
        }
        for ( map < string, Buffer* >::iterator i = _param.begin(); i != _param.end(); i++ ) {
            delete i -> second;
        }
    }

    inline Buffer* getOutput() {
        return _output;
    }

    void handleProcess() {
        for ( map < string, ChainingNode* >::iterator i = _paramLinking.begin(); i != _paramLinking.end(); i++ ) {
            updateParameter( ( i -> first ).c_str(), i -> second );
        }
        processOutput();
    }

    virtual void processOutput() = 0;
    virtual string getClassName() = 0;

    inline void inputFrom( vector < ChainingNode* > nodes ) {
        _input.clear();
        for ( vector < ChainingNode* >::iterator i = nodes.begin(); i != nodes.end(); i++ ) {
            ChainingNode* node = ( *i );
            if ( node ) {
                addInput( node );
            }
        }
    }

    inline void addInput( ChainingNode* node ) {
        if ( ( int( _input.size() ) < _allowedInputs ) || ( _allowedInputs == -1 ) ) {
            _input.push_back( node );
        }
    }

    inline int inputCount() {
        return _input.size();
    }

    inline Buffer* getPrimaryInput() {
        ChainingNode* inputNode = getInput( 0 );
        Buffer* input = NULL;
        if ( inputNode ) {
            input = inputNode -> getOutput();
        }
        return input;
    }

    inline vector < ChainingNode* > getInputs() {
        return _input;
    }

    inline list < Buffer* > getInputBuffers() {
        list < Buffer* > buffers;
        for ( vector < ChainingNode* >::iterator i = _input.begin(); i != _input.end(); i++ ) {
            ChainingNode* node = ( *i );
            if ( node ) {
                buffers.push_back( node -> getOutput() );
            } else {
                buffers.push_back( NULL );
            }
        }
        return buffers;
    }

    inline Buffer* getInputBuffer( int id ) {
        ChainingNode* inputNode = getInput( id );
        Buffer* input = NULL;
        if ( inputNode ) {
            input = inputNode -> getOutput();
        }
        return input;
    }

    inline ChainingNode* getInput( int id ) {
        if ( ( id >= 0 ) && ( id < int( _input.size() ) ) ) {
            return _input[ id ];
        }
        return NULL;
    }

    inline int getInputId( ChainingNode* node ) {
        if ( node ) {
            for ( int i = 0; i < int( _input.size() ); i++ ) {
                if ( _input[ i ] == node ) {
                    return i;
                }
            }
        }
        return -1;
    }

    void setPosition( int x, int y ) {
        _posX = x;
        _posY = y;
    }

    int getPositionX() {
        return _posX;
    }

    int getPositionY() {
        return _posY;
    }

    virtual void render( SDL_Renderer* renderer ) {
        SDL_Texture* texture = Textures::getTexture( _textureId );
        SDL_Rect rect = getBlockRect();
        if ( texture ) {
            SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
            SDL_RenderCopy( renderer, texture, NULL, &rect );
        } else {
            SDL_SetRenderDrawColor( renderer, 128, 128, 128, 255 );
            SDL_RenderFillRect( renderer, &rect );
        }
    }

    void renderInputLinks( SDL_Renderer* renderer ) {
        for ( vector < ChainingNode* >::iterator i = _input.begin(); i != _input.end(); i++ ) {
            ChainingNode* node = *i;
            renderArrow( renderer, _posX, _posY, node -> _posX, node -> _posY );
        }
    }

    void renderParamInputLinks( SDL_Renderer* renderer ) {
        for ( map < ChainingNode*, list < string > >::iterator i = _paramLinkingFullSet.begin(); i != _paramLinkingFullSet.end(); i++ ) {
            ChainingNode* node = i -> first;
            renderArrow( renderer, _posX, _posY, node -> _posX, node -> _posY );
        }
    }

    static void renderArrow( SDL_Renderer* renderer, int x1, int y1, int x2, int y2 ) {
        SDL_RenderDrawLine( renderer, x1, y1, x2, y2 );
        static int drawRects = 4;
        static int sizes[] = { 17, 13, 9, 7 };
        static double inertia[] = { 0.8, 0.85, 0.88, 0.9 };
        for ( int j = 0; j < drawRects; j++ ) {
            SDL_Rect r;
            r.w = sizes[ j ];
            r.h = sizes[ j ];
            r.x = double( x1 ) * ( inertia[ j ] ) + double( x2 ) * ( 1.0 - inertia[ j ] ) - ( r.w >> 1 );
            r.y = double( y1 ) * ( inertia[ j ] ) + double( y2 ) * ( 1.0 - inertia[ j ] ) - ( r.h >> 1 );
            SDL_RenderFillRect( renderer, &r );
        }
    }

    SDL_Rect getBlockRect() {
        SDL_Rect rect;
        rect.w = BLOCK_WIDTH;
        rect.h = BLOCK_HEIGHT;
        rect.x = getPositionX() - ( rect.w >> 1 );
        rect.y = getPositionY() - ( rect.h >> 1 );
        return rect;
    }

    bool removeInput( ChainingNode* node ) {
        bool removed = false;
        vector < ChainingNode* > inputNew;
        for ( vector < ChainingNode* >::iterator i = _input.begin(); i != _input.end(); i++ ) {
            if ( *i != node ) {
                inputNew.push_back( *i );
            } else {
                removed = true;
            }
        }
        _input = inputNew;
        map < ChainingNode*, list < string > >::iterator k = _paramLinkingFullSet.find( node );
        if ( k != _paramLinkingFullSet.end() ) {
            list < string > s = k -> second;
            for ( list < string >::iterator l = s.begin(); l != s.end(); l++ ) {
                map < string, ChainingNode* >::iterator j = _paramLinking.find( *l );
                if ( j != _paramLinking.end() ) {
                    _paramLinking.erase( j );
                    //printf( "Erased linking j\n" );
                } else {
                    //printf( "Can't find linking j: %s\n", ( k -> second ).c_str() );
                }
            }
            _paramLinkingFullSet.erase( k );
            //printf( "Erased linking k\n" );
        }
        return removed;
    }

    Buffer* getParameter( const char* id ) {
        map < string, Buffer* >::iterator i = _param.find( string( id ) );
        if ( i != _param.end() ) {
            return i -> second;
        }
        return NULL;
    }

    double* setParameter( const char* id, double defaultValue ) {
        Buffer* existing = getParameter( id );
        if ( existing ) {
            return ( double* ) existing -> getMemory();
        }
        Buffer* buffer = Buffer::Create( ( BUFFER_SIZE >> 1 ) * sizeof( double ) );
        if ( buffer ) {
            _param[ string( id ) ] = buffer;
            updateParameter( id, defaultValue );
            return ( double* ) buffer -> getMemory();
        }
        return NULL;
    }

    void updateParameter( const char* id, double value ) {
        Buffer* param = getParameter( id );
        if ( param ) {
            double* mem = ( double* ) param -> getMemory();
            ParameterControlRule* rule = ParameterControlRule::getConstRuleFor( id );
            if ( rule ) {
                value = rule -> scaledValue( value );
            }
            for ( int i = 0; i < ( BUFFER_SIZE >> 1 ); i++ ) {
                mem[ i ] = value;
            }
        }
    }

    void updateParameter( const char* id, ChainingNode* node ) {
        Buffer* param = getParameter( id );
        Buffer* output = node -> getOutput();
        if ( param && output ) {
            double* mem = ( double* ) param -> getMemory();
            short* memOut = ( short* ) output -> getMemory();
            ParameterControlRule* rule = ParameterControlRule::getRuleFor( id );
            if ( rule ) {
                for ( int i = 0; i < ( BUFFER_SIZE >> 1 ); i++ ) {
                    mem[ i ] = rule -> scaledValue( double( memOut[ i ] ) / 65536.0 + 0.5 );
                }
            } else {
                for ( int i = 0; i < ( BUFFER_SIZE >> 1 ); i++ ) {
                    mem[ i ] = double( memOut[ i ] ) / 65536.0 + 0.5;
                }
            }
        }
    }

    void linkToParameter( string paramName, ChainingNode* node ) {
        if ( node ) {
            _paramLinking[ paramName ] = node;
            map < ChainingNode*, list < string > >::iterator i = _paramLinkingFullSet.find( node );
            list < string > s;
            if ( i != _paramLinkingFullSet.end() ) {
                s = i -> second;
            }
            s.push_back( paramName );
            _paramLinkingFullSet[ node ] = s;
        } else {
            map < string, ChainingNode* >::iterator j = _paramLinking.find( paramName );
            if ( j != _paramLinking.end() ) {
                _paramLinking.erase( j );
                map < ChainingNode*, list < string > >::iterator i = _paramLinkingFullSet.find( node );
                if ( i != _paramLinkingFullSet.end() ) {
                    list < string > s;
                    s.push_back( "" );
                    _paramLinkingFullSet[ node ] = s;
                }
            }
        }
    }

    bool addParameterLinkingNode( ChainingNode* node ) {
        if ( node ) {
            if ( !parameterLinkingNodeExists( node ) ) {
                list < string > s;
                s.push_back( "" );
                _paramLinkingFullSet[ node ] = s;
                return true;
            }
        }
        return false;
    }

    bool parameterLinkingNodeExists( ChainingNode* node ) {
        map < ChainingNode*, list < string > >::iterator i = _paramLinkingFullSet.find( node );
        if ( i != _paramLinkingFullSet.end() ) {
            return true;
        }
        return false;
    }

    /*

                list < string > s = i -> second;
                list < string > sNew;
                for ( list < string >::iterator k = s.begin(); k != s.end(); k++ ) {

                }
    */

    bool removeParameterLinkingNode( ChainingNode* node ) {
        map < ChainingNode*, list < string > >::iterator i = _paramLinkingFullSet.find( node );
        if ( i != _paramLinkingFullSet.end() ) {
            list < string > s = i -> second;
            for ( list < string >::iterator k = s.begin(); k != s.end(); k++ ) {
                map < string, ChainingNode* >::iterator j = _paramLinking.find( *k );
                if ( j != _paramLinking.end() ) {
                    _paramLinking.erase( j );
                }
            }
            _paramLinkingFullSet.erase( i );
            return true;
        }
        return false;
    }

    map < string, Buffer* > getAllParameterPointers() {
        return _param;
    }

    map < string, ChainingNode* > getParameterLinkingMap() {
        return _paramLinking;
    }

    ChainingNode* getLinkedNodeToParam( const char* id ) {
        map < string, ChainingNode* >::iterator i = _paramLinking.find( string( id ) );
        if ( i != _paramLinking.end() ) {
            return i -> second;
        }
        return NULL;
    }

    map < ChainingNode*, list < string > > getParameterLinkingAvailableNodes() {
        return _paramLinkingFullSet;
    }

    int getTextureId() {
        return _textureId;
    }

    int getId() {
        return _id;
    }

    void setId( int id ) {
        _id = id;
    }

    protected:

    void setTextureId( int id ) {
        _textureId = id;
    }

    void setAllowedInputs( int allowedInputs ) {
        _allowedInputs = allowedInputs;
    }

    private:

    vector < ChainingNode* > _input;
    Buffer* _output;

    int _posX;
    int _posY;
    int _id;

    int _textureId;
    int _allowedInputs;

    map < string, Buffer* > _param;
    map < string, ChainingNode* > _paramLinking;
    map < ChainingNode*, list < string > > _paramLinkingFullSet;

    static int _globalTime;

};

int ChainingNode::_globalTime = 0;


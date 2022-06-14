#pragma once

class ChainingTree : public ChainingNode {
    protected:

    class OutputNode : public AdderNode {
        public:
        OutputNode() : AdderNode() {
            setTextureId( TEXTURE_SPECIAL_OUTPUT );
        }
        virtual ~OutputNode() {
        }

        virtual string getClassName() {
            return "OUTPUT NODE";
        }

        private:
    };


    public:
    ChainingTree( string className ) {
        _ID_GEN = 0;
        _outputNode = new OutputNode();
        _outputNode -> setPosition( WINDOW_WIDTH >> 1, WINDOW_HEIGHT >> 1 );
        addComponent( _outputNode );
        _className = className;
        //_offsetX = 0;
        //_offsetY = 0;
    }
    virtual ~ChainingTree() {
    }

    virtual void processOutput() {
        for ( list < ChainingNode* >::iterator i = _nodesQueue.begin(); i != _nodesQueue.end(); i++ ) {
            ChainingNode* node = ( *i );
            node -> handleProcess();
        }
        ChainingNode::setNextGlobalTime();
    }

    ChainingNode* getOutputNode() {
        return _outputNode;
    }

    int addComponent( ChainingNode* node ) {
        if ( !contains( node ) ) {
            int id = _ID_GEN;
            _ID_GEN++;
            _tree[ id ] = node;
            _treeRev[ node ] = id;
            node -> setId( id );
            refreshChain();
            return id;
        }
        return -1;
    }

    void addComponent( int id, ChainingNode* node ) {
        if ( !contains( node ) ) {
            _tree[ id ] = node;
            _treeRev[ node ] = id;
            map < int, ChainingNode* >::reverse_iterator i = _tree.rend();
            if ( _ID_GEN <= id ) {
                _ID_GEN = id + 1;
            }
            node -> setId( id );
            //printf( "_ID_GEN = %d\n", _ID_GEN );
            refreshChain();
        }
    }

    bool link( int idInput, int idOutput ) {
        ChainingNode* input = get( idInput );
        ChainingNode* output = get( idOutput );
        if ( input && output ) {
            vector < ChainingNode* > outputInputs = output -> getInputs();
            for ( vector < ChainingNode* >::iterator i = outputInputs.begin(); i != outputInputs.end(); i++ ) {
                if ( *i == input ) {
                    return false;
                }
            }
            output -> addInput( input );
            refreshChain();
            return true;
        }
        return false;
    }

    inline int getOutputId() {
        return idOf( _outputNode );
    }

    bool contains( ChainingNode* node ) {
        map < ChainingNode*, int >::iterator i = _treeRev.find( node );
        return ( i != _treeRev.end() );
    }

    ChainingNode* get( int id ) {
        map < int, ChainingNode* >::iterator i = _tree.find( id );
        if ( i != _tree.end() ) {
            return i -> second;
        }
        return NULL;
    }

    void refreshChain() {
        set < ChainingNode* > checkedNodes;
        _nodesQueue = _navigateInputs( _outputNode, &checkedNodes );
        printf( "Node chain (%d elements):\n", int( _nodesQueue.size() ) );
        for ( list < ChainingNode* >::iterator i = _nodesQueue.begin(); i != _nodesQueue.end(); i++ ) {
            int id = idOf( *i );
            printf( " > %d | ", id );
            map < ChainingNode*, list < string > > m = ( *i ) -> getParameterLinkingAvailableNodes();
            for ( map < ChainingNode*, list < string > >::iterator j = m.begin(); j != m.end(); j++ ) {
                ChainingNode* node = j -> first;
                printf( "%d ", idOf( node ) );
            }
            printf( "\n" );
        }
    }

    void render( SDL_Renderer* renderer ) {
        for ( map < int, ChainingNode* >::iterator i = _tree.begin(); i != _tree.end(); i++ ) {
            i -> second -> render( renderer );
        }
        SDL_SetRenderDrawColor( renderer, 64, 64, 255, 255 );
        for ( map < int, ChainingNode* >::iterator i = _tree.begin(); i != _tree.end(); i++ ) {
            i -> second -> renderInputLinks( renderer );
        }
        SDL_SetRenderDrawColor( renderer, 255, 64, 64, 255 );
        for ( map < int, ChainingNode* >::iterator i = _tree.begin(); i != _tree.end(); i++ ) {
            i -> second -> renderParamInputLinks( renderer );
        }
    }

    ChainingNode* findNearestNode( int x, int y, double maxDist ) {
        double curDist = maxDist;
        ChainingNode* selected = NULL;
        for ( map < int, ChainingNode* >::iterator i = _tree.begin(); i != _tree.end(); i++ ) {
            int x2 = i -> second -> getPositionX();
            int y2 = i -> second -> getPositionY();
            double dx = x - x2;
            double dy = y - y2;
            double d = sqrt( dx * dx + dy * dy );
            if ( d <= curDist ) {
                curDist = d;
                selected = i -> second;
            }
        }
        return selected;
    }

    int idOf( ChainingNode* node ) {
        map < ChainingNode*, int >::iterator i = _treeRev.find( node );
        if ( i != _treeRev.end() ) {
            return i -> second;
        }
        return -1;
    }

    bool removeNode( ChainingNode* node ) {
        if ( node == _outputNode ) {
            return false;
        }
        map < ChainingNode*, int >::iterator i = _treeRev.find( node );
        if ( i != _treeRev.end() ) {
            map < int, ChainingNode* >::iterator j = _tree.find( i -> second );
            _tree.erase( j );
            _treeRev.erase( i );
            for ( j = _tree.begin(); j != _tree.end(); j++ ) {
                ChainingNode* n2 = j -> second;
                n2 -> removeInput( node );
            }
            refreshChain();
            return true;
        }
        return false;
    }

    ChainingLine* nearestLink( double x, double y, double detectionRadius ) {
        ChainingLine* link = new ChainingLine();
        double shortestDist = detectionRadius;
        for ( map < int, ChainingNode* >::iterator i = _tree.begin(); i != _tree.end(); i++ ) {
            ChainingNode* node = i -> second;
            double x2 = node -> getPositionX();
            double y2 = node -> getPositionY();
            vector < ChainingNode* > inputs = node -> getInputs();
            for ( vector < ChainingNode* >::iterator j = inputs.begin(); j != inputs.end(); j++ ) {
                ChainingNode* input = ( *j );
                double x1 = input -> getPositionX();
                double y1 = input -> getPositionY();
                double dx = x2 - x1;
                double dy = y2 - y1;
                double lineDist = sqrt( dy * dy + dx * dx );
                double dist = fabs( dy * x - dx * y + x2 * y1 - y2 * x1 ) / lineDist;
                if ( dist < shortestDist ) {
                    double dx1 = x1 - x;
                    double dy1 = y1 - y;
                    double dx2 = x2 - x;
                    double dy2 = y2 - y;
                    double distFromP1 = sqrt( dy1 * dy1 + dx1 * dx1 );
                    double distFromP2 = sqrt( dy2 * dy2 + dx2 * dx2 );
                    if ( ( distFromP1 < lineDist + detectionRadius ) && ( distFromP2 < lineDist + detectionRadius ) ) {
                        shortestDist = dist;
                        link -> setLink( input, node );
                    }
                }
            }
        }
        if ( link -> isSet() ) {
            return link;
        }
        delete link;
        return NULL;
    }

    ChainingLine* nearestInputLink( double x, double y, double detectionRadius ) {
        ChainingLine* link = new ChainingLine();
        double shortestDist = detectionRadius;
        for ( map < int, ChainingNode* >::iterator i = _tree.begin(); i != _tree.end(); i++ ) {
            ChainingNode* node = i -> second;
            double x2 = node -> getPositionX();
            double y2 = node -> getPositionY();
            map < ChainingNode*, list < string > > inputs = node -> getParameterLinkingAvailableNodes();
            for ( map < ChainingNode*, list < string > >::iterator j = inputs.begin(); j != inputs.end(); j++ ) {
                ChainingNode* input = j -> first;
                double x1 = input -> getPositionX();
                double y1 = input -> getPositionY();
                double dx = x2 - x1;
                double dy = y2 - y1;
                double lineDist = sqrt( dy * dy + dx * dx );
                double dist = fabs( dy * x - dx * y + x2 * y1 - y2 * x1 ) / lineDist;
                if ( dist < shortestDist ) {
                    double dx1 = x1 - x;
                    double dy1 = y1 - y;
                    double dx2 = x2 - x;
                    double dy2 = y2 - y;
                    double distFromP1 = sqrt( dy1 * dy1 + dx1 * dx1 );
                    double distFromP2 = sqrt( dy2 * dy2 + dx2 * dx2 );
                    if ( ( distFromP1 < lineDist + detectionRadius ) && ( distFromP2 < lineDist + detectionRadius ) ) {
                        shortestDist = dist;
                        link -> setLink( input, node );
                    }
                }
            }
        }
        if ( link -> isSet() ) {
            return link;
        }
        delete link;
        return NULL;
    }

    bool removeLink( ChainingLine* link ) {
        if ( link ) {
            if ( link -> isSet() ) {
                ChainingNode* src = link -> getFromNode();
                ChainingNode* dst = link -> getToNode();
                if ( dst -> removeInput( src ) ) {
                    refreshChain();
                    return true;
                }
            }
        }
        return false;
    }

    bool removeInputLink( ChainingLine* link ) {
        if ( link ) {
            if ( link -> isSet() ) {
                ChainingNode* src = link -> getFromNode();
                ChainingNode* dst = link -> getToNode();
                if ( dst -> removeParameterLinkingNode( src ) ) {
                    refreshChain();
                    return true;
                }
            }
        }
        return false;
    }

    void moveTree( int x, int y ) {
        //_offsetX += x;
        //_offsetY += y;
        for ( map < int, ChainingNode* >::iterator i = _tree.begin(); i != _tree.end(); i++ ) {
            ChainingNode* node = i -> second;
            node -> setPosition( node -> getPositionX() + x, node -> getPositionY() + y );
        }
    }

    map < int, ChainingNode* > getAllNodes() {
        return _tree;
    }

    virtual string getClassName() {
        return _className;
    }

    void setClassName( string className ) {
        _className = className;
    }

    private:

    list < ChainingNode* > _navigateInputs( ChainingNode* node, set < ChainingNode* >* checkedNodes ) {
        list < ChainingNode* > nodesQueue;
        if ( node ) {
            if ( checkedNodes -> find( node ) == checkedNodes -> end() ) {
                checkedNodes -> insert( node );
                vector < ChainingNode* > allInputs;
                vector < ChainingNode* > nodeInputs = node -> getInputs();
                map < string, ChainingNode* > paramLinking = node -> getParameterLinkingMap();
                for ( map < string, ChainingNode* >::iterator j = paramLinking.begin(); j != paramLinking.end(); j++ ) {
                    allInputs.push_back( j -> second );
                }
                for ( vector < ChainingNode* >::iterator i = nodeInputs.begin(); i != nodeInputs.end(); i++ ) {
                    allInputs.push_back( *i );
                }
                for ( vector < ChainingNode* >::iterator i = allInputs.begin(); i != allInputs.end(); i++ ) {
                    list < ChainingNode* > nodeNavigatedInputs = _navigateInputs( *i, checkedNodes );
                    for ( list < ChainingNode* >::iterator j = nodeNavigatedInputs.begin(); j != nodeNavigatedInputs.end(); j++ ) {
                        bool exist = false;//( checkedNodes -> find( *j ) == checkedNodes -> end() );
                        for ( list < ChainingNode* >::iterator k = nodesQueue.begin(); k != nodesQueue.end(); k++ ) {
                            if ( *j == *k ) {
                                exist = true;
                            }
                        }
                        if ( !exist ) {
                            nodesQueue.push_back( *j );
                            checkedNodes -> insert( *j );
                        }
                    }
                }
                nodesQueue.push_back( node );
            }
        }
        return nodesQueue;
    }

    ChainingNode* _outputNode;
    list < ChainingNode* > _nodesQueue;
    map < int, ChainingNode* > _tree;
    map < ChainingNode*, int > _treeRev;
    int _ID_GEN;

    string _className;

    //int _offsetX;
    //int _offsetY;

};



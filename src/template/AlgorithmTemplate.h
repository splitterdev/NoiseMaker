#pragma once

class AlgorithmTemplate {
    public:
    AlgorithmTemplate( const char* path ) {
        _path = string( path );
    }
    AlgorithmTemplate( string path ) {
        _path = path;
    }
    ~AlgorithmTemplate() {
    }

    ChainingTree* read() {
        FILE* handle = fopen( _path.c_str(), "r" );
        if ( handle ) {
            ChainingTree* tree = new ChainingTree( "" );
            char* buffer = ( char* ) malloc( 4096 );
            do {
                if ( fgets( buffer, 4095, handle ) ) {
                    vector < string > split = _split( buffer, ':' );
                    if ( int( split.size() ) > 0 ) {
                        if ( strcmp( split[ 0 ].c_str(), "NODE" ) == 0 && split.size() == 5 ) {
                            ChainingNodeFactory* namedFactory = ChainingNodeFactory::getFactory( split[ 2 ] );
                            if ( namedFactory ) {
                                ChainingNode* node = namedFactory -> create();
                                node -> setPosition( _double( split[ 3 ] ), _double( split[ 4 ] ) );
                                tree -> addComponent( _double( split[ 1 ] ), node );
                            } else {
                                if ( strcmp( split[ 2 ].c_str(), "OUTPUT NODE" ) == 0 ) {
                                    ChainingNode* node = tree -> getOutputNode();
                                    node -> setPosition( _double( split[ 3 ] ), _double( split[ 4 ] ) );
                                }
                            }
                        } else if ( strcmp( split[ 0 ].c_str(), "PARAM" ) == 0 && split.size() == 4 ) {
                            ChainingNode* node = tree -> get( _double( split[ 1 ] ) );
                            if ( node ) {
                                node -> updateParameter( split[ 2 ].c_str(), _double( split[ 3 ] ) );
                            }
                        } else if ( strcmp( split[ 0 ].c_str(), "LINK" ) == 0 && split.size() == 4 ) {
                            int a = _double( split[ 1 ] );
                            int b = _double( split[ 2 ] );
                            if ( strcmp( split[ 3 ].c_str(), "INPUT" ) == 0 ) {
                                if ( !tree -> link( a, b ) ) {
                                    printf( "Can't link: %d => %d\n", a, b );
                                }
                            } else {
                                ChainingNode* source = tree -> get( a );
                                ChainingNode* destination = tree -> get( b );
                                if ( source && destination ) {
                                    destination -> linkToParameter( split[ 3 ], source );
                                } else {
                                    printf( "Can't link as parameter: %d => %d\n", a, b );
                                }
                            }
                        } else if ( strcmp( split[ 0 ].c_str(), "NAME" ) == 0 && split.size() == 2 ) {
                            tree -> setClassName( split[ 1 ] );
                        }
                    }
                }
            } while ( !feof( handle ) );
            fclose( handle );
            free( buffer );
            tree -> refreshChain();
            return tree;
        }
        return NULL;
    }

    bool write( ChainingTree* tree ) {
        map < int, ChainingNode* > nodes = tree -> getAllNodes();
        FILE* handle = fopen( _path.c_str(), "w" );
        if ( handle ) {
            fprintf( handle, "NAME:%s\n", ( tree -> getClassName() ).c_str() );
            map < ChainingLine*, string > links;
            for ( map < int, ChainingNode* >::iterator i = nodes.begin(); i != nodes.end(); i++ ) {
                ChainingNode* node = i -> second;
                if ( node ) {
                    string className = node -> getClassName();
                    fprintf( handle, "NODE:%d:%s:%d:%d\n", i -> first, className.c_str(), node -> getPositionX(), node -> getPositionY() );
                    vector < ChainingNode* > inputs = node -> getInputs();
                    for ( vector < ChainingNode* >::iterator j = inputs.begin(); j != inputs.end(); j++ ) {
                        ChainingLine* link = new ChainingLine();
                        link -> setLink( *j, i -> second );
                        links[ link ] = "INPUT";
                    }
                    map < string, Buffer* > allParams = node -> getAllParameterPointers();
                    for ( map < string, Buffer* >::iterator j = allParams.begin(); j != allParams.end(); j++ ) {
                        string paramName = j -> first;
                        Buffer* buffer = j -> second;
                        double paramValue = 0.0;
                        if ( buffer ) {
                            if ( buffer -> getMemory() ) {
                                paramValue = ( ( double* ) buffer -> getMemory() )[ 0 ];
                            }
                        }
                        fprintf( handle, "PARAM:%d:%s:%g\n", i -> first, paramName.c_str(), paramValue );
                    }
                    map < ChainingNode*, list < string > > paramLinking = node -> getParameterLinkingAvailableNodes();
                    for ( map < ChainingNode*, list < string > >::iterator i = paramLinking.begin(); i != paramLinking.end(); i++ ) {
                        list < string > s = i -> second;
                        for ( list < string >::iterator k = s.begin(); k != s.end(); k++ ) {
                            ChainingLine* link = new ChainingLine();
                            link -> setLink( i -> first, node );
                            links[ link ] = *k;
                        }
                    }
                    fprintf( handle, "\n" );
                }
            }
            for ( map < ChainingLine*, string >::iterator i = links.begin(); i != links.end(); i++ ) {
                ChainingLine* link = i -> first;
                if ( link ) {
                    if ( link -> isSet() ) {
                        int fromId = tree -> idOf( link -> getFromNode() );
                        int toId = tree -> idOf( link -> getToNode() );
                        string relation = i -> second;
                        fprintf( handle, "LINK:%d:%d:%s\n", fromId, toId, relation.c_str() );
                    }
                    delete link;
                }
            }

            fclose( handle );
            return true;
        }
        return false;
    }

    bool fileExists() {
        bool exists = false;
        FILE* handle = fopen( _path.c_str(), "r" );
        if ( handle ) {
            exists = true;
            fclose( handle );
        }
        return exists;
    }

    private:

    string _path;

};



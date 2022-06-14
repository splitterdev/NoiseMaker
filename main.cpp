#include <al/al.h>
#include <al/alc.h>
#include <SDL2/SDL.h>

#include <bass/bass.h>

#include <stdio.h>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <string.h>
using namespace std;

#define AUDIO_DRIVER_OPENAL 0
#define AUDIO_DRIVER_BASS 1

int WINDOW_WIDTH = 1024;
int WINDOW_HEIGHT = 768;
int WINDOW_FULLSCREEN = 0;
int AUDIO_DRIVER_TYPE = 0;
int BUFFER_SIZE = 4096;
int BUFFER_QUEUE = 4;

#include "src/util/Config.h"

bool helpEnabled = false;

#include "src/util/Mutex.h"
#include "src/util/Thread.h"
#include "src/util/Text.h"
#include "src/util/Vector2D.h"
#include "src/audiogen/AudioGen.h"

int mouseX = 0;
int mouseY = 0;

SDL_Rect increaseRect( SDL_Rect r, int b ) {
    r.x -= b;
    r.y -= b;
    r.w += b << 1;
    r.h += b << 1;
    return r;
}

class TestButton : public Button {
    public:
    TestButton( int id ) : Button() {
        _id = id;
        setTexture( TEXTURE_ADDER );
    }
    ~TestButton() {
    }

    virtual void onClick() {
        printf( "Hello world! id = %d\n", _id );
    }

    private:
    int _id;
};

int moveSpeed = 6;

int moveTreeX = 0;
int moveTreeY = 0;

void makeParametersPanel( ChainingNode* node, Panel* panel ) {
    if ( node == NULL ) {
        panel -> deleteAllButtons();
    } else {
        panel -> deleteAllButtons();
        map < string, Buffer* > params = node -> getAllParameterPointers();
        int row = 0;
        for ( map < string, Buffer* >::iterator i = params.begin(); i != params.end(); i++ ) {
            StaticText* staticText = new StaticText( i -> first );
            panel -> addButton( staticText, 0, row, 1, 1 );
            ParamEdit* paramEdit = new ParamEdit();
            paramEdit -> linkToValue( node, i -> first );
            paramEdit -> setEditEnabled( node -> getLinkedNodeToParam( ( i -> first ).c_str() ) == NULL );
            panel -> addButton( paramEdit, 1, row, 5, 1 );
            row++;
        }
    }
}

void makeIOCTLPanel( ChainingTree* tree, ChainingNode* node, Panel* panel ) {
    if ( node == NULL ) {
        panel -> deleteAllButtons();
    } else {
        panel -> deleteAllButtons();
        map < string, Buffer* > params = node -> getAllParameterPointers();
        int row = 0;
        for ( map < string, Buffer* >::iterator i = params.begin(); i != params.end(); i++ ) {
            StaticText* staticText = new StaticText( i -> first );
            panel -> addButton( staticText, 0, row, 1, 1 );
            int textureId = TEXTURE_BUTTON_SELECT_INPUT;
            ChainingNode* linkedNode = node -> getLinkedNodeToParam( ( i -> first ).c_str() );
            if ( linkedNode ) {
                textureId = linkedNode -> getTextureId();
            }
            ParamDialogLauncher* launcher = new ParamDialogLauncher( i -> first, node, tree, textureId );
            panel -> addButton( launcher, 1, row, 1, 1 );
            row++;
        }
    }
}

void renderHelp( SDL_Renderer* renderer, string help ) {
    SDL_Surface* txt = Text::RenderOnly( "%s", help.c_str() );
    if ( txt ) {
        SDL_Texture* tTxt = SDL_CreateTextureFromSurface( renderer, txt );
        if ( tTxt ) {
            SDL_Rect dst;
            dst.w = txt -> w;
            dst.h = txt -> h;
            dst.x = WINDOW_WIDTH - dst.w - 8;
            dst.y = WINDOW_HEIGHT - dst.h - 8;
            SDL_RenderCopy( renderer, tTxt, NULL, &dst );
            SDL_DestroyTexture( tTxt );
        }
        SDL_FreeSurface( txt );
    }
}

void renderLinkInputId( SDL_Renderer* renderer, ChainingLine* link ) {
    ChainingNode* fromNode = link -> getFromNode();
    ChainingNode* toNode = link -> getToNode();
    int inputId = toNode -> getInputId( fromNode );
    static const int blockSize = 32;
    Vector2D xy1 = Vector2D(
        fromNode -> getPositionX(),
        fromNode -> getPositionY()
    );
    Vector2D xy2 = Vector2D(
        toNode -> getPositionX(),
        toNode -> getPositionY()
    );
    SDL_Surface* txt = Text::RenderOnly( "%d", inputId );
    if ( txt ) {
        SDL_Texture* tTxt = SDL_CreateTextureFromSurface( renderer, txt );
        if ( tTxt ) {
            SDL_Rect dst;
            dst.w = blockSize;
            dst.h = blockSize;
            dst.x = ( xy1.x + xy2.x ) / 2.0 - ( dst.w >> 1 );
            dst.y = ( xy1.y + xy2.y ) / 2.0 - ( dst.h >> 1 );
            SDL_SetRenderDrawColor( renderer, 192, 192, 192, 128 );
            SDL_RenderFillRect( renderer, &dst );
            SDL_RenderCopy( renderer, tTxt, NULL, &dst );
            SDL_DestroyTexture( tTxt );
        }
        SDL_FreeSurface( txt );
    }
}

/*void save_texture( SDL_Renderer* renderer, SDL_Texture* texture ) {
    SDL_Texture* target = SDL_GetRenderTarget( renderer );
    SDL_SetRenderTarget( renderer, texture );
    SDL_Surface* surface = SDL_CreateRGBSurface( 0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0, 0, 0, 0 );
    SDL_RenderReadPixels( renderer, NULL, surface -> format -> format, surface -> pixels, surface->pitch );
    IMG_SavePNG( surface, file_name );
    SDL_FreeSurface( surface );
    SDL_SetRenderTarget( renderer, target );
}*/

int main( int argc, char** argv ) {
    Config::initialize();
    SDL_Init( SDL_INIT_VIDEO );
    SDL_Window* window = SDL_CreateWindow( "NoiseMaker 1.3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FULLSCREEN > 0 ? SDL_WINDOW_FULLSCREEN : 0 );
    SDL_Renderer* renderer = SDL_CreateRenderer( window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

    SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_BLEND );
    Text::Init( "data/font/Font.ttf", 32 );

    /*Vector2D vec[ 5 ];

    vec[ 0 ] = Vector2D( 0.0, 0.0 );
    vec[ 1 ] = Vector2D( 0.0, 1.0 );
    vec[ 2 ] = Vector2D( 1.0, 0.0 );
    vec[ 3 ] = Vector2D( 0.0, - 1.0 );
    vec[ 4 ] = Vector2D( - 1.0, 0.0 );

    for ( int i = 0; i < 5; i++ ) {
        PolarVector2D pvec = PolarVector2D( vec[ i ] );
        printf( "R[ %d ] = %g / A[ %d ] = %g\n", i, pvec.r, i, degr( pvec.a ) );
        Vector2D vecP = Vector2D( pvec );
        printf( "X[ %d ] = %g / Y[ %d ] = %g\n", i, vecP.x, i, vecP.y );
    }*/

    ChainingTree* tree = new ChainingTree( "Main" );
    //ChainingNode* treeOut = tree -> getOutputNode();
    //SineGenerator* sineGen1 = new SineGenerator( 200.0, 0.2 );
    //SineGenerator* sineGen2 = new SineGenerator( 450.0, 0.2 );
    //SineGenerator* sineGen3 = new SineGenerator( 480.0, 0.2 );
    //FSDNode* ovNode2 = new FSDNode( 0.1 );
    //DerivatorNode* dvNode1 = new DerivatorNode();
    //FSDNode* ovNode1 = new FSDNode( 0.1 );

    //ovNode2 -> addInput( sineGen1 );
    //dvNode1 -> addInput( ovNode2 );
    //ovNode1 -> addInput( dvNode1 );
    //treeOut -> addInput( ovNode1 );

    /*int sineID = tree -> addComponent( new SineGenerator( 200.0, 0.2 ) );
    int ov1ID = tree -> addComponent( new FSDNode( 0.1 ) );
    int dvID = tree -> addComponent( new DerivatorNode() );
    int ov2ID = tree -> addComponent( new FSDNode( 0.1 ) );

    tree -> link( sineID, ov1ID );
    tree -> link( ov1ID, dvID );
    tree -> link( dvID, ov2ID );
    tree -> link( ov2ID, tree -> getOutputId() );*/

    ChainPlayer* player = new ChainPlayer();
    player -> changeTree( tree );

    ChainingNode* selectTargetNode = NULL;
    ChainingNode* selectedNode = NULL;
    ChainingNode* linkStartNode = NULL;
    ChainingLine* selectedLink = NULL;
    ChainingLine* selectedInputLink = NULL;
    initChainingNodeFactory();

    Textures::init( renderer );

    int panelWidth = 6;
    int panelHeight = 10;
    TabPanel* mainUI = new TabPanel( panelWidth, panelHeight );
    mainUI -> setPosition( 32, 32 );
    Panel* generalPanel = new Panel( panelWidth, panelHeight );
    Panel* libraryPanel = new Panel( panelWidth, panelHeight );
    Panel* detailPanel = new Panel( panelWidth, panelHeight );
    Panel* ioCtlPanel = new Panel( panelWidth, panelHeight );
    Panel* templateManagementPanel = new Panel( panelWidth, panelHeight );
    mainUI -> addPanel( generalPanel, TEXTURE_BUTTON_GENERAL, "General" );
    mainUI -> addPanel( libraryPanel, TEXTURE_BUTTON_LIBRARY, "Library" );
    mainUI -> addPanel( detailPanel, TEXTURE_BUTTON_DETAILS, "Parameter Value Control" );
    mainUI -> addPanel( ioCtlPanel, TEXTURE_BUTTON_IOCTL, "Parameter Input Control" );
    mainUI -> addPanel( templateManagementPanel, TEXTURE_BUTTON_TEMPLATE_MANAGEMENT, "Import/export" );

    generalPanel -> addButton( new PlayButton( player ), 0, 0 );
    generalPanel -> addButton( new PauseButton( player ), 1, 0 );
    generalPanel -> addButton( new RecordButton( player ), 0, panelHeight - 1 );
    generalPanel -> addButton( new ChangeClickModeButton( CLICKMODE_SELECT, TEXTURE_BUTTON_SELECTION_TOOL, "Select" ), 0, 3 );
    generalPanel -> addButton( new ChangeClickModeButton( CLICKMODE_LINKING, TEXTURE_BUTTON_LINKING_TOOL, "Linking tool" ), 1, 3 );
    generalPanel -> addButton( new ChangeClickModeButton( CLICKMODE_LINKING_INPUT, TEXTURE_BUTTON_LINKING_INPUT_TOOL, "Linking input tool" ), 2, 3 );
    generalPanel -> addButton( new ChangeClickModeButton( CLICKMODE_LINK_REMOVE, TEXTURE_BUTTON_LINK_REMOVE_TOOL, "Remove link" ), 5, 3 );
    generalPanel -> addButton( new ChangeClickModeButton( CLICKMODE_NODE_REMOVE, TEXTURE_BUTTON_NODE_REMOVE_TOOL, "Remove node" ), 5, 5 );
    generalPanel -> addButton( new ChangeTabButton( mainUI, 1, TEXTURE_BUTTON_NODE_CREATE, "Create node" ), 0, 5 );
    generalPanel -> addButton( new ChangeClickModeButton( CLICKMODE_NODE_MOVE, TEXTURE_BUTTON_NODE_MOVE_TOOL, "Move" ), 1, 5 );
    generalPanel -> addButton( new HelpButton(), panelWidth - 1, panelHeight - 1 );
    generalPanel -> addButton( new VisualisationComponentSelectorLauncher( &tree ), panelWidth - 2, panelHeight - 1 );

    templateManagementPanel -> addButton( new ImportTemplateButton( &tree, player ), panelWidth - 1, 0 );
    Edit* pathEdit = new Edit( "Export to..." );
    templateManagementPanel -> addButton( pathEdit, 0, 2, panelWidth - 1, 1 );
    templateManagementPanel -> addButton( new ExportTemplateButton( &tree, pathEdit ), panelWidth - 1, 2 );

    mainUI -> addShortCut( 0, SDLK_1 );
    mainUI -> addShortCut( 1, SDLK_2 );
    mainUI -> addShortCut( 2, SDLK_3 );
    mainUI -> addShortCut( 3, SDLK_4 );
    mainUI -> addShortCut( 4, SDLK_5 );

    // test library

    int j = 0;
    int libraryItemWidth = panelWidth;
    map < int, int > libLoadedItems = ChainingNodeFactory::getFactoryUiItems();
    for ( map < int, int >::iterator i = libLoadedItems.begin(); i != libLoadedItems.end(); i++ ) {
        libraryPanel -> addButton( new LibrarySelectorButton( i -> first, i -> second ), j % libraryItemWidth, j / libraryItemWidth );
        j++;
    }

    bool moveSelectedNode = false;
    mainUI -> changeTab( 0 );

    ParameterControlRule::initRules();

    DialogSelectorBox* lastBox = NULL;
    mapVCFactory();

    bool done = false;
    while ( !done ) {
        ChainingTree* prevTree = tree;
        SDL_Event event;
        while ( SDL_PollEvent( &event ) ) {
            switch ( event.type ) {
                case SDL_QUIT: {
                    done = true;
                    break;
                }
                case SDL_KEYDOWN: {
                    if ( event.key.repeat == 0 ) {
                        if ( event.key.keysym.sym == SDLK_SPACE ) {
                            player -> setPlaying( true );
                        }
                        if ( event.key.keysym.sym == SDLK_F12 ) {
                            //takeScreenshot();
                        }
                        if ( !DialogSelectorBox::getDialog() ) {
                            if ( event.key.keysym.sym == SDLK_DELETE ) {
                                ChainingNode* del = tree -> findNearestNode( mouseX, mouseY, ChainingNode::BLOCK_WIDTH );
                                if ( tree -> removeNode( del ) ) {
                                    selectedNode = NULL;
                                    makeParametersPanel( NULL, detailPanel );
                                    makeIOCTLPanel( NULL, NULL, ioCtlPanel );
                                    delete del;
                                }
                            }
                            if ( focusedEdit ) {
                                SDL_Keycode k = event.key.keysym.sym;
                                if ( k == SDLK_BACKSPACE ) {
                                    focusedEdit -> eraseChar();
                                } else {
                                    focusedEdit -> appendChar( k );
                                }
                                //if ( focusedEdit ) {
                                    //printf( "Edit value = \"%s\", %g\n", ( focusedEdit -> getText() ).c_str(), ( focusedEdit -> getValue() ) );
                                //}
                            } else {
                                if ( event.key.keysym.sym == SDLK_l ) {
                                    clickMode = CLICKMODE_LINKING;
                                    moveSelectedNode = false;
                                    selectedNode = NULL;
                                } else if ( event.key.keysym.sym == SDLK_p ) {
                                    clickMode = CLICKMODE_LINKING_INPUT;
                                    moveSelectedNode = false;
                                    selectedNode = NULL;
                                } else if ( event.key.keysym.sym == SDLK_m ) {
                                    clickMode = CLICKMODE_NODE_MOVE;
                                    moveSelectedNode = false;
                                    selectedNode = NULL;
                                } else if ( event.key.keysym.sym == SDLK_BACKSPACE ) {
                                    if ( tree -> removeLink( selectedLink ) ) {
                                        delete selectedLink;
                                        selectedLink = NULL;
                                    } else if ( tree -> removeInputLink( selectedInputLink ) ) {
                                        delete selectedInputLink;
                                        selectedInputLink = NULL;
                                    }
                                } else if ( event.key.keysym.sym == SDLK_h ) {
                                    helpEnabled = !helpEnabled;
                                } else if ( event.key.keysym.sym == SDLK_UP ) {
                                    moveTreeY = moveSpeed;
                                } else if ( event.key.keysym.sym == SDLK_DOWN ) {
                                    moveTreeY = - moveSpeed;
                                } else if ( event.key.keysym.sym == SDLK_LEFT ) {
                                    moveTreeX = moveSpeed;
                                } else if ( event.key.keysym.sym == SDLK_RIGHT ) {
                                    moveTreeX = - moveSpeed;
                                } else {
                                    int tab;
                                    if ( mainUI -> getShortCutTab( event.key.keysym.sym, &tab ) ) {
                                        mainUI -> changeTab( tab );
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
                case SDL_KEYUP: {
                    if ( event.key.keysym.sym == SDLK_ESCAPE ) {
                        if ( focusedEdit ) {
                            focusedEdit -> finishEditing();
                        } else {
                            DialogSelectorBox::removeDialog();
                        }
                    }
                    if ( !DialogSelectorBox::getDialog() ) {
                        if ( event.key.keysym.sym == SDLK_RETURN ) {
                            if ( focusedEdit ) {
                                focusedEdit -> finishEditing();
                            } else {
                                player -> setPlaying( !player -> isSetPlaying() );
                            }
                        } else if ( event.key.keysym.sym == SDLK_SPACE ) {
                            player -> setPlaying( false );
                        } else if ( event.key.keysym.sym == SDLK_UP ) {
                            moveTreeY = 0.0;
                        } else if ( event.key.keysym.sym == SDLK_DOWN ) {
                            moveTreeY = 0.0;
                        } else if ( event.key.keysym.sym == SDLK_LEFT ) {
                            moveTreeX = 0.0;
                        } else if ( event.key.keysym.sym == SDLK_RIGHT ) {
                            moveTreeX = 0.0;
                        }
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    if ( event.button.button == SDL_BUTTON_LEFT ) {
                        if ( !DialogSelectorBox::getDialog() ) {
                            switch ( clickMode ) {
                                case CLICKMODE_LINKING_INPUT:
                                case CLICKMODE_LINKING: {
                                    linkStartNode = tree -> findNearestNode( mouseX, mouseY, ChainingNode::BLOCK_WIDTH );
                                    break;
                                }
                                case CLICKMODE_NODE_REMOVE:
                                case CLICKMODE_SELECT: {
                                    ChainingNode* lookOn = selectTargetNode = tree -> findNearestNode( mouseX, mouseY, ChainingNode::BLOCK_WIDTH );
                                    if ( selectedNode == lookOn ) {
                                        if ( clickMode != CLICKMODE_NODE_REMOVE ) {
                                            moveSelectedNode = true;
                                        }
                                    } else {
                                        selectTargetNode = lookOn;
                                    }
                                    break;
                                }
                                case CLICKMODE_NODE_MOVE: {
                                    selectedNode = tree -> findNearestNode( mouseX, mouseY, ChainingNode::BLOCK_WIDTH );
                                    makeParametersPanel( selectedNode, detailPanel );
                                    makeIOCTLPanel( tree, selectedNode, ioCtlPanel );
                                    moveSelectedNode = true;
                                    break;
                                }
                            }
                        }
                    }
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                    if ( event.button.button == SDL_BUTTON_RIGHT ) {
                        if ( !DialogSelectorBox::getDialog() ) {
                            clickMode = CLICKMODE_SELECT;
                            linkStartNode = NULL;
                            moveSelectedNode = false;
                            selectedNode = NULL;
                            makeParametersPanel( NULL, detailPanel );
                            makeIOCTLPanel( NULL, NULL, ioCtlPanel );
                        }
                    } else if ( event.button.button == SDL_BUTTON_LEFT ) {
                        if ( focusedEdit ) {
                            focusedEdit -> finishEditing();
                            focusedEdit = NULL;
                        }
                        DialogSelectorBox* currentDialog = DialogSelectorBox::getDialog();
                        if ( currentDialog ) {
                            currentDialog -> click( mouseX, mouseY );
                        } else if ( mainUI -> click( mouseX, mouseY ) ) {
                            //printf( "Clicked on mainUI\n" );
                        } else {
                            switch ( clickMode ) {
                                case CLICKMODE_LINKING_INPUT:
                                case CLICKMODE_LINKING: {
                                    if ( linkStartNode ) {
                                        ChainingNode* linkEndNode = tree -> findNearestNode( mouseX, mouseY, ChainingNode::BLOCK_WIDTH );
                                        if ( linkEndNode ) {
                                            int idStart = tree -> idOf( linkStartNode );
                                            int idEnd = tree -> idOf( linkEndNode );
                                            if ( idStart != idEnd ) {
                                                if ( clickMode == CLICKMODE_LINKING_INPUT ) {
                                                    linkEndNode -> addParameterLinkingNode( linkStartNode );
                                                    tree -> refreshChain();
                                                } else if ( clickMode == CLICKMODE_LINKING ) {
                                                    tree -> link( idStart, idEnd );
                                                }
                                            }
                                        }
                                    }
                                    linkStartNode = NULL;
                                    break;
                                }
                                case CLICKMODE_ADD: {
                                    if ( player -> lockTree() ) {
                                        ChainingNode* component = ChainingNodeFactory::createSelectedItem();
                                        if ( component ) {
                                            component -> setPosition( mouseX, mouseY );
                                            tree -> addComponent( component );
                                        }
                                        player -> releaseTree();
                                    }
                                    break;
                                }
                                case CLICKMODE_SELECT: {
                                    if ( moveSelectedNode ) {
                                        moveSelectedNode = false;
                                    } else {
                                        ChainingNode* currentTargetNode = tree -> findNearestNode( mouseX, mouseY, ChainingNode::BLOCK_WIDTH );
                                        if ( ( selectTargetNode == currentTargetNode ) && ( currentTargetNode != NULL ) ) {
                                            /// mainUI -> changeTab( 2 );
                                            selectedNode = currentTargetNode;
                                            makeParametersPanel( selectedNode, detailPanel );
                                            makeIOCTLPanel( tree, selectedNode, ioCtlPanel );
                                        }
                                    }
                                    selectTargetNode = NULL;
                                    break;
                                }
                                case CLICKMODE_LINK_REMOVE: {
                                    if ( player -> lockTree() ) {
                                        if ( tree -> removeLink( selectedLink ) ) {
                                            delete selectedLink;
                                            selectedLink = NULL;
                                        } else if ( tree -> removeInputLink( selectedInputLink ) ) {
                                            delete selectedInputLink;
                                            selectedInputLink = NULL;
                                        }
                                        player -> releaseTree();
                                    }
                                    break;
                                }
                                case CLICKMODE_NODE_REMOVE: {
                                    if ( player -> lockTree() ) {
                                        moveSelectedNode = false;
                                        ChainingNode* currentTargetNode = tree -> findNearestNode( mouseX, mouseY, ChainingNode::BLOCK_WIDTH );
                                        if ( ( selectedNode == currentTargetNode ) && ( selectTargetNode != NULL ) && ( selectTargetNode == currentTargetNode ) ) {
                                            if ( tree -> removeNode( selectedNode ) ) {
                                                selectedNode = NULL;
                                                makeParametersPanel( NULL, detailPanel );
                                                makeIOCTLPanel( NULL, NULL, ioCtlPanel );
                                                delete selectedNode;
                                            }
                                        } else if ( ( selectTargetNode == currentTargetNode ) && ( currentTargetNode != NULL ) ) {
                                            selectedNode = currentTargetNode;
                                            makeParametersPanel( selectedNode, detailPanel );
                                            makeIOCTLPanel( tree, selectedNode, ioCtlPanel );
                                        }
                                        player -> releaseTree();
                                    }
                                    break;
                                }
                                case CLICKMODE_NODE_MOVE: {
                                    selectedNode = NULL;
                                    makeParametersPanel( NULL, detailPanel );
                                    makeIOCTLPanel( NULL, NULL, ioCtlPanel );
                                    moveSelectedNode = false;
                                    break;
                                }
                            }
                        }
                    }
                    break;
                }
                case SDL_MOUSEMOTION: {
                    mouseX = event.motion.x;
                    mouseY = event.motion.y;
                    break;
                }
            }
        }

        if ( tree != prevTree ) {
            selectTargetNode = NULL;
            selectedNode = NULL;
            linkStartNode = NULL;
            selectedLink = NULL;
            selectedInputLink = NULL;
            makeParametersPanel( NULL, detailPanel );
            makeIOCTLPanel( NULL, NULL, ioCtlPanel );
        }

        tree -> moveTree( moveTreeX, moveTreeY );
        if ( ( moveSelectedNode ) && ( selectedNode != NULL ) ) {
            selectedNode -> setPosition( mouseX, mouseY );
        }
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );
        tree -> render( renderer );
        if ( selectedNode ) {
            SDL_SetRenderDrawColor( renderer, 128, 32, 32, 255 );
            SDL_Rect rect = increaseRect( selectedNode -> getBlockRect(), 8 );
            SDL_RenderDrawRect( renderer, &rect );
        }

        if ( selectedLink ) {
            delete selectedLink;
        }
        if ( selectedInputLink ) {
            delete selectedInputLink;
        }
        selectedLink = tree -> nearestLink( mouseX, mouseY, 16.0 );
        selectedInputLink = tree -> nearestInputLink( mouseX, mouseY, 16.0 );
        if ( selectedLink ) {
            selectedLink -> draw( renderer, 250, 250, 250 );
            renderLinkInputId( renderer, selectedLink );
        } else if ( selectedInputLink ) {
            selectedInputLink -> draw( renderer, 250, 250, 250 );
        }

        if ( helpEnabled ) {
            Button* pointedButton = NULL;
            DialogSelectorBox* currentDialog = DialogSelectorBox::getDialog();
            bool insideGUI = mainUI -> isInside( mouseX, mouseY );
            if ( currentDialog ) {
                pointedButton = currentDialog -> getPointedButton( mouseX, mouseY );
                if ( currentDialog -> isInside( mouseX, mouseY ) ) {
                    insideGUI = true;
                }
            } else {
                pointedButton = mainUI -> getPointedButton( mouseX, mouseY );
            }
            if ( pointedButton ) {
                renderHelp( renderer, pointedButton -> getName() );
            }
            if ( !insideGUI ) {
                ChainingNode* pointedNode = tree -> findNearestNode( mouseX, mouseY, ChainingNode::BLOCK_WIDTH );
                if ( pointedNode ) {
                    renderHelp( renderer, pointedNode -> getClassName() );
                }
            }
        }

        switch ( clickMode ) {
            case CLICKMODE_LINKING_INPUT: {
                if ( linkStartNode ) {
                    SDL_SetRenderDrawColor( renderer, 200, 0, 200, 255 );
                    ChainingNode::renderArrow( renderer, mouseX, mouseY, linkStartNode -> getPositionX(), linkStartNode -> getPositionY() );
                }
                break;
            }
            case CLICKMODE_LINKING: {
                if ( linkStartNode ) {
                    SDL_SetRenderDrawColor( renderer, 250, 250, 0, 255 );
                    ChainingNode::renderArrow( renderer, mouseX, mouseY, linkStartNode -> getPositionX(), linkStartNode -> getPositionY() );
                }
                break;
            }
            case CLICKMODE_ADD: {
                SDL_Texture* placementTexture = Textures::getTexture( ChainingNodeFactory::getUiItemTexture( ChainingNodeFactory::selectedItem() ) );
                if ( placementTexture ) {
                    SDL_SetTextureAlphaMod( placementTexture, 64 );
                    SDL_Rect rect;
                    rect.x = mouseX - ( ChainingNode::BLOCK_WIDTH >> 1 );
                    rect.y = mouseY - ( ChainingNode::BLOCK_HEIGHT >> 1 );
                    rect.w = ChainingNode::BLOCK_WIDTH;
                    rect.h = ChainingNode::BLOCK_HEIGHT;
                    SDL_RenderCopy( renderer, placementTexture, NULL, &rect );
                    SDL_SetRenderDrawColor( renderer, 128, 32, 32, 255 );
                    rect.x -= 4;
                    rect.y -= 4;
                    rect.w += 8;
                    rect.h += 8;
                    SDL_SetTextureAlphaMod( placementTexture, 255 );
                    SDL_RenderDrawRect( renderer, &rect );
                }
                break;
            }
        }
        mainUI -> render( renderer );
        if ( player -> isRecording() ) {
            SDL_Texture* texture = Textures::getTexture( TEXTURE_BUTTON_RECORDING_NOW );
            SDL_Rect rect;
            rect.w = 128;
            rect.h = 128;
            rect.x = WINDOW_WIDTH - rect.w;
            rect.y = 0;
            SDL_RenderCopy( renderer, texture, NULL, &rect );
        }

        VisualisationComponent* activeVC = VisualisationComponent::getActiveVC();
        if ( activeVC ) {
            activeVC -> render( renderer );
        }

        if ( !DialogSelectorBox::getDialog() ) {
            if ( lastBox != DialogSelectorBox::getDialog() ) {
                if ( selectedNode ) {
                    makeParametersPanel( selectedNode, detailPanel );
                    makeIOCTLPanel( tree, selectedNode, ioCtlPanel );
                }
            }
        }
        lastBox = DialogSelectorBox::getDialog();

        DialogSelectorBox::renderAll( renderer );
        SDL_RenderPresent( renderer );
    }
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    return 0;
}


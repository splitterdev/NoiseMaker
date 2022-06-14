#pragma once

class ImportTemplateSelector : public TabbedDataSelectorBox {
    private:

    class _ExportedTemplateLoaderButton : public StaticText {
        public:
        _ExportedTemplateLoaderButton( ImportTemplateSelector* box, string fileName ) : StaticText( fileName ) {
            _selectorBox = box;
            _fileName = fileName;
        }
        virtual ~_ExportedTemplateLoaderButton() {
        }

        virtual void onClick() {
            _selectorBox -> importData( _fileName );
            DialogSelectorBox::removeDialog();
        }

        private:
        ImportTemplateSelector* _selectorBox;
        string _fileName;

    };

    public:
    ImportTemplateSelector( ChainingTree** tree, ChainPlayer* player ) : TabbedDataSelectorBox( 8, 10, true ) {
        _tree = tree;
        _player = player;
    }
    virtual ~ImportTemplateSelector() {
    }

    virtual void buildComponents() {
        vector < Button* > inputNodeList;
        list < string > files = findFiles( "data/userTemplates", false );
        for ( list < string >::iterator i = files.begin(); i != files.end(); i++ ) {
            inputNodeList.push_back( new _ExportedTemplateLoaderButton( this, ( *i ) ) );
        }
        buildPanels( inputNodeList, 8, 8 );
        TabbedDataSelectorBox::buildComponents();
    }

    virtual void onReturn() {
    }

    void importData( string fileName ) {
        string filePath = "data/userTemplates/" + fileName;
        AlgorithmTemplate* temp = new AlgorithmTemplate( filePath );
        ChainingTree* tree = temp -> read();
        if ( tree ) {
            ( *_tree ) = tree;
            _player -> changeTree( tree );
        }
        delete temp;
    }

    list < string > findFiles( const char* searchPath, bool recursiveSearch ) {
        WIN32_FIND_DATA data;
        list < string > files;
        string searchExpression = string( searchPath ) + "/*.*";
        HANDLE hFind = FindFirstFile( searchExpression.c_str(), &data );
        if ( hFind != INVALID_HANDLE_VALUE) {
            do {
                const char* name = data.cFileName;
                if ( strcmp( name, "." ) != 0 && strcmp( name, ".." ) != 0 ) {
                    if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
                        if ( recursiveSearch ) {
                            string recursivePath = string( searchPath ) + "/" + string( name );
                            list < string > subDirectoryFiles = findFiles( recursivePath.c_str(), recursiveSearch );
                            for ( list < string >::iterator i = subDirectoryFiles.begin(); i != subDirectoryFiles.end(); i++ ) {
                                files.push_back( *i );
                            }
                        }
                    } else {
                        files.push_back( name );
                    }
                }
            } while ( FindNextFile( hFind, &data ) != 0 );
            FindClose( hFind );
        }
        return files;
    }

    private:

    ChainingTree** _tree;
    ChainPlayer* _player;

};



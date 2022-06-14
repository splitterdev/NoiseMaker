#pragma once

class ExportTemplateButton : public Button {
    public:
    ExportTemplateButton( ChainingTree** treeToExport, Edit* pathEdit ) : Button() {
        _treeToExport = treeToExport;
        _pathEdit = pathEdit;
        setTexture( TEXTURE_BUTTON_EXPORT_TEMPLATE );
        setName( "Export" );
    }
    virtual ~ExportTemplateButton() {
    }

    virtual void onClick() {
        if ( _pathEdit ) {
            string fileName = _pathEdit -> getText();
            bool result = false;
            bool skip = false;
            if ( fileName.size() > 0 ) {
                if ( fileName.find( '.' ) == string::npos ) {
                    fileName += ".dat";
                }
                string exportPath = "data/userTemplates/" + fileName;
                AlgorithmTemplate* temp = new AlgorithmTemplate( exportPath );
                if ( temp -> fileExists() ) {
                    MessageDialogSelectorBox::setDialog( "File exists already." );
                    skip = true;
                } else {
                    if ( temp -> write( *_treeToExport ) ) {
                        result = true;
                    }
                }
                delete temp;
            }
            if ( !skip ) {
                if ( result ) {
                    MessageDialogSelectorBox::setDialog( "Exported." );
                } else {
                    MessageDialogSelectorBox::setDialog( "Can't export!" );
                }
            }
        }
    }

    private:

    ChainingTree** _treeToExport;
    Edit* _pathEdit;

};



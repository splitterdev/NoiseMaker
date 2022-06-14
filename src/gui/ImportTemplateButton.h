#pragma once

class ImportTemplateButton : public DialogLauncher {
    public:
    ImportTemplateButton( ChainingTree** rootTree, ChainPlayer* player ) : DialogLauncher( 1, TEXTURE_BUTTON_IMPORT_TEMPLATE, "Import" ) {
        _rootTree = rootTree;
        _player = player;
    }
    virtual ~ImportTemplateButton() {
    }

    virtual void onClick() {
        DialogLauncher::onClick();
        ImportTemplateSelector* selector = new ImportTemplateSelector( _rootTree, _player );
        DialogSelectorBox::setDialog( selector );
    }

    private:

    ChainingTree** _rootTree;
    ChainPlayer* _player;

};




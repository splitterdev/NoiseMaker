#pragma once

class ParamDialogLauncher : public DialogLauncher {
    public:
    ParamDialogLauncher( string paramName, ChainingNode* node, ChainingTree* tree, int textureId ) : DialogLauncher( 0, textureId, "Select parameter" ) {
        _node = node;
        _paramName = paramName;
        _tree = tree;
    }
    virtual ~ParamDialogLauncher() {
    }

    virtual void onClick() {
        DialogLauncher::onClick();
        InputSelectDialogSelectorBox* selector = new InputSelectDialogSelectorBox( _paramName, _node, _tree );
        DialogSelectorBox::setDialog( selector );
    }

    private:

    string _paramName;
    ChainingNode* _node;
    ChainingTree* _tree;

};



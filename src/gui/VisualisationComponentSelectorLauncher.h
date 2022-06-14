#pragma once

class VisualisationComponentSelectorLauncher : public DialogLauncher {
    public:
    VisualisationComponentSelectorLauncher( ChainingTree** tree ) : DialogLauncher( 2, TEXTURE_VC_SPECTRE, "Select visualization" ) {
        _tree = tree;
    }
    virtual ~VisualisationComponentSelectorLauncher() {
    }

    virtual void onClick() {
        DialogLauncher::onClick();
        VisualisationComponentSelector* selector = new VisualisationComponentSelector( 5, 8, _tree );
        DialogSelectorBox::setDialog( selector );
    }

    private:

    ChainingTree** _tree;

};




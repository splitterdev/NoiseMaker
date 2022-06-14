#pragma once

class ChangeTabButton : public Button {
    public:
    ChangeTabButton( TabPanel* tabPanel, int tabId, int icon, string tabName ) : Button() {
        setTexture( icon );
        _tabId = tabId;
        _tabPanel = tabPanel;
        setName( tabName );
    }
    virtual ~ChangeTabButton() {
    }

    virtual void onClick() {
        _tabPanel -> changeTab( _tabId );
    }

    private:

    int _tabId;
    TabPanel* _tabPanel;

};




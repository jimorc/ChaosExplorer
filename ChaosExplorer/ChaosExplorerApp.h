#pragma once
class ChaosExplorerApp :
    public wxApp
{
public:
    ChaosExplorerApp();
    virtual ~ChaosExplorerApp();
    virtual bool OnInit() override;
};


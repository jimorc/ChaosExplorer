#pragma once
class ChaosExplorerApp :
    public wxApp
{
public:
    ChaosExplorerApp();
    ChaosExplorerApp(const ChaosExplorerApp&) = delete;
    ChaosExplorerApp(ChaosExplorerApp&&) = delete;
    virtual ~ChaosExplorerApp();
    virtual bool OnInit() override;
    ChaosExplorerApp& operator=(const ChaosExplorerApp&) = delete;
    ChaosExplorerApp& operator=(ChaosExplorerApp&&) = delete;
};


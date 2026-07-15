// Injects a "Report Player" button into the in-game (escape) menu when the
// escapeMenu reporting element is enabled and reporting is available.
modded class InGameMenu {
    protected ButtonWidget gl_reportButton;

    override Widget Init() {
        Widget root = super.Init();
        this.GL_TryAddReportButton();
        return root;
    }

    protected void GL_TryAddReportButton() {
        if(this.gl_reportButton) return;
        if(!layoutRoot) return;

        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if(!mission || !mission.GetGLReportClient()) return;
        if(!mission.GetGLReportClient().IsEscapeMenuEnabled()) return;

        Widget buttonRoot = GetGame().GetWorkspace().CreateWidgets("GameLabs/GUI/Assets/gl_report_menu_button.layout", layoutRoot);
        if(!buttonRoot) return;

        this.gl_reportButton = ButtonWidget.Cast(buttonRoot.FindAnyWidget("bReport"));
    }

    override bool OnClick(Widget w, int x, int y, int button) {
        if(this.gl_reportButton && w == this.gl_reportButton) {
            MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
            GetGame().GetUIManager().Close(this);
            if(mission && mission.GetGLReportClient()) {
                mission.GetGLReportClient().OpenReportDialog("escapeMenu");
            }
            return true;
        }
        return super.OnClick(w, x, y, button);
    }
};

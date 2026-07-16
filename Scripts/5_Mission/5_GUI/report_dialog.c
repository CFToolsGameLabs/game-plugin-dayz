// Report dialog shown from the death screen, the escape menu or the hotkey.
// In "deathScreen" mode the target is fixed to the killer (pre-filled from death
// context). In "escapeMenu" mode the target is chosen from an online-player dropdown.
class GameLabsReportDialog : ScriptedWidgetEventHandler {
    protected Widget m_Root;
    protected TextWidget m_Caption;
    protected TextWidget m_TargetLabel;
    protected XComboBoxWidget m_TargetCombo;
    protected MultilineEditBoxWidget m_Message;
    protected ButtonWidget m_bSubmit;
    protected ButtonWidget m_bCancel;

    protected GameLabsReportClient m_Client;
    protected string m_Element;
    protected string m_FixedTargetPseudo;
    protected ref array<ref GameLabsReportTarget> m_Targets;

    void OnWidgetScriptInit(Widget w) {
        m_Root = w;
        m_Caption = TextWidget.Cast(w.FindAnyWidget("Caption"));
        m_TargetLabel = TextWidget.Cast(w.FindAnyWidget("TargetLabel"));
        m_TargetCombo = XComboBoxWidget.Cast(w.FindAnyWidget("TargetCombo"));
        m_Message = MultilineEditBoxWidget.Cast(w.FindAnyWidget("MessageInput"));
        m_bSubmit = ButtonWidget.Cast(w.FindAnyWidget("bSubmit"));
        m_bCancel = ButtonWidget.Cast(w.FindAnyWidget("bCancel"));

        m_Root.SetHandler(this);
        m_Root.SetSort(1023, true);
        GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.DoUpdate);
    }

    static GameLabsReportDialog CreateReportDialog(GameLabsReportClient client, string element, GameLabsReportDeathContext deathContext) {
        Widget root = GetGame().GetWorkspace().CreateWidgets("GameLabs/GUI/Assets/gl_report_dialog.layout");
        if(!root) return NULL;

        GameLabsReportDialog instance;
        root.GetScript(instance);
        if(instance) instance.Configure(client, element, deathContext);
        return instance;
    }

    void Configure(GameLabsReportClient client, string element, GameLabsReportDeathContext deathContext) {
        m_Client = client;
        m_Element = element;
        m_Targets = new array<ref GameLabsReportTarget>;

        if(element == "deathScreen") {
            if(m_Caption) m_Caption.SetText("Report your killer");
            if(deathContext && deathContext.hasPlayerKiller) {
                if(m_TargetLabel) m_TargetLabel.SetText(string.Format("Target: %1", deathContext.killerName));
                m_FixedTargetPseudo = deathContext.killerPseudoId;
            } else {
                if(m_TargetLabel) m_TargetLabel.SetText("Target: none (no player killer)");
                m_FixedTargetPseudo = "";
            }
            if(m_TargetLabel) m_TargetLabel.Show(true);
            if(m_TargetCombo) m_TargetCombo.Show(false);
        } else {
            if(m_Caption) m_Caption.SetText("Report a player");
            if(m_TargetLabel) m_TargetLabel.Show(false);
            if(m_TargetCombo) {
                m_TargetCombo.Show(true);
                m_TargetCombo.ClearAll();
            }
            // Request the current online player list from the server.
            GetGame().RPCSingleParam(NULL, GameLabsRPCS.RQ_REPORTPLAYERS, NULL, true);
        }
    }

    void PopulateTargets(array<ref GameLabsReportTarget> targets) {
        m_Targets = new array<ref GameLabsReportTarget>;
        if(!m_TargetCombo) return;

        m_TargetCombo.ClearAll();
        for(int i = 0; i < targets.Count(); i++) {
            m_Targets.Insert(targets.Get(i));
            m_TargetCombo.AddItem(targets.Get(i).name);
        }
        if(m_Targets.Count() > 0) m_TargetCombo.SetCurrentItem(0);
    }

    void DoUpdate(float tDelta) {
        GetGame().GetUIManager().ShowUICursor(true);
        GetUApi().ActivateExclude("menu");
        GetUApi().UpdateControls();
    }

    void Submit() {
        string message = "";
        if(m_Message) m_Message.GetText(message);
        if(message == "") {
            NotificationSystem.AddNotificationExtended(4.0, "Report", "Please enter a message before submitting.");
            return;
        }

        string targetPseudo = "";
        if(m_Element == "deathScreen") {
            targetPseudo = m_FixedTargetPseudo;
        } else if(m_TargetCombo && m_Targets) {
            int idx = m_TargetCombo.GetCurrentItem();
            if(idx >= 0 && idx < m_Targets.Count()) targetPseudo = m_Targets.Get(idx).pseudoId;
        }

        GameLabsReportSubmission submission = new GameLabsReportSubmission();
        submission.element = m_Element;
        submission.message = message;
        submission.targetPseudoId = targetPseudo;

        Param1<ref GameLabsReportSubmission> payload = new Param1<ref GameLabsReportSubmission>(submission);
        GetGame().RPCSingleParam(NULL, GameLabsRPCS.RQ_SUBMITREPORT, payload, true);

        NotificationSystem.AddNotificationExtended(4.0, "Report", "Your report has been submitted. Thank you.");
        this.Close();
    }

    void Close() {
        GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(this.DoUpdate);
        if(m_Client) m_Client.OnDialogClosed();
        if(m_Root) m_Root.Unlink();
        GetGame().GetUIManager().ShowUICursor(false);
        GetUApi().ActivateGroup("menu");
        GetUApi().UpdateControls();
    }

    override bool OnClick(Widget w, int x, int y, int button) {
        if(w == m_bSubmit) { this.Submit(); return true; }
        if(w == m_bCancel) { this.Close(); return true; }
        return false;
    }

    void ~GameLabsReportDialog() {
        GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(this.DoUpdate);
    }
};

// Small overlay button rendered on the death screen when the deathScreen element
// is enabled. Clicking it opens the report dialog pre-filled with the killer.
class GameLabsReportDeathOverlay : ScriptedWidgetEventHandler {
    protected Widget m_Root;
    protected ButtonWidget m_bReport;
    protected GameLabsReportClient m_Client;

    void OnWidgetScriptInit(Widget w) {
        m_Root = w;
        m_bReport = ButtonWidget.Cast(w.FindAnyWidget("bReport"));
        m_Root.SetHandler(this);
        m_Root.SetSort(1000, true);
    }

    static GameLabsReportDeathOverlay Create(GameLabsReportClient client) {
        Widget root = GetGame().GetWorkspace().CreateWidgets("GameLabs/GUI/Assets/gl_report_deathscreen.layout");
        if(!root) return NULL;

        GameLabsReportDeathOverlay instance;
        root.GetScript(instance);
        if(instance) instance.m_Client = client;
        return instance;
    }

    Widget GetRoot() { return m_Root; }

    override bool OnClick(Widget w, int x, int y, int button) {
        if(w == m_bReport) {
            if(m_Client) m_Client.OpenReportDialog("deathScreen");
            return true;
        }
        return false;
    }
};

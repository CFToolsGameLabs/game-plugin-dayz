modded class DayZGame extends CGame {
    private ref GameLabsDialog dialogEULA;
    private ref GameLabsDialog dialogGDPR;

    private bool _passthroughConnect = false;
    private bool _isLaunchConnect = false;

    #ifdef GAMELABSCLIENTCONSENT
    override void ConnectLaunch() {
        this._isLaunchConnect = true;
        if(this._passthroughConnect) {
            Print("[GameLabs] Allowing client to pass through license check");
            super.ConnectLaunch();
            return;
        }

        if(!FileExist("$profile:.gamelabslicenseaccept.donottouch")) {
            this.dialogEULA = GameLabsDialog.CreateDialog("GameLabs license agreement", "You have loaded the GameLabs mod provided by CFTools. The GameLabs mod will transmit certain statistics to CFTools and allows mod developers to exchange data with the CFTools backend, therefore some of your personal data will be transmitted to CFTools. This requires your approval. For technical reasons, denying will prevent you from playing on this server.", "OnCallBackEULA", this);
            return;
        } else {
            super.ConnectLaunch();
            return
        }
    }

    override void TryConnect() {
        if(this._passthroughConnect || this._isLaunchConnect) {
            Print("[GameLabs] Allowing client to pass through license check");
            super.TryConnect();
            return;
        }

        if(!FileExist("$profile:.gamelabslicenseaccept.donottouch")) {
            this.dialogEULA = GameLabsDialog.CreateDialog("GameLabs license agreement", "You have loaded the GameLabs mod. The GameLabs mod will transmit certain statistics to CFTools and allows mod developers to exchange data with the CFTools backend, therefore some of your personal data will be transmitted to CFTools. This requires your approval. For technical reasons, denying will prevent you from playing on this server.", "OnCallBackEULA", this);
            return;
        } else {
            super.TryConnect();
            return
        }
    }
    #endif

    void OnCallBackEULA(int option) {
        switch(option) {
            case GL_DIAGRESULT.AGREE: {
                NotificationSystem.AddNotificationExtended(3.0, "GameLabs license agreement", "You have accepted the license agreement", "set:ccgui_enforce image:Icon40Emergency");
                //this.dialogGDPR = GameLabsDialog.CreateDialog("Privacy disclaimer", "You are about to connect to a remote game server. Due to the nature of online play, some personal information will be transmitted to the game server operator.", "OnCallBackGDPR", this);
                this.OnCallBackGDPR(GL_DIAGRESULT.AGREE);
                delete this.dialogEULA;
                break;
            }

            case GL_DIAGRESULT.DISAGREE: {
                NotificationSystem.AddNotificationExtended(3.0, "GameLabs license agreement", "You have denied the license agreement, the game will now exit", "set:ccgui_enforce image:Icon40Emergency");
                GetGame().RequestExit(1);
                break;
            }

            case GL_DIAGRESULT.LICENSE: {
                GetGame().OpenURL("https://cftools.cloud/legal/eula");
                break;
            }
        }
    }

    void OnCallBackGDPR(int option) {
        switch(option) {
            case GL_DIAGRESULT.AGREE: {
                NotificationSystem.AddNotificationExtended(3.0, "Privacy disclaimer", "You have acknowledged the disclaimer, you will now connect to the game server", "set:ccgui_enforce image:Icon40Emergency");
                FileHandle fh = OpenFile("$profile:.gamelabslicenseaccept.donottouch", FileMode.WRITE);
                FPrintln(fh, string.Format("[%1] EULA and GDPR disclaimer have been accepted", this.GetISO8601DT()));
                CloseFile(fh);
                this._passthroughConnect = true;
                if(this._isLaunchConnect) {
                    this.ConnectLaunch();
                } else {
                    this.TryConnect();
                }
                if(this.dialogGDPR) delete this.dialogGDPR;
                break;
            }

            case GL_DIAGRESULT.DISAGREE: {
                NotificationSystem.AddNotificationExtended(3.0, "Privacy disclaimer", "Disclaimer not acknowleged, the game will now exit", "set:ccgui_enforce image:Icon40Emergency");
                GetGame().RequestExit(1);
                break;
            }

            case GL_DIAGRESULT.LICENSE: {
                GetGame().OpenURL("https://gdpr.eu/what-is-gdpr/");
                break;
            }
        }
    }

    private string GetISO8601DT() {
        int s, mi, h, d, mo, y;

        GetHourMinuteSecondUTC(h, mi, s);
        GetYearMonthDayUTC(y, mo, d);

        return string.Format(
                "%1-%2-%3T%4:%5:%6+00:00",
                y.ToStringLen(2),
                mo.ToStringLen(2),
                d.ToStringLen(2),
                h.ToStringLen(2),
                mi.ToStringLen(2),
                s.ToStringLen(2),
        );
    }
};
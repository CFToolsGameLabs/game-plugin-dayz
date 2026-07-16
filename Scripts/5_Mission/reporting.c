// Sends the current reporting configuration (availability + enabled elements) to a client.
void GLSendReportConfig(PlayerIdentity identity) {
    if(!identity) return;

    GameLabsConfiguration cfg = GetGameLabs().GetConfiguration();

    GameLabsReportConfig reportConfig = new GameLabsReportConfig();
    reportConfig.available = cfg.IsReportingAvailable();
    reportConfig.deathScreen = cfg.IsReportingElementEnabled("deathScreen");
    reportConfig.escapeMenu = cfg.IsReportingElementEnabled("escapeMenu");

    Param1<ref GameLabsReportConfig> payload = new Param1<ref GameLabsReportConfig>(reportConfig);
    GetGame().RPCSingleParam(null, GameLabsRPCS.RE_REPORTCONFIG, payload, true, identity);
}

// Server-side handler for in-game player reports.
// Builds the online player list (with pseudo ids), resolves report targets and
// forwards submitted reports to the configured webhook (Discord embed or internal JSON).
class GLReportManager {
    private const int MESSAGE_MAX_LENGTH = 500;

    // Builds the list of online players a requester may report, assigning a fresh
    // pseudo id to each. Only name + pseudo id are exposed to the client.
    // The requester is only included (enabling self-reports) when the server is
    // launched with -gamelabstesting, so testers can report while alone.
    array<ref GameLabsReportTarget> BuildOnlinePlayerList(PlayerBase requester) {
        array<ref GameLabsReportTarget> result = new array<ref GameLabsReportTarget>;

        string tmp;
        bool allowSelf = GetGame().CommandlineGetParam("gamelabstesting", tmp);

        array<Man> players = new array<Man>;
        GetGame().GetWorld().GetPlayerList(players);

        for(int i = 0; i < players.Count(); i++) {
            PlayerBase player = PlayerBase.Cast(players.Get(i));
            if(!player) continue;
            if(!player.GetIdentity()) continue;

            bool isSelf = (requester && player == requester);
            if(isSelf && !allowSelf) continue;

            string steam64 = player.GetPlainId();
            if(steam64 == "") continue;

            GameLabsReportTarget target = new GameLabsReportTarget();
            target.pseudoId = GetGameLabs().RegisterReportPseudo(steam64);
            if(isSelf) {
                target.name = string.Format("%1 (You)", player.GetPlayerName());
            } else {
                target.name = player.GetPlayerName();
            }
            result.Insert(target);
        }

        return result;
    }

    // Resolves a pseudo id (from a submission) back to the target steam64, even if
    // the target is no longer online. Returns "" when the pseudo is unknown.
    string ResolveTargetSteam64(string pseudoId) {
        return GetGameLabs().ResolveReportPseudo(pseudoId);
    }

    string SanitizeMessage(string message) {
        if(message.Length() > MESSAGE_MAX_LENGTH) {
            message = message.Substring(0, MESSAGE_MAX_LENGTH);
        }
        return message;
    }

    private string _Pad2(int value) {
        if(value < 10) return string.Format("0%1", value);
        return value.ToString();
    }

    // Human-readable "x, y, z" for Discord embeds (rounded to whole units).
    private string _FormatPosition(vector position) {
        return string.Format("%1, %2, %3", Math.Round(position[0]), Math.Round(position[1]), Math.Round(position[2]));
    }

    private string _CurrentDate() {
        int year, month, day, hour, minute, second;
        GetYearMonthDayUTC(year, month, day);
        GetHourMinuteSecondUTC(hour, minute, second);
        return string.Format("%1-%2-%3T%4:%5:%6Z", year, this._Pad2(month), this._Pad2(day), this._Pad2(hour), this._Pad2(minute), this._Pad2(second));
    }

    // Forwards a report to the configured webhook. The reporter identity is taken
    // from the (server-trusted) reporter PlayerBase, never from the client payload.
    // targetSteam64 identifies who was reported (may be "" when no target was set).
    void SendWebhook(PlayerBase reporter, string element, string message, string targetSteam64) {
        if(!reporter) return;

        GameLabsConfiguration cfg = GetGameLabs().GetConfiguration();
        string url = cfg.GetReportingWebhookUrl();
        if(url == "") return;

        string reporterName = reporter.GetPlayerName();
        string steam64 = reporter.GetPlainId();
        string cftoolsId = reporter.GetUpstreamIdentity();
        string date = this._CurrentDate();
        vector reporterPosition = reporter.GetPosition();

        string targetName = "";
        string targetCftoolsId = "";
        vector targetPosition = "0 0 0";
        bool hasTargetPosition = false;
        if(targetSteam64 != "") {
            // Prefer live data from the online player; fall back to cached upstream
            // identity so the report still states who was reported if they left.
            PlayerBase target = GLGetPlayerBySteam64(targetSteam64);
            if(target) {
                targetName = target.GetPlayerName();
                targetCftoolsId = target.GetUpstreamIdentity();
                targetPosition = target.GetPosition();
                hasTargetPosition = true;
            } else {
                targetCftoolsId = GetGameLabs().GetPlayerUpstreamIdentity(targetSteam64);
            }
        }

        RestContext ctx = GetRestApi().GetRestContext(url);
        ctx.SetHeader("application/json");

        if(cfg.GetReportingWebhookUrlType() == "internal") {
            _Payload_ReportInternal payload = new _Payload_ReportInternal();
            payload.steam64 = steam64;
            payload.cftoolsId = cftoolsId;
            payload.date = date;
            payload.element = element;
            payload.message = message;
            payload.reporterName = reporterName;
            payload.position = reporterPosition;
            payload.target = targetSteam64;
            payload.targetName = targetName;
            payload.targetCftoolsId = targetCftoolsId;
            if(hasTargetPosition) payload.targetPosition = targetPosition;
            ctx.POST(new _Callback(), "", payload.ToJson());
        } else {
            _Payload_DiscordWebHook webhook = new _Payload_DiscordWebHook();
            _Payload_DiscordWebHookEmbed embed = new _Payload_DiscordWebHookEmbed();
            embed.SetTitle("New Player Report");
            embed.SetColor(3447003); // Discord blue

            string targetDisplay = targetName;
            if(targetDisplay == "" && targetSteam64 != "") targetDisplay = targetSteam64;

            if(targetDisplay != "") {
                embed.SetDescription(string.Format("%1 reported %2", reporterName, targetDisplay));
            } else {
                embed.SetDescription(string.Format("%1 submitted a report", reporterName));
            }

            embed.AddField("Reporter", reporterName, true);
            embed.AddField("Steam64", steam64, true);
            embed.AddField("CFTools Id", cftoolsId, true);
            embed.AddField("Source", element, true);
            embed.AddField("Reporter Position", this._FormatPosition(reporterPosition), true);

            if(targetSteam64 != "") {
                if(targetName != "") embed.AddField("Target", targetName, true);
                embed.AddField("Target Steam64", targetSteam64, true);
                if(targetCftoolsId != "") embed.AddField("Target CFTools Id", targetCftoolsId, true);
                if(hasTargetPosition) embed.AddField("Target Position", this._FormatPosition(targetPosition), true);
            }

            embed.AddField("Date", date, false);
            if(message != "") embed.AddField("Message", message, false);

            webhook.AddEmbed(embed);
            ctx.POST(new _Callback(), "", webhook.ToJson());
        }

        GetGameLabs().GetLogger().Info(string.Format("[Reporting] Report submitted by %1 [steam64=%2] element=%3 targetName=%4 targetSteam64=%5", reporterName, steam64, element, targetName, targetSteam64));
    }
};

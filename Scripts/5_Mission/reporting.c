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
    array<ref GameLabsReportTarget> BuildOnlinePlayerList(PlayerBase requester) {
        array<ref GameLabsReportTarget> result = new array<ref GameLabsReportTarget>;

        array<Man> players = new array<Man>;
        GetGame().GetWorld().GetPlayerList(players);

        for(int i = 0; i < players.Count(); i++) {
            PlayerBase player = PlayerBase.Cast(players.Get(i));
            if(!player) continue;
            if(requester && player == requester) continue;
            if(!player.GetIdentity()) continue;

            string steam64 = player.GetPlainId();
            if(steam64 == "") continue;

            GameLabsReportTarget target = new GameLabsReportTarget();
            target.pseudoId = GetGameLabs().RegisterReportPseudo(steam64);
            target.name = player.GetPlayerName();
            result.Insert(target);
        }

        return result;
    }

    // Resolves a pseudo id (from a submission) back to the online PlayerBase, or NULL.
    PlayerBase ResolveTarget(string pseudoId) {
        string steam64 = GetGameLabs().ResolveReportPseudo(pseudoId);
        if(steam64 == "") return NULL;
        return GLGetPlayerBySteam64(steam64);
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

    private string _CurrentDate() {
        int year, month, day, hour, minute, second;
        GetYearMonthDayUTC(year, month, day);
        GetHourMinuteSecondUTC(hour, minute, second);
        return string.Format("%1-%2-%3T%4:%5:%6Z", year, this._Pad2(month), this._Pad2(day), this._Pad2(hour), this._Pad2(minute), this._Pad2(second));
    }

    // Forwards a report to the configured webhook. The reporter identity is taken
    // from the (server-trusted) reporter PlayerBase, never from the client payload.
    void SendWebhook(PlayerBase reporter, string element, string message, PlayerBase target) {
        if(!reporter) return;

        GameLabsConfiguration cfg = GetGameLabs().GetConfiguration();
        string url = cfg.GetReportingWebhookUrl();
        if(url == "") return;

        string reporterName = reporter.GetPlayerName();
        string steam64 = reporter.GetPlainId();
        string cftoolsId = reporter.GetUpstreamIdentity();
        string date = this._CurrentDate();

        string targetName = "";
        string targetSteam64 = "";
        string targetCftoolsId = "";
        if(target) {
            targetName = target.GetPlayerName();
            targetSteam64 = target.GetPlainId();
            targetCftoolsId = target.GetUpstreamIdentity();
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
            payload.target = targetSteam64;
            payload.targetName = targetName;
            payload.targetCftoolsId = targetCftoolsId;
            ctx.POST(new _Callback(), "", payload.ToJson());
        } else {
            _Payload_DiscordWebHook webhook = new _Payload_DiscordWebHook();
            _Payload_DiscordWebHookEmbed embed = new _Payload_DiscordWebHookEmbed();
            embed.SetTitle("New Player Report");
            embed.SetColor(3447003); // Discord blue

            if(targetName != "") {
                embed.SetDescription(string.Format("%1 reported %2", reporterName, targetName));
            } else {
                embed.SetDescription(string.Format("%1 submitted a report", reporterName));
            }

            embed.AddField("Reporter", reporterName, true);
            embed.AddField("Steam64", steam64, true);
            embed.AddField("CFTools Id", cftoolsId, true);
            embed.AddField("Source", element, true);

            if(targetName != "") {
                embed.AddField("Target", targetName, true);
                if(targetSteam64 != "") embed.AddField("Target Steam64", targetSteam64, true);
                if(targetCftoolsId != "") embed.AddField("Target CFTools Id", targetCftoolsId, true);
            }

            embed.AddField("Date", date, false);
            if(message != "") embed.AddField("Message", message, false);

            webhook.AddEmbed(embed);
            ctx.POST(new _Callback(), "", webhook.ToJson());
        }

        GetGameLabs().GetLogger().Info(string.Format("[Reporting] Report submitted by %1 [steam64=%2] element=%3 target=%4", reporterName, steam64, element, targetName));
    }
};

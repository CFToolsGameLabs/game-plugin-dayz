/*
 * RQ = Request
 * RE = Response
 * RP = Report
 * SY = Synchronize (Legacy syntax for RP)
 */

enum GameLabsRPCS {
    START = 855617579,
    RQ_SERVERFPS,
    RE_SERVERFPS,
    SY_EXPANSIONCHAT,
    RQ_SYNC,
    RE_SYNC,
    RP_HITINFO, // This is currently used in debug mode only
    RQ_SERVERCFG,
    RE_SERVERCFG
};

class GameLabsClientSync {
    string cftoolsId;
    string gameSessionId;

    // Server Settings
    bool chatSanitizeBattlEyeJoinLeave;
    bool chatSanitizeBattlEyePrefix;
    bool chatBlockEventProcessing;
    bool advancedChatInterface;
};
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
    RE_SERVERCFG,
    RQ_INVALIDBULLET,

    // In-game reporting
    RE_REPORTCONFIG,   // Server -> Client: reporting availability + enabled elements
    RQ_REPORTPLAYERS,  // Client -> Server: request online player list (for target dropdown)
    RE_REPORTPLAYERS,  // Server -> Client: online player list (name + pseudo id)
    RE_REPORTDEATHCTX, // Server -> Client: killer context after death (name + pseudo id)
    RQ_SUBMITREPORT    // Client -> Server: submit a report (element, message, target)
};

class GameLabsClientSync {
    string cftoolsId;
    string gameSessionId;

    // Server Settings
    bool chatSanitizeBattlEyeJoinLeave;
    bool chatSanitizeBattlEyePrefix;
    bool chatBlockEventProcessing;
    bool advancedChatInterface;

    bool enableMagicBulletCheck;
    bool enableMagicBulletInvalidation;
};

// In-game reporting transport types

// Server -> Client: tells the client whether reporting is available and which elements are enabled.
class GameLabsReportConfig {
    bool available;
    bool deathScreen;
    bool escapeMenu;
};

// A single reportable target as shown to the client. Only a name and an opaque
// pseudo id are ever sent to the client; real steam ids never leave the server.
class GameLabsReportTarget {
    string pseudoId;
    string name;
};

// Client -> Server: a submitted report.
class GameLabsReportSubmission {
    string element;         // "deathScreen" | "escapeMenu"
    string message;
    string targetPseudoId;  // may be empty (e.g. environmental death, no target)
};

// Server -> Client: killer context sent on death so the death screen can pre-fill the target.
class GameLabsReportDeathContext {
    bool hasPlayerKiller;
    string killerName;
    string killerPseudoId;
};
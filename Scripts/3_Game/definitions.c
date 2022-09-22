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
};
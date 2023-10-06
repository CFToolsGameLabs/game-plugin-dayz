modded class DayZGame extends CGame {
    // TODO: Add internal state warnings based on GL data
    void GLSetConnectivityStatState(EConnectivityStatType type, EConnectivityStatLevel level) {
        this.SetConnectivityStatState(type, level);
    }
};
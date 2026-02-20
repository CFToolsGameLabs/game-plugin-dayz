modded class MissionServer {
    bool gl_featuresExpansionAI = false;

    void MissionServer() {
        Print("[GameLabs-Dependencies] PBO check within MissionServer...");

        #ifdef GAMELABS
            Print("[GameLabs-Dependencies] Base define found");
        #endif

        GameLabsCore gameLabsCore = GetGameLabs();
        if(gameLabsCore) {
            Print("[GameLabs-Dependencies] GameLabsCore Script API is accessible");
            this.GL_PopulateFeatureFlags();
        } else {
            Print("[GameLabs-Dependencies] GameLabsCore Script API is NOT accessible");
        }
    }

    void GL_PopulateFeatureFlags() {
        // Expansion AI Kill Tracking
        #ifdef EXPANSIONMODAI
        this.gl_featuresExpansionAI = true;
        #else
        this.gl_featuresExpansionAI = false;
        #endif
        GetGameLabs().GetLogger().Debug(string.Format("[GameLabs-Dependencies] FeatureFlag<gl_featuresExpansionAI> '%1'", this.gl_featuresExpansionAI));
    }
};

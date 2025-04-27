modded class DayZGame {
    int gl_tps = 0;
    int gl_ticks = 0;
    int gl_tpsTime = 0;

    override void OnUpdate(bool doSim, float timeslice) {
        super.OnUpdate(doSim, timeslice);

        if(g_Game && g_Game.IsServer()) {
            this.gl_ticks++;
            if(this.gl_tpsTime + 1 < GetGame().GetTickTime()) {
                this.gl_tpsTime = GetGame().GetTickTime();
                this.gl_tps = this.gl_ticks / 2;
                this.gl_ticks = 0;
                if(GetGameLabs()) GetGameLabs().SetServerFPS(this.gl_tps); // GameLabsCore can not directly access this value
            }
        }
    }
};
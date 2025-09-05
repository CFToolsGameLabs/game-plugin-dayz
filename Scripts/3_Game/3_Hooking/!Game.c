modded class DayZGame {
    int gl_tps = 0;
    int gl_ticks = 0;
    int gl_tpsTime = 0;
    int gl_ticksTotal = 0;
    float gl_lastTickTime = 0.0;
    float gl_tickTimeHigh = 0.0;
    float gl_tickTimeLow = 0.0;

    ref array<float> gl_tickTimes = {};
    int gl_tickTimeAverageWindow = 60;

    bool gl_missionLoaded = false;

    void GLSetAllTickTimeValues() {
        if(GetGameLabs()) {
            GetGameLabs().SetTickTimes(this.GLGetAverageTickTime(), this.gl_tickTimeLow, this.gl_tickTimeHigh);
            GetGameLabs().SetTickCount(this.gl_ticksTotal);
        }
    }

    float GLGetAverageTickTime() {
        if(this.gl_tickTimes.Count() < this.gl_tickTimeAverageWindow) return 0.0;

        float sum = 0.0;
        for(int i = 0; i < this.gl_tickTimes.Count(); i++ ) {
            sum += this.gl_tickTimes.Get(i);
        }
        return (sum / this.gl_tickTimes.Count());
    }

    void GLSetMissionLoaded() {
        this.gl_missionLoaded = true;
    }

    override void OnUpdate(bool doSim, float timeslice) {
        super.OnUpdate(doSim, timeslice);

        if(g_Game && g_Game.IsServer()) {
            this.gl_ticksTotal++;

            float tickTime = GetGame().GetTickTime();

            if(this.gl_missionLoaded) {
                float tickTimeScaled = tickTime * 1000; // Higher precision needed

                if (this.gl_lastTickTime == 0) {
                    this.gl_lastTickTime = tickTimeScaled;
                } else {
                    float diff = tickTimeScaled - this.gl_lastTickTime;
                    this.gl_lastTickTime = tickTimeScaled;

                    if (this.gl_tickTimeLow == 0.0) {
                        this.gl_tickTimeLow = diff;
                    } else {
                        if (diff < this.gl_tickTimeLow) {
                            this.gl_tickTimeLow = diff;
                        }
                    }
                    if (this.gl_tickTimeHigh == 0.0) {
                        this.gl_tickTimeHigh = diff;
                    } else {
                        if (diff > this.gl_tickTimeHigh) {
                            this.gl_tickTimeHigh = diff;
                        }
                    }

                    this.gl_tickTimes.Insert(diff);
                    if (this.gl_tickTimes.Count() > this.gl_tickTimeAverageWindow) {
                        this.gl_tickTimes.Remove(0); // Remove element on 0
                    }
                }
            }

            this.gl_ticks++;
            if(this.gl_tpsTime + 1 < tickTime) {
                this.gl_tpsTime = tickTime;
                this.gl_tps = this.gl_ticks / 2;
                this.gl_ticks = 0;
                if(GetGameLabs()) GetGameLabs().SetServerFPS(this.gl_tps); // GameLabsCore can not directly access this value
            }
        }
    }
};
modded class TerritoryFlag extends BaseBuildingBase {
    ref _Event _registeredInstance;
    private ref Timer timerUpdateGL;

    void TerritoryFlag () {
        // This defers initialization of the GameLabs _Event as it probably breaks execution of other modded
        // constructors because it uses a hard ref to the instance
        if(GetGameLabs()) {
            if(GetGameLabs().IsServer()) {
                GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this._InitGameLabs);
                this.timerUpdateGL = new Timer(CALL_CATEGORY_SYSTEM);
                // 3601 to ensure the timer doesnt accidentally catch the same hour due to rounding
                this.timerUpdateGL.Run(3601, this, "_updateEvent", NULL, true);
            }
        }
	}

    private void _InitGameLabs() {
        if(GetGameLabs()) {
            if(GetGameLabs().IsServer()) {
                this._registeredInstance = new _Event(this.GetType(), "pennant", this, GL_GetDisplayName());
                GetGameLabs().RegisterEvent(this._registeredInstance);
            }
        }
    }

    void ~TerritoryFlag () {
        if(GetGameLabs()) {
            if(GetGameLabs().IsServer()) {
                if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
                if(this.timerUpdateGL) this.timerUpdateGL.Stop();
            }
        }
    }

    private string GL_GetDisplayName() {
        float remainingLifetime = GetLifetime() / 3600;
        string steam64 = GL_GetSteam64();

        string displayName;
        if(steam64) {
            displayName = string.Format("<b>Territory Flag</b><br/>Flag Level: %1 %%<br/>Remaining Lifetime: ~ %2 hours<br/>Owner: %3", Math.Round(GetRefresherTime01() * 100), Math.Round(remainingLifetime), steam64);
        } else {
            displayName = string.Format("<b>Territory Flag</b><br/>Flag Level: %1 %%<br/>Remaining Lifetime: ~ %2 hours", Math.Round(GetRefresherTime01() * 100), Math.Round(remainingLifetime));
        }

        return displayName;
    }

    // Called each hour to update remaining lifetime
    private void _updateEvent() {
        _Event eventEntity = _GetEventInstance();
        if(eventEntity) {
            string displayName = GL_GetDisplayName();
            if(eventEntity.DisplayName() != displayName) {
                GetGameLabs().RemoveEvent(this._registeredInstance);
                this._InitGameLabs();
            }
        }
        // The else case should never happen, this is just an integrity check
    }

    // Public API
    _Event _GetEventInstance() {
        return this._registeredInstance;
    }

    void _SetEventInstance(_Event __registeredInstance) {
        this._registeredInstance = __registeredInstance;
    }
};
modded class TerritoryFlag extends BaseBuildingBase {
    ref _Event _registeredInstance;

    void TerritoryFlag () {
        // This defers initialization of the GameLabs _Event as it probably breaks execution of other modded
        // constructors because it uses a hard ref to the instance
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this._InitGameLabs);
	}

    private void _InitGameLabs() {
        float remainingLifetime = GetLifetime() / 3600;
        this._registeredInstance = new _Event("<b>Territory Flag</b><br/>Flag Level: " + Math.Round(GetRefresherTime01() * 100) + " %<br/>Remaining Lifetime: ~ " + Math.Round(remainingLifetime)) + " hours", "pennant", this);

        if(GetGameLabs()) {
            if(GetGameLabs().IsServer()) {
                GetGameLabs().RegisterEvent(this._registeredInstance);
            }
        }
    }

    void ~TerritoryFlag () {
        if(GetGameLabs()) {
            if(GetGameLabs().IsServer()) {
                if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
            }
        }
    }

    // Public API
    _Event _GetEventInstance() {
        return this._registeredInstance;
    }

    void _SetEventInstance(_Event _registeredInstance) {
        this._registeredInstance = _registeredInstance;
    }
};
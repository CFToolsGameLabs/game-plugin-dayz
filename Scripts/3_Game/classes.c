class _AI {
    private Object _reference;
    private vector _lastPos;
    private bool _infected = false;
    void _AI(Object __reference, bool infected) { this._reference = __reference; this._infected = infected; this._lastPos = _reference.GetPosition(); };
    Object Ref() { return this._reference; }
    bool IsInfected() { return this._infected; }
    bool IsActive() {
        if(!this.IsInfected()) return false;
        // This is a bit janky, but we have no other option
        vector currentPos = this._reference.GetPosition();
        if(currentPos != this._lastPos) {
            this._lastPos = currentPos;
            return true;
        } else {
            return false;
        }
    }
};

class _Vehicle {
    private string id;
    private string icon;
    private string vehicleType;
    private string className;
    private vector _lastPos;
    private Object _reference;
    void _Vehicle(Object __reference, string _icon, string _vehicleType) { this._reference = __reference; this.className = this.Ref().GetType(); this.id = EntityAI.Cast(this._reference).GetNetworkIDString(); this.icon = _icon; this.vehicleType = _vehicleType; };
    string GetID() { return this.id; }
    string Class() { return className; }
    string Icon() { return this.icon; }
    string VehicleType() { return this.vehicleType; }
    void SetIcon(string _icon) { this.icon = _icon; }
    void SetVehicleType(string _vehicleType) { this.vehicleType = _vehicleType; }
    Object Ref() { return this._reference; }
    bool HasUpdated() {
        if(this._reference == NULL) return true;
        vector currentPos = this._reference.GetPosition();
        if(currentPos != this._lastPos) {
            this._lastPos = currentPos;
            return true;
        } else {
            return false;
        }
    }
};

class _Event {
    private string id;
    private string icon;
    private string className;
    private string displayName;
    private Object _reference;

    void _Event(string _className, string _icon, Object __reference, string _displayName = "") {
        this.className = _className;
        if(_displayName) {
            this.displayName = _displayName;
        } else this.displayName = _className;
        this.icon = _icon;
        this._reference = __reference;
        this.id = EntityAI.Cast(this._reference).GetNetworkIDString();
    }

    string GetID() { return this.id; }
    string Class() { return this.className; }
    string DisplayName() { return this.displayName; }
    string Icon() { return this.icon; }
    Object Ref() { return this._reference; }
    bool Equals(_Event other) {
        if(this._reference == NULL) return false;
        return this._reference.GetPosition() == other._reference.GetPosition();
    }

    void SetDisplayName(string _displayName) { this.displayName = _displayName; }
};

class GameLabsActionContext {
    private string contextType = "world";
    private Class referencedObject;

    ref map<string, ref GameLabsActionParameter> parameters = new map<string, ref GameLabsActionParameter>;

    void GameLabsActionContext(string _contextType, Class _referencedObject, map<string, ref GameLabsActionParameter> _parameters) {
        this.contextType = _contextType;
        this.referencedObject = _referencedObject;
        this.parameters = _parameters;
    }

    string GetContextType() {
        return this.contextType;
    }

    Object GetReferencedObject() {
        Object casted = Object.Cast(this.referencedObject);
        if(casted == NULL) return this.referencedObject;
        return casted;
    }
};

class GameLabsActionParameterOption{
    string displayName;
    string value;

    void GameLabsActionParameterOption(string _displayName, string _value) {
        this.displayName = _displayName;
        this.value = _value;
    }
};

class GameLabsActionParameter {
    string displayName;
    string description;

    /*
     * dataType controls what fields are being displayed in the CFCloud interface
     * Available types:
     * int: any numeric value
     * float: any float value
     * string: any text value
     * boolean: true or false
     * vector: a X, Y, Z vector
     * cf_itemlist: A dropdown list of all available game items
     * list: a dropdown list from predefined options
     *
     * Advanced types:
     * webhook_url: Used in conjunction with a response of type WebHook, this will automatically populate the WebHook URL for your responses
     */
    string dataType;

    int valueInt;
    float valueFloat;
    string valueString;
    bool valueBoolean;

    ref map<string, ref GameLabsActionParameter> options = new map<string, ref GameLabsActionParameter>;

    float valueVectorX;
    float valueVectorY;
    float valueVectorZ;

    void GameLabsActionParameter(string _displayName, string _description, string _dataType) {
        this.displayName = _displayName;
        this.description = _description;
        this.dataType = _dataType;
    }

    int GetInt() {
        return this.valueInt;
    }

    float GetFloat() {
        return this.valueFloat;
    }

    bool GetBoolean() {
        return this.valueBoolean;
    }

    string GetString() {
        return this.valueString;
    }

    vector GetVector() {
        vector position;
        position[0] = this.valueVectorX;
        if(!this.valueVectorZ) {
            position[1] = GetGame().SurfaceY(this.valueVectorX, this.valueVectorY) + 0.2;
        } else {
            position[1] = this.valueVectorZ;
        }
        position[2] = this.valueVectorY;
        return position;
    }
};

class GameLabsContextAction {
    // A unique actionCode as string, identifying your action in the internal GameLabs system
    // Convey to a standard "{YourModPrefix}_{Action}" eg. "TestMod_SomeAction"
    string actionCode;

    // User-friendly name for your action which is being displayed in the CFCloud interface for users
    string actionName;

    // Prefix your action with an icon in the CFCloud interface, leave empty to disable
    // Available icons: https://fontawesome.com/v5/search (type is always dualtone)
    string actionIcon;

    // Colour code for your action, only override if the action you implement is destructive
    // Available codes: "success" (green), "danger" (red), "warning" (yellow)
    string actionColour = "default";

    // world, player, vehicle, object
    string actionContext = "world";

    // Filter to applicable object type (classname) if actionContext = object
    ref array<string> actionContextFilter;

    ref map<string, ref GameLabsActionParameter> parameters = new map<string, ref GameLabsActionParameter>;

    protected ref GameLabsActionResponse responseSuccess;
    protected ref GameLabsActionResponse responseFailed;

    // Internal
    string referenceKey;

    void Register() {
        GetGameLabs().AddGameLabsActionEx(this);
    }

    /*
     * bool Execute()
     * Implement any action logic in here which gets triggered based on the API orders module
     */
    bool Execute(GameLabsActionContext context) {
        return false;
    }

    string GetActionCode() {return this.actionCode;}
    string GetActionName() {return this.actionName;}
    string GetActionIcon() {return this.actionIcon;}
    string GetActionColour() {return this.actionColour;}
    string ToJson() { return JsonFileLoader<GameLabsContextAction>.JsonMakeData(this); }

    GameLabsActionResponse GetResponseSuccess() {
        return this.responseSuccess;
    }

    GameLabsActionResponse GetResponseFailed() {
        return this.responseFailed;
    }

    string InfoString() {
        return string.Format("GameLabsContextAction<%1, %2, %3, %4, %5>", this.actionCode, this.actionName, this.actionIcon, this.actionColour, this.actionContext);
    }
};

class GameLabsResponsePayload extends _Payload {
    bool status;
    GameLabsActionContext action;

    void GameLabsResponsePayload(bool _status, GameLabsActionContext _context) {
        this.status = _status;
        this.action = _context;
    }

    override string ToJson() { return JsonFileLoader<GameLabsResponsePayload>.JsonMakeData(this); }
};

class GameLabsActionResponse {
    /*
     * Response Types for Dynamic Actions
     * Supported:
     * [- output (Show a message in the CFCloud interface)] ONLY AVAILABLE FOR SPECIFIC SERVERS; DO NOT IMPLEMENT;
     * - webhook (Send a webhook to a specific URL)
     */
    string responseType = "webhook";

    private string webhookUrl;

    void GameLabsActionResponse(string _responseType, string _webhookUrl = "") {
        this.responseType = _responseType;

        if(_webhookUrl && _webhookUrl.Length()) {
            this.webhookUrl = _webhookUrl;
        }
    }

    bool Execute(bool orderStatus, GameLabsActionContext context) {
        if(this.responseType == "webhook") {
            GetGameLabs().GetLogger().Warn(string.Format("orderStatus=%1; context=%2;", orderStatus, context));
            RestContext curl = GetRestApi().GetRestContext(this.webhookUrl);
            curl.SetHeader("application/json");
            GameLabsResponsePayload payload = new GameLabsResponsePayload(orderStatus, context);
            curl.POST(new _Callback(), "", payload.ToJson());
            return true;
        }
        return false;
    }

    // Used for dynamically populating WebHook URL if none has been defined
    // Server side actions may pre-populate a WebHook URL and thus will be hidden from end-users and not require user input
    bool SetWebHookURL(string url) {
        if(this.webhookUrl || this.responseType != "webhook") return false;
        this.webhookUrl = url;
        return true;
    }

    string InfoString() {
        return string.Format("GameLabsActionResponse<%1, %2>", this.responseType, this.webhookUrl);
    }
};

/* These are currently unused */
class GLClientHitInfo {
    float tick_time;
    string object;

    void GLClientHitInfo(string _object) {
        this.object = _object;
        this.tick_time = GetGame().GetTickTime();
    }
};

class GLPlayerStatistics {
    int shotsFired          = 0;
    int shotsHit            = 0;
    int shotsHitPlayers     = 0;
    int shotsHitInfected    = 0;
    int shotsHitAnimals     = 0;
    int shotsHitVehicles    = 0;

    int killsInfected       = 0;
    int killsAnimals        = 0;

    float startingDistance  = 0.0;
    float distance          = 0.0;
    float vehicleDistance   = 0.0;

    int itemsDropped        = 0;
    int itemsPickedUp       = 0;
    int weaponsLooted       = 0;
    int playersLooted       = 0;
    int aiLooted            = 0;
    int grenadesUsed        = 0;

    int bleedsFixed         = 0;
    int healthItemsUsed     = 0;
    int playerRespawns      = 0;

    int foodItemsConsumed   = 0;
    float foodWeightConsumed = 0.0;
    int drinkItemsConsumed  = 0;
    float drinkVolumeConsumed = 0.0;
};
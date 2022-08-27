class _AI {
    private Object _reference;
    private vector _lastPos;
    private bool _infected = false;
    void _AI(Object _reference, bool infected) { this._reference = _reference; this._infected = infected; this._lastPos = _reference.GetPosition(); };
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
    private string className;
    private vector _lastPos;
    private Object _reference;
    void _Vehicle(Object _reference) { this._reference = _reference; this.className = this.Ref().GetType(); this.id = EntityAI.Cast(this._reference).GetNetworkIDString(); };
    string GetID() { return this.id; }
    string Class() { return className; }
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
    private Object _reference;
    void _Event(string className, string icon, Object _reference) {this.className = className; this.icon = icon; this._reference = _reference; this.id = EntityAI.Cast(this._reference).GetNetworkIDString(); }
    string GetID() { return this.id; }
    string Class() { return this.className; }
    string Icon() { return this.icon; }
    Object Ref() { return this._reference; }
    bool Equals(_Event other) {
        if(this._reference == NULL) return false;
        return this._reference.GetPosition() == other._reference.GetPosition();
    }
};

class GameLabsActionContext {
    private string contextType = "world";
    private Object referencedObject;

    ref map<string, ref GameLabsActionParameter> parameters = new map<string, ref GameLabsActionParameter>;

    void GameLabsActionContext(string contextType, Object referencedObject) {
        this.contextType = contextType;
        this.referencedObject = referencedObject;
    }

    string GetContextType() {
        return this.contextType;
    }

    Object GetReferencedObject() {
        return this.referencedObject;
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
     */
    string dataType;

    int valueInt;
    float valueFloat;
    string valueString;
    bool valueBoolean;

    float valueVectorX;
    float valueVectorY;
    float valueVectorZ;

    void GameLabsActionParameter(string displayName, string description, string dataType) {
        this.displayName = displayName;
        this.description = description;
        this.dataType = dataType;
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
        position[2] = this.valueVectorY
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

    // world, player, vehicle
    string actionContext = "world";

    ref map<string, ref GameLabsActionParameter> parameters = new map<string, ref GameLabsActionParameter>;

    // Internal
    string referenceKey;

    void GameLabsContextAction() {

    }

    void Register() {
        GetGameLabs().AddGameLabsAction(this);
    }

    /*
     * bool Execute()
     * Implement any action logic in here which gets triggered based on the API orders module
     */
    static bool Execute(GameLabsActionContext context) {
        return false;
    }

    string GetActionCode() {return this.actionCode;}
    string GetActionName() {return this.actionName;}
    string GetActionIcon() {return this.actionIcon;}
    string GetActionColour() {return this.actionColour;}
    string ToJson() { return JsonFileLoader<GameLabsContextAction>.JsonMakeData(this); }

    string InfoString() {
        return string.Format("GameLabsContextAction<%1, %2, %3, %4, %5>", this.actionCode, this.actionName, this.actionIcon, this.actionColour, this.actionContext);
    }
};

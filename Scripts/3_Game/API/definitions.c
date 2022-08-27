class _Payload {
        string _v = GetGameLabs().GetVersionIdentifier();
        string ToJson() { return JsonFileLoader<_Payload>.JsonMakeData(this); }
};

class _Response {
        int status;
        string error;

        void _Response(string content) { JsonFileLoader<_Response>.JsonLoadData(content, this); }
};

class _Callback : RestCallback {
    override void OnError(int errorCode) {};
    override void OnTimeout() {};
    override void OnSuccess(string data, int dataSize) {};
};

class _LogPlayer {
    string id;

    vector position;

    float water = -1;
    float blood = -1;
    float health = -1;
    float energy = -1;
    int bleedingSources = -1;
};


// Register: /v1/auth/register
class _Payload_Register : _Payload {
    string version = GetGameLabs().GetVersionIdentifier();
    string serverId;
    string apiKey;
    string localPort;
    bool flagReauth;
    array<ref GameLabsContextAction> availableActions;

    void _Payload_Register(string serverId, string apiKey, bool flagReauth) {
        this.serverId = serverId;
        this.apiKey = apiKey;
        string tmp;
        if(GetGame().CommandlineGetParam("port", tmp)) {
            this.localPort = tmp;
        } else {
            this.localPort = "0";
        }
        this.flagReauth = flagReauth;
        this.availableActions = GetGameLabs().GetGameLabsActions();
    }
    string ToJson() { return JsonFileLoader<_Payload_Register>.JsonMakeData(this); }
};
class _Response_Register_Features {
    int allowReporting = 2;
    int statisticsReporting = 2;

    int metricsInterval = 5;
    int reportingInterval = 10;

    int pollProtocolVersion = 1;
};
class _Response_Register : _Response {
    string authKey;

    int modLicensingStatus = 1;
    string modLicensingOffender;

    _Response_Register_Features features;

    array<ref string> monitoredActions;
    void _Response_Register(string content) { JsonFileLoader<_Response_Register>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/server/poll
class _Payload_ServerPoll : _Payload {
    float serverFps;

    int aiCount;
    int animalCount;
    int vehicleCount;

    int aiActive;

    float overcast;
    float rain;
    float fog;
    float windspeed;

    void _Payload_ServerPoll(bool isFirst = false) {
        this.serverFps = GetGameLabs().GetServerFPS();

        this.aiCount = GetGameLabs().GetAICount();
        this.animalCount = GetGameLabs().GetAnimalCount();
        this.vehicleCount = GetGameLabs().GetVehicleCount();

        if(!isFirst) this.aiActive = GetGameLabs().GetAIActiveCount();
        else this.aiActive = 0;

        Weather weather = GetGame().GetWeather();
        if(weather) {
            // Percentages 0-100
            this.overcast = weather.GetOvercast().GetActual()*100;
            this.rain = weather.GetRain().GetActual()*100;
            this.fog = weather.GetFog().GetActual()*100;
            // Windspeed in km/h
            this.windspeed = weather.GetWindSpeed();
        }
    }
    string ToJson() { return JsonFileLoader<_Payload_ServerPoll>.JsonMakeData(this); }
};
class ServerPollItem {
    string action;
    string target;
    string parameter;
    int quantity = 1;
    float x;
    float y;
};
class _Response_ServerPoll : _Response {
    ref array<ServerPollItem> orders;
    void _Response_ServerPoll(string content) { JsonFileLoader<_Response_ServerPoll>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/server/events
class _ServerEvent {
    string id;
    string icon;
    string className;
    vector position;

    void _ServerEvent(_Event _event) {
        this.id = _event.ToString();
        this.icon = _event.Icon();
        this.className = _event.Class();
        if(_event.Ref() != NULL && _event.Ref() != NULL) {
            this.position = _event.Ref().GetPosition();
        }
    }
};
class _Payload_ServerEvents : _Payload {
    int initial;
    int interval;
    ref array<ref _ServerEvent> added = new array<ref _ServerEvent>();
    ref array<ref _ServerEvent> removed = new array<ref _ServerEvent>();

    void _Payload_ServerEvents(bool initial, int interval, array<ref _Event> added, array<ref _Event> removed) {
        if(initial) { this.initial = 1; } else { this.initial = 0; }
        this.interval = interval;

        for(int i = 0; i < added.Count(); i++) {
            this.added.Insert(new _ServerEvent(added.Get(i)));
        }
        for(int y = 0; y < removed.Count(); y++) {
            this.removed.Insert(new _ServerEvent(removed.Get(y)));
        }
    }
    string ToJson() { return JsonFileLoader<_Payload_ServerEvents>.JsonMakeData(this); }
};
class _Response_ServerEvents : _Response {
    void _Response_ServerEvents(string content) { JsonFileLoader<_Response_ServerEvents>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/server/vehicles
class _ServerVehicle {
    string id;
    string className;
    vector position;

    int health;
    int speed;

    void _ServerVehicle(_Vehicle vehicle) {
        this.id = vehicle.ToString();
        this.className = vehicle.Class();
        if(vehicle != NULL && vehicle.Ref() != NULL) {
            this.position = vehicle.Ref().GetPosition();

            this.health = vehicle.Ref().GetHealth();
            this.speed = Car.Cast(vehicle.Ref()).GetSpeedometer();
        }
    }
};
class _Payload_ServerVehicles : _Payload {
    int initial;
    int interval;
    ref array<ref _ServerVehicle> added = new array<ref _ServerVehicle>();
    ref array<ref _ServerVehicle> updated = new array<ref _ServerVehicle>();
    ref array<ref _ServerVehicle> removed = new array<ref _ServerVehicle>();

    void _Payload_ServerVehicles(bool initial, int interval, array<ref _Vehicle> added, array<ref _Vehicle> updated, array<ref _Vehicle> removed) {
        if(initial) { this.initial = 1; } else { this.initial = 0; }
        this.interval = interval;

        for(int i = 0; i < added.Count(); i++) {
            this.added.Insert(new _ServerVehicle(added.Get(i)));
        }
        for(int x = 0; x < updated.Count(); x++) {
            this.updated.Insert(new _ServerVehicle(updated.Get(x)));
        }
        for(int y = 0; y < removed.Count(); y++) {
            this.removed.Insert(new _ServerVehicle(removed.Get(y)));
        }
    }
    string ToJson() { return JsonFileLoader<_Payload_ServerVehicles>.JsonMakeData(this); }
};

class _Response_ServerVehicles : _Response {
    void _Response_ServerVehicles(string content) { JsonFileLoader<_Response_ServerVehicles>.JsonLoadData(content, this); }
};
// ************************


// Register: /v1/server/players
class _ServerPlayer {
    string id;
    string name;
    int loggingOut = 0;
    int insideVehicle = 0;

    int health;
    string item;
    vector position;

    // Constructor is in reporter.c to circumvent layer limitations
};
class _Payload_ServerPlayers : _Payload {
    int initial;
    int interval;
    ref array<ref _ServerPlayer> updated = new array<ref _ServerPlayer>();

    void _Payload_ServerPlayers(bool initial, int interval, array<ref _ServerPlayer> updated) {
        if(initial) { this.initial = 1; } else { this.initial = 0; }
        this.interval = interval;
        this.updated = updated;
    }
    string ToJson() { return JsonFileLoader<_Payload_ServerPlayers>.JsonMakeData(this); }
};

class _Response_ServerPlayers : _Response {
    void _Response_ServerPlayers(string content) { JsonFileLoader<_Response_ServerPlayers>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/player/death
class _Payload_PlayerDeath : _Payload {
    string weapon;
    string weaponNiceName;
    float distance;

    ref _LogPlayer player;
    ref _LogPlayer murderer;

    void _Payload_PlayerDeath(_LogPlayer player, _LogPlayer murderer, string weapon, string weaponNiceName) {
        this.player = player;
        this.murderer = murderer;

        if(weapon) this.weapon = weapon;
        if(weaponNiceName) this.weaponNiceName = weaponNiceName;
        if(this.murderer) {
            this.distance = vector.Distance(player.position, murderer.position);
        }
    }
    string ToJson() { return JsonFileLoader<_Payload_PlayerDeath>.JsonMakeData(this); }
};
// ************************

// Register: /v1/player/damage
class _Payload_PlayerDamage : _Payload {
    string weapon;
    string zone;
    float damage;
    float distance;

    ref _LogPlayer player;
    ref _LogPlayer murderer;

    void _Payload_PlayerDamage(ref _LogPlayer player, ref _LogPlayer murderer, Object weapon, float damage, string zone) {
        this.player = player;
        this.murderer = murderer;

        this.zone = zone;
        this.damage = damage;
        if(weapon) this.weapon = weapon.GetType();
        if(this.murderer) {
            this.distance = vector.Distance(player.position, murderer.position);
        }
    }
    string ToJson() { return JsonFileLoader<_Payload_PlayerDamage>.JsonMakeData(this); }
};
// ************************

// Register: /v1/item/interact
class _Payload_ItemInteract : _Payload {
    string item;
    string target;
    string action;

    ref _LogPlayer player;


    void _Payload_ItemInteract(ref _LogPlayer player, string item, string target, string action) {
        this.player = player;

        this.item = item;
        this.target = target;
        this.action = action;
    }
    string ToJson() { return JsonFileLoader<_Payload_ItemInteract>.JsonMakeData(this); }
};
// ************************

// Register: /v1/item/place
class _Payload_ItemPlace : _Payload {
    string item;

    ref _LogPlayer player;

    void _Payload_ItemPlace(ref _LogPlayer player, string item) {
        this.player = player;

        this.item = item;
    }
    string ToJson() { return JsonFileLoader<_Payload_ItemPlace>.JsonMakeData(this); }
};
// ************************

// Register: /v1/player/chat
class _Payload_PlayerChat : _Payload {
    string channel;
    string message;

    ref _LogPlayer player;

    void _Payload_PlayerChat(ref _LogPlayer player, string channel, string message) {
        this.player = player;

        this.channel = channel;
        this.message = message;
    }
    string ToJson() { return JsonFileLoader<_Payload_PlayerChat>.JsonMakeData(this); }
};
// ************************

// Register: /v1/player/connect
//! Callback is located in 5_Mission/definitions
class _Payload_PlayerConnect : _Payload {
    string steam64;
    vector position;

    void _Payload_PlayerConnect(string steam64, vector position) {
        this.steam64 = steam64;
        this.position = position;
    }
    string ToJson() { return JsonFileLoader<_Payload_PlayerConnect>.JsonMakeData(this); }
};
class _Response_PlayerConnect : _Response {
    string steam64;
    string cftools_id;
    string gamesession_id;

    void _Response_PlayerConnect(string content) { JsonFileLoader<_Response_PlayerConnect>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/player/disconnect
//! Everything else is located in 5_Mission/definitions
class _Payload_PlayerDisconnect : _Payload {};
class _Response_PlayerDisconnect : _Response {
    void _Response_PlayerDisconnect(string content) { JsonFileLoader<_Response_PlayerDisconnect>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/item/list
class TrackedItem {
    string className;
    string displayName;

    void TrackedItem(string className, string displayName) {
        this.className = className;
        this.displayName = displayName;
    }
};
class _Payload_ItemList : _Payload {
    ref array<ref TrackedItem> items = new array<ref TrackedItem>();

    void _Payload_ItemList(array<ref TrackedItem> items) {
        this.items = items;
    }
    string ToJson() { return JsonFileLoader<_Payload_ItemList>.JsonMakeData(this); }
};
class _Response_ItemList : _Response {
    void _Response_ItemList(string content) { JsonFileLoader<_Response_ItemList>.JsonLoadData(content, this); }
};
// ************************
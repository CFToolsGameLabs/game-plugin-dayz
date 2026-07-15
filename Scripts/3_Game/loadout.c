class GLLoadoutItem {
    string className;
    int quantity;
    float health;
    ref array<ref GLLoadoutItem> attachments;
    ref array<ref GLLoadoutItem> cargo;

    void GLLoadoutItem() {
        this.className = "";
        this.quantity = -1;
        this.health = -1;
        this.attachments = new array<ref GLLoadoutItem>;
        this.cargo = new array<ref GLLoadoutItem>;
    }
};

class GLLoadoutPreset {
    string name;
    ref array<ref GLLoadoutItem> items;

    void GLLoadoutPreset() {
        this.name = "";
        this.items = new array<ref GLLoadoutItem>;
    }

    string ToJson() { return JsonFileLoader<GLLoadoutPreset>.JsonMakeData(this); }
};

class GLLoadoutManager {
    private const string presetPath = "$profile:@GameLabsStorage\\presets";

    private ref array<ref GLLoadoutPreset> presets = new array<ref GLLoadoutPreset>;

    // Not accessed via GetGameLabs() as the manager is instantiated during core
    // construction, before the singleton reference is assigned.
    private GameLabsLogger logger;

    void GLLoadoutManager(GameLabsLogger _logger) {
        this.logger = _logger;
    }

    string GetPresetPath() { return this.presetPath; }

    void EnsureExamplePreset() {
        if(!FileExist(this.presetPath)) {
            if(this.logger) this.logger.Info(string.Format("Creating GameLabs loadout presets folder..."));
            MakeDirectory(this.presetPath);
        }

        string examplePath = string.Format("%1\\example.json", this.presetPath);
        if(FileExist(examplePath)) return;

        GLLoadoutPreset example = new GLLoadoutPreset();
        example.name = "Example";

        GLLoadoutItem apple = new GLLoadoutItem();
        apple.className = "Apple";
        example.items.Insert(apple);

        GLLoadoutItem rifle = new GLLoadoutItem();
        rifle.className = "M4A1";
        example.items.Insert(rifle);

        JsonFileLoader<GLLoadoutPreset>.JsonSaveFile(examplePath, example);
        if(this.logger) this.logger.Info(string.Format("Created example loadout preset at %1", examplePath));
    }

    void LoadPresets() {
        this.presets.Clear();

        if(!FileExist(this.presetPath)) return;

        string pattern = string.Format("%1\\*.json", this.presetPath);
        string fileName;
        FileAttr fileAttributes;
        FindFileHandle fileSearch = FindFile(pattern, fileName, fileAttributes, FindFileFlags.ALL);

        bool hasResults = true;
        while(hasResults && fileName != "") {
            string fullPath = string.Format("%1\\%2", this.presetPath, fileName);
            GLLoadoutPreset preset = new GLLoadoutPreset();
            JsonFileLoader<GLLoadoutPreset>.JsonLoadFile(fullPath, preset);

            if(preset.name && preset.name != "") {
                this.presets.Insert(preset);
                if(this.logger) this.logger.Debug(string.Format("[Loadout] Loaded preset \"%1\" (%2 items) from %3", preset.name, preset.items.Count(), fullPath));
            } else {
                if(this.logger) this.logger.Warn(string.Format("[Loadout] Skipping preset without a name at %1", fullPath));
            }

            hasResults = FindNextFile(fileSearch, fileName, fileAttributes);
        }
        CloseFindFile(fileSearch);

        if(this.logger) this.logger.Info(string.Format("[Loadout] Loaded %1 preset(s)", this.presets.Count()));
    }

    array<ref GLLoadoutPreset> GetPresets() {
        return this.presets;
    }

    GLLoadoutPreset GetPreset(string presetName) {
        for(int i = 0; i < this.presets.Count(); i++) {
            if(this.presets.Get(i).name == presetName) {
                return this.presets.Get(i);
            }
        }
        return NULL;
    }
};

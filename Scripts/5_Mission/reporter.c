class GameLabsReporter {
    private bool processReporting = true;

    private int effectiveInterval = 10;
    private int lastSent = 0;
    private int lastReportedPlayerCount = 0;

    private bool isFirstReport = true;
    private bool isFirstEventsReport = true;

    private ref Timer timerPoll;
    private ref Timer timerServer;
    private ref Timer timerPlayers;

    void GameLabsReporter() {
        if(GetGameLabs().GetConfiguration().CanSendDynamicItemList()) {
            TStringArray cfgPaths = new TStringArray;
            cfgPaths.Insert("CfgWeapons");
            cfgPaths.Insert("CfgVehicles");
            cfgPaths.Insert("CfgMagazines");

            int itemCount = 0;
            ref array < ref TrackedItem > items = new array < ref TrackedItem > ();

            float nonAsciiThreshold = 0.5;
            if(GetGameLabs().GetConfiguration().GetItemNameASCIIRequired()) {
                nonAsciiThreshold = 0.0;
            }

            for (int i = 0; i < cfgPaths.Count(); ++i) {
                string cfgPath = cfgPaths.Get(i);
                int nClasses = g_Game.ConfigGetChildrenCount(cfgPath);

                for (int nClass = 0; nClass < nClasses; ++nClass) {
                    string strName;
                    if(!GetGame().ConfigGetChildName(cfgPath, nClass, strName))
                        continue;

                    int scope = GetGame().ConfigGetInt(cfgPath + " " + strName + " scope");

                    if (scope <= 0)
                        continue;

                    if(cfgPath == "CfgVehicles") {
                        int cfgType = GetGame().ConfigGetType(CFG_VEHICLESPATH + " " + strName + " inventorySlot");
                        if(cfgType == CT_ARRAY) {
                            array<string> attachments = {};
                            GetGame().ConfigGetTextArray(CFG_VEHICLESPATH + " " + strName + " inventorySlot", attachments);
                            foreach(string attachment : attachments) {
                                if (attachment == string.Empty)
                                    break;

                                attachment.ToLower();
                                if (GetGameLabs()._vehicleSlotMap[attachment] == NULL) {
                                    GetGameLabs()._vehicleSlotMap[attachment] = new array<string>;
                                }

                                if (GetGameLabs()._vehicleSlotMap[attachment].Find(strName) == -1)
                                    GetGameLabs()._vehicleSlotMap[attachment].Insert(strName);
                            }
                        } else if (cfgType == CT_STRING) {
                            string slot = string.Empty;
                            if (GetGame().ConfigGetText("CfgVehicles " + strName + " inventorySlot", slot) && slot != string.Empty) {
                                slot.ToLower();
                                if (GetGameLabs()._vehicleSlotMap[slot] == NULL){
                                    GetGameLabs()._vehicleSlotMap[slot] = new array<string>;
                                }

                                if (GetGameLabs()._vehicleSlotMap[slot].Find(strName) == -1)
                                    GetGameLabs()._vehicleSlotMap[slot].Insert(strName);
                            }
                        }
                    }

                    string displayName = "";
                    if(scope != 2)
                        continue;

                    if (!GetGame().ConfigGetText(cfgPath + " " + strName + " displayName", displayName))
                        displayName = "";

                    if(GetGameLabs().GetConfiguration().GetItemNameASCIIFilter()) {
                        if(displayName.Length()) {
                            int nonAsciiCharacters = 0;
                            for (int c = 0; c < displayName.Length(); c++) {
                                if (displayName.Get(c).ToAscii() < 0 || displayName.Get(c).ToAscii() > 127) {
                                    nonAsciiCharacters++;
                                }
                            }

                            float nonAsciiPortion;
                            if(displayName.Length() == 0) {
                                nonAsciiPortion = 0;
                            } else {
                                nonAsciiPortion = nonAsciiCharacters / displayName.Length();
                            }

                            if(nonAsciiPortion >= nonAsciiThreshold) {
                                displayName = strName;
                            }
                        }
                    }

                    itemCount++;
                    items.Insert(new TrackedItem(strName, displayName));
                }
            }

            GetGameLabs().GetLogger().Debug(string.Format("(ItemDiscovery) Discovered %1 items", itemCount));

            _Payload_ItemList payloadItemList = new _Payload_ItemList(items);
            GetGameLabs().GetApi().ItemList(new _Callback_ServerDummy(), payloadItemList);
        }

        this.timerPoll = new Timer(CALL_CATEGORY_SYSTEM);
        this.timerPoll.Run(GetGameLabs().GetMetricsInterval(), this, "activePolling", NULL, true);

        if(GetGameLabs().IsReportingEnabled()) {
            GetGameLabs().GetLogger().Debug(string.Format("(Reporter) pollProtocolVersion=%1", GetGameLabs().GetApi().GetPollProtocolVersion()));
            this.effectiveInterval = GetGameLabs().GetReportingInterval();
            this.timerServer = new Timer(CALL_CATEGORY_SYSTEM);
            this.timerServer.Run(GetGameLabs().GetReportingInterval(), this, "serverReporting", NULL, true);
        }
    }

    void Disable() {
        this.processReporting = false;
        GetGameLabs().GetLogger().Info("(Reporter) Disabled");
        if(this.timerPoll) this.timerPoll.Stop();
        if(this.timerServer) this.timerServer.Stop();
        GetGameLabs().GetLogger().Debug("(Reporter) Timers gracefully closed");
    }

    private void activePolling() {
        if(!this.processReporting || !GetGameLabs()) return;
        _Payload_ServerPoll payload = new _Payload_ServerPoll(this.isFirstEventsReport);
        GetGameLabs().GetApi().Enable(); // Enable to check if api is back
        switch(GetGameLabs().GetApi().GetPollProtocolVersion()) {
            case 2: {
                GetGameLabs().GetApi().ServerPoll2(new _Callback_ServerPoll2(), payload);
                break;
            }
            default: {
                GetGameLabs().GetApi().ServerPoll(new _Callback_ServerPoll(), payload);
            }
        }
        if(this.isFirstEventsReport) this.isFirstEventsReport = false;
    }

    private void serverReporting() {
        if(!this.processReporting  || !GetGameLabs()) return;
        if(GetGameLabs()._serverEventsBufferAdded.Count() || GetGameLabs()._serverEventsBufferRemoved.Count() || this.isFirstReport) {
            _Payload_ServerEvents payloadEvents = new _Payload_ServerEvents(this.isFirstReport, this.effectiveInterval, GetGameLabs()._serverEventsBufferAdded, GetGameLabs()._serverEventsBufferRemoved);

            GetGameLabs().GetApi().ServerEvents(new _Callback_ServerDummy(), payloadEvents);

            GetGameLabs()._serverEventsBufferAdded.Clear();
            GetGameLabs()._serverEventsBufferRemoved.Clear();
        }

        array<ref _Vehicle> updated = new array<ref _Vehicle>();
        for(int i = 0; i < GetGameLabs().GetVehicles().Count(); i++) {
            _Vehicle vehicle = GetGameLabs().GetVehicles().Get(i);
            if(vehicle.HasUpdated()) updated.Insert(vehicle);
        }

        if(GetGameLabs()._serverEventsBufferAdded.Count() || GetGameLabs()._serverEventsBufferRemoved.Count() || updated.Count() || this.isFirstReport) {
            _Payload_ServerVehicles payloadVehicles = new _Payload_ServerVehicles(this.isFirstReport, this.effectiveInterval, GetGameLabs()._serverVehiclesBufferAdded, updated, GetGameLabs()._serverVehiclesBufferRemoved);

            GetGameLabs().GetApi().ServerVehicles(new _Callback_ServerDummy(), payloadVehicles);

            GetGameLabs()._serverVehiclesBufferAdded.Clear();
            GetGameLabs()._serverVehiclesBufferRemoved.Clear();
        }

        PlayerBase player;
        array<Man> players = new array<Man>;
        GetGame().GetPlayers( players );

        array<ref _ServerPlayer> updatedPlayers = new array<ref _ServerPlayer>();
        for ( int x = 0; x < players.Count(); x++ ) {
            player = PlayerBase.Cast(players.Get(x));
            updatedPlayers.Insert(new _ServerPlayerEx(player));
        }
        if(players.Count() || players.Count() != this.lastReportedPlayerCount || this.isFirstReport) {
            this.lastReportedPlayerCount = players.Count();
            _Payload_ServerPlayers payloadPlayers = new _Payload_ServerPlayers(this.isFirstReport, this.effectiveInterval, updatedPlayers);
            GetGameLabs().GetApi().ServerPlayers(new _Callback_ServerDummy(), payloadPlayers);
        }

        if(this.isFirstReport) this.isFirstReport = false;
    }
};

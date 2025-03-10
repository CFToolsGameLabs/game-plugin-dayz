class GameLabsInternal_DummyActionWithWebHook extends GameLabsContextAction {
        void GameLabsInternal_DummyActionWithWebHook() {
            this.actionCode = "GameLabsInternal_DummyActionWithWebHook";
            this.actionName = "Dummy action (WebHook)";
            this.actionIcon = "accessible-icon";
            this.actionColour = "danger";
            this.actionContext = "world";

            this.responseSuccess = new GameLabsActionResponse("webhook");
            this.responseFailed = new GameLabsActionResponse("webhook");

            this.parameters.Insert("webhookUrl", GameLabsActionParameter("WebHook URL", "Any valid WebHook URL", "webhook_url"));
        }

        override bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Debug(string.Format("Dummy action called webhookUrl=%1", context.parameters.Get("webhookUrl").GetString()));
            return true;
        }
};
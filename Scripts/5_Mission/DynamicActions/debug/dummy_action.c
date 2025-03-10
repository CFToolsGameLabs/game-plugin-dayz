// This is an example action for internal use, and it's not transmitted unless testing mode is locally enabled
class GameLabsInternal_DummyAction extends GameLabsContextAction {
        void GameLabsInternal_DummyAction() {
            this.actionCode = "GameLabsInternal_DummyAction";
            this.actionName = "Dummy action";
            this.actionIcon = "accessible-icon";
            this.actionColour = "danger";
            this.actionContext = "world";

            this.parameters.Insert("testInt", GameLabsActionParameter("Integer", "Description goes here", "int"));
            this.parameters.Insert("testString", GameLabsActionParameter("String", "Description goes here", "string"));
            this.parameters.Insert("testBool", GameLabsActionParameter("Boolean", "Description goes here", "boolean"));
        }

        override bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Debug(string.Format("Dummy action called testInt=%1, testString=%2, testBool=%3", context.parameters.Get("testInt").GetInt(), context.parameters.Get("testString").GetString(), context.parameters.Get("testBool").GetBoolean()));
            return true;
        }
};
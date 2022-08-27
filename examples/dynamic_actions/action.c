// Place this file anywhere within your 5_Mission scoped script folder

class Example_MyCustomAction extends GameLabsContextAction {
        // world, player, vehicle
        void Example_MyCustomAction() {
            // A unique actionCode as string, identifying your action in the internal GameLabs system
            // Convey to a standard "{YourModPrefix}_{Action}" eg. "TestMod_SomeAction"
            this.actionCode = "Example_MyCustomAction";

            // User-friendly name for your action which is being displayed in the CFCloud interface for users
            this.actionName = "My custom action";

            // Prefix your action with an icon in the CFCloud interface, leave empty to disable
            // Available icons: https://fontawesome.com/v5/search (type is always dualtone)
            this.actionIcon = "accessible-icon";

            // Colour code for your action, only override if the action you implement is destructive
            // Available codes: "success" (green), "danger" (red), "warning" (yellow)
            this.actionColour = "default";

            // Context in which this will be available from the interface (world, player or vehicle)
            this.actionContext = "world";

            // Some parameters that will be displayed on the interface with appropriate field inputs
            this.parameters.Insert("testInt", GameLabsActionParameter("Integer", "Description goes here", "int"));
            this.parameters.Insert("testString", GameLabsActionParameter("String", "Description goes here", "string"));
            this.parameters.Insert("testBool", GameLabsActionParameter("Boolean", "Description goes here", "boolean"));
            this.parameters.Insert("testItemClass", GameLabsActionParameter("Item", "Description goes here", "cf_itemlist"));
        }

        // Main function that will be executed
        bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Debug(
                    string.Format(
                            "Dummy action called testInt=%1, testString=%2, testBool=%3, testItemClass=%4",
                            context.parameters.Get("testInt").GetInt(),
                            context.parameters.Get("testString").GetString(),
                            context.parameters.Get("testBool").GetBoolean(),
                            context.parameters.Get("testItemClass").GetString()
                    )
            );
            return true;
        }
};

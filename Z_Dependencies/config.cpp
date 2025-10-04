class CfgPatches
{
    class Z_Dependencies
    {
        units[]={};
        requiredVersion=0.1;
        requiredAddons[]=
        {
            "DZ_Data",
            "GameLabs_Scripts"
        };
    };

};

class CfgMods
{
    class Z_Dependencies
    {
        dir = "Z_Dependencies";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "GameLabs_Dependencies";
        authorID = "0";
        version = "1.0";
        extra = 0;
        type = "mod";

        dependencies[] = { "Core", "Game", "World", "Mission" };

        defines[] = {
            "GameLabsDependencies",
        };

        class defs
        {
            class coreScriptModule
            {
                value = "";
                files[] = {"GameLabs/Z_Dependencies/1_Core"};
            };
            class gameScriptModule
            {
                value = "";
                files[] = {"GameLabs/Z_Dependencies/3_Game"};
            };
            class worldScriptModule
            {
                value = "";
                files[] = {"GameLabs/Z_Dependencies/4_World"};
            };
            class missionScriptModule
            {
                value = "";
                files[] = {"GameLabs/Z_Dependencies/5_Mission"};
            };
        };
    };
};

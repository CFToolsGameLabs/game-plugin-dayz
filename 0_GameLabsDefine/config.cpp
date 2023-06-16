class CfgPatches
{
    class GameLabsDefine
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {};
    };
};

class CfgMods
{
    class GameLabsDefine
    {
        type="mod";
        dependencies[]={"Game", "World", "Mission"};
        class defs
        {
            class engineScriptModule
            {
                value = "";
                files[] = {
                        "GameLabs/0_GameLabsDefine/Global"
                };
            };
            class gameLibScriptModule
            {
                value = "";
                files[] = {
                    "GameLabs/0_GameLabsDefine/Global"
                };
            };
            class gameScriptModule
            {
                value = "";
                files[]={
                    "GameLabs/0_GameLabsDefine/Global"
                };
            };

            class worldScriptModule
            {
                value="";
                files[]={
                    "GameLabs/0_GameLabsDefine/Global"
                };
            };

            class missionScriptModule
            {
                value="";
                files[]={
                    "GameLabs/0_GameLabsDefine/Global"
                };
            };
        };
    };
};

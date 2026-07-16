[h1]Introduction[/h1]
The GameLabs plugin for DayZ allows your game server to interact with the CFTools Cloud GameAPI.

It will automatically transmit relevant events as well as player and environment information. Additionally, it acts as a library for other mods to interact with CFTools Cloud components or core GameLabs data.

[b]To install GameLabs follow the "Installation" section.[/b]

[h1]Requirements[/h1]
[list]
[*] CFTools Cloud server registration or Open GameLabs compatible API Server
[*] Server Id and API Key (Server, Manage, Settings, API Key)
[/list]

[h1]Installation[/h1]
Install/load this mod to you the target game server deployment.

For the configuration, create a file called [b]gamelabs.cfg[/b] in your profiles directory (Do not add any additional file extensions). Inside this file, put following JSON configuration:
[code]
{
	"debugEnabled": 1,
	"connectionVerification": 0,
	"serverId": "Server-Id",
	"apiKey": "API-Key",
	"preventDynamicItemPopulation": 0,
	"chatSanitizeBattlEyeJoinLeave": 0,
	"chatSanitizeBattlEyePrefix": 0,
	"advancedChatInterface": 0,
	"lockServerOnStart": 0,
	"enableMetricsDump": 0,
	"reportingWebhookUrl": "",
	"reportingWebhookUrlType": "discord",
	"reportingEnabled": 0,
	"reportingElements": [
		"deathScreen",
		"escapeMenu"
	]
}
[/code]

Replace the Server-Id and API-Key placeholders with your appropiate API information. The file must be saved in either ANSI or UTF-8 format, depending on your OS language settings.

[h1]Extended configuration[/h1]
[h3]connectionVerification[/h3]
GameLabs will automatically verify its access credentials and shut down the server if the API is unreachable or the credentials are invalid. To disable GameLabs shutting down your server for when the API is unreachable set [code]connectionVerification[/code] to 0.
When connection verification is disabled, we can and will not guarantee that GameLabs is functioning correctly.

[h3]debugEnabled[/h3]
Enable or disable GameLabs's debug mode. If you encounter issues, it is recommend to enable debug mode and check the log output generated in profiles/@Logging.

[h3]preventDynamicItemPopulation[/h3]
GameLabs automatically scans your game server for all available items. If your server has more than 50.000 unique items, this may lead to issues. [b]We heavily discourge disabling this setting. Consult CFTools Cloud support if you think your setup requires this.[/b]

[h3]serverId / apiKey[/h3]
These fields must contain your API credentials. You can find them under "Manage", "Settings", "API Key". Only the owner of any specific server may access them. These credentials are required in order for GameLabs to function and the server must be registered with CFTools Cloud before GameLabs can be used.

[h3]chatSanitizeBattlEyeJoinLeave[/h3]
Sanitize chat from BattlEye join/leave messages.

[h3]chatSanitizeBattlEyePrefix[/h3]
Remove BattlEye message prefix.

[h3]advancedChatInterface[/h3]
Support for stylizing RCon messages or displaying them as notification. This may break other chat mods, so testing is recommended.

[h3]lockServerOnStart[/h3]
This flag will cause the game server to become locked during startup. When enabled, a manual (or automated for example CFCloud Scheduler) unlock is required.

[h1]Extended functionality[/h1]
[h2]DayZ Expansion[/h2]
GameLabs is able to transmit Side and Team chat for DayZ Expansion.
[b]GameLabs MUST be launched on the client side for Expansion chat support[/b]

[h2]In-Game Reporting[/h2]
The in-game reporting module allows players to report other players. The report will be delivered via WebHook to an URL of your choosing. Reporting supports Discord WebHooks or non Discord formats for expanded details.

Enabling the module requires `reportingEnabled` to be set to `1`.

The display options can be overriden by modding the elements GameLabs has implemented. For base configuration, `reportingElements` allows you to customize in which situations reporting will be displayed. 
- `deathScreen` will render a report button when a player is killed (PVP kills only)
- `escapeMenu` will render a report button when the player opens their menu via ESC

[b]GameLabs MUST be launched on the client side for reporting[/b]

[h2]Message styling[/h2]
Requires "advancedChatInterface" to be enabled and GameLabs to be installed on both client and server.

To send stylized RCon messages you prefix the specific messages the following way:
- [code]|>N[/code] (Sending a notification)
- [code]|>C(HEX)[/code] (Sending a colorized message)
- [code]|>X(duration)|(icon)|(title)|(text)[/code] (Sending a customizable notification, empty fields use defaults)

Full examples for RCon messages:
"|>NThis is a notification"
"|>C37ff00This message will appear green”
"|>X10||Announcement|Server restart in 5 minutes"

Messages stylized this way, will not have any prefix.

[b]Chat styling is not compatible with every chat mod.[/b]

[h2]Loadout presets[/h2]
Loadout presets let you define reusable gear sets as JSON and grant them to players from CFTools Cloud via the [b]Give player loadout[/b] action.

Presets are stored in your server profiles directory:
[code]
@GameLabsStorage/presets/*.json
[/code]

The folder and an [code]example.json[/code] are created automatically on first server start. One [code].json[/code] file equals one preset. Presets are loaded at startup, so restart the server after adding or editing files.

Minimal preset format:
[code]
{
	"name": "Starter Kit",
	"items": [
		{
			"className": "Apple",
			"quantity": -1,
			"health": -1,
			"attachments": [],
			"cargo": []
		}
	]
}
[/code]

- [code]name[/code] is required and shown in the CFTools Cloud dropdown
- [code]className[/code] is the DayZ classname
- [code]quantity[/code] / [code]health[/code] of [code]-1[/code] leave the item defaults
- [code]attachments[/code] are slotted items (optics, pouches, magazines in weapons)
- [code]cargo[/code] are items stored inside containers (spare mags in a vest, food in a backpack)

In CFTools Cloud, run [b]Give player loadout[/b] on a player, pick a preset by name, and optionally enable [b]Strip existing gear[/b] to clear current items first.

[h1]Logging[/h1]
GameLabs has an integrated and custom logger. The log files will be generated inside a @Logging directory, which is being located in the configured profiles directory.

[h1]Managing User Consent[/h1]
Depending on your region, you might require a “cookie consent” banner when using client performance metrics (gathering of client game FPS for server metrics, etc.). You can enable this by adding:
[code]
#define GAMELABSCLIENTCONSENT
[/code]
in any mod that is loaded before GameLabs on the client.

[h1]Game server monetization[/h1]
The usage of GameLabs on monetized servers is allowed. The usage of GameLabs for commercial use is forbidden in compliance with the DayZ Tools EULA.

[h1]Usage[/h1]
The usage of GameLabs is subject to the CFTools Cloud Terms of Use.

[h1]Compatibility[/h1]
GameLabs is compatible with most modifications, however we do not guarantee explicit compatibility with any specific mod.

[h1]Support[/h1]
[u]No support will be provided via Steam or the Steam Workshop.[/u]

For support you can join our [url=https://discord.cftools.cloud]Discord[/url] or open a [url=https://app.cftools.cloud/support]ticket[/url].

[h1]Other branches[/h1]
For experimental and unstable features of GameLabs we provide following additional versions of GameLabs:
- [url=https://steamcommunity.com/sharedfiles/filedetails/?id=2519477297]Experimental[/url]
- [url=https://steamcommunity.com/sharedfiles/filedetails/?id=2611827886]Unstable[/url]
- [url=https://steamcommunity.com/sharedfiles/filedetails/?id=3349763784]Pre-Release[/url]

[h1]License[/h1]
GameLabs and its code is copyrighted content of © 2017 - 2026 CFTools™. All rights reserved.

The usage on both client and server side falls under our [url=https://cftools.cloud/legal/eula]EULA[/url].

For usage with third party Open GameLabs compatible API servers, contact the operator for licensing information.

[h1]Links[/h1]
Site: [url=https://cftools.com]cftools.com[/url]
Support: [url=https://app.cftools.cloud/support]app.cftools.cloud/support[/url]

[b]To install GameLabs follow the “Installation” section.[/b]
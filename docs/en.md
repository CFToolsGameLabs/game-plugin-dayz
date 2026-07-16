# Introduction

The GameLabs plugin for DayZ allows your game server to interact with the CFTools Cloud GameAPI.

It will automatically transmit relevant events as well as player and environment information. Additionally, it acts as a library for other mods to interact with CFTools Cloud components or core GameLabs data.

**To install GameLabs follow the "Installation" section.**

# Requirements

- CFTools Cloud server registration or Open GameLabs compatible API Server
- Server Id and API Key (Server, Manage, Settings, API Key)

# Installation

Install/load this mod to you the target game server deployment.

For the configuration, create a file called **gamelabs.cfg** in your profiles directory (Do not add any additional file extensions). Inside this file, put following JSON configuration:

```json
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
```

Replace the Server-Id and API-Key placeholders with your appropiate API information. The file must be saved in either ANSI or UTF-8 format, depending on your OS language settings.

# Extended configuration

### connectionVerification

GameLabs will automatically verify its access credentials and shut down the server if the API is unreachable or the credentials are invalid. To disable GameLabs shutting down your server for when the API is unreachable set `connectionVerification` to 0.
When connection verification is disabled, we can and will not guarantee that GameLabs is functioning correctly.

### debugEnabled

Enable or disable GameLabs's debug mode. If you encounter issues, it is recommend to enable debug mode and check the log output generated in profiles/@Logging.

### preventDynamicItemPopulation

GameLabs automatically scans your game server for all available items. If your server has more than 50.000 unique items, this may lead to issues. **We heavily discourge disabling this setting. Consult CFTools Cloud support if you think your setup requires this.**

### serverId / apiKey

These fields must contain your API credentials. You can find them under "Manage", "Settings", "API Key". Only the owner of any specific server may access them. These credentials are required in order for GameLabs to function and the server must be registered with CFTools Cloud before GameLabs can be used.

### chatSanitizeBattlEyeJoinLeave

Sanitize chat from BattlEye join/leave messages.

### chatSanitizeBattlEyePrefix

Remove BattlEye message prefix.

### advancedChatInterface

Support for stylizing RCon messages or displaying them as notification. This may break other chat mods, so testing is recommended.

### lockServerOnStart

This flag will cause the game server to become locked during startup. When enabled, a manual (or automated for example CFCloud Scheduler) unlock is required.

# Extended functionality

## DayZ Expansion

GameLabs is able to transmit Side and Team chat for DayZ Expansion.
**GameLabs MUST be launched on the client side for Expansion chat support**

## In-Game Reporting

The in-game reporting module allows players to report other players. The report will be delivered via WebHook to an URL of your choosing. Reporting supports Discord WebHooks or non Discord formats for expanded details.

Enabling the module requires `reportingEnabled` to be set to `1`.

The display options can be overriden by modding the elements GameLabs has implemented. For base configuration, `reportingElements` allows you to customize in which situations reporting will be displayed.

- `deathScreen` will render a report button when a player is killed (PVP kills only)
- `escapeMenu` will render a report button when the player opens their menu via ESC

**GameLabs MUST be launched on the client side for reporting**

## Message styling

Requires "advancedChatInterface" to be enabled and GameLabs to be installed on both client and server.

To send stylized RCon messages you prefix the specific messages the following way:

- `|>N` (Sending a notification)
- `|>C(HEX)` (Sending a colorized message)
- `|>X(duration)|(icon)|(title)|(text)` (Sending a customizable notification, empty fields use defaults)

Full examples for RCon messages:

```
"|>NThis is a notification"
"|>C37ff00This message will appear green"
"|>X10||Announcement|Server restart in 5 minutes"
```

Messages stylized this way, will not have any prefix.

**Chat styling is not compatible with every chat mod.**

## Loadout presets

Loadout presets let you define reusable gear sets as JSON and grant them to players from CFTools Cloud via the **Give player loadout** action.

Presets are stored in your server profiles directory:

```
@GameLabsStorage/presets/*.json
```

The folder and an `example.json` are created automatically on first server start. One `.json` file equals one preset. Presets are loaded at startup, so restart the server after adding or editing files.

Minimal preset format:

```json
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
```

- `name` is required and shown in the CFTools Cloud dropdown
- `className` is the DayZ classname
- `quantity` / `health` of `-1` leave the item defaults
- `attachments` are slotted items (optics, pouches, magazines in weapons)
- `cargo` are items stored inside containers (spare mags in a vest, food in a backpack)

In CFTools Cloud, run **Give player loadout** on a player, pick a preset by name, and optionally enable **Strip existing gear** to clear current items first.

# Logging

GameLabs has an integrated and custom logger. The log files will be generated inside a @Logging directory, which is being located in the configured profiles directory.

# Managing User Consent

Depending on your region, you might require a "cookie consent" banner when using client performance metrics (gathering of client game FPS for server metrics, etc.). You can enable this by adding:

```c
#define GAMELABSCLIENTCONSENT
```

in any mod that is loaded before GameLabs on the client.

# Game server monetization

The usage of GameLabs on monetized servers is allowed. The usage of GameLabs for commercial use is forbidden in compliance with the DayZ Tools EULA.

# Usage

The usage of GameLabs is subject to the CFTools Cloud Terms of Use.

# Compatibility

GameLabs is compatible with most modifications, however we do not guarantee explicit compatibility with any specific mod.

# Support

*No support will be provided via Steam or the Steam Workshop.*

For support you can join our [Discord](https://discord.cftools.cloud) or open a [ticket](https://app.cftools.cloud/support).

# Other branches

For experimental and unstable features of GameLabs we provide following additional versions of GameLabs:

- [Experimental](https://steamcommunity.com/sharedfiles/filedetails/?id=2519477297)
- [Unstable](https://steamcommunity.com/sharedfiles/filedetails/?id=2611827886)
- [Pre-Release](https://steamcommunity.com/sharedfiles/filedetails/?id=3349763784)

# License

GameLabs and its code is copyrighted content of © 2017 - 2026 CFTools™. All rights reserved.

The usage on both client and server side falls under our [EULA](https://cftools.cloud/legal/eula).

For usage with third party Open GameLabs compatible API servers, contact the operator for licensing information.

# Links

- Site: [cftools.com](https://cftools.com)
- Support: [app.cftools.cloud/support](https://app.cftools.cloud/support)

**To install GameLabs follow the "Installation" section.**

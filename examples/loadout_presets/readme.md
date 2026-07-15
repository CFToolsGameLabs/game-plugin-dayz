# Loadout Presets

Loadout presets let you define reusable sets of gear as JSON and grant them to any
player from CFTools Cloud with the **Give player loadout** action.

## Where presets live

Presets are stored on the **server profile**, not in the mod:

```
<profile>/@GameLabsStorage/presets/*.json
```

- One `.json` file = one preset.
- The folder and an `example.json` are created automatically on first server start.
- Presets are loaded once at server startup, so **restart the server** after adding
  or editing files.

## How to use a preset

1. Drop your `.json` file into the `presets` folder.
2. Restart the server.
3. In CFTools Cloud, run the **Give player loadout** action on a player.
4. Pick your preset by its `name` from the dropdown.
5. Optionally enable **Strip existing gear** to clear the player's current items first.

## Schema

```json
{
  "name": "Starter Kit",
  "items": [ /* GLLoadoutItem */ ]
}
```

| Field   | Type              | Required | Notes                                                        |
| ------- | ----------------- | -------- | ------------------------------------------------------------ |
| `name`  | string            | yes      | Preset ID shown in the CFTools dropdown. Must be non-empty.  |
| `items` | array of items    | yes      | Top-level items placed into the player's inventory.          |

### Item (`GLLoadoutItem`)

| Field         | Type           | Default | Notes                                                              |
| ------------- | -------------- | ------- | ------------------------------------------------------------------ |
| `className`   | string         | —       | DayZ classname (e.g. `M4A1`, `Apple`, `PlateCarrierVest`).         |
| `quantity`    | int            | `-1`    | Ammo/liquid/stack count. `-1` = leave default.                     |
| `health`      | float          | `-1`    | Normalized health `0.0`–`1.0`. `-1` = leave default.               |
| `attachments` | array of items | `[]`    | Items attached to this item (optics, mags in guns, pouches, etc.). |
| `cargo`       | array of items | `[]`    | Items placed inside this item's cargo (mags in a vest, etc.).      |

Items are recursive: any item in `attachments` or `cargo` uses the same structure,
so you can nest as deep as the item slots allow.

### attachments vs cargo

- **attachments** = items that snap into a slot (a scope on a rifle, pouches on a
  plate carrier, a rifle magazine inserted into the weapon).
- **cargo** = items that go *inside* the container's storage (spare mags inside a
  vest, food inside a backpack).

If an item won't accept the classname in that slot, the engine skips it and a
warning is logged.

## Files in this folder

- `template.json` — a minimal skeleton to copy and fill in.
- `example.json` — a complete "Starter Kit" showing clothing, a rifle with
  attachments and a loaded magazine, a vest with pouches and spare mags, and
  quantity/health usage.

## Tips

- The filename doesn't matter; the **`name`** field is the identifier.
- Give each preset a unique `name` so they don't collide in the dropdown.
- Set `health` to `1.0` for pristine gear.
- Use `quantity` for magazine round counts, liquids (e.g. soda), and stackable
  items like rags.

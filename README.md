# Raspberry Pi Backlight Control

An application for adjusting display backlight brightness.

Provides a GUI with a slider and quick preset buttons for day and night modes, or a CLI for headless use.

## Usage

```bash
# GUI mode (default)
rp-backlight-control
```

CLI mode accepts `day`, `night`, or a value from 0 to 100 (percent).

```bash
rp-backlight-control day       # set day brightness (80%)
rp-backlight-control night     # set night brightness (8%)
rp-backlight-control 50        # set brightness to 50%
```

### Cron example

```cron
45 22 * * * sudo rp-backlight-control night
0  7  * * * sudo rp-backlight-control day
```

## Building

```bash
meson setup builddir --wipe
meson compile -C builddir
sudo meson install -C builddir
```

## Generating translations

```bash
meson compile -C builddir rp-backlight-control-pot
meson compile -C builddir rp-backlight-control-update-po
```

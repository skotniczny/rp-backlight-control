# Raspberry Pi Backlight Control

An application for adjusting display backlight brightness.

Provides a slider and quick preset buttons for day and night modes.

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

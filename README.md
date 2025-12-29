# Arduino Stern WiFi

Projektvorlage für den UNO R4 WiFi mit einem 12+1 Neopixel-Weihnachtsstern. Enthält OTA, Web-UI und eine modulare Effektbibliothek, aufgeteilt in klar strukturierte Module für PlatformIO.

## Features
- **WiFi + mDNS**: verbindet sich automatisch und bietet den Namen `Stern.local` inkl. HTTP-Service im Netz.
- **HTTP Web-Interface**: Ein/Aus, Effektwahl, Speed-, Brightness- und RGBW-Regler.
- **OTA**: ArduinoOTA für drahtlose Updates.
- **Effekt-Bibliothek**: jedes Pattern in eigener `.cpp`-Datei, leicht erweiterbar.
- **Kalibrierung**: per-Kanal-Faktoren für farbliche Feinabstimmung.
- **Adafruit NeoPixel**: GRBW, 13 LEDs (12 Strahlen + Zentrum) gemäß Mapping.

## Projektstruktur
```
platformio.ini          # PlatformIO Ziel für UNO R4 WiFi
src/
  config.h             # zentrale Konfiguration (Pins, LEDs, WLAN, OTA)
  main.cpp             # Setup/Loop, Initialisierung
  led/StripController* # LED-Mapping, Kalibrierung, Basisfunktionen
  effects/             # Effekt-Implementierungen + Manager
  network/             # WiFi & OTA
  web/                 # Webserver + HTML/CSS/JS Assets
```

## Setup
1. WiFi-Daten in `src/config.h` setzen (`WIFI_SSID`, `WIFI_PASSWORD`).
2. Optional Kalibrierung (RGBW) und LED-Pin/-Anzahl anpassen.
3. Mit PlatformIO bauen/flashen:
   ```bash
   pio run -t upload
   ```
4. Gerät verbindet sich mit WLAN und ist per `http://Stern.local` erreichbar.

## Web API
- `GET /api/state`: aktueller Status inkl. Effektliste.
- `POST /api/state`: JSON Payload mit `isOn`, `brightness`, `speed`, `effect`, `color.{r,g,b,w}`; Werte werden validiert/geklemmt, unbekannte Effekte liefern HTTP 400.

## Effekte erweitern
1. Neue Datei in `src/effects/` anlegen (z. B. `MeteorEffect.cpp`).
2. Von `Effect` ableiten und `update` implementieren.
3. Fabrikfunktion in `EffectFactory.h` deklarieren und im `setup` registrieren.


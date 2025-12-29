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

## PlatformIO / VS Code Einrichtung
Falls der Plattform-Download fehlschlägt (z. B. `UnknownPackageError: Could not find the package with 'renesas-ra4'...` auf
Windows), hilft folgende Schrittfolge:

1. **VS Code installieren** und die **PlatformIO IDE** Extension hinzufügen.
2. Projektordner öffnen und in der PlatformIO-Terminalkonsole den aktuellen Plattformnamen installieren/aktualisieren:
   ```bash
   pio platform install renesas-ra
   pio pkg update
   ```
   Der Eintrag in `platformio.ini` nutzt `platform = renesas-ra`, da dieser offiziell für den UNO R4 WiFi unterstützt wird.
3. Abhängigkeiten laden lassen und anschließend kompilieren/flashen:
   ```bash
   pio run -t upload
   ```
4. Bei Proxy-/Firewall-Umgebungen sicherstellen, dass `https://dl.platformio.org/` erreichbar ist oder den Download offline
   mit `pio platform install renesas-ra --skip-default-package` plus manuell bereitgestellten Paketen durchführen.

## Web API
- `GET /api/state`: aktueller Status inkl. Effektliste.
- `POST /api/state`: JSON Payload mit `isOn`, `brightness`, `speed`, `effect`, `color.{r,g,b,w}`; Werte werden validiert/geklemmt, unbekannte Effekte liefern HTTP 400.

## Effekte erweitern
1. Neue Datei in `src/effects/` anlegen (z. B. `MeteorEffect.cpp`).
2. Von `Effect` ableiten und `update` implementieren.
3. Fabrikfunktion in `EffectFactory.h` deklarieren und im `setup` registrieren.


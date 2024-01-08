[Change 001]    2023-03-03  Letzter Stand Kommunikation zwischen Drohne und RC Ok
                Nur Throttle Werte
                Button müssen aus den interfaceSensor_t genommen werden
                Compilieren OK

[Change 002]    LOGGER und Serial Studio via #define umschaltbar gemacht    
                YAW angepasst
                Button und Switchs neu zugeordnet
                Payloads nicht umsortiert
                private oder protected Variablen mit "_" kennzeichnen ?
                Pitch Horizont noch nicht fertig
                'clearDisplay' was previously declared here
                void Adafruit_PCD8544::clearDisplay(uint8_t color) ????
[Change 003]    Monitor implemented and Serial Studio include 
[Change 004]    TX- und RX Payload geändert   

https://simple-circuit.com/interfacing-arduino-ili9341-tft-display/

Typ: ohne touch panel

Größe: 2,4 "SPI Serielle

Anzeige bereich: 36,72 (W) X48.96 (H) mm

Fahrer element: a-Si TFT aktive matrix

Pixel anordnung: RGB vertikale streifen

Fahrer IC: ILI9341

Hintergrundbeleuchtung: Weiß LED

Betrachtung Richtung: 6 uhr

Farbe Tiefe: 262 K/65 K

Auflösung (punkte): 240RGB * 320 Punkte

5 V kompatibel, verwenden mit 3,3 V oder 5 V logic

Müssen mindestens 4 IOs von ihrem MCU

Mit PCB platte (inklusive netzteil IC, SD), kompatibel mit 5110 interface
                
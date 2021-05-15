# Serielle Relaiskarte ser16x

C und Python code zur Steuerung der seriellen Relaiskarte ser16x.

## Hersteller

mattronik.com,
Frank Matthiensen,
Hans-Fallada-Str. 3,
38442 Wolfsburg - Fallersleben,
05362/728883,
05362/728882 (Fax),
Januar 2007.

Die Karte wurde hier angeboten: [http://www.shop.mattronik.de/product_info.php?cPath=21&products_id=29](https://web.archive.org/web/20071114150236/http://www.shop.mattronik.de/product_info.php?cPath=21&products_id=29) (Aus archive.org, 14. 11. 2007).

Dokumentation: [16 Kanal Relaiskarten RELUSB16X / RELSER16X](serielle_relaiskarte_ser16x.pdf).

## Eigenschaften

Die Karte hat eine serielle Schnittstelle über die sich mit einem einfachen Protokoll die Relais ein und ausschalten lassen.

Entgegen der Dokumentation ist die Schnittstellengeschwindigkeit nicht 9600 sondern 19200 baud.

Das Protokoll ist sehr simpel und führt zu Fehlern, wenn es zB durch Nutzung der falschen Baud-Rate aus dem Tritt gerät, dann muss das Board neu gestartet werden, was nur durch unterbrechen der Versorgungsspannung möglich ist.

## Todo

Im Moment können nur Boards mit 8 Relais geschaltet werden, weil die Argumente an das Programm einstellig sind.


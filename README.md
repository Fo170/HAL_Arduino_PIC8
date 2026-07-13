# HAL_Arduino_PIC8

[![License](https://img.shields.io/badge/license-GPL--3.0-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-PIC16F%2FPIC18F-orange)]()
[![XC8](https://img.shields.io/badge/XC8-2.x%20%7C%203.x-green)]()

**Arduino Uno-compatible HAL** for 8-bit PIC microcontrollers (PIC16F & PIC18F).
Un programme écrit pour Arduino Uno compile sans modification sur un PIC.

- GitHub : [Fo170/HAL_Arduino_PIC8](https://github.com/Fo170/HAL_Arduino_PIC8)
- Version : **1.0.4**

```c
#define HAL_CORE_IMPLEMENTATION
#define HAL_MAIN_IMPLEMENTATION
#include <Arduino.h>

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}
```

Même code compile sur **Arduino Uno** et **PIC16F887** sans modification (avec les deux macros `#define` ajoutées dans le fichier principal).

## MCUs supportés

| Famille | MCU | XC8 define | LED | Analog | PWM | Serial | INT |
|---------|-----|------------|-----|--------|-----|--------|-----|
| PIC16F | 16F876 | `__16F876` | RB0 | 8ch | CCP1/2 | RC6/7 | INT0 |
| PIC16F | 16F877 | `__16F877` | RB0 | 8ch | CCP1/2 | RC6/7 | INT0 |
| PIC16F | 16F877A | `__16F877A` | RB0 | 8ch | CCP1/2 | RC6/7 | INT0 |
| PIC16F | 16F887 | `__16F887` | RB0 | 14ch | CCP1/2 | RC6/7 | INT0 |
| PIC16F | 16F88 | `__16F88` | RB0 | 7ch | CCP1 | RC6/7 | INT0 |
| PIC16F | 16F886 | `__16F886` | RB0 | 14ch | CCP1/2 | RC6/7 | INT0 |
| PIC16F | 16F628A | `__16F628A` | RB0 | — | CCP1/2 | RB1/0 | INT0 |
| PIC18F | 18F4550 | `__18F4550` | RD0 | 13ch | CCP1 | RC6/7 | INT0/1/2 |
| PIC18F | 18F4553 | `__18F4553` | RD0 | 13ch | CCP1 | RC6/7 | INT0/1/2 |
| PIC18F | 18F46K20 | `__18F46K20` | RB0 | — | CCP1/2 | RC6/7 | INT0/1/2 |
| PIC18F | 18F4685 | `__18F4685` | RB0 | — | CCP1/2 | RC6/7 | INT0/1/2 |

## Numérotation des broches

```
pin = port × 8 + bit
```

| Broches | Port |
|---------|------|
| 0–7 | A |
| 8–15 | B |
| 16–23 | C |
| 24–31 | D |
| 32–39 | E |

**PIC18F** : sortie via LATx (read-modify-write safe)  
**PIC16F** : sortie via PORTx

## API complète

### Structure du programme

```c
void setup(void);     // appelée une fois au démarrage
void loop(void);      // appelée en boucle
// main() est fournie par la bibliothèque
```

### GPIO

```c
void pinMode(uint8_t pin, uint8_t mode);     // OUTPUT, INPUT, INPUT_PULLUP
void digitalWrite(uint8_t pin, uint8_t val); // HIGH, LOW
uint8_t digitalRead(uint8_t pin);            // retourne 0 ou 1
void togglePin(uint8_t pin);
```

`pinMode` avec `INPUT_PULLUP` active les pull-ups internes via OPTION_REG.

### Temps

```c
void delay(unsigned long ms);
void delayMicroseconds(unsigned long us);
unsigned long millis(void);    // retourne ms depuis le démarrage
unsigned long micros(void);    // retourne µs depuis le démarrage
```

`millis()` et `micros()` utilisent **Timer1** (16-bit, prescaler 1:1, overflow 1ms).

### Serial (UART)

```c
Serial.begin(unsigned long baud);
Serial.end(void);
int  Serial.read(void);         // -1 si vide
int  Serial.peek(void);         // -1 si vide
uint8_t Serial.available(void);
void Serial.flush(void);
size_t Serial.write(buf, len);
void  Serial_print(const char *str);         // pas de méthode .print() dans la struct
void  Serial_println(const char *str);       // utiliser Serial_print() / Serial_println()
void  Serial_printNumber(unsigned long n, uint8_t base); // base = DEC, HEX
```

- **Buffer RX** : 16 octets circulaire (taille configurable via `SERIAL_RX_BUFFER_SIZE`)
- **Pas de buffer TX** : écriture bloquante (attend TXIF)
- ISR alimente le buffer RX en arrière-plan

### Analogique (ADC)

```c
void analogReference(uint8_t type);   // DEFAULT, EXTERNAL, INTERNAL
uint16_t analogRead(uint8_t pin);     // retourne 0–1023
void analogWrite(uint8_t pin, uint8_t val);  // ⟶ pwmWrite()
```

- `analogWrite()` redirigée vers la PWM (CCP)
- Silencieux sur une broche non-CCP

### PWM (CCP)

```c
void pwmInit(void);
void pwmWrite(uint8_t pin, uint8_t duty);    // 0–255
void pwmFrequency(uint8_t pin, unsigned long freq_hz);
```

- Timer2 partagé sur tous les canaux (présélecteur auto: 1/4/16)
- Duty 0–255 mappé linéairement en 0–100%
- Fréquence par défaut : 5 kHz

**Mapping CCP :**

| MCU | CCP1 | CCP2 |
|-----|------|------|
| 16F877/877A/876 | RC2 (pin 18) | RC1 (pin 17) |
| 16F887/886 | RC2 (pin 18) | RC1 (pin 17) |
| 16F88 | RB0 (pin 8) | — |
| 16F628A | RB0 (pin 8) | RB1 (pin 9) |
| 18F4550/4553 | RC2 (pin 18) | — |
| 18F46K20/4685 | RC2 (pin 18) | RC1 (pin 17) |

### Interruptions

```c
void attachInterrupt(uint8_t pin, voidFuncPtr isr, uint8_t mode);
void detachInterrupt(uint8_t pin);
#define interrupts()   ei()
#define noInterrupts() di()
```

Modes : `LOW`, `CHANGE`, `RISING`, `FALLING`

| Broche | Interruption | PIC16F | PIC18F |
|--------|-------------|--------|--------|
| 8 (RB0) | INT0 | ✓ | ✓ |
| 9 (RB1) | INT1 | — | ✓ |
| 10 (RB2) | INT2 | — | ✓ |
| Port B | Port change | ✓ | ✓ |

### Tone

```c
void tone(uint8_t pin, unsigned int frequency);
void toneDuration(uint8_t pin, unsigned int frequency, unsigned long duration_ms);
void noTone(uint8_t pin);
```

- Utilise Timer2 (partagé avec PWM)
- `tone()` sur un broche CCP désactive la PWM sur le même timer

### Shift

```c
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);   // LSBFIRST, MSBFIRST
void   shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
```

- Implémentation **inline** (bit-bang logiciel)
- Zéro overhead d'appel

### Math

```c
long map(long x, long in_min, long in_max, long out_min, long out_max);
```

Macros : `abs(x)`, `min(a,b)`, `max(a,b)`, `constrain(amt,low,high)`, `sq(x)`

### Aléatoire

```c
void randomSeed(unsigned long seed);
long random(long max);                  // 0 … max-1
long randomRange(long min, long max);   // min … max-1
```

LCG standard : `state = state * 1103515245 + 12345`

### Utilitaires (macros)

```c
#define lowByte(w)   ((uint8_t)((w) & 0xFF))
#define highByte(w)  ((uint8_t)((w) >> 8))
#define bitRead(v,n) (((v) >> (n)) & 1)
#define bitWrite(v,n,b) ...
#define bitSet(v,n)     ((v) |= (1u << (n)))
#define bitClear(v,n)   ((v) &= ~(1u << (n)))
#define bit(n)          (1u << (n))
#define digitalPinToInterrupt(p) (p)
```

### SPI

```c
SPI.begin();
uint8_t SPI.transfer(uint8_t data);
SPI.end();
SPI.setBitOrder(order);        // LSBFIRST, MSBFIRST
SPI.setDataMode(mode);         // SPI_MODE0..3
SPI.setClockDivider(div);      // SPI_CLOCK_DIV2..128
```

- MSSP en mode maître
- Broches : SCK (RC3/pin19), SDO (RC5/pin21), SDI (RC4/pin20)

### I2C (Wire)

```c
Wire.begin();
Wire.beginTransmission(addr);
uint8_t Wire.endTransmission(uint8_t stop);
size_t Wire.write(data, len);
uint8_t Wire.requestFrom(addr, len);
int  Wire.read(void);
uint8_t Wire.available(void);
```

- MSSP en mode I2C maître
- Broches : SCL (RC3/pin19), SDA (RC4/pin20)

## Structure des fichiers

```
HAL_Arduino/
├── library.json
├── include/
│   ├── Arduino.h                   # Header umbrella (include ce fichier)
│   ├── Arduino_types.h             # Typedefs + constantes + macros
│   └── hal/
│       ├── hal_mcu.h               # Auto-détection du MCU
│       ├── hal_pins.h              # Pin mapping par MCU
│       ├── hal_gpio.h              # GPIO inline
│       ├── hal_time.h              # Temps + millis/micros (static inline)
│       ├── hal_serial.h            # Serial + UART (static inline)
│       ├── hal_analog.h            # ADC (static inline)
│       ├── hal_pwm.h               # PWM (static inline)
│       ├── hal_core.h              # ISR unique + globaux + attachInterrupt
│       ├── hal_main.h              # main() → setup() + loop()
│       ├── hal_tone.h              # Tone (static inline)
│       ├── hal_shift.h             # Shift inline
│       ├── hal_math.h              # Map inline
│       ├── hal_random.h            # Random (static inline)
│       ├── hal_spi.h               # SPI master (static inline)
│       └── hal_i2c.h               # I2C master (static inline)
└── config/                         # #pragma config par MCU
    ├── pic16f876.h
    ├── pic16f877.h
    ├── pic16f877a.h
    ├── pic16f887.h
    ├── pic16f88.h
    ├── pic16f628a.h
    ├── pic18f4550.h
    ├── pic18f4553.h
    ├── pic18f46k20.h
    └── pic18f4685.h

examples-platformio/                 # Exemples PlatformIO prêts à l'emploi
├── blink-pic16f877/
├── blink-pic18f4550/
├── serial-echo/
├── pwm-test/
├── analog-read/
├── interrupt-button/
├── tone-melody/
├── pic16f18f/
└── test-18f4550/
```

## Librairie header-only

Cette bibliothèque est **header-only** (aucun fichier `.c`). Il suffit d'ajouter le chemin d'inclusion `-I` dans `build_flags` — pas de lien symbolique, pas de manipulation `lib/`.

> **Pourquoi header-only ?**  
> La plateforme non-officielle **pic8bit** pour PlatformIO utilise un builder (`pic-xc8.py`) qui ne compile que les fichiers `.c` présents dans `lib/*/src/` du projet et ignore complètement `.pio/libdeps/`. Une librairie avec des `.c` obligeait à créer un lien symbolique dans `lib/` pour que les sources soient compilées — solution fragile et contre-intuitive.  
> En convertissant tout le code en `static inline` dans les `.h`, il n'y a plus rien à compiler : le seul `-I` dans `build_flags` suffit, et la librairie fonctionne sur n'importe quelle plateforme sans configuration spéciale.

> **Note plateforme pic8bit** : le builder `pic-xc8.py` ne gère pas `lib_deps` automatiquement.  
> Voir le fichier [`instruction utilisation de pic8bit non-officielle pour PlatformIO.md`](./instruction%20utilisation%20de%20pic8bit%20non-officielle%20pour%20PlatformIO.md) pour les détails (inclut l'installation de XC8).

## Guide de démarrage rapide

### 1. Créer `platformio.ini`

```ini
[env:pic16f887]
platform = pic8bit
board = pic16f887
framework = pic-xc8
build_flags =
    -Iinclude
    -I.pio/libdeps/pic16f887/HAL_Arduino_PIC8/include
lib_deps =
    https://github.com/Fo170/HAL_Arduino_PIC8.git@^1.0.4
```

### 2. Créer `include/config.h`

```c
#ifndef MCU_CONFIG_H
#define MCU_CONFIG_H
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = ON
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#endif
```

Ou utilisez un fichier depuis `config/` correspondant à votre MCU.

### 3. Créer `src/main.c`

Placez les définitions d'implémentation dans UN SEUL fichier `.c` :

```c
#define HAL_CORE_IMPLEMENTATION   // ISR + variables globales
#define HAL_MAIN_IMPLEMENTATION   // main() + init()
#include <xc.h>
#include "config.h"
#include <Arduino.h>

void setup(void) {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
}

void loop(void) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    Serial_println("Hello from PIC!");
}
```

Les autres fichiers `.c` du projet n'ont besoin que de `#include <Arduino.h>` (sans les macros).

### 4. Compiler

```bash
pio run -e pic16f887
```

## Exemples PlatformIO

Dossier `examples-platformio/` — 8 exemples prêts à compiler :

| Exemple | API | MCU |
|---------|-----|-----|
| `blink-pic16f877` | GPIO, delay | 16F877 |
| `blink-pic18f4550` | GPIO, delay (PLL 48 MHz) | 18F4550 |
| `serial-echo` | Serial, UART ISR, buffer RX | 16F877 + 18F4550 |
| `pwm-test` | PWM (CCP), Timer2, fréquence | 16F877 + 18F4550 |
| `analog-read` | ADC, analogRead, Serial print | 16F877 + 18F4550 |
| `interrupt-button` | attachInterrupt, callback INT0 | 16F877 + 18F4550 |
| `tone-melody` | tone, noTone, Timer2 | 16F877 + 18F4550 |
| `pic16f18f` / `test-18f4550` | Bare metal (XC8 direct) | 16F877 / 18F4550 |

```bash
cd examples-platformio/blink-pic18f4550
pio run -e pic18f4550
```

## Gestion des timers

| Timer | Usage | Priorité |
|-------|-------|----------|
| Timer0 | Réservé (futur) | — |
| Timer1 | `millis()` / `micros()` (overflow 1ms) | Fixe |
| Timer2 | PWM (CCP1/2) + `tone()` (partagé) | Partagé |

- `tone()` désactive la PWM sur le timer2 quand il est actif
- `pwmWrite()` et `tone()` peuvent cohabiter si sur le même timer (écrasement du PR2)

## Optimisations

| Technique | Modules | Overhead |
|-----------|---------|----------|
| `static inline` | GPIO, delay, shiftIn/Out, map | 0 (inliné) |
| `#define` | bitRead, lowByte, abs, min, max, constrain | 0 |
| struct C99 | Serial, SPI, Wire | 1 indirection |
| XC8 built-ins | `__delay_ms()`, `__delay_us()`, `ei()`, `di()` | direct SFR |
| ISR unique | Toutes les sources | flag polling |

## Changelog

### v1.0.4 — Corrections PIC16F + warnings zéro

- **PIC16F : correction des noms de registres** — `INT0IF` → `INTF`, `INTEDG0` → `INTEDG`, `INT0IE` → `INTE` dans `attachInterrupt()` et l'ISR
- **I2C : portabilité PIC16F** — remplacement de `SSPSTATbits.R_NOT_W` par `(SSPSTAT & 0x04)` (bit R/W brut)
- **`A0`..`A26` définis** — alias Arduino `#define A0 PIN_A0` etc. dans `hal_pins.h`
- **Warnings supprimés** : `-Wno-2053` (fonctions non appelées), `-Wno-759`/`-Wno-2098` (appels indirects dans l'ISR)
- **Warnings code corrigés** : casts implicites `int→unsigned char` dans `hal_pwm.h`/`hal_spi.h`, comparaison `uint8_t > 255` dans `hal_pwm.h`/`hal_tone.h`
- **Exemples propres** — 7 exemples × 2 MCU = 14 compilations, zéro erreur, zéro warning

## Licence

GPL-3.0

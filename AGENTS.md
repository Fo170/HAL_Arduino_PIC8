# AGENTS.md — HAL_Arduino_PIC8

Conventions pour les agents IA travaillant sur cette bibliothèque.

## Structure

```
HAL_Arduino/
├── include/
│   ├── Arduino.h              # Umbrella — inclut tout
│   ├── Arduino_types.h        # Constants + macros utilisateur
│   └── hal/                   # Headers internes (préfixe hal_)
│       ├── hal_core.h         # ISR unique + globaux + attachInterrupt
│       ├── hal_main.h         # main() + init() (implémentation unique)
│       ├── hal_*.h            # Modules header-only (static inline)
└── config/                    # #pragma config headers
```

**Header-only** : pas de fichiers `.c`. Tout le code est dans les `.h` en `static inline`.
Les définitions d'implémentation unique (ISR, variables globales, main) sont activées par :

```c
#define HAL_CORE_IMPLEMENTATION   // dans 1 seul .c
#define HAL_MAIN_IMPLEMENTATION   // dans 1 seul .c
```

## Conventions de code

- **Langage** : C99 (XC8 ne supporte pas C++ sur PIC16F)
- **Typage** : `#include <stdint.h>` → `uint8_t`, `uint16_t`, `uint32_t`
- **Préfixe** : `hal_` pour les fonctions/variables internes
- **API publique** : Noms Arduino (`pinMode`, `Serial.begin()`, etc.)
- **Syntaxe objet** : struct C99 avec pointeurs de fonction, instance `static const` dans le .h
  ```c
  typedef struct {
      void (*begin)(unsigned long);
  } hal_serial_t;
  static const hal_serial_t Serial = { .begin = Serial_begin };
  ```
- **Inline** : tout le code fonctionnel → `static inline` dans le .h
- **Macros** : bitRead, lowByte, abs, min → `#define`
- **ISR** : UNIQUE dans `hal_core.h` sous `#ifdef HAL_CORE_IMPLEMENTATION`
- **Built-ins XC8** : `__delay_ms()`, `__delay_us()`, `ei()`, `di()` à privilégier

## MCU auto-détection

Dans `hal_mcu.h` via les définitions XC8 :

```c
_PIC14 / _PIC14E  →  HAL_FAMILY_PIC16   (PIC16F)
_PIC18            →  HAL_FAMILY_PIC18   (PIC18F)
__16F877          →  HAL_MCU_16F877
__18F4550         →  HAL_MCU_18F4550
```

Les #ifdef differencient PIC16F (PORTx, pas de LAT) et PIC18F (LATx, plus de périphériques).

## Différences PIC16F vs PIC18F

| Aspect | PIC16F | PIC18F |
|--------|--------|--------|
| Sortie | `PORTx` | `LATx` (safe RMW) |
| Mots instruction | 14-bit | 16-bit |
| RAM | 128–368 octets | 512–2048+ |
| Vecteur ISR | 1 (0x0004) | 2 (haute 0x0008, basse 0x0018) |
| Noms SFR | `TMR1IF` (macro) | `PIR1bits.TMR1IF` (bitfield) |
| Interruptions ext. | INT0 seul | INT0 + INT1 + INT2 |

## Ajouter un nouveau MCU

1. Créer `config/picXXxxxx.h` avec les `#pragma config`
2. Ajouter au `#elif defined(__XXxxxx)` dans `hal_mcu.h`
3. Ajouter `#define` pour `LED_BUILTIN`, `PIN_SERIAL_TX/RX` dans `hal_pins.h`
4. Ajouter les broches analogiques (`PIN_A0`, etc.) et `NUM_ANALOG_PINS` dans `hal_pins.h`
5. Ajouter le mapping CCP dans `hal_pwm.h` (`pwm_pin_to_ccp()`)
6. Ajouter le pin-to-channel ADC dans `hal_analog.h` si différent

## Compilation

```bash
# PlatformIO
pio run -e pic16f887
pio run -e pic18f4550

# Direct XC8 (un seul fichier .c avec les defines)
xc8 -mcpu=16F887 --chip=16F887 main.c
```

### Utilisation

Dans le fichier principal du projet (un seul `.c`) :

```c
#define HAL_CORE_IMPLEMENTATION
#define HAL_MAIN_IMPLEMENTATION
#include <Arduino.h>

void setup(void) { ... }
void loop(void)  { ... }
```

Les autres `.c` du projet n'ont besoin que de `#include <Arduino.h>`.

### Plateforme pic8bit — Particularité

Le builder `pic-xc8.py` de la plateforme pic8bit **ne supporte pas** `lib_deps` automatiquement.
Comme la librairie est header-only, il suffit d'ajouter le chemin `-I` dans `build_flags` :

```ini
build_flags = -I.pio/libdeps/<env>/HAL_Arduino_PIC8/include
```

Plus besoin de lien symbolique ni de copie dans `lib/`.

Voir le fichier [`instruction utilisation de pic8bit non-officielle pour PlatformIO.md`](../instruction%20utilisation%20de%20pic8bit%20non-officielle%20pour%20PlatformIO.md) pour les détails (inclut l'installation de XC8).

## Tests

Pas de framework de test automatisé. Vérifier :
- Compilation sans warning pour chaque MCU cible
- Blink.hex < 1 Ko flash
- AnalogRead retourne 0–1023
- Serial.begin(9600) + écho fonctionnel
- millis() monotone, micros() précis à ~2µs

## Règles

- Ne PAS utiliser `new`/`delete` ou toute fonction C++
- Ne PAS utiliser `malloc`/`free` (PIC16F RAM limitée)
- Ne PAS utiliser de types flottants dans les ISR
- Toujours protéger les accès 32-bit avec GIE=0 / GIE=1
- RX buffer = 16 octets (configurable) — compromis RAM/tampon
- Toujours documenter les registres SFR utilisés dans chaque module
- Toujours utiliser `static inline` dans les .h (pas de .c) sauf pour les définitions uniques
- Les variables globales partagées entre modules (ISR + API) sont déclarées `extern` dans chaque .h et définies dans `hal_core.h` sous `HAL_CORE_IMPLEMENTATION`
- Les instances de struct (Serial, SPI, Wire) sont `static const` dans le .h (chaque TU reçoit sa copie ; sur PIC mono-TU c'est transparent)

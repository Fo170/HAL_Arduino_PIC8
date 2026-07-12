# AGENTS.md — HAL_Arduino_PIC8

Conventions pour les agents IA travaillant sur cette bibliothèque.

## Structure

```
HAL_Arduino/
├── include/
│   ├── Arduino.h              # Umbrella — inclut tout
│   ├── Arduino_types.h        # Constants + macros utilisateur
│   └── hal/                   # Headers internes (préfixe hal_)
├── src/
│   ├── hal_core.c             # ISR unique + shared state
│   ├── hal_*.c                # Un fichier .c par module
│   └── hal_main.c             # Point d'entrée main() → setup()/loop()
└── config/                    # #pragma config headers
```

## Conventions de code

- **Langage** : C99 (XC8 ne supporte pas C++ sur PIC16F)
- **Typage** : `#include <stdint.h>` → `uint8_t`, `uint16_t`, `uint32_t`
- **Préfixe** : `hal_` pour les fonctions/variables internes
- **API publique** : Noms Arduino (`pinMode`, `Serial.begin()`, etc.)
- **Syntaxe objet** : struct C99 avec pointeurs de fonction
  ```c
  typedef struct {
      void (*begin)(unsigned long);
  } hal_serial_t;
  extern const hal_serial_t Serial;
  ```
- **Inline** : GPIO, delay, shift, math → `static inline` dans le .h
- **Macros** : bitRead, lowByte, abs, min → `#define`
- **ISR** : UNIQUE dans `hal_core.c` (PIC = 1 seul vecteur d'interruption)
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
5. Ajouter le mapping CCP dans `hal_pwm.c` (`pwm_pin_to_ccp()`)
6. Ajouter le pin-to-channel ADC dans `hal_analog.c` si différent

## Compilation

```bash
# PlatformIO
pio run -e pic16f887
pio run -e pic18f4550

# Direct XC8
xc8 -mcpu=16F887 --chip=16F887 main.c
```

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

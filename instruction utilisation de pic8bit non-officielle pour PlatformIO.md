# Connaissances PlatformIO + pic8bit / HAL_Arduino_PIC8

## Contexte

Ce workspace utilise une plateforme **pic8bit** non-officielle pour PlatformIO, qui utilise `xc8-wrapper` pour compiler du code C vers les microcontrôleurs PIC 8-bit (PIC16F, PIC18F).

## Installation du compilateur XC8

Le compilateur XC8 de Microchip est nécessaire pour compiler le code. La plateforme pic8bit l'utilise via `xc8-wrapper`.

### Sous Linux

1. Télécharger XC8 depuis le site de Microchip (ou via `wget`) :
   ```bash
   wget https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/xc8-v3.00-full-linux-installer.tar.gz
   ```
   *L'URL exacte change selon les versions ; chercher "XC8 compiler Linux" sur microchip.com*

2. Extraire et installer :
   ```bash
   tar -xzf xc8-v3.00-full-linux-installer.tar.gz
   sudo ./xc8-v3.00-full-linux-installer.run
   ```
   Suivre l'assistant (installation par défaut dans `/opt/microchip/xc8/v3.00/`).

3. Ajouter au PATH (dans `~/.bashrc` ou `~/.zshrc`) :
   ```bash
   export PATH=$PATH:/opt/microchip/xc8/v3.00/bin
   ```

4. Recharger le profil :
   ```bash
   source ~/.bashrc
   ```

### Sous Windows

1. Télécharger l'installateur Windows depuis le [site Microchip](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers/xc8).

2. Lancer l'installateur `.exe` ou `.msi` en tant qu'administrateur.

3. Suivre l'assistant (installation par défaut dans `C:\Program Files\Microchip\xc8\v3.00\`).

4. Ajouter au PATH système :
   - Ouvrir **Paramètres système avancés** → **Variables d'environnement**
   - Ajouter `C:\Program Files\Microchip\xc8\v3.00\bin` à la variable `Path`

### Vérification

```bash
xc8 --version
# Doit afficher : Microchip (MPLAB XC8) v3.00 ...
```

### Device Family Pack (DFP)

XC8 a besoin des définitions de périphériques (DFP) pour chaque famille de MCU. PlatformIO télécharge automatiquement les DFP via la plateforme pic8bit, mais vous pouvez aussi spécifier un chemin explicite dans `build_flags` :

```ini
build_flags = 
    -mdfp=/opt/microchip/mplabx/v6.00/packs/Microchip/PIC18Fxxxx_DFP/1.3.36/xc8
```

Sous Windows, le chemin typique est :
```ini
build_flags = 
    -mdfp=C:/Program\ Files/Microchip/MPLABX/v6.00/packs/Microchip/PIC18Fxxxx_DFP/1.3.36/xc8
```

> **Astuce** : Si `build_flags` n'est pas renseigné, la plateforme pic8bit cherche le DFP automatiquement. En cas d'erreur "Cannot find DFP", ajoutez le flag `-mdfp` avec le chemin de votre installation MPLAB X.

## Problème connu : lib_deps non supporté

Le builder `pic-xc8.py` de cette plateforme **ne gère pas** automatiquement les librairies installées via `lib_deps`. En particulier :

1. **Les chemins d'inclusion (`-I`)** ne sont pas ajoutés automatiquement pour :
   - Le dossier `include/` du projet
   - Les dossiers `include/` des librairies installées

2. **Les fichiers source des librairies** ne sont pas compilés automatiquement.

## Solution pour utiliser HAL_Arduino_PIC8 (header-only)

**HAL_Arduino_PIC8 est désormais header-only** : pas de fichiers `.c` à compiler.  
Le projet utilisateur active les définitions uniques (ISR, main) via des macros dans un seul `.c`.

### Prérequis
- Déclarer la librairie dans `lib_deps` du `platformio.ini`
- Exécuter `pio run` une première fois pour télécharger la librairie dans `.pio/libdeps/<env>/`

### Étapes

1. **Ajoute les chemins d'inclusion** dans `build_flags` de `platformio.ini` :
   ```ini
   build_flags = 
       -mdfp=/opt/microchip/mplabx/v6.00/packs/Microchip/PIC18Fxxxx_DFP/1.3.36/xc8
       -Iinclude
       -I.pio/libdeps/pic18f4550/HAL_Arduino_PIC8/include
   ```

2. **Dans le fichier principal `.c`**, active les implémentations uniques :
   ```c
   #define HAL_CORE_IMPLEMENTATION
   #define HAL_MAIN_IMPLEMENTATION
   #include <Arduino.h>
   
   void setup(void) { ... }
   void loop(void)  { ... }
   ```

   Les autres fichiers `.c` du projet n'ont besoin que de `#include <Arduino.h>`.

### Particularités de HAL_Arduino_PIC8

- Fournit `main()` et `init()` via `#define HAL_MAIN_IMPLEMENTATION`
- L'ISR unique et les variables globales sont activées par `#define HAL_CORE_IMPLEMENTATION`
- Les fichiers de configuration MCU sont dans `config/` (ex: `config/pic18f4550.h`)
- Le projet doit avoir son propre `include/config.h` avec les `#pragma config` pour le MCU cible

### Fichier `library.json` de HAL_Arduino_PIC8
```json
{
  "name": "HAL_Arduino_PIC8",
  "frameworks": ["pic-xc8"],
  "platforms": ["pic8bit"]
}
```

## Structure du builder pic-xc8

Le fichier clé est :
```
~/.platformio/platforms/pic8bit/builder/frameworks/pic-xc8.py
```

Points importants :
- Collection des sources : `get_project_sources()` scanne `src/` et `lib/*/src/`
- Les flags sont passés via `build_flags` dans `platformio.ini`
- Pas de support automatique des `-I` paths pour les librairies
- Pas de support automatique du `lib_extra_dirs` ou `lib_search_paths` standard de PlatformIO
- **HAL_Arduino_PIC8 étant header-only** (pas de `.c`), plus besoin de lien symbolique `lib/` — seul le chemin `-I` suffit

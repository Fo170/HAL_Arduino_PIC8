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

## Solution pour utiliser HAL_Arduino_PIC8 (ou toute librairie externe)

### Prérequis
- La librairie doit être déclarée dans `lib_deps` dans `platformio.ini`
- Exécuter `pio run` une première fois pour télécharger la librairie dans `.pio/libdeps/<env>/`

### Étapes

1. **Crée un lien symbolique** de la librairie dans `lib/` du projet :
   ```bash
   mkdir -p lib
   ln -s ../.pio/libdeps/pic18f4550/HAL_Arduino_PIC8 lib/HAL_Arduino_PIC8
   ```
   Le builder scanne `lib/*/src/*.c` pour les sources additionnelles.

2. **Ajoute les chemins d'inclusion** dans `build_flags` de `platformio.ini` :
   ```ini
   build_flags = 
       -mdfp=/opt/microchip/mplabx/v6.00/packs/Microchip/PIC18Fxxxx_DFP/1.3.36/xc8
       -Iinclude
       -I.pio/libdeps/pic18f4550/HAL_Arduino_PIC8/include
   ```

### Particularités de HAL_Arduino_PIC8

- Fournit son propre `main()` dans `src/hal_main.c` qui appelle `init()` → `setup()` → `loop()`
- Le projet utilisateur n'a besoin que de définir `setup()` et `loop()` (pas de `main()`)
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

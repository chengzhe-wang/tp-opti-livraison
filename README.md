# tp-opti-livraison

## Lancer le programme sur une autre machine

### Prérequis

Un compilateur C doit être installé : `clang` ou `gcc`.

- macOS : installer les outils de développement Apple avec `xcode-select --install` si nécessaire.
- Linux : installer GCC avec le gestionnaire de paquets de la distribution.

### Compilation et exécution

Dans un terminal, se placer à la racine du projet puis exécuter :

```bash
cc src/*.c -o programme && ./programme
```

Cette commande compile tous les fichiers `.c` du dossier `src`, crée l'exécutable `programme`, puis le lance.

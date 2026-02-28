# Malloc — Implémentation d'un allocateur mémoire

Implémentation from scratch de malloc, free, calloc et realloc en C,
sous forme de bibliothèque partagée (.so) préchargeable via LD_PRELOAD.

## Fonctionnalités

- `malloc` — allocation mémoire alignée sur long double
- `free` — libération et réutilisation immédiate des blocs
- `calloc` — allocation initialisée à zéro avec vérification overflow
- `realloc` — réallocation avec extension intelligente de l'espace

## Contraintes techniques

- Uniquement `mmap`, `munmap`, `mremap`, `sysconf`
- Aucun appel à la libc malloc
- Compilé en bibliothèque partagée : `libmalloc.so`
- Thread-safe (mutex pthread)
- Compatible LD_PRELOAD

## Compilation
```bash
make        # génère libmalloc.so
make check  # lance les tests
make clean  # nettoie
```

## Utilisation
```bash
# Précharger la bibliothèque
LD_PRELOAD=./libmalloc.so ls
LD_PRELOAD=./libmalloc.so cat Makefile

# Tester avec les binaires fournis
LD_PRELOAD=./libmalloc.so ./speed
LD_PRELOAD=./libmalloc.so ./memoryfootprint 512
```

## Technologies

- Langage : C (C99)
- Mémoire : mmap/munmap/mremap
- Thread safety : pthread mutex
- Build : Makefile → shared library (.so)

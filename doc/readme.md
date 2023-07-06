
# Table of Contents

1.  [Jeu par apprentissage de règles](#org50c7a60)
    1.  [Résumé](#org73bfdbe)
    2.  [Pour commencer](#org79532b9)
        1.  [Pré-requis :](#org6263a46)
    3.  [Démarrage](#org13c4b41)
        1.  [Linux](#orgf4db39c)
        2.  [Windows](#org3abf490)
    4.  [Outils](#org30bd0d9)
    5.  [Versions](#org6e70555)
    6.  [Auteurs](#orga84cf8e)
        1.  [Responsable de projet](#org4bb725f)
        2.  [Equipe de développement](#org9e313f9)
    7.  [Licence](#org49927dd)


<a id="org50c7a60"></a>

# Jeu par apprentissage de règles


<a id="org73bfdbe"></a>

## Résumé

Un écosystème de type proie prédateurs est dans sa version la plus simple :

Un monde : par exemple une grille à maille rectangulaire, avec ou sans quelques obstacles, avec sa topologie (rectangulaire ou torique le plus souvent) des proies et des prédateurs sont placés sur le monde, le système fonctionne à temps discret : à chaque date de temps, les créatures réalisent une action les prédateurs peuvent ’manger’ les proies (dès qu’il y a contact, dès qu’au moins deux prédateurs entourent une proie, …) les proies essaient de survivre en se sauvant au mieux de leurs capacités.

Dans une version à peine étendue, on retrouve le jeu PACMAN, mais on peut rentrer dans ce cadre aussi bien un jeu de raquettes (la balle est la proie, le prédateur la raquette), un shooter, … que des simulations environnementales ou économiques.


<a id="org79532b9"></a>

## Pour commencer


<a id="org6263a46"></a>

### Pré-requis :

1.  C (standard)

    Liste des bibliothèques utilisées :
    
    -   `stdio.h`
    -   `stdlib.h`
    -   `threads.h`
    -   `string.h`
    -   `math.h`
    -   `dirent.h`
    -   `stdarg.h`
    -   `stdbool.h`
    -   `float.h`
    -   `limits.h`
    -   `unistd.h`
    -   `time.h`
    -   `errno.h`
    
    Liste des bibliothèques tierces:
    
    -   `SDL2/SDL.h`
    -   `SDL2/SDL_image.h`
    -   `SDL2/SDL_ttf.h`
    
    Installation SDL2:
    
        git clone https://github.com/libsdl-org/SDL.git -b SDL2
        cd SDL
        mkdir build
        cd build
        ../configure
        make
        sudo make install


<a id="org13c4b41"></a>

## Démarrage


<a id="orgf4db39c"></a>

### Linux

1.  Seance 3

        cd build
        make -Bj
        ./app
    
    Liste des arguments:
    
    Premier Argument:
    `-l` : activer tout les logs
    `-d` : activer tout les logs sauf les debugs
    `-n` : aucun log d'activer
    
    Deuxième Argument
    `--parallel` : lancer une simulation parallèle (défaut: 8 threads)
    `--load` : lancer avec le chargement de la dernière seed sauvegardée


<a id="org3abf490"></a>

### Windows

Aucune compabilité garantie


<a id="org30bd0d9"></a>

## Outils

-   Emacs
-   GDB
-   Valgrind
-   Doxygen
-   VSCode


<a id="org6e70555"></a>

## Versions

**Dernière version stable :** 1.0

**Dernière version :** 1.0

[Liste des versions](../../tags)


<a id="orga84cf8e"></a>

## Auteurs


<a id="org4bb725f"></a>

### Responsable de projet

-   DANIEL Yves-Jean


<a id="org9e313f9"></a>

### Equipe de développement

-   DUREL Enzo
-   LEROY Louis
-   VILLEPREUX Thibault


<a id="org49927dd"></a>

## Licence

Le projet est **open-source**



# Table of Contents

1.  [Jeu par apprentissage de règles](#org6c90627)
    1.  [Résumé](#org8ddccd5)
    2.  [Pour commencer](#org6c97ad5)
        1.  [Pré-requis :](#orge360b62)
    3.  [Démarrage](#org16a8225)
        1.  [Linux](#org7820482)
        2.  [Windows](#org545fc2c)
    4.  [Outils](#org26ae861)
    5.  [Versions](#orgadb71ec)
    6.  [Auteurs](#org482fbb3)
        1.  [Responsable de projet](#org3f89df5)
        2.  [Equipe de développement](#org4a37a64)
    7.  [Licence](#org953648e)
    8.  [Documentation](#org03cb309)


<a id="org6c90627"></a>

# Jeu par apprentissage de règles


<a id="org8ddccd5"></a>

## Résumé

Un écosystème de type proie prédateurs est dans sa version la plus simple :

Un monde : par exemple une grille à maille rectangulaire, avec ou sans quelques obstacles, avec sa topologie (rectangulaire ou torique le plus souvent) des proies et des prédateurs sont placés sur le monde, le système fonctionne à temps discret : à chaque date de temps, les créatures réalisent une action les prédateurs peuvent ’manger’ les proies (dès qu’il y a contact, dès qu’au moins deux prédateurs entourent une proie, …) les proies essaient de survivre en se sauvant au mieux de leurs capacités.

Dans une version à peine étendue, on retrouve le jeu PACMAN, mais on peut rentrer dans ce cadre aussi bien un jeu de raquettes (la balle est la proie, le prédateur la raquette), un shooter, … que des simulations environnementales ou économiques.


<a id="org6c97ad5"></a>

## Pour commencer


<a id="orge360b62"></a>

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


<a id="org16a8225"></a>

## Démarrage


<a id="org7820482"></a>

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


<a id="org545fc2c"></a>

### Windows

Aucune compabilité garantie


<a id="org26ae861"></a>

## Outils

-   Emacs
-   GDB
-   Valgrind
-   Doxygen
-   VSCode


<a id="orgadb71ec"></a>

## Versions

**Dernière version stable :** 1.0

**Dernière version :** 1.0

[Liste des versions](../../tags)


<a id="org482fbb3"></a>

## Auteurs


<a id="org3f89df5"></a>

### Responsable de projet

-   DANIEL Yves-Jean


<a id="org4a37a64"></a>

### Equipe de développement

-   DUREL Enzo
-   LEROY Louis
-   VILLEPREUX Thibault


<a id="org953648e"></a>

## Licence

Le projet est **open-source**


<a id="org03cb309"></a>

## Documentation

Lien vers la documentation: <https://perso.isima.fr/~endurel/zz1/projet/doc_pp/html/index.html>


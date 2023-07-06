# Jeu par apprentissage de règles

<a id="org2fcdf04"></a>

## Résumé

Le projet se décompose en deux phases, correspondant chacune approximativement à une semaine de travail. La première phase est commune à tous les groupes et met en place des outils permettant de manipuler des images et d’interagir avec l’utilisateur. Cette phase est ponctuée de petits exercices assez libres où vous devrez démontrer votre maîtrise des points abordés en individuel et se termine par la création d’un petit jeu.

La seconde phase, plus spécifique à chaque groupe, consiste à réinvestir les acquis de la première phase afin de créer un prototype de votre propre « jeu vidéo » respectant la thématique du projet.


<a id="orge391187"></a>

## Pour commencer


<a id="org4464b70"></a>

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


<a id="orgc5ddb01"></a>

## Démarrage


<a id="orge25c5ed"></a>

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


<a id="org490d11f"></a>

### Windows

Aucune compabilité garantie


<a id="orge0d928b"></a>

## Outils

-   Emacs
-   GDB
-   Valgrind
-   Doxygen
-   VSCode


<a id="orge90245f"></a>

## Versions

**Dernière version stable :** 1.0

**Dernière version :** 1.0

[Liste des versions](../../tags)


<a id="org4352090"></a>

## Auteurs


<a id="org13ab56a"></a>

### Responsable de projet

-   DANIEL Yves-Jean


<a id="orgadd82c4"></a>

### Equipe de développement

-   DUREL Enzo
-   LEROY Louis
-   VILLEPREUX Thibault


<a id="orgdbf0b69"></a>

## Licence

Le projet est **open-source**


# Tetris

Installation :

Ce projet utilise 2 librairies externes :
wxWidgets 3.2.2.1
ENet 1.3.17

J'utilise Visual Studio 2019 donc voici les étapes pour configurer les librairies si vous l'utilisez

Pour wxWidget :

Téléchargez la bonne version et extrayez les fichiers
Accédez au dossier build/msw et ouvrez wx_vc16.sln (pour visual studio 2017 ce serait wx_vc15.sln)  
Construire les configurations 64 bits (debug et release en x64)
Sur votre PC ajoutez une variable d'environnement / variable système, nom : WXWIN, valeur : répertoire du fichier wxWidgets que vous avez extraits  
Dans le projet, ouvrez le gestionnaire de propriétés (view -> other windows -> property manager), utilisez-le pour faire "add existing property sheet" et sélectionnez wxwidgets.props dans le répertoire wxWidgets  
Dans les propriétés du projet, assurez-vous dans l'onglet advanced que character set utilise unicode character set  
Dans  linker -> system, mettez le subsystem à Windows  

Pour ENet :

Téléchargez la bonne version et extrayez les fichiers  
Dans les propriétés du projet, onglet VC++ Directories, ajoutez a include directories le chemin d'accès au dossier include d'ENet  
Dans linker -> input, ajoutez le chemin vers enet64.lib  

Voici les liens des librairies pour les installer pour d'autres compilateurs ou os:

https://www.wxwidgets.org/downloads/

http://enet.bespin.org/Installation.html

.

Remarques :

Les mouvements se font avec W A S D, P pour pause, space pour fair un quick drop 

Le mode multijoueurs ne marche pas, le client arrive à s'y connecter mais le serveur ne le détecte pas, ce mode est pour l'instant un "work in progress"

La majorité des règlent et informations sur le jeu de tetris utilisées dans ce projet proviennent de
https://tetris.fandom.com/wiki/Tetris_Wiki

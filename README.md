# Project2026

## Objectif du projet

Le projet va traiter de la création d'une application pour travailler son calcul mental implémenté en C++ et en utilisant l'orienté objet.
Dans l'exemple fournit, il s'agit de tester l'utilisateur sur sa capacité à réaliser des multiplications.

## Rendus

Beaucoup d'informations sur le travail réalisé pourront être proposées soit à travers des commentaires directement dans le code, soit à travers un document (README ou autre) intégré au dépôt.

Mais dans tous les cas, je souhaite disposer d'un README pour m'indiquer :
* Comment compiler votre code.
* Comment exécuter votre code.
* Quelles sont les différentes fonctionnalités implémentées.
* Où trouver les différents composants de votre code.

Pour rappel, il n'y a aucune obligation d'inclure dans le projet un maximum de design patterns.
Si vous souhaitez en utiliser car cela vous semble pertinent, c'est très bien. Mais ce n'est pas une obligation et l'utilisation à mauvais escient d'un design pattern simplement pour utiliser un design pattern sera pénalisante.

### Fonctionnalités

L'application doit pouvoir de base proposer des questions de calcul mental à l'utilisateur et lui permettre d'y répondre. Il s'agit de la fonctionnalité de base du programme.

Autour de ça, vous devez rajouter trois fonctionnalités supplémentaires au choix.
On peut imaginer par exemple :
- gestion de niveau de difficulté,
- gestion d'une série de questions chronométrées,
- gestion d'un historique des questions posées et des réponses données,
- sérialisation/désérialisation des questions et réponses vues,
- etc.

En plus de ces fonctionnalités, vous devez implémenter :
- La gestion d'une mascotte que l'on pourra choisir d'afficher ou de cacher.
- Une fonctionnalité de votre choix liée à la gamification. Il peut s'agir par exemple :
    - d'un système de points qui permettent de débloquer des features,
    - d'un système de badges, de récompenses,
    - d'un système de classement,
    - d'un système de défis,
    - etc.

### Dépôt git

Je vous demande de fournir l'historique git des commits réalisés pour ce projet.
Dans ce cadre, j'attends à ce que cet historique soit clair et compréhensible. Il doit être possible de comprendre à travers les messages de commit ce qui a été réalisé à chaque étape du projet.

### Architecture du projet

Il n'y a pas d'architecture imposée pour ce projet.
Vous êtes libres de choisir l'architecture qui vous semble la plus pertinente pour réaliser ce projet.
Pour autant il faut que celle-ci soit claire et compréhensible. Il doit être facile de comprendre comment le projet est organisé et comment les différentes composantes interagissent entre elles.

### Séparation des fonctionnalités

Les différentes fonctionnalités doivent être implémentées de manière séparée de façon à faciliter la maintenance et l'évolutivité du code.
De manière générale, il faut essayer de ne laisser qu'une responsabilité à chaque composant : une classe a une responsabilité, une fonction a une responsabilité, etc.

C'est le cas entre autre de l'interface graphique. Il doit être possible de changer l'interface graphique sans avoir à changer le code métier.

### Qualité du code
De manière plus générale, au delà des fonctionnalités implémentées, vous serez notés également sur la qualité du code réalisé. Il s'agit donc de faire un code plus propre que lors d'un TP classique.
Je pense par exemple :
* à nommer correctement ses variables et ses fonctions,
* à gérer correctement les erreurs,
* à limiter la duplication de code,
* à regrouper le code dans des fonctions qui ont du sens, etc.

### Explications des choix techniques
Je souhaite que chaque composante de cette implémentation soit expliquée.

Il s’agit de commentaires qui expliqueraient à celui qui lit votre code pourquoi vous avez fait tel ou tel choix technique.


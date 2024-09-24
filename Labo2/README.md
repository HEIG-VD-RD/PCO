# Rapport du labo 2 PCO : Cracking MD5

#### Auteurs : Rafael Dousse et Massimo Stefani

## Question 1 : Description de l'implémentation

L'implémentation du laboratoire visait à réaliser une attaque par force brute pour déchiffrer un mot de passe à l'aide
de threads, dans le cadre du cours de programmation concurrente.

Pour ce faire nous avons "découpé" l'arbre de recherche en plusieurs parties, chacune étant traitée par un thread.
Chaque thread a donc un charset de caractères fixes qu'il placera au début de la chaîne de caractères à tester. Ensuite,
pour les caractères suivants, il va tester toutes les combinaisons possibles avec le charset commun à tous les threads.
Cela permet de réduire le nombre de combinaisons à tester par thread, et donc de réduire le temps d'exécution de l'
attaque par force brute.

## Question 2 : Les différentes étapes de l'implémentation

Nous avons pris la liberté de faire quelques modifications au code fourni pour une meilleure encapsulation, une
meilleure comprehension et une meilleure gestion des threads. L'implémentation du laboratoire s'est déroulée en
plusieurs étapes clés :

1. **Modification de la classe `ThreadManager`** : En tant que groupe, nous avons commencé par étendre la
   classe `ThreadManager` en ajoutant de nouveaux attributs et méthodes. Cela incluait l'ajout des
   attributs `charset`, `hash`, `salt`, `result`, `passLength`, et `nbToCompute`, ainsi que la création des
   méthodes `fillThreadManager`, `prepareThreads`, `clean`, `startThreads`, `waitThreads`, et `deleteThreads`. Le but
   était de rendre une classe `ThreadManager` capable de stocker les informations commune à tous les threads pour
   effectuer l'attaque et de gérer les `myThreds` qui sont créées.


2. **Création des threads** : Nous avons mis en place la création des threads en instanciant la classe `MyThread`.
   Chaque thread s'est vu attribuer une partie du charset pour générer des combinaisons. Comme attribut, chaque myThread
   a reçu un pointeur vers le `ThreadManager` pour pouvoir accéder aux informations communes à tous les threads, chaque
   myThread a également un pointeur sur `PcoThread` nous permettant de créer le thread qu'on utilise dans le cadre de ce
   cours. Chaque MyThread a également le nombre d'itérations qu'il a effectué.


3. **Algorithme de distribution du charset** : La distribution du charset se fait grâce à la
   methode `getCharsetToCompute`, qui est appelée par la méthode `prepareThreads`. Cette méthode permet de distribuer le
   charset commun à tous les threads en fonction du nombre de threads et de la longueur du charset commun. Il essaie de
   distribuer le charset de manière équitable entre les threads, en attribuant à chaque thread un nombre de caractères
   égal ou proche du nombre de caractères du charset commun divisé par le nombre de threads. Si le nombre de caractères
   du charset commun n'est pas divisible par le nombre de threads, on distribue les caractères restants aux autres
   threads.


4. **Algorithmes de génération de combinaisons** : Nous avons développé un algorithme dans la
   méthode `tryAllCombinations` de chaque thread pour générer de manière itérative toutes les combinaisons possibles du
   mot de passe à partir du charset qui lui a été assigné et le charset commun. Cette fonction a une complexité en O(n*
   m) où n est la longueur du charset commun et m la longueur du charset du thread.


5. **Comparaison des hachages** : À chaque tentative de combinaison, nous avons calculé le hachage MD5 du mot de passe
   et l'avons comparé au hachage cible pour vérifier si le mot de passe avait été trouvé.


6. **Arrêt de l'exécution** : Lorsqu'un thread réussissait à trouver le mot de passe, il mettait à jour la
   variable `manager->result`, ce qui déclenchait l'arrêt de l'exécution de tous les autres threads. Étant donné que le
   mot de passe est unique, un seul thread réussissait à atteindre cet objectif. Ce qui nous enlève la nécessité de
   mettre en place un mécanisme de blocage entre les threads.

## Question 3 : Vérification du fonctionnement et test de performance

Le fonctionnement de l'implémentation a été vérifié à plusieurs niveaux :

- **Vérification manuelle des résultats** : Après avoir exécuté l'attaque par force brute, nous avons examiné
  manuellement le résultat stocké dans `manager->result` pour confirmer que le mot de passe avait été correctement
  craqué.


- **Verification du temps d'exécution** : Afin de vérifier l'execution des threads et de l'efficacité d'en avoir
  plusieurs
  (ou pas) nous avons hashés des mots de passe de longueurs différentes avec un nombre de threads différents.
  Les mots de passes que nous avons utilisés sont : abc, abcd, abcde, HHA, HHHA, HHHHA, \*\*\*, \*\*\*\*, (\*\*\*\*\*).
  Nous avons donc utilisé des mots de passes qui nous permettent de tester le début du dictionnaire, le milieu et la
  fin.
  Les différents résultats sont affiché dans les tableaux ci-dessous.
  <p>

  Il est important de préciser que ces résultats ont été exécuté sur un seul ordinateur et que ce sont des moyennes de
  plusieurs tests.
  De plus l'efficacité des threads et des calculs dépend aussi de la puissance de l'ordinateur ainsi ces résultats ne
  sont pas
  aussi précis que s'ils avaient été pris sur plusieurs ordinateurs. De plus, le choix d'implémentations du code et des
  algorithmes peuvent aussi faire que le code soit plus ou moins efficace que d'autres implémentations.

<u>3 lettres :</u>

Pour craquer un mot de passe composé de trois lettres, on remarque que, lorsqu'on commence par le début du dictionnaire,
le temps d'exécution est très rapide, car les combinaisons les plus simples sont testées en premier. Cependant, l'ajout
de threads peut introduire une certaine complexité, car plusieurs threads travaillent simultanément, et certains peuvent
progresser plus rapidement que d'autres.
Dans la plupart des cas, on constate qu'à partir de l'utilisation de 2 threads, le temps d'exécution diminue
sensiblement, surtout si nous nous utilisons le milieu du dictionnaire. Même lorsque l'on atteint la fin du
dictionnaire, l'utilisation de plusieurs threads reste généralement plus
rapide que l'utilisation d'un seul thread.

| mdp | nThreads | temps d'exécution [ms] |
|-----|----------|------------------------|
| abc | 1        | 6                      |
|     | 2        | 2                      |
|     | 3        | 6                      |
|     | 4        | 22                     |
|     | 8        | 40                     |
| HHA | 1        | 160                    |
|     | 2        | 6                      |
|     | 3        | 66                     |
|     | 4        | 157                    |
|     | 8        | 125                    |
| *** | 1        | 299                    |
|     | 2        | 155                    |
|     | 3        | 139                    |
|     | 4        | 119                    |
|     | 8        | 167                    |

<u>Avec 4 lettres :</u>

Comme on peut le prévoir, le temps d'exécution augmente lorsque le mot de passe contient 4 lettres, bien que 24 ms
demeure relativement rapide lorsqu'un seul thread est utilisé au début du dictionnaire. Cependant, lorsque 8 threads
travaillent simultanément sur l'ensemble du charset, le temps d'exécution est quand même plus long.
En revanche, au milieu du dictionnaire, il est nettement plus rapide d'utiliser plusieurs threads en parallèle plutôt
qu'un seul. Ce résultat est également observé pour la fin du dictionnaire, où le temps d'exécution est plus court
lorsqu'on utilise plusieurs threads plutôt qu'un seul.

| mdp  | nThreads | temps d'exécution [ms] |
|------|----------|------------------------|
| abcd | 1        | 24                     |
|      | 2        | 18                     |
|      | 3        | 26                     |
|      | 4        | 21                     |
|      | 8        | 34                     |
| HHHA | 1        | 9576                   |
|      | 2        | 136                    |
|      | 3        | 4234                   |
|      | 4        | 8153                   |
|      | 8        | 6689                   |
| **** | 1        | 19587                  |
|      | 2        | 10791                  |
|      | 3        | 8477                   |
|      | 4        | 8389                   |
|      | 8        | 7505                   |

<u>Avec 5 lettres :</u>

En passant à des mots de passe de 5 lettres, nous avons constaté une nouvelle augmentation des temps d'attente. En fait,
nous n'avons même pas tenté de craquer un mot de passe situé en fin de dictionnaire, car même pour ceux se trouvant
au milieu, le processus devenait excessivement long en raison de la longueur des mots de passe et des calculs
nécessaires.
On peut à nouveau voir qu'un mot de passe en début de dictionnaire, rajouter des threads ne fait qu'augmenter le temps d'exécution donc
ce n'est pas efficace d'en avoir plus. Par contre, le cas moyen qui est le milieu du dictionnaire, avoir plus de threads
raccourcis considérablement le temps pour trouver le mot de passe. On peut quand même noter la différence surprenante
d'avoir 4 threads
qui prend plus de temps que d'avoir 8 threads. On ne peut de nouveau pas savoir comment vont travailler les threads en
parallel donc
il est possible que les résultats soient différents si nous avions testé plus d'une fois ces cas-là.

| mdp   | nThreads | temps d'exécution [ms] |
|-------|----------|------------------------|
| abcde | 1        | 1259                   |
|       | 2        | 1404                   |
|       | 3        | 1860                   |
|       | 4        | 2308                   |
|       | 8        | 4604                   |
| HHHHA | 1        | 820111 (~13.6min)      |
|       | 2        | 9286                   |
|       | 3        | 417784  (~6min)        |
|       | 4        | 785985 (~13min)        |
|       | 8        | 634247 (~10.5min)      |
| ***** | 1        | pas testé              |
|       | 2        | pas testé              |
|       | 3        | pas testé              |
|       | 4        | pas testé              |
|       | 8        | pas testé              |

## Conclusion

Notre choix d'implémentation et d'algorithme est fonctionnel, mais il peut ne pas être l'approche la plus efficace en ce
qui concerne la répartition des charges de travail entre les threads. Les résultats sont influencés par divers facteurs,
notamment la puissance de calcul de nos ordinateurs, le nombre de cœurs et de threads disponibles, ainsi que la
complexité des mots de passe.
Il peut s'avérer avantageux d'utiliser un plus grand nombre de threads pour distribuer les tâches de calcul, mais cela
n'est pas toujours nécessaire, en particulier lorsque le mot de passe se situe au début du dictionnaire. En effet, la
distribution des threads peut entraîner une augmentation du temps d'exécution si plusieurs d'entre eux travaillent
simultanément sur un mot de passe situé en début de dictionnaire. De même, si un seul thread est utilisé pour un mot de
passe en fin de dictionnaire, cela peut également entraîner une durée d'exécution plus longue.
Il est essentiel de noter que, étant donné que nous ne pouvons pas prédire où se situe le mot de passe dans le
dictionnaire, les temps d'exécution varieront inévitablement en fonction des circonstances particulières de chaque
situation.







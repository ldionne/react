## Captain's log: side effects of the design process

### To consider
* the tag dispatching is kinda broken because it does not work as expected when we mix it with inheritance
* fix the implementation of `computation::union_`: it should use `react::execute` instead of `X::execute` and `Y::execute`.
* pour implémenter des bundles de computations, il devrait être possible de modifier le cloaking dépendant de la computation qui est retrievée de l'env. on pourrait ensuite implémenter un bundle comme une computation vide qui dépend d'autres computations et qui modifie le cloaking pour que les computations du bundle soient accessibles aussi.
* add cloaking when updating computations on update_retrieve branch
* avec le travail sur la branche update_retrieve: on a un feature_set spécial qui utilise une default value pour les computations qui ne sont aps spécifiées, sauf si la computation est marquée comme extern. <<<<<<<<<  dans le fond, je pense qu'on a juste rien à faire avec les computations qui sont extern. lors de l'update, si une computation n'est pas implémentée dans le set ça va chier parce qu'une dependency n'est pas satisfaite, ce qui est correct.

* est-ce que ça serait possible d'utiliser n'importe quel séquence de computations comme un computation group? avec un wrapper qui les visite en ordre topologique, on pourrait en tout cas implémenter la partie operator() si toutes les computations sont implémentées dans la séquence. pour celles qui ne sont pas implémentées on pourrait utiliser un autre wrapper qui génère seulement celles qui sont nécessaire. sinon, celles qui ne sont pas implémentées dans une séquence pourraient en réalité être ceux qui sont `extern`.
* est-ce possible que feature_set == computation_group, et qu'on aie un équivalent feature_group pour décrire un computation_group?
* fixer le cloaked feature_set: il doit utiliser le feature_dependency_graph pour checker si deux computations sont adjacentes au lieu de checker si une feature fait partie des dependencies de la computation.
* allow named template parameters in react::computations.
* avoir une erreur claire si une feature est implémentée par plus d'une computation
* est-ce que les events ont toujours besoin de connaître leur domaine? est-ce que le concept d'event a toujours une raison d'être?
* comment je fais un pont entre les features et l'instrumentation? y a-t-il un pont à faire? est-ce possible?
* documenter le tour de passe-passe pour qu'un substitute_with fonctionne dans un contexte non-dépendant (struct apply { typedef foo<substitue_with<lock_id> > type; };)
* ne pas oublier cette idée: avoir des points prédéfinis où on génère des events, genre end_of_program.
* tester l'utilisation de metafonctions arbitraires ET de pattern matching.
* bug: comment on fait pour caller concretize<factory(factory qu'on veut pas appliquer)>? on pourrait avoir un truc comme concretize<factory(protect<x>)> mais c'est un peu laid. on peut aussi décider de s'en ficher et d'attendre d'avoir un use case où c'est utile pour le gérer.
* wtf: les features définissent des abstract_features desquelles ils dépendent. le problème, c'est que l'abstract feature est découplée de l'implémentation, ce qui fait que c'est impossible de satisfaire les dependencies automatiquement parce qu'on ne peut pas obtenir une feature à partir d'une abstract feature sans paramètres. la seule option possible que je vois avec le design actuel est d'utiliser la valeur par défaut de l'abstract feature, si elle existe: `concretize<abstract_feature()>::type`. si une telle défault n'existe pas, alors l'user doit spécifier la feature à utiliser dans le feature set pour satisfaire la dependency.
* generate n'est qu'un wrapper qui trouve un featureSet par défaut given un event puis le call. pour utiliser dyno dans du code existant, on définit le featureSet à utiliser pour tel ou tel event, puis ce featureSet sera utilisé. évidemment, on doit tout recompiler, mais ça serait aussi possible d'avoir un featureSet qui fait du type erasure, ou juste certaines features qui font du type erasure. Pour s'assurer que le code connait le featureSet à utiliser, on utilise la lib à partir d'un header qu'on contrôle, où on spécialise un trait qui est normalement incomplet. en gros; si on tente d'utiliser dyno sans définir de featureSet pour tous les events qui sont générés, on a une erreur lors de la compilation.
* pouvoir passer des paramètres à une feature lorsqu'on retrieve le résultat serait utile
* initialiser les features dans l'ordre, et donner la possibilité aux features d'accéder aux autres features du set pendant qu'il se construit. ça va nécessiter de créer un feature_set temporaire pendant que le vrai feature_set se fait construire.
* inverser l'ordre des arguments dans l'`operator()()` des Features et faire comme si ça matchait tout le temps si operator()() n'a pas de 2eme argument: on se rapprochera de l'interface d'une fonction et, sauf erreur, on supportera presque drop-in les features dans Boost.Accumulator.
* fuck les factories qui modifient le storage duration: ce qui nous intéresse réellement est quelque chose de plus haut niveau, genre one_per_thread ou one_per_program. est-ce que ça fait du sens d'avoir un one_per[x] ? avec une manière d'accéder à celui qu'on veut, ça serait damn utile.
* considérer l'utilisation d'un type spécial function<void()> pour protéger les vrais function types dans concretize
* avoir une manière de créer des features qui peuvent traiter plusieurs traces à la fois.
* est-ce que ça serait intelligent d'avoir un seul concept; quelque chose comme FeatureNode. est-ce que ça serait intéressant d'avoir du contrôle sur le feature graph? évaluer le tradeoff flexibilité/utilisabilité.
* est-ce que dans le fond on a plus qu'un graphe? on a clairement un graphe de dépendances; mais a-t-on un espèce de graphe pour les subfeatures? est-ce que c'est utile ou juste correct mathématiquement de constater ça?
* est-ce une bonne idée de faire un captain's log avec les idées présentées ici?? :)

* est-qu'une abstract feature pourrait être implémentée par plusieurs features? ça prendrait une manière de décider quelle partie de l'abstract feature on va chercher lorsqu'on call fset[abstract]
* le feature set a un interface qui ressemble à celui des features. serait-il possible de s'organiser pour avoir un perfect match (à `operator[]` près), ce qui nous permettrait d'implémenter des abstract features en utilisant des feature sets.
* je m'imagine bien faire `fset[segment][parent]`
* avoir une manière de bundler des features, genre de dire que telle abstract feature est en réalité l'union de telles autres features/abstract features.
* le `.result()` d'un feature set devrait retourner le `feature_set` lui-même, je pense. ça nous permettrait de chainer les `[][]` entre plusieurs feature sets.
* avoir une manière de faire ceci avec un feature_set: `fset[abstract.nested] == fset[abstract][abstract.nested]`. On pourrait avoir un espèce de trait qui est checké par le feature_set et qui forward automatiquement pour certaines abstract features. à voir
* concept de subfeature intéressant?

### Done
* implémenter le support pour les subfeatures dans le default_ feature set.
* être capable de caller des abstract features sur un domaine
* smell: this_domain est requis par tout le monde, mais seulement parce qu'ils utilisent abstract_feature<>. en réalité this_domain ne devrait p-t pas exister et on aurait alors une autre manière d'accéder à l'info dont on a besoin dans abstract_feature<>. cela est d'ailleurs relié au fait qu'on a deux manières d'utiliser l'environement et qu'on aimerait p-t être capable de caller une abstract_feature sur un domaine directement. il y a une généralisation ou une meilleure séparation des tâches à faire ici.
* c'est inconsistent d'avoir accès à env[abstract_feature] ET abstract_feature(env). il serait p-t mieux de ne pouvoir qu'en faire un des deux?

* il y a deux choses: des adapteurs à features/abstract features et des features/abstract features. typed est un adapteur, alors que gatelocks est une implémentation. est-ce qu'on doit faire la différence dans l'organisation des fichiers/namespaces/noms?
* comment on deal avec les modifiers: parfois ils peuvent vouloir représenter la même feature que la feature qu'ils modifient, mais on a plus de trait feature_of comme dans Boost.Accumulator. qu'est-ce qu'on fait?
* si on a un typed_abstract_feature, alors abstract_feature est en réalité un untyped_abstract_feature, non?
* les abstract features sont vraiment abstraites. pour cette raison, avoir une typed_abstract_feature ou une static_abstract_feature sont des nonsens. en réalité, ce qu'on a c'est une typed_feature et une static_feature. la raison pour laquelle on est arrivé à une horreur comme la typed_abstract_feature est qu'on a besoin d'une manière de spécifier un wrapper pour une feature lorsqu'on crée le domaine, i.e. lorsqu'on est seulement en présence de l'abstract feature et de ses paramètres. idéalement, il ne serait pas nécessaire de modifier l'implémentation de l'abstract feature ni de la feature pour être capable de lui appliquer un wrapper.

* fournir un générateur à abstract_features pourrait être utile, parce qu'on sauverait le maudit using operator=.

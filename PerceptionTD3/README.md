### Exercice 1 : Gradient

*   **Objectif** : Extraire les contours de l'image `escalier.png` en utilisant les fonctions Sobel et Scharr d'OpenCV.
*   **Implémentation** : Dans le code fourni, les fonctions `applySobel` et `applyScharr` sont déjà définies pour appliquer les opérateurs Sobel et Scharr et extraire les contours d'une image. L'appel dans le `main` qui charge l'image `escalier.png` et affiche les contours de Sobel et Scharr est commenté. Il suffit de le décommenter pour réaliser cet exercice.

### Exercice 2 : La méthode de Hough

*   **Objectif** : Écrire l'algorithme de la Transformée de Hough pour détecter les lignes, en identifiant les paramètres d'entrée.
*   **Paramètres d'entrée pour la Transformée de Hough** :
    *   **Image binaire** avec les contours détectés.
    *   **Intervalle des angles (Theta)** : En général, de 0 à π.
    *   **Intervalle des distances (Rho)**, calculé comme la distance maximale entre deux points de l'image.
    *   **Accumulateur (matrice de votes)** : Comptabilise les votes des paramètres (rho et theta) pour chaque ligne potentielle.
    *   **Seuil** : Le nombre minimum de votes pour considérer la présence d'une ligne en `(rho, theta)`.

### Exercice 3 : Implémentation

*   **Objectif** : Implémenter la Transformée de Hough en C++ pour l'image `escalier.png`.
*   **Étapes pour terminer l'Exercice 3** :
    1.  **Charger l'image** : Utilisez `loadImage` pour charger `escalierWithoutTree.png` en niveaux de gris.
    2.  **Appliquer Canny** : Détectez les contours avec la fonction `applyCanny`.
    3.  **Transformée de Hough manuelle** : Utilisez `manualHoughTransform` sur l'image avec contours pour identifier les lignes en fonction de l'accumulateur de votes.
    4.  **Tracer les lignes** : Avec `drawManualLines`, dessinez les lignes détectées sur l'image de sortie.

Pour tester, il suffit de décommenter la section correspondante dans le `main`, déjà préparée pour exécuter l'exercice 3 et afficher les résultats.

### Exercice 4 : OpenCV

*   **Objectif** : Tester la fonction `HoughLines` d'OpenCV et vérifier les résultats par rapport à l'implémentation manuelle.
*   **Implémentation existante** : La fonction `applyHoughTransform` dans le code applique la Transformée de Hough en utilisant `HoughLines` d'OpenCV, et la fonction `drawLines` dessine les lignes détectées sur l'image. Pour vérifier si les résultats sont similaires, il suffit de décommenter la section du `main` correspondant à l'exercice 4.

### Exercice 5 : Localisation de balle

*   **Objectif** : Utiliser la Transformée de Hough pour localiser une balle jaune, en intégrant la détection de couleur, les opérations morphologiques et la Transformée de Hough.
*   **Implémentation existante** : Le code contient déjà la fonction `detectYellowBall`, qui détecte une balle jaune en utilisant la Transformée de Hough sur un masque de couleur jaune. Le masque jaune est amélioré avec des opérations morphologiques pour une détection plus précise, satisfaisant l'intégration de la couleur et de la morphologie demandée.

### Exercice 6 : Localisation de balle dans une vidéo

*   **Objectif** : Appliquer l'algorithme de l'exercice précédent pour détecter la balle dans une vidéo et analyser la complexité de l'algorithme.
*   **Implémentation existante** : La fonction `processVideo` implémente déjà cette fonctionnalité, en chargeant la vidéo et en traitant chaque image avec le même algorithme de détection de la balle.
*   **Complexité de l'algorithme** : La complexité temporelle de cet algorithme est approximativement \(O(N \cdot M)\) pour chaque image, où \(N\) et \(M\) représentent la largeur et la hauteur de l'image, respectivement. Cette complexité augmente proportionnellement au nombre d'images dans la vidéo.

En décommentant les sections dans le `main`, vous pourrez tester chaque exercice comme demandé.

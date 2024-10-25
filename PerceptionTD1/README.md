# Exercices avec OpenCV

La bibliothèque OpenCV est incontournable pour le traitement d'images. Elle est utilisable dans plusieurs langages, notamment C++ et Python. Dans ce TP, nous allons mettre en pratique les concepts vus en cours en utilisant OpenCV.

## Exercices

### Exercice 1 : Ouvrir et manipuler un fichier image

- **Ouvrir et afficher `balle.png` :** Le code dans la section d'ouverture d'image (`main`) utilise `imread` pour charger l'image et `imshow` pour l'afficher.

- **Afficher les coordonnées et la couleur du pixel lors d'un clic :** Le code implémente déjà cette fonctionnalité dans la fonction `mouseCallback`, activée avec `setMouseCallback`. Avec cette fonction, les coordonnées `(x, y)` et les valeurs RGB du pixel cliqué sont affichées dans la console.

- **Convertir en image monochrome :** Pour une conversion automatique, utilisez `convetToMonochrome`, qui utilise la méthode `cvtColor`. Pour une conversion manuelle, `convetToMonochromeManual` effectue la transformation en appliquant la formule de luminosité. Les deux fonctions sont prêtes à être utilisées, et peuvent être affichées comme suit :

    ```cpp
    Mat imageMonochrome = convetToMonochrome(image);
    imshow("Image Monochrome Automatique", imageMonochrome);

    Mat imageMonochromeManual = convetToMonochromeManual(image);
    imshow("Image Monochrome Manuelle", imageMonochromeManual);


### Exercice 2 : Conversion en image monochrome

Cet exercice est implémenté dans la section 2 du code, utilisant deux fonctions différentes :

- **Automatique avec OpenCV :** Fonction `convetToMonochrome`.
- **Manuelle sans OpenCV :** Fonction `convetToMonochromeManual`.

Les deux fonctions répondent à la consigne en appliquant différentes méthodes de conversion.

---

## Exercice 3 : Variabilité (1)

Pour calculer la moyenne et l'écart-type des composantes RGB d'une zone sélectionnée avec la souris, le code contient la fonction `calculateMeanStdDev`, qui prend une région d'intérêt (ROI) en entrée. En utilisant `meanStdDev`, elle calcule et affiche les valeurs moyennes et l'écart-type pour chaque canal RGB.

La ROI peut être sélectionnée via la fonction `mouseClick`, permettant de capturer la zone cliquée.

---

## Exercice 4 : Variabilité (2)

Cet exercice demande de colorer en rouge tous les pixels dont la couleur RGB est similaire à celle d'une région sélectionnée. La fonction `colorPixelsSimilarToMean` compare la couleur moyenne de la ROI avec les pixels de l'image et remplace ceux similaires par la couleur rouge. Un seuil de similarité peut être ajusté en paramètre pour améliorer la sélection.

**Expérience avec `chair.jpg` :** Il peut y avoir des différences de résultats entre `balle.png` et `chair.jpg` en raison de la variation de l'éclairage et des couleurs similaires, qui peuvent apparaître selon la complexité de l'image.

---

## Exercice 5 : Espace HSV/HSL

Pour réaliser la sélection d'objets comme la balle et la chaise, l'espace HSV est préférable. Le code inclut `colorYellowAndOrangeShades`, qui convertit l'image en HSV et crée un masque avec les limites de couleur fournies, appliquant une coloration rouge aux pixels détectés.

---

## Exercice 6 : Suivi de la balle avec le centre de gravité

- **Centre de la balle :** La fonction `calculateCenterOfGravity` calcule le centre de gravité en utilisant `moments` à partir du masque de sélection de la balle.

- **Suivi dans la vidéo :** `detectAndMarkBall` applique un masque avec des limites HSV pour suivre la balle dans `balle.mp4`, et marque le centre avec un "X" bleu, en utilisant les limites HSV fournies.

### Étudiants : Pedro Campos et Vinicius Rocha

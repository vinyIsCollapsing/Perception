# Convolution et Morphologie avec OpenCV

---

### 1. Convolution

#### Exercice 1 : Introduction

- **Formule de la Convolution :**  
  La convolution d'une image \( I \) avec un noyau ou masque \( K \) est donnée par la formule :
  \[
  (I * K)(x, y) = \sum_{i=-a}^{a} \sum_{j=-b}^{b} I(x+i, y+j) \cdot K(i, j)
  \]
  où \( a \) et \( b \) sont les limites du masque, et l'opération multiplie les valeurs de \( I \) dans le voisinage d'un pixel \( (x, y) \) par le masque \( K \).

- **Taille de l'Image de Sortie :**  
  Dans une convolution simple, l'image résultante aura la même taille que l'image originale si on applique du "padding" pour maintenir les bords. Cependant, sans padding, l'image résultante sera plus petite, car les valeurs proches des bords ne peuvent pas être complètement calculées en utilisant le masque.

- **Contraintes pour Utiliser des Masques :**  
  Pour que les masques soient applicables, ils doivent avoir des dimensions impaires (comme 3x3 ou 5x5), ce qui permet de définir un "point central" dans le masque pour l'aligner avec le pixel central du voisinage.

#### Exercice 2 : Gradients Directionnels

- **Tester avec des Masques de Gradient :**  
  Dans le code, les masques de Sobel (maskX et maskY) sont appliqués pour calculer les gradients horizontaux et verticaux. Pour que tous les contours apparaissent, nous pouvons calculer la magnitude du gradient en combinant les images résultantes des gradients horizontaux et verticaux. Cela se fait dans la fonction `gradientMagnitude`, qui calcule la racine carrée de la somme des carrés des gradients \( G = \sqrt{G_x^2 + G_y^2} \).

---

### 2. Utilisation d'OpenCV

#### Exercice 3 : Utilisation d'OpenCV avec filter2D

- **Fonction filter2D :**  
  Dans l'exercice 3, nous utilisons la fonction `filter2D` d'OpenCV pour appliquer des masques de convolution comme ceux de Sobel directement sur l'image. Cette méthode simplifie l'implémentation de la convolution, car OpenCV calcule automatiquement la convolution avec le masque fourni. Le code crée les gradients horizontal et vertical, puis calcule la magnitude du gradient en utilisant la fonction `magnitude`.

---

### 3. Filtre Moyenneur

#### Exercice 4 : Application d'un Filtre Moyenneur

- **Filtre Moyenneur avec un Voisinage de 5x5 :**  
  Le filtre moyenneur calculé dans cet exercice utilise un masque 5x5 pour calculer la moyenne des pixels autour de chaque pixel central. Ce filtre lisse l'image, réduisant ainsi les bruits et les détails fins. Dans le code, cette opération est réalisée avec un masque où chaque élément est égal à \( 1/25 \), appliqué en utilisant `filter2D`. L'effet visuel est un flou, rendant l'image plus homogène.

---

### 2. Morphologie

#### Exercice 5 : Erosion et Dilatation

- **Erosion et Dilatation :**  
  L'érosion réduit les zones d'une couleur donnée (dans ce cas, les pixels détectés dans le masque) en faisant en sorte que les pixels d'arrière-plan "envahissent" la région détectée. La dilatation fait l'inverse : elle étend les zones détectées. Cela aide à affiner le masque, en supprimant les bruits (avec l'érosion) ou en remplissant les lacunes (avec la dilatation), comme montré dans les méthodes `erodeMask` et `dilateMask` dans le code.

#### Exercice 6 : Ouverture et Fermeture

- **Ouverture et Fermeture :**  
  L'opération d'ouverture est une érosion suivie d'une dilatation. Elle est utile pour supprimer de petites régions et lisser les bords des objets tout en maintenant la structure générale. La fermeture est une dilatation suivie d'une érosion, utile pour remplir de petits trous. Les deux sont appliqués en utilisant la fonction `morphologyEx` d'OpenCV dans le code.

### Étudiants : Pedro Campos et Vinicius Rocha

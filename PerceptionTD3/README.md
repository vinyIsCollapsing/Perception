# Projet : Transformée de Hough

Ce projet explore la détection de contours et de lignes dans les images en utilisant différentes techniques de vision par ordinateur, y compris la Transformée de Hough. Le développement commence par l'extraction de contours avec les opérateurs Sobel et Scharr et évolue vers l'application de la Transformée de Hough.

## Prérequis

- OpenCV 4.x ou supérieur
- C++11 ou supérieur

## Structure du projet

- **main.cpp** : Code principal avec l'implémentation des fonctions d'extraction de contours et de la Transformée de Hough.
- **escalier.png** : Image utilisée comme exemple dans la phase de détection de contours.
- **README.md** : Documentation du projet.

## Questions et Développement

### Partie 1 : Extraction des Contours avec Sobel et Scharr

#### Enoncé
Avec l'aide des fonctions Sobel et Scharr d'OpenCV, extrayez les contours de l'image `escalier.png`.

#### Développement
Dans la première partie du projet, l'extraction des contours a été implémentée en utilisant les opérateurs Sobel et Scharr. Les fonctions `appliquerSobel` et `appliquerScharr` ont été créées pour faciliter la modularisation du code. L'image est chargée en niveaux de gris pour se concentrer sur les variations d'intensité, qui sont essentielles pour la détection des contours.

#### Résultats
Les bords de l'image `escalier.png` ont été détectés et affichés dans des fenêtres séparées, permettant de comparer les méthodes Sobel et Scharr.

#### Instructions d'exécution

1. Installez OpenCV si ce n'est pas déjà fait.
2. Compilez le code :
   ```bash
   g++ main.cpp -o main `pkg-config --cflags --libs opencv4`
   ./main


### Parte 2: Transformada de Hough para Detecção de Linhas

#### Enunciado
Escreva, no papel (ou no README), o algoritmo e as equações da Transformada de Hough no caso da busca por linhas. Quais são os parâmetros de entrada necessários para a implementação deste método?

#### Algoritmo e Equações

A Transformada de Hough é utilizada para a detecção de linhas em imagens, convertendo o problema de detecção de linhas no espaço da imagem em um problema de votação em um espaço paramétrico, conhecido como espaço de Hough.

A equação de uma reta no plano cartesiano é convertida para sua **forma polar**:

\[ \rho = x \cdot \cos(\theta) + y \cdot \sin(\theta) \]

Onde:
- \( \rho \) é a distância da linha até a origem.
- \( \theta \) é o ângulo da normal da linha em relação ao eixo horizontal.
- \( x \) e \( y \) são as coordenadas dos pontos de borda da imagem.

#### Algoritmo da Transformada de Hough:

1. **Pré-processamento**: Aplicar um detector de bordas (ex: Sobel, Canny) para identificar os pontos de borda da imagem.
   
2. **Espaço de Hough**: Para cada ponto de borda \( (x, y) \), calcular os possíveis valores de \( \rho \) e \( \theta \) utilizando a equação \( \rho = x \cdot \cos(\theta) + y \cdot \sin(\theta) \).

3. **Acumulador**: Incrementar os votos no acumulador (uma matriz que registra quantas vezes cada par \( (\rho, \theta) \) aparece).

4. **Detecção de Picos**: Identificar os picos no acumulador, que correspondem às linhas mais votadas no espaço de Hough.

5. **Traçar as Linhas**: Converter os picos no espaço de Hough para o espaço da imagem e traçar as linhas detectadas.

#### Parâmetros de Entrada Necessários:

Para implementar a Transformada de Hough, são necessários os seguintes parâmetros:

1. **Imagem de entrada**: Uma imagem binária ou de borda (obtida através de Sobel, Canny, etc.).
2. **Resolução de \( \rho \)**: A resolução da distância \( \rho \) no acumulador (geralmente em pixels).
3. **Resolução de \( \theta \)**: A resolução angular no acumulador (geralmente em radianos).
4. **Limite de votos**: O número mínimo de votos necessários no acumulador para que uma linha seja detectada.
5. **Intervalo de \( \theta \)**: Normalmente varia entre \( -\pi \) e \( \pi \).

#### Exemplo Prático:

No OpenCV, a Transformada de Hough pode ser implementada utilizando a função `HoughLines`, que recebe a imagem de entrada, as resoluções de \( \rho \) e \( \theta \), e o limite de votos:

cv::HoughLines(image, lines, rho_resolution, theta_resolution, threshold);

### Parte 3: Implementação da Transformada de Hough em C++

#### Enunciado
Implemente a Transformada de Hough para detecção de linhas em C++, utilizando apenas o OpenCV para visualizar a imagem final, mas sem utilizar a função pronta `HoughLines`.

#### Desenvolvimento
Nesta etapa, a Transformada de Hough foi implementada manualmente. A função que detecta linhas realiza os seguintes passos:

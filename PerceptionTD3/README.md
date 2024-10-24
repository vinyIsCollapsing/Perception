# Projeto: Transformée de Hough

Este projeto explora a detecção de contornos e linhas em imagens utilizando diferentes técnicas da visão computacional, incluindo a Transformada de Hough. O desenvolvimento começa com a extração de contornos com operadores Sobel e Scharr e evolui para a aplicação da Transformada de Hough.

## Requisitos

- OpenCV 4.x ou superior
- C++11 ou superior

## Estrutura do Projeto

- **main.cpp**: Código principal com a implementação das funções de extração de contornos e Transformada de Hough.
- **escalier.png**: Imagem utilizada como exemplo na fase de detecção de contornos.
- **README.md**: Documentação do projeto.

## Questões e Desenvolvimento

### Parte 1: Extração de Contornos com Sobel e Scharr

#### Enunciado
Com a ajuda das funções Sobel e Scharr do OpenCV, extraia os contornos da imagem `escalier.png`.

#### Desenvolvimento
Na primeira parte do projeto, foi implementada a extração de contornos utilizando os operadores Sobel e Scharr. As funções `aplicarSobel` e `aplicarScharr` foram criadas para facilitar a modularização do código. A imagem é carregada em escala de cinza para focar nas variações de intensidade, que são essenciais para a detecção de contornos.

#### Resultados
As bordas da imagem `escalier.png` foram detectadas e exibidas em janelas separadas, permitindo a comparação entre os métodos Sobel e Scharr.

#### Instruções para Executar

1. Instale o OpenCV se ainda não tiver.
2. Compile o código:
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

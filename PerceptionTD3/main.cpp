#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace cv;
using namespace std;

// Definir constantes
#define DEG2RAD 0.01745329251  // Fator de conversão de graus para radianos

Mat aplicarSobel(const Mat& imagem);
Mat aplicarScharr(const Mat& imagem);
Mat carregarImagem(const string& caminho);
Mat aplicarCanny(const Mat& src, int low_threshold, int high_threshold);
vector<pair<int, int>> transformadaDeHoughManual(const Mat& edges, int threshold, int& max_dist, int& num_thetas, double& theta_step);
Mat desenharLinhasManual(const Mat& edges, const vector<pair<int, int>>& lines, int max_dist, int num_thetas, double theta_step);
vector<Vec2f> aplicarTransformadaHough(const Mat& edges, double rho_resolution, double theta_resolution, int threshold);
Mat desenharLinhas(const Mat& edges, const vector<Vec2f>& lines);

int main() {
    /*
    //Exercice 1
    // Carregar a imagem
    string caminho = "escalier.png";
    Mat imagem = carregarImagem(caminho);
    if (imagem.empty()) return -1;

    // Aplicar Sobel e Scharr
    Mat sobelContornos = aplicarSobel(imagem);
    Mat scharrContornos = aplicarScharr(imagem);

    // Exibir os resultados
    imshow("Imagem Original", imagem);
    imshow("Contornos Sobel", sobelContornos);
    imshow("Contornos Scharr", scharrContornos);

    // Aguardar a tecla de fechamento
    waitKey(0);
    */
    /*
    // Exercice 3
    string caminho = "escalierWithoutTree.png";
    Mat src = carregarImagem(caminho);
    if (src.empty()) return -1;

    // Aplicar a detecção de bordas de Canny
    Mat edges = aplicarCanny(src, 100, 200);

    // Executar a Transformada de Hough
    int max_dist, num_thetas;
    double theta_step;
    vector<pair<int, int>> lines = transformadaDeHoughManual(edges, 200, max_dist, num_thetas, theta_step);

    // Desenhar as linhas detectadas
    Mat dst = desenharLinhasManual(edges, lines, max_dist, num_thetas, theta_step);

    // Exibir a imagem original e a imagem com as linhas detectadas
    imshow("Original Image", src);
    imshow("Manual Canny Image", edges);
    imshow("Manual Detected Lines", dst);

    waitKey(0);
    */
    /*
    //Exercice 4
    // Carregar a imagem
    string caminho = "escalierWithoutTree.png";
    Mat src = carregarImagem(caminho);
    if (src.empty()) return -1;

    // Aplicar a detecção de bordas usando o detector de Canny
    Mat edges = aplicarCanny(src, 100, 200);

    // Aplicar a Transformada de Hough para detecção de linhas
    vector<Vec2f> lines = aplicarTransformadaHough(edges, 1, CV_PI / 180, 200);

    // Desenhar as linhas detectadas
    Mat dst = desenharLinhas(edges, lines);

    // Exibir a imagem original e a imagem com as linhas detectadas
    imshow("Original Image", src);
    imshow("Canny Image", edges);
    imshow("Detected Lines", dst);

    waitKey(0);
    */

       //Exercice 5
    // Carregar a imagem
    string caminho = "balle_small.jpg";
    Mat src = carregarImagem(caminho);
    if (src.empty()) return -1;

    // Aplicar a detecção de bordas usando o detector de Canny
    Mat edges = aplicarCanny(src, 100, 200);

    // Aplicar a Transformada de Hough para detecção de linhas
    vector<Vec2f> lines = aplicarTransformadaHough(edges, 1, CV_PI / 180, 200);

    // Desenhar as linhas detectadas
    Mat dst = desenharLinhas(edges, lines);

    // Exibir a imagem original e a imagem com as linhas detectadas
    imshow("Original Image", src);
    imshow("Canny Image", edges);
    imshow("Detected Lines", dst);

    waitKey(0);

    return 0;
}

// Função para aplicar o operador Sobel
Mat aplicarSobel(const Mat& imagem) {
    Mat sobelX, sobelY, sobelResultado;

    // Sobel nas direções X e Y
    Sobel(imagem, sobelX, CV_64F, 1, 0, 3); // X-direction
    Sobel(imagem, sobelY, CV_64F, 0, 1, 3); // Y-direction

    // Calcular a magnitude do gradiente
    magnitude(sobelX, sobelY, sobelResultado);
    sobelResultado.convertTo(sobelResultado, CV_8U);

    return sobelResultado;
}

// Função para aplicar o operador Scharr
Mat aplicarScharr(const Mat& imagem) {
    Mat scharrX, scharrY, scharrResultado;

    // Scharr nas direções X e Y
    Scharr(imagem, scharrX, CV_64F, 1, 0); // X-direction
    Scharr(imagem, scharrY, CV_64F, 0, 1); // Y-direction

    // Calcular a magnitude do gradiente
    magnitude(scharrX, scharrY, scharrResultado);
    scharrResultado.convertTo(scharrResultado, CV_8U);

    return scharrResultado;
}

// Função para carregar a imagem
Mat carregarImagem(const string& caminho) {
    Mat src = imread(caminho, IMREAD_GRAYSCALE);
    if (src.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
    }
    return src;
}

// Função para aplicar a detecção de bordas de Canny
Mat aplicarCanny(const Mat& src, int low_threshold, int high_threshold) {
    Mat edges;
    Canny(src, edges, low_threshold, high_threshold, 3);
    return edges;
}

// Função para executar a Transformada de Hough manual
vector<pair<int, int>> transformadaDeHoughManual(const Mat& edges, int threshold, int& max_dist, int& num_thetas, double& theta_step) {
    int width = edges.cols;
    int height = edges.rows;
    max_dist = sqrt(width * width + height * height); // Rho máximo
    num_thetas = 180; // Número de ângulos
    int num_rhos = 2 * max_dist; // Quantidade de valores de rho
    theta_step = CV_PI / num_thetas;

    // Criar o acumulador
    vector<vector<int>> accumulator(num_rhos, vector<int>(num_thetas, 0));

    // População do acumulador
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (edges.at<uchar>(y, x) > 0) { // Detecção de ponto de borda
                for (int t = 0; t < num_thetas; t++) {
                    double theta = t * theta_step;
                    double rho = x * cos(theta) + y * sin(theta);
                    int rho_idx = cvRound(rho + max_dist); // Índice de rho no acumulador
                    accumulator[rho_idx][t]++;
                }
            }
        }
    }

    // Identificação dos picos no acumulador
    vector<pair<int, int>> lines; // Armazena pares de (rho, theta)

    for (int r = 0; r < num_rhos; r++) {
        for (int t = 0; t < num_thetas; t++) {
            if (accumulator[r][t] > threshold) {
                lines.push_back({r, t});
            }
        }
    }

    return lines;
}

// Função para desenhar as linhas detectadas
Mat desenharLinhasManual(const Mat& edges, const vector<pair<int, int>>& lines, int max_dist, int num_thetas, double theta_step) {
    Mat dst;
    cvtColor(edges, dst, COLOR_GRAY2BGR);

    for (size_t i = 0; i < lines.size(); i++) {
        int r = lines[i].first;
        int t = lines[i].second;

        double rho = r - max_dist;
        double theta = t * theta_step;

        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(dst, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }

    return dst;
}

// Função para aplicar a Transformada de Hough
vector<Vec2f> aplicarTransformadaHough(const Mat& edges, double rho_resolution, double theta_resolution, int threshold) {
    vector<Vec2f> lines;
    HoughLines(edges, lines, rho_resolution, theta_resolution, threshold, 350, 50);
    return lines;
}

// Função para desenhar as linhas detectadas
Mat desenharLinhas(const Mat& edges, const vector<Vec2f>& lines) {
    Mat dst;
    cvtColor(edges, dst, COLOR_GRAY2BGR);

    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(dst, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }

    return dst;
}
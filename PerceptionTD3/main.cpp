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
Mat colorirTonsDeAmareloELaranja(const Mat& imagem, const Scalar& limite_inferior, const Scalar& limite_superior);
void detectarBolaAmarela(Mat imagem);

VideoCapture loadVideo(const string& videoPath);
Mat getYellowMask(const Mat& frame);
vector<Vec3f> detectCircles(const Mat& mask);
void drawCircles(Mat& frame, const vector<Vec3f>& circles);
void processVideo(VideoCapture& cap);

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
    //Exercice 4
    /*
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
    /* 
    // Carregar a imagem
    Mat imagem = imread("balle.jpg");

    if(imagem.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    // Detecta a bola amarela
    detectarBolaAmarela(imagem);
    */


    // Exercice 6
    /*
    VideoCapture cap = loadVideo("balle.mp4");

    // Processar o vídeo para detectar a bola amarela
    processVideo(cap);

    cap.release();
    destroyAllWindows();
    */
    return 0;
    
}

// Exercice 1
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

// Funcao generica
// Função para carregar a imagem
Mat carregarImagem(const string& caminho) {
    Mat src = imread(caminho, IMREAD_GRAYSCALE);
    if (src.empty()) {
        cout << "Erro ao carregar a imagem!" << endl;
    }
    return src;
}

// Exercice 3
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

// Exercice 4
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

// Exercice 5
// Função para segmentar a bola amarela
Mat segmentarBolaAmarela(const Mat& imagem) {
    Mat imagem_hsv, mascara;
    
    // Converte a imagem para o espaço de cor HSV
    cvtColor(imagem, imagem_hsv, COLOR_BGR2HSV);

    // Limites de cor para a bola amarela (ajuste esses valores conforme necessário)
    Scalar limite_inferior(20, 100, 100);  // Amarelo mais escuro
    Scalar limite_superior(30, 255, 255);  // Amarelo mais claro

    // Cria a máscara para isolar a bola amarela
    inRange(imagem_hsv, limite_inferior, limite_superior, mascara);

    return mascara;
}

// Função para detectar a bola amarela usando a Transformada de Hough
void detectarBolaAmarela(Mat imagem) {
    // Segmenta a bola amarela
    Mat mascara = segmentarBolaAmarela(imagem);

    // Aplica um blur para suavizar a imagem e reduzir ruídos
    GaussianBlur(mascara, mascara, Size(9, 9), 3, 3);

    // Detecta círculos usando a Transformada de Hough
    vector<Vec3f> circulos;
    HoughCircles(mascara, circulos, HOUGH_GRADIENT, 1, mascara.rows/8, 5, 40, 50, 750); // Ajuste nos parâmetros para maior sensibilidade

    // Verifica se algum círculo foi detectado
    if (circulos.size() == 0) {
        cout << "Nenhum círculo foi detectado!" << endl;
        return;
    }

    // Desenha os círculos detectados
    for(size_t i = 0; i < circulos.size(); i++) {
        Point centro(cvRound(circulos[i][0]), cvRound(circulos[i][1]));
        int raio = cvRound(circulos[i][2]);

        // Desenha o círculo do centro da bola
        circle(imagem, centro, 3, Scalar(0, 255, 0), -1, 8, 0);

        // Desenha o contorno da bola
        circle(imagem, centro, raio, Scalar(255, 0, 0), 5, 8, 0);  // Linha de contorno mais grossa
    }

    // Redimensiona a imagem para 640x480
    Mat imagem_redimensionada;
    resize(imagem, imagem_redimensionada, Size(640, 480));

    // Cria uma janela ajustável para exibir o resultado
    namedWindow("Deteccao de Bola Amarela", WINDOW_NORMAL);
    imshow("Deteccao de Bola Amarela", imagem_redimensionada);
    waitKey(0);
}

//Exerice 6
// Função para carregar o vídeo
VideoCapture loadVideo(const string& videoPath) {
    VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        cerr << "Erro ao abrir o vídeo!" << endl;
        exit(-1);
    }
    return cap;
}

// Função para aplicar a máscara para a cor amarela (agora ajustada para tons variados)
Mat getYellowMask(const Mat& frame) {
    Mat hsvFrame, mask;
    
    // Intervalo de cor ajustado para capturar tons variados de amarelo e laranja
    Scalar lower_yellow_orange(15, 80, 100);    // Limite inferior mais amplo
    Scalar upper_yellow_orange(30, 255, 255);  // Limite superior mais amplo

    // Converter para espaço de cor HSV
    cvtColor(frame, hsvFrame, COLOR_BGR2HSV);

    // Criar a máscara para detectar tons de amarelo e laranja
    inRange(hsvFrame, lower_yellow_orange, upper_yellow_orange, mask);

    // Aplicar um blur para reduzir ruídos e uma operação morfológica para contornos mais precisos
    GaussianBlur(mask, mask, Size(7, 7), 2, 2);
    morphologyEx(mask, mask, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    return mask;
}

// Função para detectar círculos usando a Transformada de Hough
vector<Vec3f> detectCircles(const Mat& mask) {
    vector<Vec3f> circles;
    HoughCircles(mask, circles, HOUGH_GRADIENT, 1, mask.rows / 8, 100, 20, 10, 100);
    return circles;
}

// Função para desenhar os círculos detectados no frame
void drawCircles(Mat& frame, const vector<Vec3f>& circles) {
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3f c = circles[i];
        Point center(cvRound(c[0]), cvRound(c[1]));
        int radius = cvRound(c[2]);

        // Desenhar o centro do círculo
        circle(frame, center, 3, Scalar(0, 255, 0), -1, LINE_AA);
        // Desenhar a borda do círculo
        circle(frame, center, radius, Scalar(0, 0, 255), 3, LINE_AA);
    }
}

// Função principal para processar o vídeo
void processVideo(VideoCapture& cap) {
    while (true) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        // Obter a máscara para a cor amarela
        Mat mask = getYellowMask(frame);

        // Detectar círculos na máscara
        vector<Vec3f> circles = detectCircles(mask);

        // Desenhar os círculos detectados
        drawCircles(frame, circles);

        // Exibir o resultado
        imshow("Detecção de Bola Amarela", frame);

        // Parar ao pressionar a tecla 'q'
        if (waitKey(30) == 'q') break;
    }
}

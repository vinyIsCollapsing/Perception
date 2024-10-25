// g++ main.cpp -o main `pkg-config --cflags --libs opencv4`
// ./main
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>

#include <iostream>
#include <fstream>
 
using namespace cv;
using namespace std;

Mat capturedImage;  // Imagem original clonada para processar
Mat roi;            // Região de interesse
Scalar meanColor;   // Guarda a média de cor da ROI
bool regionSelected = false;  // Variável de controle para saber se a ROI foi selecionada

void mouseCallback(int event, int x, int y, int flags, void *userdata);
Mat convetToMonochrome(Mat &originalImage);
Mat convetToMonochromeManual(Mat &originalImage);
void mouseClick(int event, int x, int y, int flags, void *param);
void calculateMeanStdDev(Mat &roi);
void colorSimilarPixels(Mat &image, Scalar meanColor, double threshold);

void selectROI(Mat &image);
Scalar calculateMeanColor(const Mat &roi);
void colorPixelsSimilarToMean(Mat &image, Scalar meanColor, double threshold);
Mat segmentarObjetoPorCor(const Mat& image, const Scalar& limite_inferior, const Scalar& limite_superior);

int main(){
    // Abrindo imagem
    string image_path = samples::findFile("balle.jpg");
    Mat image = imread(image_path, IMREAD_COLOR);

    if(image.empty()){
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    //Exercice 1
    /*
    namedWindow("Full Image", WINDOW_NORMAL);
    setMouseCallback("Full Image", mouseCallback, &image);
    imshow("Full Image", image);
    */

    //Exercice 2
    /*
    Mat imageMonochrome = convetToMonochrome(image);
    namedWindow("Automatic Monochromatic Image", WINDOW_NORMAL);
    imshow("Automatic Monochromatic Image", imageMonochrome);

    Mat imageMonochromeManual = convetToMonochromeManual(image);
    namedWindow("Manual Monochromatic Image", WINDOW_NORMAL);
    imshow("Manual Monochromatic Image", imageMonochromeManual);
    */

    // Exercice 3
    /*
    capturedImage = image.clone();

    namedWindow("Clickable Image", WINDOW_NORMAL);
    setMouseCallback("Clickable Image", mouseClick, &roi);
    imshow("Clickable Image", image);
 
    // Loop para manter a janela aberta
    while (true) {
        int k = waitKey(1); 
        if (k == 27){ // Pressione ESC para sair
            break;
        }
    }

    if (!roi.empty()) {
        calculateMeanStdDev(roi);
        roi.release();
    }
    */

    // Exercice 4
    /* 
    // Clonando a imagem original para ser manipulada
    capturedImage = image.clone();

    // Exibição inicial da imagem
    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", capturedImage);

    // Função para selecionar uma área (ROI) na imagem
    selectROI(capturedImage);

    // Verifica se a ROI foi selecionada corretamente
    if (!roi.empty()) {
        // Calcula a média da cor da ROI
        meanColor = calculateMeanColor(roi);

        // Colorir os pixels da imagem que possuem cor semelhante à média da ROI
        colorPixelsSimilarToMean(capturedImage, meanColor, 60.0); // Usa 30 como limiar de similaridade

        // Exibe a imagem modificada
        namedWindow("Modified Image", WINDOW_NORMAL);
        imshow("Modified Image", capturedImage);
        waitKey(0); // Espera até que o usuário pressione uma tecla
    }
    */

    // Exercice 5
    // Define os limites de cor para a seleção no espaço HSV
    Scalar limite_inferior(5, 100, 100);  // Limite inferior para Hue, Saturação e Valor
    Scalar limite_superior(45, 255, 255); // Limite superior

    // Chama a função de segmentação
    Mat resultado = segmentarObjetoPorCor(image, limite_inferior, limite_superior);

    // Exibe a imagem original e o resultado da segmentação
    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", image);

    namedWindow("HSV Image", WINDOW_NORMAL);
    imshow("HSV Image", resultado);

    waitKey(0);
    
    return 0;
}

// Exercice 1
// Captura de eventos do mouse
// Captura de coordenadas e RGB
void mouseCallback(int event, int x, int y, int flags, void *userdata){
    if (event == EVENT_LBUTTONDOWN){
        Mat* image = static_cast<Mat*>(userdata);
        Vec3b pixel = image->at<Vec3b>(y,x);
        cout << "Coordenadas (x, y): (" << x << ", " << y << ")" << endl;
        cout << "Valores de cor - R: " << (int)pixel[2] <<
                                " G: " << (int)pixel[1] <<
                                " B: " << (int)pixel[0] << endl;
    }
}

// Exerice 2.1 (avec OpenCV)
// Conversao de cores automatica
Mat convetToMonochrome(Mat &originalImage){
    Mat monoImage; 
    cvtColor(originalImage, monoImage, COLOR_BGR2GRAY);
    return monoImage;
}

// Exerice 2.2 (sans OpenCV)
// Conversao de cores manual
Mat convetToMonochromeManual(Mat &originalImage){
    int width = originalImage.cols;
    int height = originalImage.rows;

    // CV_8UC1: tipo de matriz 8bits unsigned
    Mat monoImage(height, width, CV_8UC1);

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            Vec3b pixel = originalImage.at<Vec3b>(y, x);
            // Intensidade de cinza
            uchar grayValue = static_cast<uchar>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
            monoImage.at<uchar>(y, x) = grayValue;
        }
    }
    
    return monoImage;
}

// Exercice 3
// Função para capturar uma área da imagem com o mouse
void mouseClick(int event, int x, int y, int flags, void *param) {
    static Point pt1, pt2;
    static bool dragging = false; // Variável para controlar o arrasto do mouse

    switch (event) {
        case EVENT_LBUTTONDOWN:
            cout << "Mouse Pressed at (" << x << ", " << y << ")" << endl;
            pt1 = Point(x, y);  // Primeira coordenada da ROI
            dragging = true;    // Começa a capturar a região
            break;

        case EVENT_MOUSEMOVE:
            if (dragging) {
                Mat tempImage = capturedImage.clone();
                pt2 = Point(x, y);
                rectangle(tempImage, pt1, pt2, Scalar(0, 255, 0), 2); // Desenha o retângulo durante a seleção
                imshow("Clickable Image", tempImage); // Atualiza a exibição da imagem enquanto o mouse se move
            }
            break;

        case EVENT_LBUTTONUP:
            cout << "Mouse Released at (" << x << ", " << y << ")" << endl;
            pt2 = Point(x, y); // Segunda coordenada da ROI

            if (dragging) {
                // Captura a região de interesse (ROI)
                Rect roiRect(pt1, pt2);
                roi = capturedImage(roiRect).clone(); // Faz uma cópia da área selecionada
                imshow("Captured Region", roi); // Mostra a área selecionada em uma nova janela

                regionSelected = true;  // Define que a região foi selecionada
                dragging = false;       // Para o arrasto do mouse
            }
            break;

        default:
            break;
    }
}

// Função para calcular a média e desvio padrão da ROI
void calculateMeanStdDev(Mat &roi) {
    if (roi.empty()) {
        cout << "ROI vazia" << endl;
        return;
    }

    // Calcular a média de cada canal de cor
    Scalar mean, stddev;
    meanStdDev(roi, mean, stddev); // Função do OpenCV para calcular média e desvio padrão

    meanColor = mean; // Armazenar a média da cor para usar na comparação
    cout << "Média - R: " << mean[2] << ", G: " << mean[1] << ", B: " << mean[0] << endl;
    cout << "Desvio Padrão - R: " << stddev[2] << ", G: " << stddev[1] << ", B: " << stddev[0] << endl;
}

// Exercice 4
// Função para selecionar a ROI com o mouse
void selectROI(Mat &image) {
    // Captura a região de interesse (ROI)
    Rect roiRect = selectROI("Original Image", image); // Função do OpenCV para selecionar ROI manualmente

    if (roiRect.width > 0 && roiRect.height > 0) {
        roi = image(roiRect).clone(); // Clona a ROI para uso posterior
        imshow("Selected Region", roi); // Exibe a região selecionada em uma nova janela
        waitKey(0); // Espera o usuário visualizar a região selecionada
    } else {
        cout << "Nenhuma região foi selecionada." << endl;
    }
}

// Função para calcular a média da cor da ROI
Scalar calculateMeanColor(const Mat &roi) {
    Scalar meanValue, stddevValue;
    meanStdDev(roi, meanValue, stddevValue); // Calcula a média e o desvio padrão da ROI

    cout << "Média de cores (R, G, B) - R: " << meanValue[2] << ", G: " << meanValue[1] << ", B: " << meanValue[0] << endl;
    return meanValue; // Retorna a média da cor da ROI
}

// Função para colorir pixels semelhantes à média da ROI
void colorPixelsSimilarToMean(Mat &image, Scalar meanColor, double threshold) {
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            Vec3b pixel = image.at<Vec3b>(y, x);

            // Verifica se o pixel é semelhante à média da ROI
            if (abs(pixel[0] - meanColor[0]) < threshold && 
                abs(pixel[1] - meanColor[1]) < threshold && 
                abs(pixel[2] - meanColor[2]) < threshold) {
                // Colorir o pixel de vermelho
                image.at<Vec3b>(y, x) = Vec3b(0, 0, 255); // RGB para vermelho
            }
        }
    }
}

// Exercice 5
// Função para segmentar uma área específica da imagem com base nos valores HSV
Mat segmentarObjetoPorCor(const Mat& image, const Scalar& limite_inferior, const Scalar& limite_superior) {
    Mat imagem_hsv, mascara, resultado;

    // Converte a imagem para o espaço de cor HSV
    cvtColor(image, imagem_hsv, COLOR_BGR2HSV);

    // Cria a máscara com os limites de cor definidos
    inRange(imagem_hsv, limite_inferior, limite_superior, mascara);

    // Aplica a máscara na imagem original para obter o resultado
    bitwise_and(image, image, resultado, mascara);

    return resultado;
}
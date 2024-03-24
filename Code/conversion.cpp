#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

void ToPPM(const std::string &inputPath, const std::string &outputPath) {
    cv::Mat image = cv::imread(inputPath, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cerr << "Image non trouvée" << inputPath << std::endl;
        return;
    }
    std::string outputPPMPath = outputPath;
        outputPPMPath += ".ppm";
    bool success = cv::imwrite(outputPPMPath, image);
    if (!success) {
        std::cerr << "Erreur" << std::endl;
    } else {
        std::cout << "Succès" << outputPPMPath << std::endl;
    }
}
void PPMtoJPEG(const std::string& inputPath, const std::string& outputPath) {
    cv::Mat image = cv::imread(inputPath, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Erreur : Impossible de lire l'image - " << inputPath << std::endl;
        return;
    }
    std::string outputJPEGPath = outputPath;
        outputJPEGPath += ".jpeg";
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(95);
    bool success = cv::imwrite(outputJPEGPath, image, compression_params);
    if (!success) {
        std::cerr << "Erreur lors de l'enregistrement de l'image au format JPEG." << std::endl;
    } else {
        std::cout << "Image convertie et sauvegardée en JPEG avec succès : " << outputJPEGPath << std::endl;
    }
}
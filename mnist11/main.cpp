#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include "mnist.h"
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/highgui.hpp>
#include <cstdlib>

//training and test the model

// ================= Main =================
int main() {
    
    //    cv::Mat img = cv::imread( "/Users/jinzhixu/Desktop/project/HelloWorld/mnist/mnist/mnist/img.png");
    //    cv::imshow("window",img);
    //    cv::waitKey(0);
    
    std::string filename ="/Users/jinzhixu/Desktop/project/HelloWorld/mnist/mnist/mnist/train-images.idx3-ubyte";
    std::string label_filename = "/Users/jinzhixu/Desktop/project/HelloWorld/mnist/mnist/mnist/train-labels.idx1-ubyte";
    
    std::vector<std::vector<unsigned char>> imagesFile = readIDX3UByteFile(filename);
    std::vector<std::vector<unsigned char>> labelsFile = readLabelFile(label_filename);
    
    std::vector<cv::Mat> imagesData;  // Store your images
    std::vector<int> labelsData;      // Corresponding labels
    
    //std::cout<< (int)imagesFile.size();
    
    for(int imgCnt=0; imgCnt<(int)imagesFile.size(); imgCnt++)
    {
        int rowCounter = 0;
        int colCounter = 0;
        
        cv::Mat tempImg = cv::Mat::zeros(cv::Size(28,28),CV_8UC1); //tempIMG SHAPE: 28*28
        for (int i = 0; i < (int)imagesFile[imgCnt].size(); i++) {
            
            tempImg.at<uchar>(cv::Point(colCounter++,rowCounter)) = (int)imagesFile[imgCnt][i];
            if ((i) % 28 == 0) {
                rowCounter++;
                colCounter= 0;
                if(i == 756)
                    break;
            }
        }
        
        //std::cout<<(int)labelsFile[imgCnt][0]<<std::endl;
        
        //std::cout<<tempImg<<std::endl;
        
        imagesData.push_back(tempImg);
        labelsData.push_back((int)labelsFile[imgCnt][0]);
        
        //to visualize each image ,n dataset  to check only
        //cv::imshow("TempImg",tempImg);
        //cv::waitKey(0);
        
        //std::cout<<1;
        
    }
    
    std::cout<<"1st step: Successfully finished reading image\n"<<"numberSamples: "<<imagesData.size()<<std::endl;
    
    
    //2rd: prepare the training data
    
    int inputlayerSize = imagesData[0].total(); //28*28=784
    int hiddenLayerSize = 100;
    int outputLayerSize = 10;
    
    int numberSamples = imagesData.size();
    cv::Mat trainingData(numberSamples,inputlayerSize,CV_32F);
    cv::Mat labelData(numberSamples,outputLayerSize,CV_32F);
    
    for (int i=0; i<imagesData.size();i++){
        //.reshape(cn: channel number:1,1 channel grey scale rownum)
        cv::Mat image = imagesData[i].reshape(1,1); //28*28 --> [784,1]
        
//        double minVal = 0.0, maxVal = 0.0;
//        cv::minMaxLoc(image, &minVal, &maxVal);
//        std::cout << "min=" << minVal << " max=" << maxVal << std::endl;
        
        //image.convertTo(trainingData.row(i),CV_32F,1.0 / 255.0);
        image.convertTo(trainingData.row(i),CV_32F); //float32
        
        cv::Mat label = cv::Mat::zeros(1, outputLayerSize, CV_32F);
        label.at<float>(0,labelsData[i]) = 1.0;
        
        label.copyTo(labelData.row(i));
    }
    
    std::cout<<"2rd step\n"<<"trainingData size"<<trainingData.size()<<std::endl;
    std::cout << "labelData size = " << labelData.size() << std::endl;
    
    
    // ---------------- Create ANN ----------------
    
    
    cv::Ptr<cv::ml::ANN_MLP> mlp = cv::ml::ANN_MLP::create();
    
    cv::Mat layers = (cv::Mat_<int>(4, 1)<<
        inputlayerSize,
        256,
        128,
        outputLayerSize);

    mlp->setLayerSizes(layers);

    // RELU is MUCH more stable than SIGMOID
    mlp->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM);

    mlp->setTermCriteria(
        cv::TermCriteria(
            cv::TermCriteria::MAX_ITER +  cv::TermCriteria::EPS,
            10000,
            0.0001
        )
    );
    
    // Backprop with SMALL learning rate
    mlp->setTrainMethod(
        cv::ml::ANN_MLP::BACKPROP,
        0.0001,   // learning rate
        0.1       // momentum
    );
   
    
    //training
    
    std::cout << "Training started...\n";

    mlp->train(trainingData, cv::ml::ROW_SAMPLE, labelData);

    std::cout << "Training finished\n";
    
    
    std::cout << "\nweights[0]:\n" << mlp->getWeights( 0 ) << std::endl;
    std::cout << "\nweights[1]:\n" << mlp->getWeights( 1 ) << std::endl;
    std::cout << "\nweights[2]:\n" << mlp->getWeights( 2 ) << std::endl;
    std::cout << "\nweights[3]:\n" << mlp->getWeights( 3 ) << std::endl;
    std::cout << "\nweights[4]:\n" << mlp->getWeights( 4 ) << std::endl;
    std::cout << "\nweights[5]:\n" << mlp->getWeights( 5 ) << std::endl;
    
    
    std::cout << "\nweights[0]:\n" << mlp->getWeights( 0 ).size() << std::endl;
    std::cout << "\nweights[1]:\n" << mlp->getWeights( 1 ).size() << std::endl;
    std::cout << "\nweights[2]:\n" << mlp->getWeights( 2 ).size() << std::endl;
    std::cout << "\nweights[3]:\n" << mlp->getWeights( 3 ).size() << std::endl;
    std::cout << "\nweights[4]:\n" << mlp->getWeights( 4 ).size() << std::endl;
    std::cout << "\nweights[5]:\n" << mlp->getWeights( 5 ).size() << std::endl;
    //std::cout << "\nweights[6]:\n" << mlp->getWeights( 6 ).size() << std::endl;
    
    mlp->save("/Users/jinzhixu/Desktop/project/HelloWorld/mnist/mnist/mnist/trained_digit_model_2hiddenlayers.xml");
    
    //3th step initialize MLP model(3 layers in total)
    
    /**
    
    cv::Ptr<cv::ml::ANN_MLP> mlp = cv::ml::ANN_MLP::create();
    
//    int inputlayerSize = imagesData[0].total(); //28*28=784
//    int hiddenLayerSize = 100;
//    int outputLayerSize = 10;

    //cv::Mat layers  = (cv::Mat_<int>(3, 1)<< inputlayerSize,hiddenLayerSize,outputLayerSize);
    
    cv::Mat layersSize = cv::Mat(3, 1, CV_16U);
    layersSize.row(0) = cv::Scalar(trainingData.cols);
    layersSize.row(1) = cv::Scalar(hiddenLayerSize);
    layersSize.row(2) = cv::Scalar(labelData.cols);
    
    mlp->setLayerSizes(layersSize);
    mlp->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM); //cv::ml::ANN_MLP::SIGMOID_SYM/RELU
     
    cv::TermCriteria termCrit(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 10000,0.001);
    //cv::TermCriteria termCrit(cv::TermCriteria::MAX_ITER,3000,0.001);
    
    mlp->setTermCriteria(termCrit);
    
    //mlp->setTrainMethod(cv::ml::ANN_MLP::BACKPROP,0.001,0.1);
    
    mlp->setTrainMethod(cv::ml::ANN_MLP::BACKPROP,0.0001);
    
    
    //4th step training
    mlp->train(trainingData,cv::ml::ROW_SAMPLE,labelData);
    
    mlp->save("/Users/jinzhixu/Desktop/project/HelloWorld/mnist/mnist/mnist/trained_digit_model_1hiddenlayers.xml");
    
    
//    std::cout << "\nweights[0]:\n" << mlp->getWeights( 0 ) << std::endl;
//    std::cout << "\nweights[1]:\n" << mlp->getWeights( 1 ) << std::endl;
//    std::cout << "\nweights[2]:\n" << mlp->getWeights( 2 ) << std::endl;
//    std::cout << "\nweights[3]:\n" << mlp->getWeights( 3 ) << std::endl;
//    
//    
//    std::cout << "\nweights[0]:\n" << mlp->getWeights( 0 ).size() << std::endl;
//    std::cout << "\nweights[1]:\n" << mlp->getWeights( 1 ).size() << std::endl;
//    std::cout << "\nweights[2]:\n" << mlp->getWeights( 2 ).size() << std::endl;
//    std::cout << "\nweights[3]:\n" << mlp->getWeights( 3 ).size() << std::endl;

     */
    //5th test
    cv::Mat testOut;
    
    //std::cout<<trainingData.row(0)<<std::endl;
    
    mlp->predict(trainingData.row(0), testOut);
    std::cout << "Training sample output = " << testOut << std::endl;
    
    std::cout<<"4rd step model training finished"<<std::endl;
    
    std::cout<< mlp->getLayerSizes();
    
    
    return 0;

}




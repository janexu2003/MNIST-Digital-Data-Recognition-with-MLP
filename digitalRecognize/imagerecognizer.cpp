#include "imagerecognizer.h"
#include <QDebug>
#include <opencv2/opencv.hpp>


ImageRecognizer::ImageRecognizer() {}

void ImageRecognizer::recognizeImage(QImage image)
{
    qDebug() <<"recognizeDigit";

    //load the model
    cv::Ptr<cv::ml::ANN_MLP> mlp = cv::ml::ANN_MLP::load("/Users/jinzhixu/Desktop/project/HelloWorld/mnist/mnist/mnist/trained_digit_model_2hiddenlayers.xml");

    cv::Mat testImage(image.height(), image.width(),CV_8UC4, image.bits());

    cv::cvtColor(testImage,testImage,cv::COLOR_RGB2GRAY);
    //cv::imwrite("/Users/jinzhixu/Desktop/digitalRecognize/test.png",testImage);

    //resize and flatten the image
    cv::resize(testImage,testImage,cv::Size(28,28));
    cv::Mat flattenedImage = testImage.reshape(1, 1);
    cv::Mat input;
    //flattenedImage.convertTo(input, CV_32F,1.0 / 255.0);
    flattenedImage.convertTo(input, CV_32F);


    // Perform prediction
    cv::Mat output;
    mlp->predict(input, output);

    std::cout<<output<<std::endl;
    //output is a 1*10 matrix of probibility of each label
    //output =
    //    [ 0.01  0.03  0.05  0.02  0.04  0.01  0.10  0.60  0.09  0.05 ]
    //        0     1     2     3     4     5     6     7     8     9

    // Find the class with the highest probability
    cv::Point classIdPoint;
    double confidence;
    cv::minMaxLoc(output, nullptr, &confidence, nullptr, &classIdPoint);

    int predictedClass = classIdPoint.x;

    std::cout<<"Predicted Class: "<<predictedClass <<" with confidence: "<<confidence<<std::endl;


    QString result = "Recognized digit is "+ QString::number(predictedClass);
    setResultStr(result);

}

QString ImageRecognizer::resultStr() const
{
    return m_resultStr;
}

void ImageRecognizer::setResultStr(const QString &newResultStr)
{
    if (m_resultStr == newResultStr)
        return;
    m_resultStr = newResultStr;
    Q_EMIT resultStrChanged();
}

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <opencv2/opencv.hpp>
#include <loadimage.h>

cv::Mat sigmoid(const cv::Mat& x)
{
    cv::Mat y;
    cv::exp(-x, y);
    return 1.0 / (1.0 + y);
};

void manual_forward(
    const cv::Mat& sample,
    const std::vector<cv::Mat>& weights)
{
    // weights[1]: [100 x 785]
    cv::Mat W1 = weights[1](cv::Rect(0, 0, 784, 100));
    cv::Mat b1 = weights[1](cv::Rect(784, 0, 1, 100));

    // weights[2]: [10 x 101]
    cv::Mat W2 = weights[2](cv::Rect(0, 0, 100, 10));
    cv::Mat b2 = weights[2](cv::Rect(100, 0, 1, 10));

    // Forward propagation
    cv::Mat z1 = W1 * sample.t() + b1;
    cv::Mat a1 = sigmoid(z1);

    cv::Mat z2 = W2 * a1 + b2;
    cv::Mat output = sigmoid(z2);

    std::cout << "Manual forward output:\n" << output.t() << std::endl;
};

int main()
{
    // Load the trained model
    cv::Ptr<cv::ml::ANN_MLP> mlp = cv::ml::ANN_MLP::load("/Users/jinzhixu/Desktop/project/HelloWorld/mnist/mnist/mnist/trained_digit_model_1hiddenlayers.xml");


    // Load the image you want to classify
    //cv::Mat testImage = cv::imread("/home/cvlab/Downloads/5.png", cv::IMREAD_GRAYSCALE);

    //using the test set
    std::string filename ="/Users/jinzhixu/Desktop/project/HelloWorld/mnist/mnist/mnist/t10k-images.idx3-ubyte";
    std::string label_filename = "/Users/jinzhixu/Desktop/project/HelloWorld/mnist/mnist/mnist/t10k-labels.idx1-ubyte";

    loadimage loader;

    std::vector<std::vector<unsigned char>> imagesFile = loader.readIDX3UByteFile(filename);
    std::vector<std::vector<unsigned char>> labelsFile = loader.readLabelFile(label_filename);

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
        // cv::imshow("TempImg",tempImg);
        // cv::waitKey(0);
    }

    std::cout<<labelsData.size()<<std::endl;

    int predict_right_sum = 0;

    //confusion matrix: rows = true label, cols = predicted
    cv::Mat confusion = cv::Mat::zeros(10, 10, CV_32S);

    for (int imgCnt=0; imgCnt<(int)imagesData.size(); imgCnt++)
    {
        //using model to predict labels for test dataset

        //cv::resize(testImage,testImage,cv::Size(28,28));
        // Flatten the image
        cv::Mat flattenedImage = imagesData[imgCnt].reshape(1, 1);
        cv::Mat input;
        //flattenedImage.convertTo(input, CV_32F,1.0 / 255.0);
        flattenedImage.convertTo(input, CV_32F);


        double minVal = 0.0, maxVal = 0.0;
        cv::minMaxLoc(input, &minVal, &maxVal);
        std::cout << "min=" << minVal << " max=" << maxVal << std::endl;


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
        int truelabel = (int)labelsFile[imgCnt][0];

        confusion.at<int>(truelabel,predictedClass)++;


        if (predictedClass == truelabel){
            predict_right_sum+=1;
        }
    }


    std::cout<<"total number of images: "<<labelsFile.size()<<std::endl;
    std::cout<<"number of images predicted right: "<<predict_right_sum<<std::endl;
    std::cout<<"correct rate: "<<(float)predict_right_sum/labelsFile.size()<<std::endl;

    std::cout << "\nConfusion Matrix (rows=true, cols=predicted)\n\n    ";
    for (int j = 0; j < 10; ++j)
        std::cout << j << "   ";
    std::cout << "\n---------------------------------------------\n";

    for (int i = 0; i < 10; ++i)
    {
        std::cout << i << " | ";
        for (int j = 0; j < 10; ++j)
            std::cout << confusion.at<int>(i, j) << " ";
        std::cout << std::endl;
    }

    cv::Ptr<cv::ml::ANN_MLP> mlp1 = cv::ml::ANN_MLP::load("/Users/jinzhixu/Desktop/project/HelloWorld/mnist/mnist/mnist/trained_digit_model_1hiddenlayers.xml");

    cv::Mat sizes = mlp->getLayerSizes();
    std::cout << sizes << std::endl;


    return 0;
}

#ifndef IMAGERECOGNIZER_H
#define IMAGERECOGNIZER_H
#include<QObject>
#include<QImage>

class ImageRecognizer: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString resultStr READ resultStr WRITE setResultStr NOTIFY resultStrChanged FINAL)

public:
    ImageRecognizer();

    Q_INVOKABLE void recognizeImage(QImage img);
    QString resultStr() const;
    void setResultStr(const QString &newResultStr);

Q_SIGNALS:
    void resultStrChanged();
private:
    QString m_resultStr = "Default";
};

#endif // IMAGERECOGNIZER_H


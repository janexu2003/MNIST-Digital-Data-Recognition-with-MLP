#ifndef PAINTERITEM_H
#define PAINTERITEM_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QImage>

class Painteritem: public QQuickPaintedItem
{
    Q_OBJECT
public:
    Painteritem(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);
    //OVERWRITE SOME FEATURES OF QQuickPaintedItem

    Q_INVOKABLE QImage toQimage();
    Q_INVOKABLE void clearImage();

    // QQuickItem interface

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    std::vector<QPoint> m_linePoints{};

};

#endif // PAINTERITEM_H


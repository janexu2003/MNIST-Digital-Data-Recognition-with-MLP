#include "painteritem.h"


Painteritem::Painteritem(QQuickItem *parent)
    :QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
}

void Painteritem::paint(QPainter *painter)
{
    painter->fillRect(0,0,100,100,Qt::black);
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::white,5));

    if(m_linePoints.size()>1)
    {
        for(int i = 1;i<m_linePoints.size();i++)
        {
            painter->drawLine(m_linePoints[i-1],m_linePoints[i]);
        }
    }

}

QImage Painteritem::toQimage()
{

    qDebug() <<"toqimage debug";

    QImage qimage(100,100,QImage::Format_ARGB32);
    qimage.fill(Qt::black);

    QPainter painter(&qimage);
    paint(&painter);

    return qimage;

}

void Painteritem::clearImage()
{
    m_linePoints.clear();
    update();

}

void Painteritem::mousePressEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        m_linePoints.push_back(event->pos());
        update();
        event->accept();
    }
}

void Painteritem::mouseMoveEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton)
    {
        m_linePoints.push_back(event->pos());
        update();
        event->accept();
    }
}

void Painteritem::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        m_linePoints.push_back(event->pos());
        update();
        event->accept();
    }
}

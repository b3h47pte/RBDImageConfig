#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>

class ImageView: public QGraphicsView
{
public:
  ImageView();
  ~ImageView();

protected:
  void mousePressEvent(QMouseEvent* e);
  void mouseReleaseEvent(QMouseEvent* e);
};

#endif // IMAGEVIEW_H

#include <QDebug>
#include <QMouseEvent>
#include "ImageView.h"

ImageView::
ImageView() {

}

ImageView::
~ImageView() {

}

void ImageView::
mousePressEvent(QMouseEvent* e) {
  QGraphicsView::mousePressEvent(e);
  e->ignore();
}

void ImageView::
mouseReleaseEvent(QMouseEvent* e) {
  QGraphicsView::mouseReleaseEvent(e);
  e->ignore();
}

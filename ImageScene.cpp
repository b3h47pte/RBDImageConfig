#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "ImageScene.h"

ImageScene::
ImageScene(QObject* parent): 
  QGraphicsScene(parent) {
}

ImageScene::
~ImageScene() {
}

void ImageScene::
mousePressEvent(QGraphicsSceneMouseEvent* e) {
  QGraphicsScene::mousePressEvent(e);
  emit NotifyMousePress(e);
}

void ImageScene::
mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
  QGraphicsScene::mouseReleaseEvent(e);
  emit NotifyMouseRelease(e);
}

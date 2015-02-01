#ifndef IMAGESCENE_H
#define IMAGESCENE_H

#include <QGraphicsScene>

class ImageScene: public QGraphicsScene
{
  Q_OBJECT

public:
  explicit ImageScene(QObject* parent = 0);
  virtual ~ImageScene();

  friend class MainWindow;

signals:
  void NotifyMousePress(QGraphicsSceneMouseEvent* e);
  void NotifyMouseRelease(QGraphicsSceneMouseEvent* e);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent* e);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

};

#endif // IMAGESCENE_H

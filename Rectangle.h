#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include <QDebug>
#include <QRectF>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <assert.h>

class Rectangle {
public:
  Rectangle() {}
  virtual ~Rectangle() {}

  virtual void SetX(qreal inX) { x = inX; };
  virtual qreal GetX() const { return x; }

  virtual void SetY(qreal inY) { y = inY; };
  virtual qreal GetY() const { return y;}

  virtual void SetWidth(qreal w) { width = w; };
  virtual qreal GetWidth() const { return width; }

  virtual void SetHeight(qreal h) {height = h; };
  virtual qreal GetHeight() const { return height; }

  virtual void Normalize() {
    if(width < 0) {
      x += width;
      width *= -1;
    }

    if (height < 0) {
      y += height;
      height *= -1;
    }

    if (x < 0.0) {
      width -= fabs(x);
      x = 0.0;
    }

    if(y < 0.0) {
      height -= fabs(y);
      y = 0.0;
    }
}

  virtual QRectF ToQRectF(int index = 0) const {
    (void)index;
    return QRectF(x, y, width, height);
  }

  virtual std::string ToString() const {
    std::stringstream ss;
    ss << x << " " << y << " " << width << " " << height;
    return ss.str();
  }

  virtual void FromSplitString(std::vector<std::string>& parts) {
    assert(parts.size() >= 4);
    try {
      x = std::stod(parts[0]);
      y = std::stod(parts[1]);
      width = std::stod(parts[2]);
      height = std::stod(parts[3]);
    } catch (...) {
      qDebug() << "Error: Creating a rectangle";
    } 
  }

  virtual bool CanContainMultipleRectangles() const { return false; }

protected:
  qreal x;
  qreal y;
  qreal width;
  qreal height;
};

#endif

//
// Created by byter on 12/14/17.
//

#ifndef THREEPP_AXES_H
#define THREEPP_AXES_H

#include "quick/scene/Scene.h"
#include <helper/Axes.h>

namespace three {
namespace quick {

class Axes : public ThreeQObject
{
  Q_OBJECT
  Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)

  size_t _size = 1;

  helper::Axes::Ptr _axes;

protected:
  three::Object3D::Ptr _create(Scene *scene) override
  {
    _axes = helper::Axes::make("axes", _size);

    return _axes;
  }

public:
  Axes(QObject *parent = nullptr) : ThreeQObject(parent) {}

  void addTo(ObjectContainer *container) override
  {

  }

  size_t size() {return _size;}

  void setSize(size_t size) {
    if(_size != size) {
      _size = size;
      emit sizeChanged();
    }
  }

signals:
  void sizeChanged();
};

}
}

#endif //THREEPP_AXES_H
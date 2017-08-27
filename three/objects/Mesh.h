//
// Created by byter on 29.07.17.
//

#ifndef THREEQT_MESH
#define THREEQT_MESH

#include <core/Object3D.h>
#include <material/Material.h>

namespace three {

class Mesh : public Object3D
{
  Geometry::Ptr _geometry;
  Material::Ptr _material;

  DrawMode drawMode;

public:
  using Ptr = std::shared_ptr<Mesh>;

  Mesh(const Geometry::Ptr &geometry, const Material::Ptr &material)
     : _geometry(geometry), _material(material) {}
};

}


#endif //THREEQT_MESH

//
// Created by byter on 13.10.17.
//

#ifndef THREEPP_SKELETON_H
#define THREEPP_SKELETON_H

#include <threepp/core/Object3D.h>
#include <threepp/textures/Texture.h>

namespace three {

class Bone : public Object3D
{
  Geometry::Ptr _geometry;

  Bone(Geometry::Ptr geometry) : Object3D(), _geometry(geometry) {}
  Bone(const Bone &bone)
     : Object3D(bone), _geometry(bone.geometry()->cloned()) {}

public:
  using Ptr = std::shared_ptr<Bone>;
  static Ptr make(Geometry::Ptr geometry) {
    return Ptr(new Bone(geometry));
  }

  Bone *cloned() const override {
    return new Bone(*this);
  }
};

class Skeleton
{
  std::vector<Bone::Ptr> _bones;
  std::vector<float> _boneMatrices;
  std::vector<math::Matrix4> _boneInverses;
  Texture::Ptr _boneTexture;
  size_t _boneTextureSize;

  Skeleton() {}

  explicit Skeleton(const std::vector<Bone::Ptr> bones) : _bones(bones)
  {
    for(Bone::Ptr bone : _bones) {
      _boneInverses.push_back(bone->matrixWorld().inverted());
    }
  }
public:
  using Ptr = std::shared_ptr<Skeleton>;
  static Ptr make(const std::vector<Bone::Ptr> bones) {
    return Ptr(new Skeleton(bones));
  }

  const std::vector<Bone::Ptr> bones() const {return _bones;}
  const std::vector<float> boneMatrices() const {return _boneMatrices;}
  const std::vector<math::Matrix4> boneInverses() const {return _boneInverses;}
  const Texture::Ptr boneTexture() const {return _boneTexture;}
  const size_t boneTextureSize() const {return _boneTextureSize;}

  std::vector<float> &boneMatrices() {return _boneMatrices;}

  Skeleton &setBoneTexture(Texture::Ptr texture) {
    _boneTexture = texture;
    return *this;
  }

  Skeleton &setBoneTextureSize(size_t size) {
    _boneTextureSize = size;
    return *this;
  }

  void pose()
  {
    // recover the bind-time world matrices
    size_t index = 0;
    for(Bone::Ptr bone : _bones) {
      bone->matrixWorld() = _boneInverses[index++].inverted();
    }

    // compute the local matrices, positions, rotations and scales
    for(Bone::Ptr bone : _bones) {
      const Bone *parent = bone->parent() ? dynamic_cast<const Bone *>(bone->parent()) : nullptr;
      if (parent) {
        bone->matrix() = parent->matrixWorld().inverted();
        bone->matrix() *= bone->matrixWorld();
      }
      else {
        bone->matrix() = bone->matrixWorld();
      }
      bone->matrix().decompose( bone->position(), bone->quaternion(), bone->scale());
    }
  }

  void update()
  {
    // flatten bone matrices to array
    size_t index = 0;
    for(Bone::Ptr bone : _bones) {
      // compute the offset between the current and the original transform
      math::Matrix4 matrix = bone ? bone->matrixWorld() : math::Matrix4::identity();

      math::Matrix4 offsetMatrix = matrix * _boneInverses[index];
      offsetMatrix.writeTo(_boneMatrices.data(), index * 16);
    }

    if (_boneTexture) {
      _boneTexture->needsUpdate();
    }
  }
};

}
#endif //THREEPP_SKELETON_H

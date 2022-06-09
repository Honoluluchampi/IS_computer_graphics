#pragma once

// hge
#include <hge_actor.hpp>
#include <utility.hpp>
#include <hge_components/mesh_component.hpp>

// std
#include <memory>

namespace iscg {

using hnll::Transform;
template<class T> using s_ptr = std::shared_ptr<T>;
template<class T> using u_ptr = std::unique_ptr<T>;

class Bone : public hnll::HgeActor
{
  constexpr static float BONE_LENGTH = 2.f;

public:
  Bone(s_ptr<Bone> parent = nullptr) : HgeActor(), parent_(parent)
  {}
  bool isRoot() const { return parent_ == nullptr; }
  void updateTransform(const Transform& transform);
  glm::vec3 head()
  { 
    auto transform = getRenderableComponent()->getTransform();
    glm::vec3 diff = BONE_LENGTH * transform.mat4() * glm::vec4(0.f, 1.f, 0.f, 0.f);
    return transform.translation_m + diff;
  }
  glm::vec3 tail()
  { return getRenderableComponent()->getTransform().translation_m; }

  void alignToParent()
  {
    getRenderableComponent()->setTranslation(parent_->head());
    getRenderableComponent()->setRotation(parent_->getRenderableComponent()->getTransform().rotation_m);
  }

private:
  s_ptr<Bone> parent_ = nullptr;
  std::vector<s_ptr<Bone>> children_;

  float length_ = 3.0f;
  // take care to only accept rigit transform
  u_ptr<Transform> rigidTransform_{};
  u_ptr<hnll::MeshComponent> boneModel_;
};

} // namespace iscg
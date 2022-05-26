#pragma once

// hge
#include <hge_actor.hpp>
#include <utility.hpp>
#include <hge_components/model_component.hpp>

// std
#include <memory>

namespace iscg {

using hnll::Transform;
template<class T> using s_ptr = std::shared_ptr<T>;
template<class T> using u_ptr = std::unique_ptr<T>;

class Bone : public hnll::HgeActor
{
public:
  Bone();
  bool isRoot() const { return parent_ == nullptr; }
  void updateTransform(const Transform& transform);

private:
  s_ptr<Bone> parent_ = nullptr;
  std::vector<s_ptr<Bone>> children_;

  float length_ = 3.0f;
  // take care to only accept rigit transform
  u_ptr<Transform> rigidTransform_{};
  u_ptr<hnll::ModelComponent> boneModel_;
};

} // namespace iscg
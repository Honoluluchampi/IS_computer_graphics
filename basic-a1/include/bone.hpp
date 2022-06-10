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

inline float abs(glm::vec2& vec)
{
  return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

inline float dot(glm::vec2& a, glm::vec2& b)
{
  return a.x * b.x + a.y * b.y;
}

inline float abs(glm::vec3& vec)
{
  return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

inline glm::vec3 rotateDifference(const glm::vec3& a, const glm::vec3& b)
{
  glm::vec2 axy = {a.x, a.y}, ayz = {a.y, a.z}, azx = {a.z, a.x};
  glm::vec2 bxy = {b.x, b.y}, byz = {b.y, b.z}, bzx = {b.z, b.x};

  float x = std::acos((ayz.x * byz.y - ayz.y * byz.x >= 0.f) * dot(ayz, byz) / (abs(ayz) * abs(byz))) * 180 / M_PI;
  float y = std::acos((azx.x * bzx.y - azx.y * bzx.x >= 0.f) * dot(azx, bzx) / (abs(azx) * abs(bzx))) * 180 / M_PI;
  float z = std::acos((axy.x * bxy.y - axy.y * bxy.x >= 0.f) * dot(axy, bxy) / (abs(axy) * abs(bxy))) * 180 / M_PI;

  if (!(x >= 0 && x < 360)) x = 0;
  if (!(y >= 0 && y < 360)) y = 0;
  if (!(z >= 0 && z < 360)) z = 0;
  return { x, y, z };
}

class Bone : public hnll::HgeActor
{
  constexpr static float BONE_LENGTH = 2.f;

public:
  Bone(s_ptr<Bone> parent = nullptr) : HgeActor(), parent_(parent)
  {}
  bool isRoot() const { return parent_ == nullptr; }

  Transform& transform() { return getRenderableComponent()->getTransform(); }

  // void updateActor(float dt) override
  // {
  //   std::cout << transform().translation_m.x << " " << transform().translation_m.y << " " << transform().translation_m.z << std::endl;
  // }

  void updateIk(const glm::vec3& controllPoint, const glm::vec3& wholeHeadPoint)
  {
    auto diffToCp = controllPoint - this->tail();
    auto diffToHp = wholeHeadPoint - this->tail();
    
    Transform rotateTransform{};
    rotateTransform.rotation_m += rotateDifference(diffToCp, diffToHp);
    // float dot = diffToCp.x * diffToHp.x + diffToCp.y * diffToHp.y + diffToCp.z * diffToHp.z;
    // float degree = dot / (abs(diffToHp) * abs(diffToCp));
    // glm::mat4 rotation = glm::rotate(glm::mat4{}, degree, glm::cross(diffToHp, diffToCp));

    // rotate myself
    rotateAroundPoint(this->transform(), rotateTransform, this->tail());
    // rotate childlen
    auto& child = this->child_;
    while (child != nullptr) {
      child->rotateAroundPoint(child->transform(), rotateTransform, this->tail());
      child = child->child_;
    }
  }

  // Transform which will be modified, rotating transform, base point position
  void rotateAroundPoint(Transform& targetTransform, Transform& rotateTransform, const glm::vec3& basePoint)
  {
    targetTransform.translation_m -= basePoint;

    targetTransform.translation_m = rotateTransform.rotateMat4() * glm::vec4(targetTransform.translation_m, 1.0f);
    targetTransform.rotation_m += rotateTransform.rotation_m;

    targetTransform.translation_m += basePoint;
  }
  
  glm::vec3 head()
  { 
    auto tf = transform();
    glm::vec3 diff = BONE_LENGTH * tf.rotateMat4() * glm::vec4(0.f, 1.f, 0.f, 0.f);
    return tf.translation_m + diff;
  }
  glm::vec3 tail()
  { return transform().translation_m; }

  void alignToParent()
  {
    getRenderableComponent()->setTranslation(parent_->head());
    getRenderableComponent()->setRotation(parent_->transform().rotation_m);
  }

private:
  s_ptr<Bone> parent_ = nullptr;
  s_ptr<Bone> child_ = nullptr;

  float length_ = 3.0f;
  // take care to only accept rigit transform
  u_ptr<Transform> rigidTransform_{};
  u_ptr<hnll::MeshComponent> boneModel_;
};

} // namespace iscg
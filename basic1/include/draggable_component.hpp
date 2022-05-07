#pragma once

// hge
#include <hge_component.hpp>
#include <utility.hpp>

// lib
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

using hnll::Transform;

template <class S>
using s_ptr = std::shared_ptr<S>;
template <class U>
using u_ptr = std::unique_ptr<U>;

namespace iscg {

// forward declaration
class DragManager;

class DraggableComponent : public hnll::HgeComponent
{
  public:
    DraggableComponent(Transform& transform, float radius) 
      : transform_(transform), radius_(radius){}

    Transform& getTransform()
    { return transform_; }
    float getRadius() { return radius_; }

  private:
    bool isBindedToMouse_ = false;
    Transform& transform_;
    float radius_;
};

} // namespace hnll
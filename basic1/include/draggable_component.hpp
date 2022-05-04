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
    DraggableComponent(DragManager& owner, Transform& transform);

  private:
    void updateComponent(float dt) override;

    void moveToClickProjection();

    bool isBindedToMouse_ = false;

    Transform& transform_;
    float radius = 0.3;

    // temporal
    DragManager& owner_;
};

} // namespace hnll
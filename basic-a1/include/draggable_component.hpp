#pragma once

// hge
#include <hge_component.hpp>
#include <utility.hpp>

// lib
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

// std
#include <functional>

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

    // TODO : template
    // to identify centroid drag
    void setDragCallback(std::function<void(bool)> func)
    { dragCallback_ = func; }
    void executeDragCallback(bool param)
    { dragCallback_(param); }

    Transform& getTransform()
    { return transform_; }
    float getRadius() { return radius_; }

  private:
    Transform& transform_;
    float radius_;
    std::function<void(bool)> dragCallback_ = [](bool){};
};

} // namespace hnll
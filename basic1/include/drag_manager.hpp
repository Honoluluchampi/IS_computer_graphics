#pragma once

#include <draggable_component.hpp>

// hge
#include <hge_actor.hpp>
#include <hge_actors/hge_default_camera.hpp>

// lib
#include <GLFW/glfw3.h>

// std
#include <unordered_map>

namespace iscg {

class DragManager : public hnll::HgeActor
{
  using map = std::unordered_map<hnll::HgeComponent::id_t, s_ptr<DraggableComponent>>;
  
  public:
    DragManager(GLFWwindow* window, hnll::HgeCamera& camera);

    // complete transport
    template <class SP> void addDragComps(SP&& spDragComp)
    { 
      hnll::HgeComponent::id_t id = spDragComp->getCompId();
      dragCompMap_.emplace(id, std::forward<SP>(spDragComp)); 
    }

    inline bool isChanged() const { return isChanged_; }

  private:
    void updateActor(float dt) override;

    void calcCursorProjectionIntersect();
    glm::vec2 calcRawClickPoint();
    glm::vec3 calcWorldClickPoint(const glm::vec2& clickPoint);

    // controll mouse-point binding
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    map dragCompMap_;

    GLFWwindow* window_;
    static bool isDragging_;
    static bool isBinded_;
    bool isChanged_ = false;
    // -1 : no comp is binded
    hnll::HgeComponent::id_t bindedCompId_ = -1;
    // for calculation of new comp pos
    float cameraOriginatedCompZ_;
    hnll::HgeCamera& camera_;
};

} // namespace iscg
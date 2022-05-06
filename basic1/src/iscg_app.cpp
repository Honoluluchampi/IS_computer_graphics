#include <iscg_app.hpp>
#include <drag_manager.hpp>

// hge
#include <hge_components/point_light_component.hpp>

namespace iscg {
void IscgApp::createActor()
{
  // init drag manager
  dragManager_ = std::make_unique<DragManager>(glfwWindow_m, *upCamera_m);
  
  // create 4 controll points
  glm::vec3 positions[4] = {
    {1.f, 0.f, 1.f},
    {1.f, 0.f, -1.f},
    {-1.f, 0.f, 1.f},
    {-1.f, 0.f, -1.f}
  };

  glm::vec3 color = {.1f, 1.f, 1.f};

  for (int i = 0; i < 4; i++) {
    auto controllPoint = std::make_shared<ControllPoint>(positions[i], color, 0.05f);
    addControllPoint(controllPoint);
  }
}

void IscgApp::addControllPoint(s_ptr<ControllPoint>& controllPoint)
{
  addPointLightWithoutOwner(controllPoint->lightComp());
  dragManager_->addDragComps(controllPoint->dragComp());
}

void IscgApp::updateGame(float dt)
{
  dragManager_->update(dt);
}

} // namespace iscg
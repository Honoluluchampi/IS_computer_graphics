#include <iscg_app.hpp>
#include <drag_manager.hpp>

// hge
#include <hge_components/point_light_component.hpp>

namespace iscg {
void IscgApp::createActor()
{
  auto dragManager = new DragManager(this->glfwWindow_m);

  auto actor = std::make_unique<hnll::HgeActor>();
  auto lightComp = hnll::PointLightComponent::createPointLight(actor->getId(), 0.0f, 0.5f, glm::vec3{.1f, .1f, 1.f});
  auto dragComp = std::make_shared<DraggableComponent>(*dragManager, lightComp->getTransform());
  actor->addSharedComponent(std::move(lightComp));
  actor->addSharedComponent(dragComp);
  dragManager->addDragComps(dragComp);
  dragManager_.reset(dragManager);
  addActor(actor);
  addActor(dragManager_);
}
} // namespace iscg
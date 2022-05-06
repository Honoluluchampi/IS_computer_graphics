#include <iscg_app.hpp>
#include <drag_manager.hpp>

// hge
#include <hge_components/point_light_component.hpp>

namespace iscg {
void IscgApp::createActor()
{
  auto actor = std::make_unique<hnll::HgeActor>();
  auto lightComp = hnll::PointLightComponent::createPointLight(actor->getId(), 0.0f, 0.05f, glm::vec3{.1f, .1f, 1.f});
  auto dragComp = std::make_shared<DraggableComponent>(lightComp->getTransform());
  addPointLight(actor, lightComp);
  addDragComp(actor, dragComp);
  addActor(actor);
  addActor(dragManager_);
}

// TODO : multiple dragcomp
void IscgApp::addDragComp(u_ptr<hnll::HgeActor>& owner, s_ptr<DraggableComponent>& dragComp)
{
  auto dragManager = std::make_unique<DragManager>(glfwWindow_m, *upCamera_m);
  owner->addSharedComponent(dragComp);
  dragManager->addDragComps(dragComp);
  dragManager_ = std::move(dragManager);
}
} // namespace iscg
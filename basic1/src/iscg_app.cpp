#include <iscg_app.hpp>
#include <drag_manager.hpp>

// hge
#include <hge_components/point_light_component.hpp>

namespace iscg {

IscgApp::IscgApp() : HgeGame("coons surface")
{
    // init camera transform
  Transform transform{};
  transform.translation_m = {0.f, -10.f, -12.f};
  transform.rotation_m = {glm::radians(-45.f), 0.f, 0.f};
  setCameraTransform(transform);

  createActor();
}

void IscgApp::createActor()
{
  glm::vec3 position[4] = {
    {5.f, -5.f, 5.f},
    {5.f, -5.f, -5.f},
    {-5.f, -5.f, -5.f},
    {-5.f, -5.f, 5.f}
  };

  glm::vec3 color[4] = {
    {.9f, .0f, .0f},
    {.0f, .0f, .9f},
    {.0f, .9f, .0f},
    {.9f, .9f, .0f}
  };
  // create light
  for (int i = 0; i < 4; i++) {
    auto light = std::make_unique<hnll::HgeActor>();
    auto lightComp = hnll::PointLightComponent::createPointLight
      (light->getId(), 100.f, 0.f, color[i]);
    lightComp->setTranslation(position[i]);
    addPointLight(light, lightComp);
  }

  coonsSurface_ = std::make_unique<CoonsSurface>(*this);
}

void IscgApp::updateGame(float dt)
{
  coonsSurface_->updateSurface();
}

} // namespace iscg
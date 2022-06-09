#include <kinematics_app.hpp>

namespace iscg {

ktApp::ktApp()
{
  auto lightActor = createActor();
  auto lightComp = hnll::PointLightComponent::createPointLight(lightActor->getId(), 10.f, 0.f, {1.f, 1.f, 1.f});
  lightComp->setTranslation(glm::vec3(0.f, -10.f, 0.f));
  addPointLight(lightActor, lightComp);
  // root
  bones_.emplace_back(createBone());
  // first
  bones_.emplace_back(createBone());
  bones_[1]->getRenderableComponent()->setTranslation(glm::vec3(0.f, 2.f, 0.f));
}

s_ptr<Bone> ktApp::createBone()
{
  auto bone = hnll::HgeGame::createActor<Bone>();
  auto boneModel = getHveModel("bone");
  auto boneModelComp = std::make_shared<hnll::MeshComponent>(bone->getId(), boneModel);
  bone->addRenderableComponent(boneModelComp);

  return bone;
}

void ktApp::updateGame(float dt)
{
  computeIk();
}

void ktApp::updateGameImgui()
{
  ImGui::Button("create bone");
}

} // namespace iscg
#include <bezier_curve.hpp>

namespace iscg {

ControllPoint::ControllPoint(const glm::vec3& position, const glm::vec3& color, float radius) : HgeActor()
{
  lightComp_ = hnll::PointLightComponent::createPointLight(this->getId(), 0.0f, radius, color);
  lightComp_->getTransform().translation_m = position;
  dragComp_ = std::make_shared<DraggableComponent>(lightComp_->getTransform());
}

// copy s_ptr to head_ and tail_
BezierCurve::BezierCurve(s_ptr<ControllPoint>& head, s_ptr<ControllPoint>& tail) 
  : HgeActor(), head_(head), tail_(tail)
{
  auto head2tail = tail_->getTranslation() - head_->getTranslation();
  for (int i = 0; i < controllPointCount_ - 2; i++) {

  }
}

} // namespace iscg
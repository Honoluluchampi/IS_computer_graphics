#include <bezier_curve.hpp>

namespace iscg {

ControllPoint::ControllPoint(const glm::vec3& position, const glm::vec3& color, float radius) : HgeActor()
{
  lightComp_ = hnll::PointLightComponent::createPointLight(this->getId(), 0.0f, radius, color);
  lightComp_->getTransform().translation_m = position;
  dragComp_ = std::make_shared<DraggableComponent>(lightComp_->getTransform());
}

int BezierCurve::controllPointCount_ = 4;

// copy s_ptr to head_ and tail_
BezierCurve::BezierCurve(s_ptr<ControllPoint>& head, s_ptr<ControllPoint>& tail) 
  : HgeActor(), head_(head), tail_(tail)
{
  auto head2tail = tail_->getTranslation() - head_->getTranslation();
  for (int i = 0; i < controllPointCount_ - 2; i++) {

  }
}

// TODO : use cache of unchaged curve
void BezierCurve::recreateCurve(int dividingCount)
{
  positions_.resize((dividingCount + 1) * (controllPointCount_ - 1) + 1);

  auto headPos = head_->getTranslation(), tailPos = tail_->getTranslation();
  positions_[0] = headPos;
  // TODO : implement for general bezier
  // equally sampling
  for (float i = 1; i < positions_.size(); i++) {
    float t = i * (1.f / (positions_.size() - 1));
    auto ti = 1.f - t;
    
    auto trib1 = headPos;
    trib1 *= std::pow(ti, 3);
    auto trib2 = midControllPoints_[0]->getTranslation();
    trib2 *= 3 * t * std::pow(ti, 2);
    auto trib3 = midControllPoints_[1]->getTranslation();
    trib3 *= 3 * std::pow(t, 2) * ti;
    auto trib4 = tailPos;
    trib4 *= std::pow(ti, 3);

    positions_[i] = trib1 + trib2 + trib3 + trib4;
  }
}

} // namespace iscg
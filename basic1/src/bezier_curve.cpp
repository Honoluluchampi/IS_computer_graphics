#include <bezier_curve.hpp>

namespace iscg {

ControllPoint::ControllPoint(const glm::vec3& position, const glm::vec3& color, float radius) : HgeActor()
{
  lightComp_ = hnll::PointLightComponent::createPointLight(this->getId(), 0.0f, radius, color);
  lightComp_->getTransform().translation_m = position;
  dragComp_ = std::make_shared<DraggableComponent>(lightComp_->getTransform(), radius);
}

int BezierCurve::controllPointCount_ = 4;
int BezierCurve::dividingCount_ = 15;

// copy s_ptr to head_ and tail_
BezierCurve::BezierCurve(s_ptr<ControllPoint>& head, s_ptr<ControllPoint>& tail) 
  : HgeActor(), head_(head), tail_(tail)
{
}

// TODO : use cache of unchaged curve
void BezierCurve::recreateCurve()
{
  positions_.resize((dividingCount_ + 1) * (controllPointCount_ - 1) + 1);

  auto headPos = head_->getTranslation(), tailPos = tail_->getTranslation();
  positions_[0] = headPos;
  // TODO : implement
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
    trib4 *= std::pow(t, 3);

    positions_[i] = trib1 + trib2 + trib3 + trib4;
  }
}

} // namespace iscg
#include <bezier_curve.hpp>

namespace iscg {

ControllPoint::ControllPoint(const glm::vec3& position, const glm::vec3& color, float radius) : HgeActor()
{
  lightComp_ = hnll::PointLightComponent::createPointLight(this->getId(), 0.0f, radius, color);
  lightComp_->getTransform().translation_m = position;
  dragComp_ = std::make_shared<DraggableComponent>(lightComp_->getTransform(), radius);
}

ControllPoint::ControllPoint(const std::vector<s_ptr<ControllPoint>>& basePoints, const glm::vec3& color, float radius)
  : HgeActor()
{
  isCentroid_ = true;
  lightComp_ = hnll::PointLightComponent::createPointLight(this->getId(), 0.0f, radius, color);
  // calc centroid position
  glm::vec3 pos;
  for (const auto& point : basePoints) 
    pos += point->lightComp()->getTransform().translation_m;
  pos /= basePoints.size();
  lightComp_->getTransform().translation_m = pos;
  dragComp_ = std::make_shared<DraggableComponent>(lightComp_->getTransform(), radius);
}

int BezierCurve::controllPointCount_ = 4;
int BezierCurve::dividingCount_ = 15;

// copy s_ptr to head_ and tail_
BezierCurve::BezierCurve(s_ptr<ControllPoint>& head, s_ptr<ControllPoint>& tail) 
  : HgeActor(), head_(head), tail_(tail)
{
}

int combination(int n, int c)
{
  c = std::min(c, n - c);
  int res = 1;
  for (int i = 0; i < c; i++) {
    res *= n - i;
    res /= i + 1;
  }
  return res;
}

void BezierCurve::recreateCurve()
{
  positions_.resize((dividingCount_ + 1) * (controllPointCount_ - 1) + 1);

  auto headPos = head_->getTranslation(), tailPos = tail_->getTranslation();
  positions_[0] = headPos;
  // equally sampling
  for (float i = 1; i < positions_.size(); i++) {
    float t = i * (1.f / (positions_.size() - 1));
    auto ti = 1.f - t;
    
    glm::vec3 tribs[controllPointCount_];
    // fill tribs
    tribs[0] = head_->getTranslation();
    tribs[controllPointCount_-1] = tail_->getTranslation();
    for (int j = 1; j < controllPointCount_ - 1; j++) {
      tribs[j] = midControllPoints_[j-1]->getTranslation();
    }
    // calc coeffecient of each tribs
    for (int j = 0; j < controllPointCount_; j++) {
      tribs[j] *= combination(controllPointCount_ - 1, j) * std::pow(t, j) * std::pow(ti, controllPointCount_ - 1 - j);
    }
    // write answer
    positions_[i] = glm::vec3{0.f, 0.f, 0.f};
    for (const auto& trib : tribs) {
      positions_[i] += trib; 
    } 
  }
}

void BezierCurve::recreateControllLine(hnll::HgeGame& app, glm::vec3& color, float radius)
{
  // first segment
  auto line = std::make_shared<hnll::LineComponent>(
    getId(),
    head_->getTranslation(),
    midControllPoints_[0]->getTranslation(),
    color, 
    radius
  );
  app.addRenderableComponent(line);
  controllLines_.push_back(std::move(line));

  // mid segments
  for (int i = 0; i < controllPointCount_ - 3; i++) {
    line = std::make_shared<hnll::LineComponent>(
      getId(),
      midControllPoints_[i]->getTranslation(),
      midControllPoints_[i + 1]->getTranslation(),
      color,
      radius
    );
    app.addRenderableComponent(line);
    controllLines_.push_back(std::move(line));
  }
  
  // last segment
  line = std::make_shared<hnll::LineComponent>(
    getId(),
    midControllPoints_[controllPointCount_ - 3]->getTranslation(),
    tail_->getTranslation(),
    color, 
    radius
  );
  app.addRenderableComponent(line);
  controllLines_.push_back(std::move(line));
}

} // namespace iscg
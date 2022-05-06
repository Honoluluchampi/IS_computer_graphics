#include <iscg_app.hpp>
#include <drag_manager.hpp>

// hge
#include <hge_components/point_light_component.hpp>

namespace iscg {
void IscgApp::createActor()
{
  // init drag manager
  dragManager_ = std::make_unique<DragManager>(glfwWindow_m, *upCamera_m);

  // init surface param
  basePoints_.resize(4);
  bezierCurves_.resize(4);
  
  // create 4 controll points
  // these positions should be placed clockwise or counter-clockwise
  glm::vec3 positions[4] = {
    {1.f, 0.f, 1.f},
    {1.f, 0.f, -1.f},
    {-1.f, 0.f, -1.f},
    {-1.f, 0.f, 1.f}
  };

  // create 4 base points
  for (int i = 0; i < 4; i++) {
    auto controllPoint = std::make_shared<ControllPoint>(positions[i], pointColor_, pointRadius_);
    basePoints_[i] = controllPoint;
    addControllPoint(controllPoint);
  }

  // init 4 bezier curves
  for (int i = 0; i < 4; i++) {
    bezierCurves_[i] = std::make_unique<BezierCurve>(basePoints_[i], basePoints_[(i + 1) % 4]);
  }

  // create each mid points and move them to bezier curve
  createMidControllPoints();
}

void IscgApp::addControllPoint(s_ptr<ControllPoint>& controllPoint)
{
  addPointLightWithoutOwner(controllPoint->lightComp());
  dragManager_->addDragComps(controllPoint->dragComp());
}

void IscgApp::createMidControllPoints(int pointCount)
{
  for (int i = 0; i < 4; i++) {
    // create from scratch
    bezierCurves_[i]->clearMidControllPoint();
    auto head2tail = basePoints_[(i + 1) % pointCount]->getTranslation() - basePoints_[i]->getTranslation();
    for (int j = 0; j < pointCount - 1; j++) {
      // calc vec the head to the new
      auto diff = head2tail;
      diff *= (j + 1) * (1.f / (pointCount - 1));

      auto controllPoint = std::make_shared<ControllPoint>(basePoints_[i]->getTranslation() + diff, pointColor_, pointRadius_);
      addControllPoint(controllPoint);
      // add to bezier
      bezierCurves_[i]->addMidControllPoint(controllPoint);
    }
  } 
}

void IscgApp::updateGame(float dt)
{
  dragManager_->update(dt);
}

} // namespace iscg
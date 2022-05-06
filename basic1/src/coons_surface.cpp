#include <coons_surface.hpp>
#include <iscg_app.hpp>

namespace iscg {

CoonsSurface::CoonsSurface(IscgApp& iscgApp) : HgeActor(), iscgApp_(iscgApp)
{
  // init drag manager
  dragManager_ = std::make_unique<DragManager>(iscgApp_.getGlfwWindow(), iscgApp_.getCamera());

  // init surface param
  basePoints_.resize(4);
  bezierCurves_.resize(4);
  
  // create 4 controll points
  // these positions should be placed clockwise or counter-clockwise
  float val = 5.f;
  glm::vec3 positions[4] = {
    {val, 0.f, val},
    {val, 0.f, -val},
    {-val, 0.f, -val},
    {-val, 0.f, val}
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

  // first mesh
  recreateSurfaceMesh();
}

void CoonsSurface::updateSurface()
{
  dragManager_->update(0.f);
  if (dragManager_->isChanged())
    recreateSurfaceMesh();
}

void CoonsSurface::createMidControllPoints()
{
  auto controllPointCount = BezierCurve::getControllPointCount();

  for (int i = 0; i < 4; i++) {
    // create from scratch
    bezierCurves_[i]->clearMidControllPoint();
    auto head2tail = basePoints_[(i + 1) % controllPointCount]->getTranslation() - basePoints_[i]->getTranslation();
    for (int j = 0; j < controllPointCount - 1; j++) {
      // calc vec the head to the new
      auto diff = head2tail;
      diff *= (j + 1) * (1.f / (controllPointCount - 1));

      auto controllPoint = std::make_shared<ControllPoint>(basePoints_[i]->getTranslation() + diff, pointColor_, pointRadius_);
      addControllPoint(controllPoint);
      // add to bezier
      bezierCurves_[i]->addMidControllPoint(controllPoint);
    }
  } 
}

void CoonsSurface::addControllPoint(s_ptr<ControllPoint>& controllPoint)
{
  iscgApp_.addPointLightWithoutOwner(controllPoint->lightComp());
  dragManager_->addDragComps(controllPoint->dragComp());
}

void CoonsSurface::recreateSurfaceMesh()
{
  // calc 4 edges first (bezier curves)
  for (auto& bezierCurve : bezierCurves_)
    bezierCurve->recreateCurve(dividingCount_);

  auto controllPointCount = BezierCurve::getControllPointCount();

  glm::vec3 positions[(dividingCount_ + 1) * (controllPointCount - 1) + 1][(dividingCount_ + 1) * (controllPointCount - 1) + 1];

  // calc 4 edges first (bezier curves)

}

} // namespace iscg
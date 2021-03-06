#include <coons_surface.hpp>
#include <iscg_app.hpp>

// hve
#include <utility.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std {

struct Hash
{
  size_t operator() (hnll::HveModel::Vertex const &vertex) const
  {
    // stores final hash value
    size_t seed = 1;
    hnll::hashCombine(seed, vertex.position_m, vertex.color_m, vertex.normal_m, vertex.uv_m);
    return seed;
  }
};
}

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
  recreateMidControllPoints();
  recreateCentroid();
  recreateControllLines();
  // first mesh
  recreateSurfaceMesh();
}

void CoonsSurface::updateSurface()
{
  dragManager_->update(0.f);

  if (controllPointCountCache_ != BezierCurve::getControllPointCount()) {
    recreateMidControllPoints();
    recreateControllLines();
    recreateSurfaceMesh();
  }

  else if (dragManager_->isChanged() || dividingCountCache_ != BezierCurve::getDividingCount()) 
    recreateSurfaceMesh();
}

void CoonsSurface::recreateMidControllPoints()
{
  auto controllPointCount = BezierCurve::getControllPointCount();

  // bezier curve loop
  for (int i = 0; i < 4; i++) {
    // create from scratch
    // clear point light
    for (auto &point : bezierCurves_[i]->getMidControllPointsRef()) {
      iscgApp_.removePointLightWithoutOwner(point->lightComp()->getCompId());
      dragManager_->removeDragComp(point->dragComp()->getCompId());
    }
    bezierCurves_[i]->clearMidControllPoint();

    auto head2tail = basePoints_[(i + 1) % basePoints_.size()]->getTranslation() - basePoints_[i]->getTranslation();
    // mid-controll point loop
    for (int j = 1; j < controllPointCount - 1; j++) {
      // calc vec the head to the new
      auto diff = head2tail;
      diff *= j * (1.f / (controllPointCount - 1));

      auto controllPoint = std::make_shared<ControllPoint>(basePoints_[i]->getTranslation() + diff, pointColor_, pointRadius_);
      addControllPoint(controllPoint);
      // add to bezier
      bezierCurves_[i]->addMidControllPoint(controllPoint);
    }
    if (iscgApp_.hideControllPoints()) changeControllPointsRadius(0);
  } 

  // cache 
  controllPointCountCache_ = BezierCurve::getControllPointCount();
}

// call after basepoint creation
void CoonsSurface::recreateCentroid()
{ 
  centroid_ = std::make_unique<ControllPoint>(basePoints_, centroidColor_, pointRadius_);
}

void CoonsSurface::addControllPoint(s_ptr<ControllPoint>& controllPoint)
{
  iscgApp_.addPointLightWithoutOwner(controllPoint->lightComp());
  dragManager_->addDragComps(controllPoint->dragComp());
}

void CoonsSurface::recreateSurfaceMesh()
{
  auto controllPointCount = BezierCurve::getControllPointCount();
  auto dividingCount = BezierCurve::getDividingCount();
  int linePointCount = (dividingCount + 1) * (controllPointCount - 1) + 1;
  glm::vec3 positions[linePointCount][linePointCount];
  
  // calc 4 edges first (bezier curves)
  for (auto& bezierCurve : bezierCurves_)
      bezierCurve->recreateCurve();
  // get caluculated bezier curves
  // 1 2
  // 4 3
  auto& bezierLinePositions = bezierCurves_[0]->getLinePointPositions();
  for (int i = 0; i < bezierLinePositions.size(); i++) {
    positions[0][i] = bezierLinePositions[i];
  }
  bezierLinePositions = bezierCurves_[1]->getLinePointPositions();
  for (int i = 0; i < bezierLinePositions.size(); i++) {
    positions[i][linePointCount - 1] = bezierLinePositions[i];
  }
  bezierLinePositions = bezierCurves_[2]->getLinePointPositions();
  for (int i = bezierLinePositions.size() - 1; i >= 0; i--) {
    positions[linePointCount - 1][i] = bezierLinePositions[bezierLinePositions.size() - 1 - i];
  }
  bezierLinePositions = bezierCurves_[3]->getLinePointPositions();
  for (int i = bezierLinePositions.size() - 1; i >= 0; i--) {
    positions[i][0] = bezierLinePositions[bezierLinePositions.size() - 1 - i];
  }

  // add new point -> register two triangle to buffer
  hnll::HveModel::Builder builder;
  // to create index buffer
  std::unordered_map<hnll::HveModel::Vertex, uint32_t, std::Hash> uniqueVertices{};

  // TODO : parallelize
  for (int i = 1; i < linePointCount - 1; i++) {
    for (int j = 1; j < linePointCount - 1; j++) {
      // calc new point pos
      float t = (float)i / (float)(linePointCount - 1);
      float s = (float)j / (float)(linePointCount - 1);
      float ti = 1.f - t, si = 1.f - s;
      glm::vec3 lc = sclXvec(ti, positions[0][j]) + sclXvec(t, positions[linePointCount - 1][j]);
      glm::vec3 ld = sclXvec(si, positions[i][0]) + sclXvec(s, positions[i][linePointCount - 1]);
      glm::vec3 b = sclXvec(si * ti, positions[0][0]) + sclXvec(s * ti, positions[0][linePointCount - 1])
        + sclXvec(si * t, positions[linePointCount - 1][0]) + sclXvec(s * t, positions[linePointCount - 1][linePointCount - 1]);
      positions[i][j] = lc + ld - b;
    }
  }

  for (int i = 1; i < linePointCount; i++) {
    for (int j = 1; j < linePointCount; j++) {
      // register two triangle to buffer
      // upper triangle
      glm::vec3 normal = glm::cross(positions[i][j-1] - positions[i-1][j-1], positions[i-1][j] - positions[i-1][j-1]); 
      hnll::HveModel::Vertex vertex[3] = {};
      // record three vertex
      vertex[0].position_m = positions[i-1][j-1];
      vertex[0].color_m = surfaceColor_;
      vertex[0].normal_m = normal;
      vertex[1].position_m = positions[i][j-1];
      vertex[1].color_m = surfaceColor_;
      vertex[1].normal_m = normal;
      vertex[2].position_m = positions[i-1][j];
      vertex[2].color_m = surfaceColor_;
      vertex[2].normal_m = normal;
      for (auto& vert : vertex) {
        if (uniqueVertices.count(vert) == 0) {
          uniqueVertices[vert] = static_cast<uint32_t>(builder.vertices_m.size());
          builder.vertices_m.push_back(vert);
        }
        builder.indices_m.push_back(uniqueVertices[vert]);
      }
      // lower triangle (use same color)
      normal = glm::cross(positions[i][j] - positions[i][j-1], positions[i-1][j] - positions[i][j]); 
      vertex[0].position_m = positions[i][j];
      vertex[0].normal_m = normal;
      vertex[1].position_m = positions[i-1][j];
      vertex[1].normal_m = normal;
      vertex[2].position_m = positions[i][j-1];
      vertex[2].normal_m = normal;
      for (auto& vert : vertex) {
        if (uniqueVertices.count(vert) == 0) {
          uniqueVertices[vert] = static_cast<uint32_t>(builder.vertices_m.size());
          builder.vertices_m.push_back(vert);
        }
        builder.indices_m.push_back(uniqueVertices[vert]);
      }
    }
    dividingCountCache_ = BezierCurve::getDividingCount();
  }

  // recreate model comp
  auto hveSurface = std::make_shared<hnll::HveModel>(iscgApp_.hveDevice(), builder);
  
  if (hasModel_) {
    modelComp_->replaceHveModel(hveSurface);
  }
  else {
    modelComp_ = std::make_shared<hnll::MeshComponent>(getId(), std::move(hveSurface));
    addRenderableComponent(modelComp_);
    iscgApp_.addRenderableComponent(modelComp_);
    hasModel_ = true;
  }
}

void CoonsSurface::recreateControllLines()
{
  // bezier curve loop
  for (int bez = 0; bez < 4; bez++) {
    // clear existing controll lines
    for (auto& line : bezierCurves_[bez]->getControllLinesRef()) {
      iscgApp_.removeRenderableComponent(line->getRenderType(), line->getCompId()); 
    }
    bezierCurves_[bez]->clearControllLine();

    // create line
    bezierCurves_[bez]->recreateControllLine(iscgApp_, lineColor_, lineRadius_);
  }
}

void CoonsSurface::changeControllPointsRadius(float radius)
{
  if (radius == -1) radius = pointRadius_;
  // basepoint
  for (auto &basePoint : basePoints_)
    basePoint->lightComp()->setRadius(radius);
  // mid point
  for (auto &bezier : bezierCurves_)
    for (auto & point :bezier->getMidControllPointsRef())
      point->lightComp()->setRadius(radius);
}

} // namespace iscg
#pragma once

#include <bezier_curve.hpp>
#include <drag_manager.hpp>

// hve
#include <hve_model.hpp>

// hge
#include <hge_actor.hpp>
#include <hge_components/model_component.hpp>

namespace iscg {

class CoonsSurface : hnll::HgeActor
{
  public:
    CoonsSurface(IscgApp& iscgApp);

    void updateSurface();

    // -1 indicates default size
    void changeControllPointsRadius(float radius = -1);

  private:  
    // create each mid points and move them to bezier curve
    void recreateMidControllPoints();
    void recreateControllLines();
    void addControllPoint(s_ptr<ControllPoint>& controllPoint);
    void recreateSurfaceMesh();

    IscgApp& iscgApp_;
    u_ptr<DragManager> dragManager_;

    std::vector<u_ptr<BezierCurve>> bezierCurves_;
    std::vector<s_ptr<ControllPoint>> basePoints_;

    // to check wheather those param was changed
    int controllPointCountCache_ = -1;
    int dividingCountCache_ = -1;
    // point param
    glm::vec3 pointColor_ = {1.f, .1f, .1f};
    float pointRadius_ = 0.2f;
    // line param
    glm::vec3 lineColor_ = {.9f, .2f, .2f};
    float lineRadius_ = 0.05f;
    // surface param
    glm::vec3 surfaceColor_ = {1.f, 1.f, 1.f};

    s_ptr<hnll::ModelComponent> modelComp_;
    bool hasModel_ = false;
};

} // namespace
#pragma once

#include <bezier_curve.hpp>
#include <drag_manager.hpp>

// hve
#include <hve_model.hpp>

// hge
#include <hge_actor.hpp>
#include <hge_components/model_component.hpp>

namespace iscg {

// forward declaration
class IscgApp;

class CoonsSurface : hnll::HgeActor
{
  public:
    CoonsSurface(IscgApp& iscgApp);

    void updateSurface();

  private:  
    // create each mid points and move them to bezier curve
    void createMidControllPoints();
    void addControllPoint(s_ptr<ControllPoint>& controllPoint);
    void recreateSurfaceMesh();

    IscgApp& iscgApp_;
    u_ptr<DragManager> dragManager_;

    std::vector<u_ptr<BezierCurve>> bezierCurves_;
    std::vector<s_ptr<ControllPoint>> basePoints_;

    // point param
    glm::vec3 pointColor_ = {1.f, .1f, .1f};
    float pointRadius_ = 0.2f;

    // number of vertices between two controll point
    int dividingCount_ = 15;

    glm::vec3 surfaceColor_ = {1.f, 1.f, 1.f};

    s_ptr<hnll::HveModel> surfaceMesh_;
    bool hasModel_ = false;
};

} // namespace
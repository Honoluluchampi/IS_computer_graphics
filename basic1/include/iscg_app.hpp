#pragma once

#include <drag_manager.hpp>
#include <bezier_curve.hpp>
#include <coons_surface.hpp>

// hnll
#include <hge_game.hpp>

// IS computer graphics
namespace iscg {

class IscgApp : public hnll::HgeGame
{
  public:
    IscgApp();

    // getter
    GLFWwindow* getGlfwWindow() const { return glfwWindow_m; }
    hnll::HgeCamera& getCamera() { return *upCamera_m; }

    bool hideControllPoints() const { return hideControllPoints_; }
    
  private:
    void updateGame(float dt) override;
    void updateGameImgui() override;
    void loadActor() override;   

    u_ptr<CoonsSurface> coonsSurface_;
    bool hideControllPoints_ = false;
    bool hideControllPointsCache_ = false;
};

} // namespace iscg
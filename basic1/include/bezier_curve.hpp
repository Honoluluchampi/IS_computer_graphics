#pragma once

#include <hge_actor.hpp>

namespace hnll {

class BezierCurve : public HgeActor
{
public:
  BezierCurve();

private:
  int controllPointCount_;
};

} // namespace hnll
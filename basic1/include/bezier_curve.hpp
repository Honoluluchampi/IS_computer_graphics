#pragma once

#include <hge_actor.hpp>

namespace hnll {

class BezierCurve : public HgeActor
{
public:
  BezierCurve() : HgeActor() {}

private:
  int controllPointCount_;
};

} // namespace hnll
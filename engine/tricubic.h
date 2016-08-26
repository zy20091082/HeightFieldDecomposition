#ifndef TRI_CUBIC_INTERPOLATOR_H
#define TRI_CUBIC_INTERPOLATOR_H

#include <Eigen/Core>
#include "common/arrays.h"
#include "common/point.h"

typedef float gridreal;

namespace TricubicInterpolator {

    void getCoefficients(Array4D<gridreal>& coeffs, const Array3D<gridreal> &weights);

    double getValue(const Pointd &p, const gridreal* coeffs);
}

#endif

#ifndef TRI_CUBIC_INTERPOLATOR_H
#define TRI_CUBIC_INTERPOLATOR_H

#include <Eigen/Core>
#include "common/arrays.h"
#include "common/point.h"

namespace TricubicInterpolator {

    void getCoefficients(Array4D<double>& coeffs, const Array3D<double> &weights);

    double getValue(const Pointd &p, const double* coeffs);
}

#endif

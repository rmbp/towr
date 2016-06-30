/*
 * optimization_variables.cc
 *
 *  Created on: May 24, 2016
 *      Author: winklera
 */

#include <xpp/zmp/optimization_variables.h>

namespace xpp {
namespace zmp {

static constexpr int kDim2d = 2; // X,Y

OptimizationVariables::OptimizationVariables ()
{
}

OptimizationVariables::~OptimizationVariables ()
{
}

void
OptimizationVariables::Init (int n_spline_coeff, int n_steps)
{
  nlp_structure_.Init(n_spline_coeff, n_steps);
  initialized_ = true;
}

void
OptimizationVariables::Init (const VectorXd& x_coeff_abcd,
                             const StdVecEigen2d& footholds)
{
  Init(x_coeff_abcd.rows(), footholds.size());

  nlp_structure_.SetSplineCoefficients(x_coeff_abcd);
  nlp_structure_.SetFootholds(ConvertStdToEig(footholds));
}

void
xpp::zmp::OptimizationVariables::SetVariables (const VectorXd& x)
{
  assert(initialized_);
  nlp_structure_.SetAllVariables(x);
  NotifyObservers();
}

void
xpp::zmp::OptimizationVariables::SetVariables (const double* x)
{
  assert(initialized_);
  nlp_structure_.SetAllVariables(x);
  NotifyObservers();
}

OptimizationVariables::VectorXd
OptimizationVariables::GetOptimizationVariables () const
{
  return nlp_structure_.GetOptimizationVariables();
}

int
OptimizationVariables::GetOptimizationVariableCount () const
{
  assert(initialized_);
  return nlp_structure_.GetOptimizationVariableCount();
}

OptimizationVariables::StdVecEigen2d
OptimizationVariables::GetFootholdsStd () const
{
  assert(initialized_);

  Eigen::VectorXd footholds_xy = nlp_structure_.GetFootholdsEig();
  StdVecEigen2d fooothold_vec(footholds_xy.rows()/2.);
  for (int i=0; i<fooothold_vec.size(); ++i) {
    fooothold_vec.at(i) = footholds_xy.segment<kDim2d>(kDim2d*i);
  }

  return fooothold_vec;
}

OptimizationVariables::VectorXd
OptimizationVariables::ConvertStdToEig(const StdVecEigen2d& footholds_xy) const
{
  int n_steps = footholds_xy.size();

  VectorXd vec(kDim2d*n_steps);
  int c=0;
  for (int step=0; step<n_steps; ++step)
  {
    vec[c++] = footholds_xy.at(step).x();
    vec[c++] = footholds_xy.at(step).y();
  }
  return vec;
}

OptimizationVariables::VectorXd
OptimizationVariables::GetSplineCoefficients () const
{
  assert(initialized_);
  return nlp_structure_.GetSplineCoefficients();
}

} /* namespace zmp */
} /* namespace xpp */

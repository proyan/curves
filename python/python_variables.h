#include "curves/linear_variable.h"
#include "curves/bezier_curve.h"
#include "curves/polynomial.h"
#include "curves/exact_cubic.h"
#include "curves/curve_constraint.h"
#include "curves/curve_conversion.h"
#include "curves/bernstein.h"
#include "curves/cubic_hermite_spline.h"
#include "curves/piecewise_curve.h"
#include "curves/so3_linear.h"
#include "curves/se3_curve.h"
#include "python_definitions.h"
#include <eigenpy/memory.hpp>
#include <eigenpy/eigenpy.hpp>
#include <eigenpy/geometry.hpp>
#include <Eigen/Dense>

#include <vector>

#ifndef _VARIABLES_PYTHON_BINDINGS
#define _VARIABLES_PYTHON_BINDINGS


namespace curves
{
  static const int dim = 3;
  typedef linear_variable<real, true> linear_variable_t;
  typedef quadratic_variable<real> quadratic_variable_t;
  typedef bezier_curve  <real, real, true, linear_variable_t> bezier_linear_variable_t;

  /*linear variable control points*/
  bezier_linear_variable_t* wrapBezierLinearConstructor(const point_list_t& matrices, const point_list_t& vectors);
  bezier_linear_variable_t* wrapBezierLinearConstructorBounds
  (const point_list_t& matrices, const point_list_t& vectors, const real T_min, const real T_max);

  typedef Eigen::Matrix<real, Eigen::Dynamic, Eigen::Dynamic> matrix_x_t;

  struct matrix_pair
  {
      matrix_pair() {}
      matrix_pair(const Eigen::Ref<const matrix_x_t > A, const Eigen::Ref<const matrix_x_t > b)
          : A_(A), b_(b){}
      matrix_x_t A_;
      matrix_x_t b_;
      matrix_x_t A() {return A_;}
      matrix_x_t b() {return b_;}
  };

  Eigen::Matrix<real, Eigen::Dynamic, Eigen::Dynamic> cost_t_quad(const quadratic_variable_t& p);
  Eigen::Matrix<real, Eigen::Dynamic, 1> cost_t_linear(const quadratic_variable_t & p);
  real cost_t_constant(const quadratic_variable_t & p);

  linear_variable_t *wayPointsToLists(const bezier_linear_variable_t& self);

  struct LinearBezierVector
  {
    std::vector<bezier_linear_variable_t> beziers_;
    std::size_t size() {return beziers_.size();}
    bezier_linear_variable_t* at(std::size_t i)
    {
      assert (i<size());
      return new bezier_linear_variable_t(beziers_[i]);
    }
  };


  /*** TEMPLATE SPECIALIZATION FOR PYTHON ****/
  typedef double real;
  typedef Eigen::VectorXd time_waypoints_t;

  typedef Eigen::VectorXd pointX_t;
  typedef Eigen::Matrix<double,3, 1> point3_t;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1, 0, Eigen::Dynamic, 1> ret_pointX_t;
  typedef std::pair<pointX_t, pointX_t> pair_pointX_tangent_t;
  typedef Eigen::MatrixXd pointX_list_t;
  typedef std::vector<pointX_t,Eigen::aligned_allocator<pointX_t> >  t_pointX_t;
  typedef std::vector<pointX_t,Eigen::aligned_allocator<point3_t> >  t_point3_t;
  typedef std::vector<pair_pointX_tangent_t,Eigen::aligned_allocator<pair_pointX_tangent_t> > t_pair_pointX_tangent_t;
  typedef curves::curve_constraints<pointX_t> curve_constraints_t;
  typedef curves::curve_constraints<point3_t> curve_constraints3_t;
  typedef std::pair<real, pointX_t> waypoint_t;
  typedef std::vector<waypoint_t> t_waypoint_t;
  typedef Eigen::Matrix<real,3, 3> matrix3_t;
  typedef Eigen::Matrix<real,4, 4> matrix4_t;
  typedef Eigen::Transform<double,3,Eigen::Affine> transform_t;
  typedef Eigen::Quaternion<real> quaternion_t;


  // Curves
  typedef curve_abc<real, real, true, pointX_t> curve_abc_t; // generic class of curve
  typedef curve_abc<real, real, true, point3_t> curve_3_t; // generic class of curve of size 3
  typedef curve_abc<real, real, true, matrix3_t,point3_t> curve_rotation_t; // templated class used for the rotation (return dimension are fixed)
  typedef curves::cubic_hermite_spline <real, real, true, pointX_t> cubic_hermite_spline_t;
  typedef curves::bezier_curve  <real, real, true, pointX_t> bezier_t;
  typedef curves::bezier_curve  <real, real, true, Eigen::Vector3d> bezier3_t;
  typedef curves::polynomial  <real, real, true, pointX_t, t_pointX_t> polynomial_t;
  typedef polynomial_t::coeff_t coeff_t;
  typedef curves::piecewise_curve <real, real, true, pointX_t, t_pointX_t, polynomial_t> piecewise_polynomial_curve_t;
  typedef curves::piecewise_curve <real, real, true, pointX_t, t_pointX_t, bezier_t> piecewise_bezier_curve_t;
  typedef curves::piecewise_curve <real, real, true, pointX_t, t_pointX_t, cubic_hermite_spline_t> piecewise_cubic_hermite_curve_t;
  typedef curves::piecewise_curve <real, real, true, linear_variable_t, std::vector<linear_variable_t, Eigen::aligned_allocator<linear_variable_t> >, bezier_linear_variable_t> piecewise_bezier_linear_curve_t;
  typedef curves::exact_cubic  <real, real, true, pointX_t, t_pointX_t> exact_cubic_t;
  typedef SO3Linear  <double, double, true> SO3Linear_t;
  typedef SE3Curve  <double, double, true> SE3Curve_t;

  // Bezier 3
  typedef curves::bezier_curve  <real, real, true, Eigen::Vector3d> bezier3_t;
  typedef curves::Bern<double> bernstein_t;

  /*** TEMPLATE SPECIALIZATION FOR PYTHON ****/
} //namespace curve.


EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::bernstein_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::cubic_hermite_spline_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::bezier_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::bezier3_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::polynomial_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::curve_constraints_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::piecewise_polynomial_curve_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::piecewise_bezier_curve_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::piecewise_cubic_hermite_curve_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::piecewise_bezier_linear_curve_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::exact_cubic_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::SO3Linear_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::SE3Curve_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::matrix_x_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::pointX_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::linear_variable_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::bezier_linear_variable_t)
EIGENPY_DEFINE_STRUCT_ALLOCATOR_SPECIALIZATION(curves::matrix_pair)


#endif //_VARIABLES_PYTHON_BINDINGS

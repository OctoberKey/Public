//******************************************************************************
//
/// \file       RotationTransformation.h
/// \brief      
/// \author     诸佳琪/zhujiaqi@jsjd.cc
/// \version    V1.0
/// \date       2019-07-30
/// \copyright  杭州中为光电技术有限公司
//
//******************************************************************************

#pragma once

#include <opencv2/opencv.hpp>

///
/// \brief 线性代数命名空间
///
namespace my_linear_algebra {
///
/// \brief    将角度值转换成弧度值
/// \param    [in]  d 角度值
/// \return   弧度值
///
double Deg2Rad(const double d)
{
  return d * CV_PI / 180;
}
///
/// \brief    将弧度值转成角度值
/// \param    [in]  r 弧度值
/// \return   角度值
///
double Rad2Deg(const double r)
{
  return r * 180 / CV_PI;
}
///
/// \brief    计算x轴旋转矩阵
/// 
/// 计算坐标系绕x轴旋转的3*3矩阵，以左手系为例：旋转方向遵循左手螺旋法则
/// 
/// \param    [in]  psi 绕x轴旋转的角度
/// \return   3*3矩阵
///
cv::Matx33d Angle2Rx(const double psi)
{
  double rad = Deg2Rad(psi);
  return cv::Matx33d(1, 0, 0,
                     0, cos(rad), sin(rad),
                     0, -sin(rad), cos(rad));
}
///
/// \brief    计算y轴旋转矩阵
/// 
/// 计算坐标系绕x轴旋转的3*3矩阵，以左手系为例：旋转方向遵循左手螺旋法则
/// 
/// \param    [in]  phi 绕y轴旋转的角度
/// \return   3*3矩阵
///
cv::Matx33d Angle2Ry(const double phi)
{
  double rad = Deg2Rad(phi);
  return cv::Matx33d(cos(rad), 0, -sin(rad),
                     0, 1, 0,
                     sin(rad), 0, cos(rad));
}
///
/// \brief    计算z轴旋转矩阵
/// 
/// 计算坐标系绕x轴旋转的3*3矩阵，以左手系为例：旋转方向遵循左手螺旋法则
/// 
/// \param    [in]  theta 绕z轴旋转的角度
/// \return   3*3矩阵
///
cv::Matx33d Angle2Rz(const double theta)
{
  double rad = Deg2Rad(theta);
  return cv::Matx33d(cos(rad), sin(rad), 0,
                     -sin(rad), cos(rad), 0,
                     0, 0, 1);
}
///
/// \brief    计算旋转矩阵
/// 
/// 计算坐标系绕x、y、z轴旋转的3*3矩阵，以左手系为例：旋转方向遵循左手螺旋法则
/// 
/// \param    [in]  psi 绕x轴旋转的角度
/// \param    [in]  phi 绕y轴旋转的角度
/// \param    [in]  theta 绕z轴旋转的角度
/// \return   3*3矩阵
///
cv::Matx33d Angles2R(const double psi, const double phi, const double theta)
{
  return Angle2Rx(psi) * Angle2Ry(phi) * Angle2Rz(theta);
}
///
/// \brief    计算坐标轴的旋转角度
/// 
/// 计算根据旋转矩阵，计算绕x、y、z轴旋转的角度，以左手系为例：旋转方向遵循左手螺旋法则
/// 
/// \param    [in]  R 3*3旋转矩阵
/// \return   绕x、y、z轴旋转的角度，依次保存在一个向量中
///
cv::Vec3d R2Angles(const cv::Matx33d& R)
{
  cv::Vec3d angles;
  angles[0] = atan2(R(1, 2), R(2, 2));
  angles[1] = atan2(-R(0, 2), sqrt(pow(R(1, 2), 2) + pow(R(2, 2), 2)));
  angles[2] = atan2(R(0, 1), R(0, 0));
  angles[0] = Rad2Deg(angles[0]);
  angles[1] = Rad2Deg(angles[1]);
  angles[2] = Rad2Deg(angles[2]);
  return angles;
}
///
/// \brief    计算旋转向量
/// 
/// 用罗德里格斯变换，把旋转矩阵转换成旋转向量
/// 
/// \param    [in]  R 3*3旋转矩阵
/// \return   旋转向量
///
cv::Vec3d R2Vr(const cv::Matx33d& R)
{
  cv::Vec3d V;
  cv::Rodrigues(R, V);
  return V;
}
///
/// \brief    计算旋转矩阵
/// 
/// 用罗德里格斯变换，把旋转矩阵向量换成旋转矩阵
/// 
/// \param    [in]  V 旋转向量
/// \return   3*3旋转矩阵
///
cv::Matx33d Vr2R(const cv::Vec3d& V)
{
  cv::Matx33d R;
  cv::Rodrigues(V, R);
  return R;
}
///
/// \brief    计算旋转向量
/// 
/// 先计算3*3旋转矩阵，再用罗德里格斯变换，把旋转矩阵换换成旋转向量
/// 
/// \param    [in]  psi 绕x轴旋转的角度
/// \param    [in]  phi 绕y轴旋转的角度
/// \param    [in]  theta 绕z轴旋转的角度
/// \return   3*3旋转矩阵
///
cv::Vec3d Angles2Vr(const double psi, const double phi, const double theta)
{
  cv::Matx33d R = Angles2R(psi, phi, theta);
  return R2Vr(R);
}
///
/// \brief    计算坐标轴的旋转角度
/// 
/// 先计算3*3旋转矩阵，再计算坐标轴的旋转角度
/// 
/// \param    [in]  V 旋转向量
/// \return   绕x、y、z轴旋转的角度，依次保存在一个向量中
///
cv::Vec3d Vr2Angles(const cv::Vec3d& V)
{
  cv::Matx33d R = Vr2R(V);
  return R2Angles(R);
}

///
/// \brief 封装旋转矩阵的基本功能
/// 
class RotationMatrix
{
public:
  ///
  /// \brief    默认构造函数
  ///
  RotationMatrix()
  { }
  ///
  /// \brief    拷贝构造函数
  ///
  RotationMatrix(const RotationMatrix& R)
  {
    matrix_ = R.matrix_;
  }
  ///
  /// \brief    构造函数
  /// \param    [in]  R 3*3旋转矩阵
  ///
  RotationMatrix(const cv::Matx33d& R)
  {
    matrix_ = R;
  }
  ///
  /// \brief    构造函数
  /// \param    [in]  v 旋转向量
  ///
  RotationMatrix(const cv::Vec3d& v)
  {
    matrix_ = Vr2R(v);
  }
  ///
  /// \brief    构造函数
  /// \param    [in]  psi 绕x轴旋转角度
  /// \param    [in]  phi 绕y轴旋转角度
  /// \param    [in]  theta 绕z轴旋转角度
  ///
  RotationMatrix(const double psi, const double phi, const double theta)
  {
    matrix_ = Angles2R(psi, phi, theta);
  }
  ///
  /// \brief    计算绕x、y、z轴旋转的角度
  /// \return   绕x、y、z轴的旋转角度，依次保存在一个向量例
  ///
  cv::Vec3d Angles() const
  {
    return R2Angles(matrix_);
  }
  ///
  /// \brief    计算3*3旋转矩阵
  /// \return   3*3旋转矩阵
  ///
  cv::Vec3d Vector() const
  {
    return R2Vr(matrix_);
  }
  ///
  /// \brief    返回cv::Matx33d形式的旋转矩阵
  /// \return   cv::Matx33d形式的旋转矩阵
  ///
  cv::Matx33d Matrix() const { return matrix_; }

private:
  cv::Matx33d matrix_;  ///< 旋转矩阵
};
///
/// \brief 封装旋转向量的基本功能
/// 
class RotationVector
{
public:
  ///
  /// \brief 封装旋转矩阵的基本功能
  /// 
  RotationVector()
  { }
  ///
  /// \brief    拷贝构造函数
  ///
  RotationVector(const RotationVector& Rv)
  {
    vector_ = Rv.vector_;
  }
  ///
  /// \brief    构造函数
  /// \param    [in]  R 3*3旋转矩阵
  ///
  RotationVector(const cv::Matx33d& R)
  {
    vector_ = R2Vr(R);
  }
  ///
  /// \brief    构造函数
  /// \param    [in]  v 旋转向量
  ///
  RotationVector(const cv::Vec3d& v)
  {
    vector_ = v;
  }
  ///
  /// \brief    构造函数
  /// \param    [in]  psi 绕x轴旋转角度
  /// \param    [in]  phi 绕y轴旋转角度
  /// \param    [in]  theta 绕z轴旋转角度
  ///
  RotationVector(const double psi, const double phi, const double theta)
  {
    vector_ = Angles2Vr(psi, phi, theta);
  }
  ///
  /// \brief    计算绕x、y、z轴旋转的角度
  /// \return   绕x、y、z轴的旋转角度，依次保存在一个向量例
  ///
  cv::Vec3d Angles()
  {
    return Vr2Angles(vector_);
  }
  ///
  /// \brief    计算3*3旋转矩阵
  /// \return   3*3旋转矩阵
  ///
  cv::Vec3d Vector()
  {
    return vector_;
  }
  ///
  /// \brief    返回cv::Matx33d形式的旋转矩阵
  /// \return   cv::Matx33d形式的旋转矩阵
  ///
  cv::Matx33d Matrix()
  {
    return Vr2R(vector_);
  }
  ///
  /// \brief    返回绕旋转向量旋转的角度
  /// \return   绕旋转向量旋转的角度
  ///
  double Theta()
  {
    return Rad2Deg(cv::norm(vector_));
  }
  ///
  /// \brief    返回单位向量
  /// \return   单位向量
  ///
  cv::Vec3d Identity()
  {
    return vector_ / Deg2Rad(Theta());
  }

private:
  cv::Vec3d vector_;  ///< 旋转向量
};

} // my_linear_algebra

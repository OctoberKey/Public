//******************************************************************************
//
/// \file       RotationTransformation.h
/// \brief      
/// \author     �����/zhujiaqi@jsjd.cc
/// \version    V1.0
/// \date       2019-07-30
/// \copyright  ������Ϊ��缼�����޹�˾
//
//******************************************************************************

#pragma once

#include <opencv2/opencv.hpp>

///
/// \brief ���Դ��������ռ�
///
namespace my_linear_algebra {
///
/// \brief    ���Ƕ�ֵת���ɻ���ֵ
/// \param    [in]  d �Ƕ�ֵ
/// \return   ����ֵ
///
double Deg2Rad(const double d)
{
  return d * CV_PI / 180;
}
///
/// \brief    ������ֵת�ɽǶ�ֵ
/// \param    [in]  r ����ֵ
/// \return   �Ƕ�ֵ
///
double Rad2Deg(const double r)
{
  return r * 180 / CV_PI;
}
///
/// \brief    ����x����ת����
/// 
/// ��������ϵ��x����ת��3*3����������ϵΪ������ת������ѭ������������
/// 
/// \param    [in]  psi ��x����ת�ĽǶ�
/// \return   3*3����
///
cv::Matx33d Angle2Rx(const double psi)
{
  double rad = Deg2Rad(psi);
  return cv::Matx33d(1, 0, 0,
                     0, cos(rad), sin(rad),
                     0, -sin(rad), cos(rad));
}
///
/// \brief    ����y����ת����
/// 
/// ��������ϵ��x����ת��3*3����������ϵΪ������ת������ѭ������������
/// 
/// \param    [in]  phi ��y����ת�ĽǶ�
/// \return   3*3����
///
cv::Matx33d Angle2Ry(const double phi)
{
  double rad = Deg2Rad(phi);
  return cv::Matx33d(cos(rad), 0, -sin(rad),
                     0, 1, 0,
                     sin(rad), 0, cos(rad));
}
///
/// \brief    ����z����ת����
/// 
/// ��������ϵ��x����ת��3*3����������ϵΪ������ת������ѭ������������
/// 
/// \param    [in]  theta ��z����ת�ĽǶ�
/// \return   3*3����
///
cv::Matx33d Angle2Rz(const double theta)
{
  double rad = Deg2Rad(theta);
  return cv::Matx33d(cos(rad), sin(rad), 0,
                     -sin(rad), cos(rad), 0,
                     0, 0, 1);
}
///
/// \brief    ������ת����
/// 
/// ��������ϵ��x��y��z����ת��3*3����������ϵΪ������ת������ѭ������������
/// 
/// \param    [in]  psi ��x����ת�ĽǶ�
/// \param    [in]  phi ��y����ת�ĽǶ�
/// \param    [in]  theta ��z����ת�ĽǶ�
/// \return   3*3����
///
cv::Matx33d Angles2R(const double psi, const double phi, const double theta)
{
  return Angle2Rx(psi) * Angle2Ry(phi) * Angle2Rz(theta);
}
///
/// \brief    �������������ת�Ƕ�
/// 
/// ���������ת���󣬼�����x��y��z����ת�ĽǶȣ�������ϵΪ������ת������ѭ������������
/// 
/// \param    [in]  R 3*3��ת����
/// \return   ��x��y��z����ת�ĽǶȣ����α�����һ��������
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
/// \brief    ������ת����
/// 
/// ���޵����˹�任������ת����ת������ת����
/// 
/// \param    [in]  R 3*3��ת����
/// \return   ��ת����
///
cv::Vec3d R2Vr(const cv::Matx33d& R)
{
  cv::Vec3d V;
  cv::Rodrigues(R, V);
  return V;
}
///
/// \brief    ������ת����
/// 
/// ���޵����˹�任������ת��������������ת����
/// 
/// \param    [in]  V ��ת����
/// \return   3*3��ת����
///
cv::Matx33d Vr2R(const cv::Vec3d& V)
{
  cv::Matx33d R;
  cv::Rodrigues(V, R);
  return R;
}
///
/// \brief    ������ת����
/// 
/// �ȼ���3*3��ת���������޵����˹�任������ת���󻻻�����ת����
/// 
/// \param    [in]  psi ��x����ת�ĽǶ�
/// \param    [in]  phi ��y����ת�ĽǶ�
/// \param    [in]  theta ��z����ת�ĽǶ�
/// \return   3*3��ת����
///
cv::Vec3d Angles2Vr(const double psi, const double phi, const double theta)
{
  cv::Matx33d R = Angles2R(psi, phi, theta);
  return R2Vr(R);
}
///
/// \brief    �������������ת�Ƕ�
/// 
/// �ȼ���3*3��ת�����ټ������������ת�Ƕ�
/// 
/// \param    [in]  V ��ת����
/// \return   ��x��y��z����ת�ĽǶȣ����α�����һ��������
///
cv::Vec3d Vr2Angles(const cv::Vec3d& V)
{
  cv::Matx33d R = Vr2R(V);
  return R2Angles(R);
}

///
/// \brief ��װ��ת����Ļ�������
/// 
class RotationMatrix
{
public:
  ///
  /// \brief    Ĭ�Ϲ��캯��
  ///
  RotationMatrix()
  { }
  ///
  /// \brief    �������캯��
  ///
  RotationMatrix(const RotationMatrix& R)
  {
    matrix_ = R.matrix_;
  }
  ///
  /// \brief    ���캯��
  /// \param    [in]  R 3*3��ת����
  ///
  RotationMatrix(const cv::Matx33d& R)
  {
    matrix_ = R;
  }
  ///
  /// \brief    ���캯��
  /// \param    [in]  v ��ת����
  ///
  RotationMatrix(const cv::Vec3d& v)
  {
    matrix_ = Vr2R(v);
  }
  ///
  /// \brief    ���캯��
  /// \param    [in]  psi ��x����ת�Ƕ�
  /// \param    [in]  phi ��y����ת�Ƕ�
  /// \param    [in]  theta ��z����ת�Ƕ�
  ///
  RotationMatrix(const double psi, const double phi, const double theta)
  {
    matrix_ = Angles2R(psi, phi, theta);
  }
  ///
  /// \brief    ������x��y��z����ת�ĽǶ�
  /// \return   ��x��y��z�����ת�Ƕȣ����α�����һ��������
  ///
  cv::Vec3d Angles() const
  {
    return R2Angles(matrix_);
  }
  ///
  /// \brief    ����3*3��ת����
  /// \return   3*3��ת����
  ///
  cv::Vec3d Vector() const
  {
    return R2Vr(matrix_);
  }
  ///
  /// \brief    ����cv::Matx33d��ʽ����ת����
  /// \return   cv::Matx33d��ʽ����ת����
  ///
  cv::Matx33d Matrix() const { return matrix_; }

private:
  cv::Matx33d matrix_;  ///< ��ת����
};
///
/// \brief ��װ��ת�����Ļ�������
/// 
class RotationVector
{
public:
  ///
  /// \brief ��װ��ת����Ļ�������
  /// 
  RotationVector()
  { }
  ///
  /// \brief    �������캯��
  ///
  RotationVector(const RotationVector& Rv)
  {
    vector_ = Rv.vector_;
  }
  ///
  /// \brief    ���캯��
  /// \param    [in]  R 3*3��ת����
  ///
  RotationVector(const cv::Matx33d& R)
  {
    vector_ = R2Vr(R);
  }
  ///
  /// \brief    ���캯��
  /// \param    [in]  v ��ת����
  ///
  RotationVector(const cv::Vec3d& v)
  {
    vector_ = v;
  }
  ///
  /// \brief    ���캯��
  /// \param    [in]  psi ��x����ת�Ƕ�
  /// \param    [in]  phi ��y����ת�Ƕ�
  /// \param    [in]  theta ��z����ת�Ƕ�
  ///
  RotationVector(const double psi, const double phi, const double theta)
  {
    vector_ = Angles2Vr(psi, phi, theta);
  }
  ///
  /// \brief    ������x��y��z����ת�ĽǶ�
  /// \return   ��x��y��z�����ת�Ƕȣ����α�����һ��������
  ///
  cv::Vec3d Angles()
  {
    return Vr2Angles(vector_);
  }
  ///
  /// \brief    ����3*3��ת����
  /// \return   3*3��ת����
  ///
  cv::Vec3d Vector()
  {
    return vector_;
  }
  ///
  /// \brief    ����cv::Matx33d��ʽ����ת����
  /// \return   cv::Matx33d��ʽ����ת����
  ///
  cv::Matx33d Matrix()
  {
    return Vr2R(vector_);
  }
  ///
  /// \brief    ��������ת������ת�ĽǶ�
  /// \return   ����ת������ת�ĽǶ�
  ///
  double Theta()
  {
    return Rad2Deg(cv::norm(vector_));
  }
  ///
  /// \brief    ���ص�λ����
  /// \return   ��λ����
  ///
  cv::Vec3d Identity()
  {
    return vector_ / Deg2Rad(Theta());
  }

private:
  cv::Vec3d vector_;  ///< ��ת����
};

} // my_linear_algebra

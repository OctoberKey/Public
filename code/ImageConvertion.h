///*****************************************************************************
///
/// \file       ImageConvertion.h
/// \brief      图像转换类
///
///             在OpenCV Mat、Qt QImage、Open eVision EImageBW8等图像类之间进行相
///             互转换
///
/// \author     诸佳琪/zhujiaqi@jsjd.cc
/// \version    V1.0
/// \date       2019-07-30
/// \copyright  杭州中为光电技术有限公司
///
///*****************************************************************************

#pragma once

#ifdef USE_EVISION_2_8
#include <Open_eVision_2_8.h>
using namespace Euresys::Open_eVision_2_8;
#endif

#include <QtCore/QtGui>
#include <opencv2/opencv.hpp>

namespace my_cv {

class ImageConvertion
{
public:

  static QImage toQImageGrayscal8(const uchar* data,
                                  int width,
                                  int height,
                                  int bytesPerLine,
                                  bool deepCopy = true)
  {
    Q_ASSERT(data != nullptr);

    // Create QImage with same dimensions as input Image
    QImage dst(data, width, height, bytesPerLine, QImage::Format_Grayscale8);

    // Set the color table (used to translate color indexes to qRgb values)
    QVector<QRgb> colorTable;
    for (int i = 0; i < 256; i++)
      colorTable.push_back(qRgb(i, i, i));

    dst.setColorTable(colorTable);

    // Return a deep or shadow copy
    return deepCopy ? dst.copy() : dst;
  }

#ifdef USE_EVISION_2_8
  static QImage toQImage(const EImageBW8& image, bool deepCopy = true)
  {
    return toQImageGrayscal8(static_cast<const uchar *>(image.GetImagePtr()),
                             image.GetWidth(),
                             image.GetHeight(),
                             image.GetRowPitch(),
                             deepCopy);
  }
#endif

  static QImage toQImage(const cv::Mat& mat, bool deepCopy = true)
  {
    Q_ASSERT(mat.data != nullptr);

    switch (mat.type())
    {
      case CV_8UC1:
      {
        return toQImageGrayscal8(mat.data, mat.cols, mat.rows, mat.step, deepCopy);
      }
      break;
      case CV_8UC3:
      {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
      }
      break;
      case CV_8UC4:
      {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return deepCopy ? image : image.copy();
      }
      break;
      default:
      {
        Q_ASSERT_X(false,
                   "static QImage toQImage(const cv::Mat& mat, bool deepCopy = true)",
                   "ERROR: Mat could not be converted to QImage.");
        return QImage();
      }
      break;
    }
  }

  static cv::Mat toMat(const QImage& image, bool deepCopy = true)
  {
    Q_ASSERT(!image.isNull());
   
    switch (image.format())
    {
      case QImage::Format_Indexed8:
      {
        cv::Mat mat(image.height(), image.width(), CV_8UC1,
                    const_cast<uchar*>(image.constBits()), image.bytesPerLine());
        return deepCopy ? mat.clone() : mat;
      }
      break;
      case QImage::Format_RGB32:
      case QImage::Format_ARGB32:
      case QImage::Format_ARGB32_Premultiplied:
      {
        cv::Mat mat(image.height(), image.width(), CV_8UC4,
                    const_cast<uchar*>(image.constBits()), image.bytesPerLine());
        return deepCopy ? mat.clone() : mat;
      }
      break;
      case QImage::Format_RGB888:
      {
        cv::Mat src(image.height(), image.width(), CV_8UC3,
                    const_cast<uchar*>(image.constBits()), image.bytesPerLine());
        cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
        return src;
      }
      break;
      default:
      {
        Q_ASSERT_X(false,
                   "static cv::Mat toMat(const QImage& image, bool deepCopy = true)",
                   "ERROR: QImage could not be converted to Mat.");
        return cv::Mat();
      }
      break;
    }
  }
};

} // my_cv

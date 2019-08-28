///*****************************************************************************
///
/// \file       QDC.h
/// \brief      兼容Qt的DC类
/// \author     诸佳琪/zhujiaqi@jsjd.cc
/// \version    V1.0
/// \date       2019-07-30
/// \copyright  杭州中为光电技术有限公司
///
///*****************************************************************************

#pragma once

#include <QtWinExtras/QtWin>

namespace my_painting {

///
/// \brief 兼容Windows DC和Qt-QImage，QPixmap的DC类
///
class QDC
{
public:
  HDC m_hScreenDC;
  HDC m_hDC;

  QDC()
  {
    m_hScreenDC = GetDC(NULL);
    m_hDC = CreateCompatibleDC(m_hScreenDC);
  }

  QDC(HBITMAP hBitmap)
  {
    m_hScreenDC = GetDC(NULL);
    m_hDC = CreateCompatibleDC(m_hScreenDC);
    m_hOldBitmap = SelectBitmap(hBitmap);
  }

  QDC(const QPixmap& pixmap)
  {
    m_hScreenDC = GetDC(NULL);
    m_hDC = CreateCompatibleDC(m_hScreenDC);
    m_hOldBitmap = SelectQPixmap(pixmap);
  }

  QDC(const QImage& image)
  {
    m_hScreenDC = GetDC(NULL);
    m_hDC = CreateCompatibleDC(m_hScreenDC);
    m_hOldBitmap = SelectQImage(image);
  }

  ~QDC()
  {
    SelectBitmap(m_hOldBitmap);
    DeleteDC(m_hDC);
    ReleaseDC(NULL, m_hScreenDC);
  }

  HBITMAP SelectBitmap(HBITMAP hBitmap)
  {
    Q_ASSERT(m_hDC != NULL);
    Q_ASSERT((hBitmap == NULL) || (::GetObjectType(hBitmap) == OBJ_BITMAP));
    return (HBITMAP)::SelectObject(m_hDC, hBitmap);
  }

  HBITMAP SelectQPixmap(const QPixmap& pixmap)
  {
    Q_ASSERT(!pixmap.isNull());
    return SelectBitmap(QtWin::toHBITMAP(pixmap));
  }

  HBITMAP SelectQImage(const QImage& image)
  {
    Q_ASSERT(!image.isNull());
    return SelectBitmap(QtWin::toHBITMAP(QPixmap::fromImage(image)));
  }

  HBITMAP GetCurrentBitmap() const
  {
    Q_ASSERT(m_hDC != NULL);
    return (HBITMAP)::GetCurrentObject(m_hDC, OBJ_BITMAP);
  }

  QPixmap GetCurrentQPixmap() const
  {
    return QtWin::fromHBITMAP(GetCurrentBitmap());
  }

  QImage GetCurrentQImage() const
  {
    return GetCurrentQPixmap().toImage();
  }

  operator HDC() const { return m_hDC; }

private:
  HBITMAP m_hOldBitmap;
};

} // my_painting

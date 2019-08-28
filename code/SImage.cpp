#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

namespace cv {

static void
thresh_8u2(const Mat& _src, Mat& _dst, uchar lowthresh, uchar highthresh, uchar maxval)
{
  Size roi = _src.size();
  roi.width *= _src.channels();
  size_t src_step = _src.step;
  size_t dst_step = _dst.step;

  if (_src.isContinuous() && _dst.isContinuous())
  {
    roi.width *= roi.height;
    roi.height = 1;
    src_step = dst_step = roi.width;
  }

  int j = 0;
  const uchar* src = _src.ptr();
  uchar* dst = _dst.ptr();
#if CV_SIMD128
  bool useSIMD = checkHardwareSupport(CV_CPU_SSE2) || checkHardwareSupport(CV_CPU_NEON);
  if (useSIMD)
  {
    v_uint8x16 lowthresh_u = v_setall_u8(lowthresh);
    v_uint8x16 highthresh_u = v_setall_u8(highthresh);
    v_uint8x16 maxval16 = v_setall_u8(maxval);

    for (int i = 0; i < roi.height; i++, src += src_step, dst += dst_step)
    {
      for (j = 0; j <= roi.width - 16; j += 16)
      {
        v_uint8x16 v0;
        v0 = v_load(src + j);
        v0 = v0 >= lowthresh_u && v0 <= hithresh_u;
        v0 = v0 & maxval16;
        v_store(dst + j, v0);
      }
    }
#endif

  int j_scalar = j;
  if (j_scalar < roi.width)
  {
    const int low_thresh_pivot = lowthresh;
    const int high_thresh_pivot = highthresh;
    uchar tab[256] = { 0 };

    if (low_thresh_pivot > 0)
      memset(tab, 0, low_thresh_pivot);
    memset(tab + low_thresh_pivot, maxval, high_thresh_pivot - low_thresh_pivot + 1);
    if (high_thresh_pivot < 255)
      memset(tab + high_thresh_pivot + 1, 0, 255 - high_thresh_pivot);

    src = _src.ptr();
    dst = _dst.ptr();
    for (int i = 0; i < roi.height; i++, src += src_step, dst += dst_step)
    {
      j = j_scalar;
#if CV_ENABLE_UNROLLED
      for (; j <= roi.width - 4; j += 4)
      {
        uchar t0 = tab[src[j]];
        uchar t1 = tab[src[j + 1]];

        dst[j] = t0;
        dst[j + 1] = t1;

        t0 = tab[src[j + 2]];
        t1 = tab[src[j + 3]];

        dst[j + 2] = t0;
        dst[j + 3] = t1;
      }
#endif
      for (; j < roi.width; j++)
        dst[j] = tab[src[j]];
    }
  }
}

class ThresholdRunner2 : public ParallelLoopBody
{
public:
  ThresholdRunner2(Mat _src, Mat _dst, double _lowthresh, double _highthresh, double _maxval)
  {
    src = _src;
    dst = _dst;

    lowthresh = _lowthresh;
    highthresh = _highthresh;
    maxval = _maxval;
  }

  void operator () (const Range& range) const CV_OVERRIDE
  {
    int row0 = range.start;
    int row1 = range.end;

    Mat srcStripe = src.rowRange(row0, row1);
    Mat dstStripe = dst.rowRange(row0, row1);

    //CALL_HAL(threshold, cv_hal_threshold, srcStripe.data, srcStripe.step, dstStripe.data, dstStripe.step,
    //         srcStripe.cols, srcStripe.rows, srcStripe.depth(), srcStripe.channels(),
    //         thresh, maxval, thresholdType);

    //if (srcStripe.depth() == CV_8U)
    //{
      thresh_8u2(srcStripe, dstStripe, (uchar)lowthresh, (uchar)highthresh, (uchar)maxval);
    //}
    //else if (srcStripe.depth() == CV_16S)
    //{
    //  thresh_16s(srcStripe, dstStripe, (short)thresh, (short)maxval, thresholdType);
    //}
    //else if (srcStripe.depth() == CV_16U)
    //{
    //  thresh_16u(srcStripe, dstStripe, (ushort)thresh, (ushort)maxval, thresholdType);
    //}
    //else if (srcStripe.depth() == CV_32F)
    //{
    //  thresh_32f(srcStripe, dstStripe, (float)thresh, (float)maxval, thresholdType);
    //}
    //else if (srcStripe.depth() == CV_64F)
    //{
    //  thresh_64f(srcStripe, dstStripe, thresh, maxval, thresholdType);
    //}
  }

private:
  Mat src;
  Mat dst;

  double lowthresh;
  double highthresh;
  double maxval;
};

void threshold2(const Mat& src, Mat& dst,
                const double _lowthresh, const double _highthresh,
                const double _maxval)
{
  //CV_INSTRUMENT_REGION();

  //CV_OCL_RUN_(_src.dims() <= 2 && _dst.isUMat(),
  //            ocl_threshold(_src, _dst, thresh, maxval, type), thresh)

  //Mat src = _src.getMat();
  //int automatic_thresh = (type & ~CV_THRESH_MASK);
  //type &= THRESH_MASK;

  //CV_Assert(automatic_thresh != (CV_THRESH_OTSU | CV_THRESH_TRIANGLE));
  //if (automatic_thresh == CV_THRESH_OTSU)
  //{
  //  CV_Assert(src.type() == CV_8UC1);
  //  thresh = getThreshVal_Otsu_8u(src);
  //}
  //else if (automatic_thresh == CV_THRESH_TRIANGLE)
  //{
  //  CV_Assert(src.type() == CV_8UC1);
  //  thresh = getThreshVal_Triangle_8u(src);
  //}

  dst.create(src.size(), src.type());
  //Mat dst = _dst.getMat();
  double lowthresh = _lowthresh;
  double highthresh = _highthresh;
  uchar maxval = _maxval;
  if (src.depth() == CV_8U)
  {
    int ilowthresh = cvFloor(lowthresh);
    int ihighthresh = cvFloor(highthresh);
    lowthresh = ilowthresh;
    highthresh = ihighthresh;
    int imaxval = cvRound(maxval);
    //if (type == THRESH_TRUNC)
    //  imaxval = ithresh;
    imaxval = saturate_cast<uchar>(imaxval);

    //if (ithresh < 0 || ithresh >= 255)
    //{
    //  if (type == THRESH_BINARY || type == THRESH_BINARY_INV ||
    //    ((type == THRESH_TRUNC || type == THRESH_TOZERO_INV) && ithresh < 0) ||
    //      (type == THRESH_TOZERO && ithresh >= 255))
    //  {
    //    int v = type == THRESH_BINARY ? (ithresh >= 255 ? 0 : imaxval) :
    //      type == THRESH_BINARY_INV ? (ithresh >= 255 ? imaxval : 0) :
    //      /*type == THRESH_TRUNC ? imaxval :*/ 0;
    //    dst.setTo(v);
    //  }
    //  else
    //    src.copyTo(dst);
    //  return thresh;
    //}

    //CV_OVX_RUN(!ovx::skipSmallImages<VX_KERNEL_THRESHOLD>(src.cols, src.rows),
    //           openvx_threshold(src, dst, ithresh, imaxval, type), (double)ithresh)

    //thresh = ithresh;
    maxval = imaxval;
  }
  //else if (src.depth() == CV_16S)
  //{
  //  int ithresh = cvFloor(thresh);
  //  thresh = ithresh;
  //  int imaxval = cvRound(maxval);
  //  if (type == THRESH_TRUNC)
  //    imaxval = ithresh;
  //  imaxval = saturate_cast<short>(imaxval);

  //  if (ithresh < SHRT_MIN || ithresh >= SHRT_MAX)
  //  {
  //    if (type == THRESH_BINARY || type == THRESH_BINARY_INV ||
  //      ((type == THRESH_TRUNC || type == THRESH_TOZERO_INV) && ithresh < SHRT_MIN) ||
  //        (type == THRESH_TOZERO && ithresh >= SHRT_MAX))
  //    {
  //      int v = type == THRESH_BINARY ? (ithresh >= SHRT_MAX ? 0 : imaxval) :
  //        type == THRESH_BINARY_INV ? (ithresh >= SHRT_MAX ? imaxval : 0) :
  //        /*type == THRESH_TRUNC ? imaxval :*/ 0;
  //      dst.setTo(v);
  //    }
  //    else
  //      src.copyTo(dst);
  //    return thresh;
  //  }
  //  thresh = ithresh;
  //  maxval = imaxval;
  //}
  //else if (src.depth() == CV_16U)
  //{
  //  int ithresh = cvFloor(thresh);
  //  thresh = ithresh;
  //  int imaxval = cvRound(maxval);
  //  if (type == THRESH_TRUNC)
  //    imaxval = ithresh;
  //  imaxval = saturate_cast<ushort>(imaxval);

  //  int ushrt_min = 0;
  //  if (ithresh < ushrt_min || ithresh >= (int)USHRT_MAX)
  //  {
  //    if (type == THRESH_BINARY || type == THRESH_BINARY_INV ||
  //      ((type == THRESH_TRUNC || type == THRESH_TOZERO_INV) && ithresh < ushrt_min) ||
  //        (type == THRESH_TOZERO && ithresh >= (int)USHRT_MAX))
  //    {
  //      int v = type == THRESH_BINARY ? (ithresh >= (int)USHRT_MAX ? 0 : imaxval) :
  //        type == THRESH_BINARY_INV ? (ithresh >= (int)USHRT_MAX ? imaxval : 0) :
  //        /*type == THRESH_TRUNC ? imaxval :*/ 0;
  //      dst.setTo(v);
  //    }
  //    else
  //      src.copyTo(dst);
  //    return thresh;
  //  }
  //  thresh = ithresh;
  //  maxval = imaxval;
  //}
  //else if (src.depth() == CV_32F)
  //  ;
  //else if (src.depth() == CV_64F)
  //  ;
  //else
  //  CV_Error(CV_StsUnsupportedFormat, "");

  parallel_for_(Range(0, dst.rows),
                ThresholdRunner2(src, dst, lowthresh, highthresh, maxval),
                dst.total() / (double)(1 << 16));
  //return thresh;
}

} // cv

namespace my_cv {

class SImage
{
public:
  SImage()
  { }

  SImage(const cv::Mat& img)
  {
    image_ = img.clone();
  }

  bool Read(const std::string& file_name)
  {
    image_ = cv::imread(file_name, cv::IMREAD_UNCHANGED);
    return image_.data != nullptr;
  }

  bool Write(const std::string& file_name)
  {
    return cv::imwrite(file_name, image_);
  }

  SImage Threshold(const float MinGray, const float MaxGray)
  {
    cv::Mat dst;
    cv::threshold2(image_, dst, MinGray, MaxGray, 255);
    return SImage(dst);
  }

private:
  cv::Mat image_;
};

} // zvision
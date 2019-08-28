#pragma once

namespace my_cv {

class SImage
{
public:
  SImage()
  { }

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
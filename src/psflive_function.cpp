#include <CCfits/CCfits>
#include <unistd.h>

#include "psflive_function.h"
// ====================================================================================================================










// ====================================================================================================================
bool psflive::function::validateFile(const std::string& _fileName, std::string& _source, std::string& _version, psflive::AcquireType& _type) {

  bool valid = false;
  std::string _acquireTypeString;

  CCfits::FITS* ptrFitsFile = new CCfits::FITS(_fileName, CCfits::Read, false);

  CCfits::PHDU& primaryHDU = ptrFitsFile->pHDU();
  primaryHDU.readAllKeys();

  try {
    primaryHDU.keyWord(PSFLIVE_SOURCE_STR).value(_source);
    psflive::function::LogMessage("function","validateFile()","_source = "+_source);

    if ((_source == PSFLIVE_STR) || (_source == PSFLIVE_TEST_STR)) {
      primaryHDU.keyWord(PSFLIVE_VERSION_STR).value(_version);
      psflive::function::LogMessage("function","validateFile()","_version = "+_version);

      if (_version == PSFLIVE_VERSION_NUMBER_STR) {
        primaryHDU.keyWord(PSFLIVE_DATATYPE_STR).value(_acquireTypeString);
        psflive::function::LogMessage("function","validateFile()","_acquireTypeString = "+_acquireTypeString);

        if (stringToAcquireDataType(_acquireTypeString, _type))
          valid = true;
      }
    }
  } catch(const CCfits::HDU::NoSuchKeyword& _exception) {
    _acquireTypeString = _exception.message();
    valid = false;
  }

  if (valid)
    psflive::function::LogMessage("function","validateFile()","File = "+_fileName+" [valid]");
  else
    psflive::function::LogMessage("function","validateFile()","File = "+_fileName+" [invalid]");

  return valid;
}
// ====================================================================================================================










// ====================================================================================================================
int psflive::function::rangedRand(const int _min, const int _max) {
  int n = _max - _min + 1;
  int remainder = RAND_MAX % n;
  int x;
  srand(time(NULL)); // initialize random seed
  do {
    x = rand();
  } while (x >= RAND_MAX - remainder);
  return _min + x % n;
}
// ====================================================================================================================










// ====================================================================================================================
void psflive::function::interruptibleUSleep(const int sleep_time_us, const bool skip) {
  int timer_us = 0;
  while (skip) {
    timer_us += PSFLIVE_SHORT_SLEEP_US;
    if (timer_us > sleep_time_us)
      break;
    usleep(PSFLIVE_SHORT_SLEEP_US);
  }
}
// ====================================================================================================================










// ====================================================================================================================
const int psflive::function::cvTypeToGLFormat(const int _cvtype) {
  if (_cvtype == CV_8UC1)
    return GL_UNSIGNED_BYTE;
  else if (_cvtype == CV_16UC1)
    return GL_UNSIGNED_SHORT;
  else if (_cvtype == CV_8UC4) // unsupported by opencv
    return GL_UNSIGNED_INT;
  // else if (_cvtype == CV_8UC(8)) // unsupported by opencv and opengl
  //   return LONGLONG_IMG;
  else if (_cvtype == CV_32FC1)
    return GL_FLOAT;
  else if (_cvtype == CV_64FC1)
    return GL_DOUBLE;
  else if (_cvtype == CV_8SC1)
    return GL_BYTE;
  else if (_cvtype == CV_16SC1)
    return GL_SHORT;
  else if (_cvtype == CV_32SC1)
    return GL_INT;
  // else if (cvtype == CV_8SC(8)) // unsupported by opencv and opengl
  //   return ULONGLONG_IMG;
  else
    return GL_UNSIGNED_SHORT;
}
// ====================================================================================================================










// ====================================================================================================================
const int psflive::function::cvTypeToFITSBitPix(const int _cvtype) {
  if (_cvtype == CV_8UC1)
    return BYTE_IMG;
  else if (_cvtype == CV_16UC1)
    return USHORT_IMG;
  else if (_cvtype == CV_8UC4) // unsupported by opencv
    return ULONG_IMG;
  else if (_cvtype == CV_8UC(8)) // unsupported by opencv
    return ULONGLONG_IMG;
  else if (_cvtype == CV_32FC1)
    return FLOAT_IMG;
  else if (_cvtype == CV_64FC1)
    return DOUBLE_IMG;
  else if (_cvtype == CV_8SC1)
    return SBYTE_IMG;
  else if (_cvtype == CV_16SC1)
    return SHORT_IMG;
  else if (_cvtype == CV_32SC1)
    return LONG_IMG;
  else if (_cvtype == CV_8SC(8)) // unsupported by opencv
    return LONGLONG_IMG;
  else
    return USHORT_IMG;
}
// ====================================================================================================================










// ====================================================================================================================
const int psflive::function::cvTypeToSize(const int _cvtype) {
  if (_cvtype == CV_8UC1)
    return 8;
  else if (_cvtype == CV_16UC1)
    return 16;
  else if (_cvtype == CV_8UC4) // unsupported by opencv
    return 32;
  else if (_cvtype == CV_8UC(8)) // unsupported by opencv
    return 64;
  else if (_cvtype == CV_32FC1)
    return 32;
  else if (_cvtype == CV_64FC1)
    return 64;
  else if (_cvtype == CV_8SC1)
    return 8;
  else if (_cvtype == CV_16SC1)
    return 16;
  else if (_cvtype == CV_32SC1)
    return 32;
  else if (_cvtype == CV_8SC(8)) // unsupported by opencv
    return 64;
  else
    return 16;
}
// ====================================================================================================================










// ====================================================================================================================
const std::string psflive::function::cvTypeToString(const int _cvtype) {
  if (_cvtype == CV_8UC1)
    return "CV_8UC1";
  else if (_cvtype == CV_16UC1)
    return "CV_16UC1";
  else if (_cvtype == CV_8UC4) // unsupported by opencv
    return "CV_8UC4";
  else if (_cvtype == CV_8UC(8)) // unsupported by opencv
    return "CV_8UC(8)";
  else if (_cvtype == CV_32FC1)
    return "CV_32FC1";
  else if (_cvtype == CV_64FC1)
    return "CV_64FC1";
  else if (_cvtype == CV_8SC1)
    return "CV_8SC1";
  else if (_cvtype == CV_16SC1)
    return "CV_16SC1";
  else if (_cvtype == CV_32SC1)
    return "CV_32SC1";
  else if (_cvtype == CV_8SC(8)) // unsupported by opencv
    return "CV_8SC(8)";
  else
    return "CV_16UC1";
}
// ====================================================================================================================










// ====================================================================================================================
const int psflive::function::fitsBitPixToCVType(const int _bitpix) {
  if (_bitpix == BYTE_IMG )
    return CV_8UC1;
  else if (_bitpix == USHORT_IMG )
    return CV_16UC1;
  else if (_bitpix == ULONG_IMG ) // unsupported by opencv
    return CV_8UC4;
  else if (_bitpix == ULONGLONG_IMG) // unsupported by opencv
    return CV_8UC(8);
  else if (_bitpix == FLOAT_IMG )
    return CV_32FC1;
  else if (_bitpix == DOUBLE_IMG )
    return CV_64FC1;
  else if (_bitpix == SBYTE_IMG )
    return CV_8SC1;
  else if (_bitpix == SHORT_IMG )
    return CV_16UC1;
    // return CV_16SC1;
  else if (_bitpix == LONG_IMG )
    return CV_32SC1;
  else if (_bitpix == LONGLONG_IMG) // unsupported by opencv
    return CV_8SC(8);
  else
    return CV_16UC1;
}
// ====================================================================================================================










// ====================================================================================================================
const std::string psflive::function::timeStamp() {
  std::time_t currentTime = std::time(nullptr);
  char tempFileName[32];
  std::strftime(tempFileName, sizeof(tempFileName), "%Y%m%d.%H%M%S", std::localtime(&currentTime));
  return std::string(tempFileName);
}
// ====================================================================================================================










// ====================================================================================================================
const ImageData psflive::function::imageMean(std::vector<ImageData> _images) {

  cv::Mat tempImage_double(_images[0].rows, _images[0].cols, CV_64FC1, cv::Scalar::all(0));
  ImageData meanImage(_images[0].rows, _images[0].cols, CV_64FC1, cv::Scalar::all(0));

  for (ImageData& image:_images) {
    image.convertTo(tempImage_double, CV_64FC1);
    meanImage += tempImage_double;
  }
  meanImage /= _images.size();
  
  psflive::function::LogMessage("function","imageMean()","Averaging "+std::to_string(_images.size())+" frames of "+imageInfo(_images[0])+" into "+imageInfo(meanImage));

  return meanImage;
}
// ====================================================================================================================










// ====================================================================================================================
const ImageData psflive::function::imageFilter(const ImageData _image, const int ksize) {
  // // TODO: make filtering options available to user
  // cv::Mat filteredImage;
  // int ddepth = -1;
  // cv::Mat kernel = cv::Mat::ones(ksize,ksize,CV_64FC1)/(double)(ksize*ksize);
  // cv::Point anchor(-1,-1);
  // double delta = 0;
  // cv::filter2D(_image, filteredImage, ddepth, kernel, anchor, delta, cv::BORDER_DEFAULT);
  // return filteredImage;
  // TODO: filtering downgrades image from double to float
  cv::Mat floatImage, filteredImage;
  _image.convertTo(floatImage, CV_32FC1);
  cv::medianBlur(floatImage, filteredImage, ksize);
  psflive::function::LogMessage("function","imageFilter()","Applying median blur kernel size = "+std::to_string(ksize));
  return filteredImage;
}
// ====================================================================================================================










// ====================================================================================================================
const std::string psflive::function::imageInfo(const ImageData& _image) {
  std::stringstream out;
  out << "[" << cvTypeToString(_image.type()) << " : ";
  for(int iDim = 0; iDim < _image.dims; ++iDim) {
    if(iDim)
      out << " x ";
    out << _image.size[iDim];
  }
  out << " : Ch ";
  out << _image.channels();
  out << "]";
  return out.str();
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
const std::string psflive::function::imageInfo(std::vector<ImageData> _images) {
  std::stringstream out;
  out << "[" << psflive::function::cvTypeToString(_images[0].type()) << " : ";
  if (_images.size()) {
    out << _images.size();
    out << " x ";
    for(int iDim = 0; iDim < _images[0].dims; ++iDim) {
      if(iDim)
        out << " x ";
      out << _images[0].size[iDim];
    }
  }
  out << " : Ch ";
  out << _images[0].channels();
  out << "]";
  return out.str();
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
const std::string psflive::function::imageInfo(std::vector<std::vector<ImageData>> _images) {
  std::stringstream out;
  out << "[" << psflive::function::cvTypeToString(_images[0][0].type()) << " : ";
  if (_images.size()) {
    out << _images.size();
    out << " x ";
    if (_images[0].size()) {
      out << _images[0].size();
      out << " x ";
      for(int iDim = 0; iDim < _images[0][0].dims; ++iDim) {
        if(iDim)
          out << " x ";
        out << _images[0][0].size[iDim];
      }
    }
  }
  out << " : Ch ";
  out << _images[0][0].channels();
  out << "]";
  return out.str();
}
// ====================================================================================================================










// ====================================================================================================================
void psflive::function::LogMessage(const char* _source, const char* _function, const char* _message) {
#if PSFLIVE_CORE_DEBUG 
  std::cout << "[" << psflive::function::timeStamp() << "] : " << std::setw(18) << _source << " : " << std::setw(30) << _function << " : " << _message << std::endl;
#endif
}
// ====================================================================================================================










// ====================================================================================================================
void psflive::function::LogMessage(const char* _source, const std::string& _function, const std::string& _message) {
  return psflive::function::LogMessage(_source, _function.c_str(), _message.c_str());
}
// ====================================================================================================================










// ====================================================================================================================
const std::string CCfits::typeToString(const int _type) {
  if (_type == BYTE_IMG)
    return "BYTE_IMG";
  else if (_type == USHORT_IMG)
    return "USHORT_IMG";
  else if (_type == ULONG_IMG) // unsupported by opencv
    return "ULONG_IMG";
  else if (_type == ULONGLONG_IMG) // unsupported by opencv
    return "ULONGLONG_IMG";
  else if (_type == FLOAT_IMG)
    return "FLOAT_IMG";
  else if (_type == DOUBLE_IMG)
    return "DOUBLE_IMG";
  else if (_type == SBYTE_IMG)
    return "SBYTE_IMG";
  else if (_type == SHORT_IMG)
    return "SHORT_IMG";
  else if (_type == LONG_IMG)
    return "LONG_IMG";
  else if (_type == LONGLONG_IMG) // unsupported by opencv
    return "LONGLONG_IMG";
  else
    return "USHORT_IMG";
}
// ====================================================================================================================










// ====================================================================================================================
const Json::Value psflive::function::shapeToJsonValue(const ImGuiCanvasShape _shape) {
  Json::Value value;
  value["label"] = _shape.getLabel();
  value["type"] = shapeTypeToString(_shape.getType());
  value["clip"] = clipToString(_shape.getClip());
  value["center"]["x"] = _shape.getCenter().position.x;
  value["center"]["y"] = _shape.getCenter().position.y;
  value["points"] = Json::arrayValue;
  for (Json::Value::ArrayIndex iPoints = 0; iPoints < (size_t)_shape.getCtrlPoints().size(); iPoints++) {
    value["points"][iPoints]["x"] = _shape.getCtrlPoint(iPoints).position.x;
    value["points"][iPoints]["y"] = _shape.getCtrlPoint(iPoints).position.y;
  }
  return value;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
const ImGuiCanvasShape psflive::function::jsonValueToShape(const Json::Value& _shapeJson) {
  std::string label = _shapeJson["label"].asString();
  ImGuiCanvasShapeType type;
  stringToShapeType(_shapeJson["type"].asString(), type);
  ImGuiCanvasClip clip;
  stringToClip(_shapeJson["clip"].asString(), clip);
  ImVec2 center = {_shapeJson["center"]["x"].asFloat(), _shapeJson["center"]["y"].asFloat()};
  ImVector<ImVec2> ctrlPoints;
  for (Json::Value::ArrayIndex iPoint = 0; iPoint < _shapeJson["points"].size(); iPoint++)
    ctrlPoints.push_back({_shapeJson["points"][iPoint]["x"].asFloat(), _shapeJson["points"][iPoint]["y"].asFloat()});
  return ImGuiCanvasShape(label, center, type, ctrlPoints, clip, true);
}
// ====================================================================================================================










// ====================================================================================================================
const bool psflive::function::stringToAcquireDataType(const std::string& _acquireDataTypeString, psflive::AcquireType& _acquireDataType) {
  std::string acquireDataTypeStrings[(uint)psflive::AcquireType::ntypes] = PSFLIVE_ACQUISITION_TYPE_LABELS;
  auto itAcquireDataType = std::find(std::begin(acquireDataTypeStrings), std::end(acquireDataTypeStrings), _acquireDataTypeString);
  if (itAcquireDataType != std::end(acquireDataTypeStrings)) {
    _acquireDataType = (psflive::AcquireType)(uint)std::distance(std::begin(acquireDataTypeStrings), itAcquireDataType);
    return true;
  }
  return false;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
const std::string psflive::function::acquireDataTypeToString(const psflive::AcquireType _acquireDataType) {
  if (_acquireDataType == psflive::AcquireType::capture)
    return PSFLIVE_CAPTURE_TYPE_STR;
  else
    return "Invalid Type";
}
// ====================================================================================================================


#include "psflive_Configuration.h"
#include "psflive_function.h"
// ====================================================================================================================










// ====================================================================================================================
psflive::Configuration::Configuration(const std::string& _fileName) : m_fileName(_fileName) {
  Read();
  return;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::Read() {
  m_fileStream.open(m_fileName, std::fstream::in);
  try {
    m_fileStream >> m_root;
    psflive::function::LogMessage("Configuration","Read()","File = "+m_fileName);
  } catch(const std::exception& _exception) {
    std::cerr << _exception.what() << '\n';
  }
  m_fileStream.close();
  return;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::Write() {
  m_fileStream.open(m_fileName, std::fstream::out);
  Json::StreamWriterBuilder builder;
  builder["commentStyle"] = "None";
  builder["indentation"] = "  ";
  Json::StreamWriter* ptrWriter(builder.newStreamWriter());
  ptrWriter->write(m_root, &m_fileStream);
  psflive::function::LogMessage("Configuration","Write()","File = "+m_fileName);
  m_fileStream.close();
  return;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getCameraInfo(CameraInfo& _cameraInfo) {
  CameraInfo cameraInfo;
  if (getString("Imager Options","CameraInfo",cameraInfo.serial,"serial") && getString("Imager Options","CameraInfo",cameraInfo.name,"name") && getString("Imager Options","CameraInfo",cameraInfo.identifier,"identifier") && getString("Imager Options","CameraInfo",cameraInfo.connection_type,"connection_type")) {
    _cameraInfo = cameraInfo;
    psflive::function::LogMessage("Configuration","getCameraInfo()","CameraInfo = "+_cameraInfo.toString());
    return true;
  } else {
    psflive::function::LogMessage("Configuration","getCameraInfo()","CameraInfo not found");
    return false;
  }
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putCameraInfo(const CameraInfo& _cameraInfo) {
  putString("Imager Options","CameraInfo",_cameraInfo.serial,"serial");
  putString("Imager Options","CameraInfo",_cameraInfo.name,"name");
  putString("Imager Options","CameraInfo",_cameraInfo.identifier,"identifier");
  putString("Imager Options","CameraInfo",_cameraInfo.connection_type,"connection_type");
  psflive::function::LogMessage("Configuration","putCameraInfo()","CameraInfo = "+_cameraInfo.toString());
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::pickCameraInfo(const std::vector<CameraInfo>& _cameraInfos, CameraInfo& _cameraInfo) {
  int index = -1;
  if (getCameraInfo(_cameraInfo)) {
    auto itPosCameraInfo = std::find(_cameraInfos.begin(), _cameraInfos.end(), _cameraInfo);
    if (itPosCameraInfo == std::end(_cameraInfos))
      index = 0;
    else
      index = itPosCameraInfo - _cameraInfos.begin();
  } else
    index = 0;
  _cameraInfo = _cameraInfos[index];
  return true;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getFrameSize(FrameSize& _frameSize) {
  FrameSize frameSize;
  if (getNumber("Imager Options","FrameSize",frameSize.height,"height") && getNumber("Imager Options","FrameSize",frameSize.width,"width")) {
    _frameSize = frameSize;
    psflive::function::LogMessage("Configuration","getFrameSize()","FrameSize = "+_frameSize.toString());
    return true;
  } else {
    psflive::function::LogMessage("Configuration","getFrameSize()","FrameSize not found");
    return false;
  }
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putFrameSize(const FrameSize& _frameSize) {
  putNumber("Imager Options","FrameSize",_frameSize.height,"height");
  putNumber("Imager Options","FrameSize",_frameSize.width,"width");
  psflive::function::LogMessage("Configuration","putFrameSize()","FrameSize = "+_frameSize.toString());
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::pickFrameSize(const std::vector<FrameSize>& _frameSizes, FrameSize& _frameSize) {
  int index = -1;
  if (getFrameSize(_frameSize)) {
    auto itPosFrameSize = std::find(_frameSizes.begin(), _frameSizes.end(), _frameSize);
    if (itPosFrameSize == std::end(_frameSizes))
      index = 0;
    else
      index = itPosFrameSize - _frameSizes.begin();
  } else
    index = 0;
  _frameSize = _frameSizes[index];
  return true;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getFrameFormat(FrameFormat& _frameFormat) {
  FrameFormat frameFormat;
  if (getString("Imager Options","FrameFormat",frameFormat)) {
    _frameFormat = frameFormat;
    psflive::function::LogMessage("Configuration","getFrameFormat()","FrameFormat = "+_frameFormat);
    return true;
  } else {
    psflive::function::LogMessage("Configuration","getFrameFormat()","FrameFormat not found");
    return false;
  }
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putFrameFormat(const FrameFormat& _frameFormat) {
  putString("Imager Options","FrameFormat",_frameFormat);
  psflive::function::LogMessage("Configuration","putFrameFormat()","FrameFormat = "+_frameFormat);
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::pickFrameFormat(const std::vector<FrameFormat>& _frameFormats, FrameFormat& _frameFormat) {
  int index = -1;
  if (getFrameFormat(_frameFormat)) {
    auto itPosFrameFormat = std::find(_frameFormats.begin(), _frameFormats.end(), _frameFormat);
    if (itPosFrameFormat == std::end(_frameFormats))
      index = 0;
    else
      index = itPosFrameFormat - _frameFormats.begin();
  } else
    index = 0;
  _frameFormat = _frameFormats[index];
  return true;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getFrameRate(FrameRate& _frameRate) {
  FrameRate frameRate;
  if (getNumber("Imager Options","FrameRate",frameRate.denominator,"denominator") && getNumber("Imager Options","FrameRate",frameRate.numerator,"numerator")) {
    _frameRate = frameRate;
    psflive::function::LogMessage("Configuration","getFrameRate()","FrameRate = "+_frameRate.toString());
    return true;
  } else {
    psflive::function::LogMessage("Configuration","getFrameRate()","FrameRate not found");
    return false;
  }
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putFrameRate(const FrameRate& _frameRate) {
  putNumber("Imager Options","FrameRate",_frameRate.denominator,"denominator");
  putNumber("Imager Options","FrameRate",_frameRate.numerator,"numerator");
  psflive::function::LogMessage("Configuration","putFrameRate()","FrameRate = "+_frameRate.toString());
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::pickFrameRate(const std::vector<FrameRate>& _frameRates, FrameRate& _frameRate) {
  int index = -1;
  if (getFrameRate(_frameRate)) {
    auto itFrameRate = std::find(_frameRates.begin(), _frameRates.end(), _frameRate);
    if (itFrameRate == std::end(_frameRates))
      index = 0;
    else
      index = itFrameRate - _frameRates.begin();
  } else
    index = 0;
  _frameRate = _frameRates[index];
  return true;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getROI(bool& _roiEnableX, bool& _roiEnableY, ImRect& _roi) {
  if (m_root.isMember("Imager Options")) {
    if (m_root["Imager Options"].isMember("ROI")) {
      _roiEnableX = m_root["Imager Options"]["ROI"]["x"].asBool();
      _roiEnableY = m_root["Imager Options"]["ROI"]["y"].asBool();
      _roi.Min.x = m_root["Imager Options"]["ROI"]["Min"]["x"].asFloat();
      _roi.Min.y = m_root["Imager Options"]["ROI"]["Min"]["y"].asFloat();
      _roi.Max.x = m_root["Imager Options"]["ROI"]["Max"]["x"].asFloat();
      _roi.Max.y = m_root["Imager Options"]["ROI"]["Max"]["y"].asFloat();
      psflive::function::LogMessage("Configuration","getROI()","ROI = {"+std::to_string(_roi.Min.x)+","+std::to_string(_roi.Min.y)+","+std::to_string(_roi.Max.x)+","+std::to_string(_roi.Max.y)+"}");
      return true;
    }
    psflive::function::LogMessage("Configuration","getCameraInfo()","ROI not found");
    return false;
  }
  psflive::function::LogMessage("Configuration","getCameraInfo()","Imager Options not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putROI(const bool _roiEnableX, const bool _roiEnableY, const ImRect& _roi) {
  m_root["Imager Options"]["ROI"]["x"] = _roiEnableX;
  m_root["Imager Options"]["ROI"]["y"] = _roiEnableY;
  m_root["Imager Options"]["ROI"]["Min"]["x"] = _roi.Min.x;
  m_root["Imager Options"]["ROI"]["Min"]["y"] = _roi.Min.y;
  m_root["Imager Options"]["ROI"]["Max"]["x"] = _roi.Max.x;
  m_root["Imager Options"]["ROI"]["Max"]["y"] = _roi.Max.y;
  psflive::function::LogMessage("Configuration","putROI()","ROI = {"+std::to_string(_roi.Min.x)+","+std::to_string(_roi.Min.y)+","+std::to_string(_roi.Max.x)+","+std::to_string(_roi.Max.y)+"} = ["+std::to_string(_roi.Max.x - _roi.Min.x)+" x "+std::to_string(_roi.Max.y - _roi.Min.y)+"]");
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getROI(bool& _roiEnableX, bool& _roiEnableY, cv::Rect& _roi) {
  if (m_root.isMember("Imager Options")) {
    if (m_root["Imager Options"].isMember("ROI")) {
      _roiEnableX = m_root["Imager Options"]["ROI"]["x"].asBool();
      _roiEnableY = m_root["Imager Options"]["ROI"]["y"].asBool();
      _roi.x = m_root["Imager Options"]["ROI"]["Min"]["x"].asFloat();
      _roi.y = m_root["Imager Options"]["ROI"]["Min"]["y"].asFloat();
      _roi.width = m_root["Imager Options"]["ROI"]["Max"]["x"].asFloat() - _roi.x;
      _roi.height = m_root["Imager Options"]["ROI"]["Max"]["y"].asFloat() - _roi.y;
      psflive::function::LogMessage("Configuration","getROI()","ROI = {"+std::to_string(_roi.br().x)+","+std::to_string(_roi.br().y)+","+std::to_string(_roi.tl().x)+","+std::to_string(_roi.tl().y)+"}");
      return true;
    }
    psflive::function::LogMessage("Configuration","getCameraInfo()","ROI not found");
    return false;
  }
  psflive::function::LogMessage("Configuration","getCameraInfo()","Imager Options not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putROI(const bool _roiEnableX, const bool _roiEnableY, const cv::Rect& _roi) {
  m_root["Imager Options"]["ROI"]["x"] = _roiEnableX;
  m_root["Imager Options"]["ROI"]["y"] = _roiEnableY;
  m_root["Imager Options"]["ROI"]["Min"]["x"] = _roi.br().x;
  m_root["Imager Options"]["ROI"]["Min"]["y"] = _roi.br().y;
  m_root["Imager Options"]["ROI"]["Max"]["x"] = _roi.tl().x;
  m_root["Imager Options"]["ROI"]["Max"]["y"] = _roi.tl().y;
  psflive::function::LogMessage("Configuration","putROI()","ROI = {"+std::to_string(_roi.br().x)+","+std::to_string(_roi.br().y)+","+std::to_string(_roi.tl().x)+","+std::to_string(_roi.tl().y)+"} = ["+std::to_string(_roi.width)+" x "+std::to_string(_roi.height)+"]");
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getMasks(bool& _maskEnable, std::vector<ImGuiCanvasShape>& _maskShapes) {
  if (m_root.isMember("Imager Options")) {
    if (m_root["Imager Options"].isMember("Masks")) {
      _maskEnable = m_root["Imager Options"]["Masks"]["enable"].asBool();
      if (_maskEnable) {
        for (Json::Value::ArrayIndex iShape = 0; iShape < m_root["Imager Options"]["Masks"]["shapes"].size(); iShape++)
          _maskShapes.push_back(psflive::function::jsonValueToShape(m_root["Imager Options"]["Masks"]["shapes"][iShape]));
      }
      return true;
    }
    return false;
  }
  psflive::function::LogMessage("Configuration","getMasks()","Imager Options not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putMasks(const bool _maskEnable, const std::vector<ImGuiCanvasShape>& _maskShapes) {
  if (_maskEnable) {
    m_root["Imager Options"]["Masks"]["enable"] = _maskEnable;
    m_root["Imager Options"]["Masks"]["shapes"] = Json::arrayValue;
    int index = 0;
    for (size_t iShape = 0; iShape < _maskShapes.size(); iShape++) {
      if ((_maskShapes[iShape].getType() != ImGuiCanvasShapeType::HLine) && (_maskShapes[iShape].getType() != ImGuiCanvasShapeType::VLine)) {
        m_root["Imager Options"]["Masks"]["shapes"][index] = psflive::function::shapeToJsonValue(_maskShapes[iShape]); 
        index++;
      }
    }
  }
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getExposureSetting(bool& _auto, int& _expTime_us) {
  if (!getBoolean("Imager Options","Exposure",_auto,"Auto"))
    if (!getNumber("Imager Options","Exposure",_expTime_us,"Time (us)"))
      _auto = true;
  psflive::function::LogMessage("Configuration","getExposureTime()","Exposure"+(_auto?" = Auto":(" Time = "+std::to_string(_expTime_us)+" us")));
  return true;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putExposureSetting(const bool _auto, const int _expTime_us) {
  m_root["Imager Options"]["Exposure"]["Auto"] = _auto;
  m_root["Imager Options"]["Exposure"]["Time (us)"] = _expTime_us;
  psflive::function::LogMessage("Configuration","getExposureTime()","Exposure time = "+std::to_string(_expTime_us)+" us");
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getGainSetting(bool& _auto, int& _gain) {
  if (!getBoolean("Imager Options","Gain",_auto,"Auto"))
    if (!getNumber("Imager Options","Gain",_gain,"Factor"))
      _auto = true;
  psflive::function::LogMessage("Configuration","getExposureTime()","Gain"+(_auto?" = Auto":(" Time = "+std::to_string(_gain)+" us")));
  return false;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putGainSetting(const bool _auto, const int _gain) {
  m_root["Imager Options"]["Gain"]["Auto"] = _auto;
  m_root["Imager Options"]["Gain"]["Factor"] = _gain;
  psflive::function::LogMessage("Configuration","putGain()","Gain = "+std::to_string(_gain));
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getBoolean(const std::string _section, const std::string _base, bool& _boolean, const std::string _sub) {
  if (m_root.isMember(_section)) {
    Json::Value value;
    if (!_sub.empty())
      value = m_root[_section][_base][_sub];
    else
      value = m_root[_section][_base];
    
    if (value!=Json::nullValue) {
      _boolean = value.asBool();
      psflive::function::LogMessage("Configuration","getBoolean()","["+_section+"]["+_base+"] = "+(_boolean?"true":"false"));
      return true;
    } else {
      psflive::function::LogMessage("Configuration","getBoolean()","["+_section+"]["+_base+"] not found");
      return false;
    }
  }
  psflive::function::LogMessage("Configuration","getBoolean()","Section : "+_section+" not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putBoolean(const std::string _section, const std::string _base, const bool _boolean, const std::string _sub) {
  if (!_sub.empty())
    m_root[_section][_base][_sub] = _boolean;
  else
    m_root[_section][_base] = _boolean;
  psflive::function::LogMessage("Configuration","putBoolean()","["+_section+"]["+_base+"] = "+(_boolean?"true":"false"));
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getString(const std::string _section, const std::string _base, std::string& _string, const std::string _sub) {
  if (m_root.isMember(_section)) {
    Json::Value stringValue;
    if (!_sub.empty())
      stringValue = m_root[_section][_base][_sub];
    else
      stringValue = m_root[_section][_base];

    if (stringValue!=Json::nullValue) {
      _string = stringValue.asString();
      psflive::function::LogMessage("Configuration","getString()","["+_section+"]["+_base+"] = "+_string);
      return true;
    } else {
      psflive::function::LogMessage("Configuration","getString()","["+_section+"]["+_base+"] not found");
      return false;
    }
  }
  psflive::function::LogMessage("Configuration","getString()","Section : "+_section+" not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putString(const std::string _section, const std::string _base, const std::string _string, const std::string _sub) {
  if (!_sub.empty())
    m_root[_section][_base][_sub] = _string;
  else
    m_root[_section][_base] = _string;
  psflive::function::LogMessage("Configuration","putString()","["+_section+"]["+_base+"] = "+_string);
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getNumber(const std::string _section, const std::string _base, int& _signedInteger, const std::string _sub) {
  if (m_root.isMember(_section)) {
    Json::Value signedIntegerValue;
    if (!_sub.empty())
      signedIntegerValue = m_root[_section][_base][_sub];
    else
      signedIntegerValue = m_root[_section][_base];

    if (signedIntegerValue!=Json::nullValue) {
      _signedInteger = signedIntegerValue.asInt();
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] = "+std::to_string(_signedInteger));
      return true;
    } else {
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] not found");
      return false;
    }
  }
  psflive::function::LogMessage("Configuration","getNumber()","Section : "+_section+" not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putNumber(const std::string _section, const std::string _base, const int _signedInteger, const std::string _sub) {
  if (!_sub.empty())
    m_root[_section][_base][_sub] = _signedInteger;
  else
    m_root[_section][_base] = _signedInteger;
  psflive::function::LogMessage("Configuration","putNumber()","["+_section+"]["+_base+"] = "+std::to_string(_signedInteger));
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getNumber(const std::string _section, const std::string _base, uint& _unsignedInteger, const std::string _sub) {
  if (m_root.isMember(_section)) {
    Json::Value unsignedIntegerValue;
    if (!_sub.empty())
      unsignedIntegerValue = m_root[_section][_base][_sub];
    else
      unsignedIntegerValue = m_root[_section][_base];

    if (unsignedIntegerValue!=Json::nullValue) {
      _unsignedInteger = unsignedIntegerValue.asUInt();
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] = "+std::to_string(_unsignedInteger));
      return true;
    } else {
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] not found");
      return false;
    }
  }
  psflive::function::LogMessage("Configuration","getNumber()","Section : "+_section+" not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putNumber(const std::string _section, const std::string _base, const uint _unsignedInteger, const std::string _sub) {
  if (!_sub.empty())
    m_root[_section][_base][_sub] = _unsignedInteger;
  else
    m_root[_section][_base] = _unsignedInteger;
  psflive::function::LogMessage("Configuration","putNumber()","["+_section+"]["+_base+"] = "+std::to_string(_unsignedInteger));
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getNumber(const std::string _section, const std::string _base, short& _signedShort, const std::string _sub) {
  if (m_root.isMember(_section)) {
    Json::Value signedShortValue;
    if (!_sub.empty())
      signedShortValue = m_root[_section][_base][_sub];
    else
      signedShortValue = m_root[_section][_base];

    if (signedShortValue!=Json::nullValue) {
      _signedShort = signedShortValue.asInt();
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] = "+std::to_string(_signedShort));
      return true;
    } else {
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] not found");
      return false;
    }
  }
  psflive::function::LogMessage("Configuration","getNumber()","Section : "+_section+" not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putNumber(const std::string _section, const std::string _base, const short _signedShort, const std::string _sub) {
  if (!_sub.empty())
    m_root[_section][_base][_sub] = _signedShort;
  else
    m_root[_section][_base] = _signedShort;
  psflive::function::LogMessage("Configuration","putNumber()","["+_section+"]["+_base+"] = "+std::to_string(_signedShort));
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getNumber(const std::string _section, const std::string _base, ushort& _unsignedShort, const std::string _sub) {
  if (m_root.isMember(_section)) {
    Json::Value unsignedShortValue;
    if (!_sub.empty())
      unsignedShortValue = m_root[_section][_base][_sub];
    else
      unsignedShortValue = m_root[_section][_base];

    if (unsignedShortValue!=Json::nullValue) {
      _unsignedShort = unsignedShortValue.asUInt();
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] = "+std::to_string(_unsignedShort));
      return true;
    } else {
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] not found");
      return false;
    }
  }
  psflive::function::LogMessage("Configuration","getNumber()","Section : "+_section+" not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putNumber(const std::string _section, const std::string _base, const ushort _unsignedShort, const std::string _sub) {
  if (!_sub.empty())
    m_root[_section][_base][_sub] = _unsignedShort;
  else
    m_root[_section][_base] = _unsignedShort;
  psflive::function::LogMessage("Configuration","putNumber()","["+_section+"]["+_base+"] = "+std::to_string(_unsignedShort));
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getNumber(const std::string _section, const std::string _base, long& _signedLong, const std::string _sub) {
  if (m_root.isMember(_section)) {
    Json::Value signedLongValue;
    if (!_sub.empty())
      signedLongValue = m_root[_section][_base][_sub];
    else
      signedLongValue = m_root[_section][_base];

    if (signedLongValue!=Json::nullValue) {
      _signedLong = signedLongValue.asInt64();
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] = "+std::to_string(_signedLong));
      return true;
    } else {
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] not found");
      return false;
    }
  }
  psflive::function::LogMessage("Configuration","getNumber()","Section : "+_section+" not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------
// void psflive::Configuration::putNumber(const std::string _section, const std::string _base, const long _signedLong, const std::string _sub) {
//   if (!_sub.empty())
//     m_root[_section][_base][_sub] = _signedLong;
//   else
//     m_root[_section][_base] = _signedLong;
//   psflive::function::LogMessage("Configuration","putNumber()","["+_section+"]["+_base+"] = "+std::to_string(_signedLong));
// }
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getNumber(const std::string _section, const std::string _base, ulong& _unsignedLong, const std::string _sub) {
  if (m_root.isMember(_section)) {
    Json::Value unsignedLongValue;
    if (!_sub.empty())
      unsignedLongValue = m_root[_section][_base][_sub];
    else
      unsignedLongValue = m_root[_section][_base];

    if (unsignedLongValue!=Json::nullValue) {
      _unsignedLong = unsignedLongValue.asUInt64();
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] = "+std::to_string(_unsignedLong));
      return true;
    } else {
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] not found");
      return false;
    }
  }
  psflive::function::LogMessage("Configuration","getNumber()","Section : "+_section+" not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------
// void psflive::Configuration::putNumber(const std::string _section, const std::string _base, const ulong _unsignedLong, const std::string _sub) {
//   if (!_sub.empty())
//     m_root[_section][_base][_sub] = _unsignedLong;
//   else
//     m_root[_section][_base] = _unsignedLong;
//   psflive::function::LogMessage("Configuration","putNumber()","["+_section+"]["+_base+"] = "+std::to_string(_unsignedLong));
// }
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getNumber(const std::string _section, const std::string _base, float& _float, const std::string _sub) {
  if (m_root.isMember(_section)) {
    Json::Value floatValue;
    if (!_sub.empty())
      floatValue = m_root[_section][_base][_sub];
    else
      floatValue = m_root[_section][_base];

    if (floatValue!=Json::nullValue) {
      _float = floatValue.asFloat();
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] = "+std::to_string(_float));
      return true;
    } else {
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] not found");
      return false;
    }
  }
  psflive::function::LogMessage("Configuration","getNumber()","Section : "+_section+" not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putNumber(const std::string _section, const std::string _base, const float _float, const std::string _sub) {
  if (!_sub.empty())
    m_root[_section][_base][_sub] = _float;
  else
    m_root[_section][_base] = _float;
  psflive::function::LogMessage("Configuration","putNumber()","["+_section+"]["+_base+"] = "+std::to_string(_float));
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::Configuration::getNumber(const std::string _section, const std::string _base, double& _double, const std::string _sub) {
  if (m_root.isMember(_section)) {
    Json::Value doubleValue;
    if (!_sub.empty())
      doubleValue = m_root[_section][_base][_sub];
    else
      doubleValue = m_root[_section][_base];

    if (doubleValue!=Json::nullValue) {
      _double = doubleValue.asDouble();
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] = "+std::to_string(_double));
      return true;
    } else {
      psflive::function::LogMessage("Configuration","getNumber()","["+_section+"]["+_base+"] not found");
      return false;
    }
  }
  psflive::function::LogMessage("Configuration","getNumber()","Section : "+_section+" not found");
  return false;
}
// --------------------------------------------------------------------------------------------------------------------
void psflive::Configuration::putNumber(const std::string _section, const std::string _base, const double _double, const std::string _sub) {
  if (!_sub.empty())
    m_root[_section][_base][_sub] = _double;
  else
    m_root[_section][_base] = _double;
  psflive::function::LogMessage("Configuration","putNumber()","["+_section+"]["+_base+"] = "+std::to_string(_double));
}
// --------------------------------------------------------------------------------------------------------------------

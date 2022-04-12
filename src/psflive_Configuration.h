#ifndef __PSFLIVE_CONFIGURATION_H__
#define __PSFLIVE_CONFIGURATION_H__

#pragma once

#include <fstream>
#include <json/json.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui/addons/canvas/imgui_canvas.h"

#include "psflive_def.h"

#include "psflive_device_Imager.h"
// ====================================================================================================================










// ====================================================================================================================
namespace psflive {
  class Configuration {
  private:
    Json::Value m_root;
    std::string m_fileName;
    std::fstream m_fileStream;
  public:
    // ---- constructor -----------------------------------------------------------------------------------------------
    Configuration(const std::string& _fileName = PSFLIVE_CONFIGURATION_FILE_STR);
    // ---- destructor ------------------------------------------------------------------------------------------------
    ~Configuration() = default;
    // ----------------------------------------------------------------------------------------------------------------
    void Read();
    void Write();
    // ----------------------------------------------------------------------------------------------------------------
    bool getCameraInfo(CameraInfo& _cameraInfo);
    void putCameraInfo(const CameraInfo& _cameraInfo);
    bool pickCameraInfo(const std::vector<CameraInfo>& _cameraInfos, CameraInfo& _cameraInfo);
    // ----------------------------------------------------------------------------------------------------------------
    bool getFrameSize(FrameSize& _frameSize);
    void putFrameSize(const FrameSize& _frameSize);
    bool pickFrameSize(const std::vector<FrameSize>& _frameSizes, FrameSize& _frameSize);
    // ----------------------------------------------------------------------------------------------------------------
    bool getFrameFormat(FrameFormat& _frameFormat);
    void putFrameFormat(const FrameFormat& _frameFormat);
    bool pickFrameFormat(const std::vector<FrameFormat>& _frameFormats, FrameFormat& _frameFormat);
    // ----------------------------------------------------------------------------------------------------------------
    bool getFrameRate(FrameRate& _frameRate);
    void putFrameRate(const FrameRate& _frameRate);
    bool pickFrameRate(const std::vector<FrameRate>& _frameRates, FrameRate& _frameRate);
    // ----------------------------------------------------------------------------------------------------------------
    bool getROI(bool& _roiEnableX, bool& _roiEnableY, ImRect& _roi);
    void putROI(const bool _roiEnableX, const bool _roiEnableY, const ImRect& _roi);
    // ----------------------------------------------------------------------------------------------------------------
    bool getROI(bool& _roiEnableX, bool& _roiEnableY, cv::Rect& _roi);
    void putROI(const bool _roiEnableX, const bool _roiEnableY, const cv::Rect& _roi);
    // ----------------------------------------------------------------------------------------------------------------
    bool getExposureSetting(bool& _auto, int& _expTime);
    void putExposureSetting(const bool _auto, const int _expTime);
    // ----------------------------------------------------------------------------------------------------------------
    bool getGainSetting(bool& _auto, int& _gain);
    void putGainSetting(const bool _auto, const int _gain);
    // ----------------------------------------------------------------------------------------------------------------
    bool getMasks(bool& _maskEnable, std::vector<ImGuiCanvasShape>& _maskShapes);
    void putMasks(const bool _maskEnable, const std::vector<ImGuiCanvasShape>& _maskShapes);
    // ----------------------------------------------------------------------------------------------------------------
    bool getBoolean(const std::string _section, const std::string _base, bool& _boolean, const std::string _sub="");
    void putBoolean(const std::string _section, const std::string _base, const bool _boolean, const std::string _sub="");
    // ----------------------------------------------------------------------------------------------------------------
    bool getString(const std::string _section, const std::string _base, std::string& _string, const std::string _sub="");
    void putString(const std::string _section, const std::string _base, const std::string _string, const std::string _sub="");
    // ----------------------------------------------------------------------------------------------------------------
    bool getNumber(const std::string _section, const std::string _base, char& _char, const std::string _sub="");
    void putNumber(const std::string _section, const std::string _base, const char _char, const std::string _sub="");
    // ----------------------------------------------------------------------------------------------------------------
    bool getNumber(const std::string _section, const std::string _base, uchar& _unsignedChar, const std::string _sub="");
    void putNumber(const std::string _section, const std::string _base, const uchar _unsignedChar, const std::string _sub="");
    // ----------------------------------------------------------------------------------------------------------------
    bool getNumber(const std::string _section, const std::string _base, int& _signedInteger, const std::string _sub="");
    void putNumber(const std::string _section, const std::string _base, const int _signedInteger, const std::string _sub="");
    // ----------------------------------------------------------------------------------------------------------------
    bool getNumber(const std::string _section, const std::string _base, uint& _unsignedInteger, const std::string _sub="");
    void putNumber(const std::string _section, const std::string _base, const uint _unsignedInteger, const std::string _sub="");
    // ----------------------------------------------------------------------------------------------------------------
    bool getNumber(const std::string _section, const std::string _base, short& _signedShort, const std::string _sub="");
    void putNumber(const std::string _section, const std::string _base, const short _signedShort, const std::string _sub="");
    // ----------------------------------------------------------------------------------------------------------------
    bool getNumber(const std::string _section, const std::string _base, ushort& _unsignedShort, const std::string _sub="");
    void putNumber(const std::string _section, const std::string _base, const ushort _unsignedShort, const std::string _sub="");
    // ----------------------------------------------------------------------------------------------------------------
    bool getNumber(const std::string _section, const std::string _base, long& _signedLong, const std::string _sub="");
    void putNumber(const std::string _section, const std::string _base, const long _signedLong, const std::string _sub="");
    // ----------------------------------------------------------------------------------------------------------------
    bool getNumber(const std::string _section, const std::string _base, ulong& _unsignedLong, const std::string _sub="");
    void putNumber(const std::string _section, const std::string _base, const ulong _unsignedLong, const std::string _sub="");
    // ----------------------------------------------------------------------------------------------------------------
    bool getNumber(const std::string _section, const std::string _base, float& _float, const std::string _sub="");
    void putNumber(const std::string _section, const std::string _base, const float _float, const std::string _sub="");
    // ----------------------------------------------------------------------------------------------------------------
    bool getNumber(const std::string _section, const std::string _base, double& _double, const std::string _sub="");
    void putNumber(const std::string _section, const std::string _base, const double _double, const std::string _sub="");
    // ----------------------------------------------------------------------------------------------------------------
  };
};
// ====================================================================================================================
#endif //__PSFLIVE_CONFIGURATION_H__
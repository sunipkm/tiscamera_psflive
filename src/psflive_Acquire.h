#ifndef __PSFLIVE_ACQUIREDATA_H__
#define __PSFLIVE_ACQUIREDATA_H__

#pragma once

#include <string>
#include <thread>

#include "imgui/addons/canvas/shader/shader.h"

#include "psflive_def.h"
// ====================================================================================================================










// ====================================================================================================================
namespace psflive {

  enum class AcquireType {capture, ntypes};

  class AcquireData { // abstract
  public:
    // ---- UI --------------------------------------------------------------------------------------------------------
    bool m_initWindow=0;
    bool m_show=0;
    Shader* m_ptrShader;
    GLuint m_imageTexture, m_maskTexture, m_compositeTexture, m_renderBuffers, m_frameBuffers;
    ImVec2 m_mapSize, m_viewSize;
    // ----------------------------------------------------------------------------------------------------------------
  protected:
    AcquireType m_dataType; std::string m_acquireTypeComment;
    std::time_t m_time;
  private:
    float m_wavelength_nm=0.0; std::string m_wavelengthComment="Wavelength (nm)";
    uint m_exposureTime_us=0; std::string m_exposureTimeComment="Exposure time (us)";
    uint m_gain=0; std::string m_gainComment="Gain units";
  protected:
    std::string m_timeComment="Time created", m_dateComment="Date created", m_source, m_sourceComment="Source application", m_version, m_versionComment="version", m_comment="";
    // ---- mask ------------------------------------------------------------------------------------------------------
    ImageData m_mask; // [_rows x _cols]
    std::string m_maskComment="";
    // ----------------------------------------------------------------------------------------------------------------
    std::string m_fileName="";
  public:
    AcquireData() = default;
    AcquireData(const AcquireType _dataType, uint _exposureTime_us=0, uint _gain=0) : m_dataType(_dataType), m_exposureTime_us(_exposureTime_us), m_gain(_gain) {}
    AcquireData(const AcquireType _dataType, const std::string& _fileName) : m_dataType(_dataType), m_fileName(_fileName) {}
    // ---- destructor ------------------------------------------------------------------------------------------------
    ~AcquireData() = default;
    // ----------------------------------------------------------------------------------------------------------------
    virtual void setFileName(const std::string& _fileName) = 0;
    std::string getFileName() const {
      return m_fileName;
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setWavelength_nm(const float& _wavelength_nm, const std::string _comment="Wavelength (nm)") {
      m_wavelength_nm = _wavelength_nm;
      m_wavelengthComment = _comment;
    }
    float getWavelength_nm() const {
      return m_wavelength_nm;
    }
    std::string getWavelengthComment() const {
      return m_wavelengthComment;
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setExposureTime_us(const uint& _exposureTime_us, const std::string _comment="Exposure time (us)") {
      m_exposureTime_us = _exposureTime_us;
      m_exposureTimeComment = _comment;
    }
    uint getExposureTime_us() const {
      return m_exposureTime_us;
    }
    std::string getExposureTimeComment() const {
      return m_exposureTimeComment;
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setGain(const uint& _gain, const std::string _comment="Gain units") {
      m_gain = _gain;
      m_gainComment = _comment;
    }
    uint getGain() const {
      return m_gain;
    }
    std::string getGainComment() const {
      return m_gainComment;
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setMask(const ImageData& _mask) {
      m_mask = _mask;
    }
    ImageData getMask() const {
      return m_mask;
    }
    // ----------------------------------------------------------------------------------------------------------------
    AcquireType getAcquireType() const {
      return m_dataType;
    };
    std::string getTypeComment() const {
      return m_source;
    }
    std::string getSource() const {
      return m_source;
    }
    std::string getSourceComment() const {
      return m_sourceComment;
    }
    std::string getVersion() const {
      return m_version;
    }
    std::string getVersionComment() const {
      return m_versionComment;
    }
    std::string getComment() const {
      return m_comment;
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setTime(const std::string& _date, const std::string& _time, const std::string _dateComment="Date created", const std::string _timeComment="Time created") {
      struct tm time;
      std::string date_time = _date+"-"+_time;
      strptime(date_time.c_str(), "%Y/%m/%d-%H:%M:%S", &time);
      m_time = mktime(&time);
      m_dateComment = _dateComment;
      m_timeComment = _timeComment;
    }
    void setTime(std::time_t _time, const std::string _dateComment="Date created", const std::string _timeComment="Time created") {
      m_time = _time;
    }
    std::string getDate() const {
      char dateCharArr[12];
      std::strftime(dateCharArr, sizeof(dateCharArr), "%Y/%m/%d", std::localtime(&m_time));
      return std::string(dateCharArr);
    }
    std::string getDateComment() const {
      return m_dateComment;
    }
    std::string getTime() const {
      char timeCharArr[10];
      std::strftime(timeCharArr, sizeof(timeCharArr), "%H:%M:%S", std::localtime(&m_time));
      return std::string(timeCharArr);
    }
    std::string getTimeComment() const {
      return m_timeComment;
    }
    // ----------------------------------------------------------------------------------------------------------------
    virtual uint getRowCount() const = 0;
    virtual uint getColumnCount() const = 0;
    virtual float getAspectRatio() const = 0;
    // ----------------------------------------------------------------------------------------------------------------
    virtual bool ReadFromFile(const std::string& _fullPath) = 0;
    // virtual bool WriteToFile(const std::string& _fileName, bool _saveFrames) = 0;
    // ----------------------------------------------------------------------------------------------------------------
  };

  class AcquireOperation {  // abstract
  protected:
    static uint CURRENTINDEX; // global
    static std::time_t CURRENTTIME; // global
    static bool BUSY; // global
    static std::string LOCATION;// global
  private:
    const AcquireType m_operationType;
  public:
    std::thread m_operationThread;
    // ---- constructor -----------------------------------------------------------------------------------------------
    AcquireOperation() = default;
    AcquireOperation(const AcquireType _operationType) : m_operationType(_operationType) {}
    // ---- destructor ------------------------------------------------------------------------------------------------
    virtual ~AcquireOperation() = default;
    // ----------------------------------------------------------------------------------------------------------------
    virtual void Execute() = 0;
    // ----------------------------------------------------------------------------------------------------------------
    static bool GETBUSY() {
      return BUSY;
    }
    static void SETBUSY(const bool _busy=true) {
      BUSY = _busy;
    }
    // ----------------------------------------------------------------------------------------------------------------
    static void RESETCURRENTINDEX() {
      CURRENTINDEX = 0;
    }
    static uint GETCURRENTINDEX() {
      return CURRENTINDEX;
    }
    static void INCREMENTCURRENTINDEX() {
      CURRENTINDEX++;
    }
    // ----------------------------------------------------------------------------------------------------------------
    static std::time_t GETCURRENTTIME() {
      return CURRENTTIME;
    }
    static tm* GETLOCALTIME() {
      return std::localtime(&CURRENTTIME);
    }
    static void UPDATECURRENTTIME() {
      CURRENTTIME = std::time(nullptr);
    }
    // ----------------------------------------------------------------------------------------------------------------
    static std::string GETLOCATION() {
      return LOCATION;
    }
    static void SETLOCATION(const std::string _location) {
      LOCATION = _location;
    }
    // ----------------------------------------------------------------------------------------------------------------
  };

};
// ====================================================================================================================
#endif //__PSFLIVE_ACQUIREDATA_H__
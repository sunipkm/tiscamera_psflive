#ifndef __PSFLIVE_DEVICE_IMAGER_H__
#define __PSFLIVE_DEVICE_IMAGER_H__

#pragma once

#include "cameras/camera.h"

#include "psflive_def.h"
// ====================================================================================================================










// ====================================================================================================================
namespace psflive::device {
  class Imager {
  public:
    static std::vector<CameraInfo> CAMERAINFOS; // global
  private:
    CameraInfo m_cameraInfo;
    std::shared_ptr<Camera> m_ptrCamera;
    std::vector<FrameSize> m_frameSizes;
    std::vector<FrameFormat> m_frameFormats;
    std::vector<FrameRate> m_frameRates;
    FrameSize m_frameSize;
    FrameFormat m_frameFormat;
    FrameRate m_frameRate;
    bool m_enable;
    struct STFrame { // structure passed to the callback
      uint count = 0; // counter
      ImageData image; // pixel data
      bool save = false; // save image?
      bool crop = false;
      cv::Rect roi;
      std::mutex lock;
      std::condition_variable condition;
    } m_frame;
    static GstFlowReturn SamplingCallback(GstAppSink* _ptrAppSink, void* _ptrData);
  public:
    // ---- static functions ------------------------------------------------------------------------------------------
    static void Initialize(int* _ptrArgc, char*** _ptrPtrArgv);

    // ---- constructor -----------------------------------------------------------------------------------------------
    Imager() = default;
    Imager(const CameraInfo& _cameraInfo);
    // ---- destructor ------------------------------------------------------------------------------------------------
    ~Imager() = default;
    // ----------------------------------------------------------------------------------------------------------------

    std::shared_ptr<Property> getProperty(std::string _propName);
    template<typename T> std::shared_ptr<T> getProperty(std::string _propName) const {
      return std::dynamic_pointer_cast<T>(m_ptrCamera->get_property(_propName));
    }
    bool setProperty(std::shared_ptr<Property> _pProperty);
    template<typename T> bool setProperty(std::shared_ptr<T> _pProperty) {
      return _pProperty->set(*m_ptrCamera, _pProperty->value);
    }
    bool isAvailable() const {
      return (CAMERAINFOS.size() > 0);
    }
    void Update(const FrameSize& _frameSize, const FrameFormat& _frameFormat, const FrameRate& _frameRate);
    void Update(const FrameSize& _frameSize);
    void Update(const FrameFormat& _frameFormat);
    void Update(const FrameRate& _frameRate);
    void Update() {
      return Update(m_frameSize, m_frameFormat, m_frameRate);
    }
    const bool Start();
    bool Stop() {
      if (m_enable) {
        m_enable = false;
        return m_ptrCamera->stop();
      }
      return true;
    }
    uint getCount() const {
      return m_frame.count;
    }
    size_t getBitsPerPixel() const {
      return m_frame.image.elemSize();
    }
    int getType() const {
      return m_frame.image.type();
    }
    int getRows() const {
      return m_frame.image.rows;
    }
    int getColumns() const {
      return m_frame.image.cols;
    }
    float getAspectRatio() const {
      return ((float)m_frame.image.rows/(float)m_frame.image.cols);
    }
    uchar* getData() const {
      return m_frame.image.data;
    }
    size_t getNpx() const {
      return m_frame.image.total();
    }
    size_t getSize() const {
      return m_frame.image.total() * m_frame.image.elemSize();
    }
    bool isEnabled() const {
      return m_enable;
    }
    std::vector<FrameSize> getFrameSizes() const {
      return m_frameSizes;
    }
    std::vector<FrameFormat> getFrameFormats() const {
      return m_frameFormats;
    }
    std::vector<FrameRate> getFrameRates() const {
      return m_frameRates;
    }
    CameraInfo getCameraInfo() const {
      return m_cameraInfo;
    }
    FrameSize getFrameSize() const {
      return m_frameSize;
    }
    FrameFormat getFrameFormat() const {
      return m_frameFormat;
    }
    FrameRate getFrameRate() const {
      return m_frameRate;
    }
    cv::Mat getFrame() {
      return m_frame.image;
    }
    cv::Mat getROIFrame() const {
      return m_frame.crop?m_frame.image(m_frame.roi).clone():m_frame.image;
    }
    float getROIAspectRatio() const {
      return ((float)m_frame.roi.height/(float)m_frame.roi.width);
    }
    void setRoi(const cv::Rect _roi) {
      if ((_roi.x > 0) || (_roi.y > 0) || (_roi.width < m_frameSize.width) || (_roi.height < m_frameSize.height))
        m_frame.crop = true;
      m_frame.roi = cv::Rect(_roi.x,_roi.y,_roi.width,_roi.height);
    }
    void resetRoi(const cv::Rect _roi) {
      m_frame.crop = false;
      m_frame.roi = {0,0,m_frameSize.width,m_frameSize.height};
    }
    cv::Rect getRoi() const {
      return m_frame.roi;
    }
    bool SnapImage(uint _timeout_ms);
    bool SnapImage();
  };
};

#endif //__PSFLIVE_DEVICE_IMAGER_H__
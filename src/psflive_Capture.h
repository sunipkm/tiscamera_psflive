#ifndef __PSFLIVE_CAPTURE_H__
#define __PSFLIVE_CAPTURE_H__

#pragma once

#include "psflive_device_Imager.h"

#include "psflive_Acquire.h"
#include "psflive_function.h"
// ====================================================================================================================










// ====================================================================================================================
namespace psflive {
  class CaptureData : public AcquireData {
  private:
    // ---- (primary) mean frame --------------------------------------------------------------------------------------
    ImageData m_meanFrame; // [_rows x _cols]
    ImageData m_disp_meanFrame; // [_rows x _cols]
    // std::string m_comment="";
    // ---- (extension 1) frames --------------------------------------------------------------------------------------
    std::vector<ImageData> m_frames; // _nFrames x [_rows x _cols]
    std::string m_framesComment="";
    // ---- (extension 2) mask ----------------------------------------------------------------------------------------
    // ImageData m_mask; // [_rows x _cols]
    // std::string m_maskComment="";
    // ----------------------------------------------------------------------------------------------------------------
  public:
    // ---- constructor -----------------------------------------------------------------------------------------------
    CaptureData() = default;
    CaptureData(uint _nFrames, int _nRows, int _nCols, int _type, uint _exposureTime_us=0, uint _gain=0);
    CaptureData(const std::string& _fileName);
    // ---- destructor ------------------------------------------------------------------------------------------------
    ~CaptureData() = default;
    // ----------------------------------------------------------------------------------------------------------------
    void setFileName(const std::string& _fileName="." PSFLIVE_CAPTURE_PREFIX_STR "_"+function::timeStamp()+".fits") override {
      m_fileName = _fileName;
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setFrame(uint _iFrame, const ImageData& _aFrame) {
      m_frames[_iFrame] = _aFrame;
    }
    ImageData getFrame(uint _iFrame) {
      return m_frames[_iFrame];
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setFrames(const std::vector<ImageData>& _frames) {
      m_frames = _frames;
    }
    std::vector<ImageData> getFrames() const {
      return m_frames;
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setMeanFrame() {
      m_meanFrame = function::imageMean(m_frames);
    }
    void setMeanFrame(const ImageData& _aMeanFrame) {
      m_meanFrame = _aMeanFrame;
    }
    ImageData getMeanFrame() const {
      return m_meanFrame;
    }
    ImageData getDispMeanFrame() const {
      return m_disp_meanFrame;
    }
    // ----------------------------------------------------------------------------------------------------------------
    uint getRowCount() const override {
      return m_meanFrame.rows;
    };
    uint getColumnCount() const override {
      return m_meanFrame.cols;
    };
    uint getFrameCount() const {
      return m_frames.size();
    }
    uint getPerFramePixelCount() const {
      return getRowCount()*getColumnCount();
    }
    uint getPixelCount() const {
      return getPerFramePixelCount()*getFrameCount();
    }
    float getAspectRatio() const override {
      return ((float)m_meanFrame.rows/(float)m_meanFrame.cols);
    };
    // ----------------------------------------------------------------------------------------------------------------
    bool ReadFromFile(const std::string& _fileName) override;
    bool WriteToFile(const std::string& _fileName="." PSFLIVE_CAPTURE_PREFIX_STR "_"+function::timeStamp()+".fits", bool _saveFrames=true);
    // ----------------------------------------------------------------------------------------------------------------
    std::string getWindowTitle() const {
      return "Capture: "+m_fileName;
    }
    // ----------------------------------------------------------------------------------------------------------------
  };

  class CaptureOperation : public AcquireOperation {
  private:
    struct CaptureThreadData { // structure passed to the callback
      bool saveOnClick=false, saveFrames=false;
      uint frameInterval_us=0;
      uint iFrame=0;
    } m_threadData;
  public:
    CaptureData m_captureData;
    psflive::device::Imager* m_ptrImager = nullptr;
    // ---- constructor -----------------------------------------------------------------------------------------------
    CaptureOperation() = default;
    CaptureOperation(psflive::device::Imager* _ptrImager, const uint _nFrames=1, const uint _frameInterval_us=0);
    // ---- destructor ------------------------------------------------------------------------------------------------
    ~CaptureOperation() = default;
    // ----------------------------------------------------------------------------------------------------------------
    void setSaveFrames(bool _saveFrames=true) {
      m_threadData.saveFrames=_saveFrames;
    }
    bool getSaveFrames() const {
      return m_threadData.saveFrames;
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setInterval_us(uint _frameInterval_us=0) {
      m_threadData.frameInterval_us = _frameInterval_us;
    }
    uint getInterval_us() const {
      return m_threadData.frameInterval_us;
    }
    // ----------------------------------------------------------------------------------------------------------------
    uint getIframe() const {
      return m_threadData.iFrame;
    }
    void setIframe(const uint _iFrame) {
      m_threadData.iFrame = _iFrame;
    }
    // ----------------------------------------------------------------------------------------------------------------
    uint getFrameCount() const {
      return m_captureData.getFrameCount();
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setSaveOnClick(const bool _saveOnClick=true) {
      m_threadData.saveOnClick = _saveOnClick;
    }
    bool getSaveOnClick() const {
      return m_threadData.saveOnClick;
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setFileName(const std::string _fileName) {
      m_captureData.setFileName(_fileName);
    }
    std::string getFileName() {
      return m_captureData.getFileName();
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setTime() {
      m_captureData.setTime(GETCURRENTTIME());
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setMask(const ImageData& _mask) {
      m_captureData.setMask(_mask);
    }
    ImageData getMask() const {
      return m_captureData.getMask();
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setWavelength_nm(const float _wavelength_nm) {
      m_captureData.setWavelength_nm(_wavelength_nm);
    }
    float getWavelength_nm() const {
      return m_captureData.getWavelength_nm();
    }
    // ----------------------------------------------------------------------------------------------------------------
    void CaptureFunction(CaptureThreadData& _threadData, CaptureData& _calibrateData, device::Imager* _ptrImager);
    void Execute() override;
  };

  namespace ui {
    void CaptureDataWindow(CaptureData& _captureData);
  }

};
// ====================================================================================================================
#endif //__PSFLIVE_CAPTURE_H__
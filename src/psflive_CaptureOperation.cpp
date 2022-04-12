#include "psflive_Capture.h"
// ====================================================================================================================










// ====================================================================================================================
psflive::CaptureOperation::CaptureOperation(psflive::device::Imager* _ptrImager, uint _nFrames, uint _frameInterval_us) : AcquireOperation(psflive::AcquireType::capture), m_ptrImager(_ptrImager) {
  static std::shared_ptr<IntegerProperty> ptrExpTimeIntProp = std::dynamic_pointer_cast<IntegerProperty>(_ptrImager->getProperty("Exposure Time (us)"));
  static std::shared_ptr<IntegerProperty> ptrGainIntProp = std::dynamic_pointer_cast<IntegerProperty>(_ptrImager->getProperty("Gain"));
  m_captureData = CaptureData(_nFrames, _ptrImager->getRoi().height,  _ptrImager->getRoi().width, _ptrImager->getType(), ptrExpTimeIntProp->value, ptrGainIntProp->value);
  m_threadData.frameInterval_us = _frameInterval_us;
  m_threadData.iFrame = 0;
  return;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::CaptureOperation::CaptureFunction(CaptureThreadData& _threadData, CaptureData& _captureData, device::Imager* _ptrImager) {
  psflive::CaptureOperation::SETBUSY();

  psflive::function::LogMessage("CaptureOperation","CaptureFunction()","Begin capture.");
  
  _threadData.iFrame = 0;
  while ((_threadData.iFrame < _captureData.getFrameCount()) && psflive::CaptureOperation::GETBUSY()) {
    psflive::function::LogMessage("CaptureOperation","CaptureFunction()","Frame "+std::to_string(_threadData.iFrame)+" of "+std::to_string(_captureData.getFrameCount()));
    psflive::function::interruptibleUSleep(_threadData.frameInterval_us, psflive::CaptureOperation::GETBUSY()); // delay between frames
    if (_ptrImager->SnapImage())
      _captureData.setFrame(_threadData.iFrame, _ptrImager->getROIFrame());
    _threadData.iFrame++;
  }

  _captureData.setMeanFrame();

  if (_threadData.saveOnClick)
    _captureData.WriteToFile(GETLOCATION()+_captureData.getFileName(), _threadData.saveFrames);

  psflive::function::LogMessage("CaptureOperation","CaptureFunction()","End capture.");
  
  psflive::CaptureOperation::SETBUSY(false);
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::CaptureOperation::Execute() {
  if (!psflive::CaptureOperation::GETBUSY()) // should not spawn multiple thread
    m_operationThread = std::thread(&CaptureOperation::CaptureFunction, this, std::ref(m_threadData), std::ref(m_captureData), m_ptrImager);
  return;
}
// ====================================================================================================================
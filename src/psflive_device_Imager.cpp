#include "psflive_device_Imager.h"
// ====================================================================================================================










// ====================================================================================================================
void psflive::device::Imager::Initialize(int* _ptrArgc, char*** _ptrPtrArgv) {
  gst_init(_ptrArgc, _ptrPtrArgv);
  psflive::device::Imager::CAMERAINFOS = DeviceList();
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
psflive::device::Imager::Imager(const CameraInfo& _cameraInfo) : m_cameraInfo(_cameraInfo), m_ptrCamera(new Camera(_cameraInfo.serial)) {
  m_frameSizes = m_ptrCamera->get_framesize_list();
  m_frameSize = m_frameSizes[0];
  m_frameFormats = m_ptrCamera->get_frameformat_list(m_frameSize);
  m_frameFormat = m_frameFormats[0];
  m_frameRates = m_ptrCamera->get_framerate_list(m_frameSize, m_frameFormat);
  m_frameRate = m_frameRates[0];
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
std::shared_ptr<Property> psflive::device::Imager::getProperty(std::string _name) {
  return m_ptrCamera->get_property(_name);
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::device::Imager::Update(const FrameSize& _frameSize, const FrameFormat& _frameFormat, const FrameRate& _frameRate) {
  if (_frameFormat=="GRAY8")
    m_frame.image.create(_frameSize.height, _frameSize.width, CV_8UC1);
  else if (_frameFormat=="GRAY16_LE")
    m_frame.image.create(_frameSize.height, _frameSize.width, CV_16UC1);
  else
    m_frame.image.create(_frameSize.height, _frameSize.width, CV_8UC4);
  m_ptrCamera->set_capture_format(_frameFormat, _frameSize, _frameRate);
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::device::Imager::Update(const FrameSize& _frameSize) {
  m_frameSize = _frameSize;
  return psflive::device::Imager::Update();
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::device::Imager::Update(const FrameFormat& _frameFormat) {
  m_frameFormat = _frameFormat;
  return psflive::device::Imager::Update();
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
void psflive::device::Imager::Update(const FrameRate& _frameRate) {
  m_frameRate = _frameRate;
  return psflive::device::Imager::Update();
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
const bool psflive::device::Imager::Start() {
  m_ptrCamera->set_new_frame_callback(SamplingCallback, &m_frame);
  m_enable = true;
  return m_ptrCamera->start();
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
GstFlowReturn psflive::device::Imager::SamplingCallback(GstAppSink* _appSink, void* _ptrData) {
  STFrame* _ptrFrame = (STFrame*)_ptrData;
  // if( !pFrameData->save)
  //   return GST_FLOW_OK;
  std::lock_guard<std::mutex> lck(_ptrFrame->lock);

  _ptrFrame->save = false;
  _ptrFrame->count++;

  GstSample *sample = gst_app_sink_pull_sample(_appSink);
  GstBuffer *buffer = gst_sample_get_buffer(sample);
  GstMapInfo info;
  gst_buffer_map(buffer, &info, GST_MAP_READ);

  if (info.data != NULL)
    memcpy(_ptrFrame->image.data, info.data, _ptrFrame->image.total() * _ptrFrame->image.elemSize());

  gst_buffer_unmap(buffer, &info);
  gst_sample_unref(sample);
  _ptrFrame->condition.notify_all();
  return GST_FLOW_OK;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::device::Imager::SnapImage(uint _timeout_ms) {
  std::unique_lock<std::mutex> lock(m_frame.lock);
  m_frame.save = true;
  if(m_frame.condition.wait_for(lock, std::chrono::milliseconds(_timeout_ms)) != std::cv_status::timeout)
    return true;
  return false;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::device::Imager::SnapImage() {
  static std::shared_ptr<IntegerProperty> ptrExpTimeIntProp = std::dynamic_pointer_cast<IntegerProperty>(getProperty("Exposure Time (us)"));
  return SnapImage(ptrExpTimeIntProp->value*1000*10); // willing to wait 10x exposure time
}
// ====================================================================================================================

#include <CCfits/CCfits>

#include "psflive_Capture.h"
// ====================================================================================================================










// ====================================================================================================================
psflive::CaptureData::CaptureData(uint _nFrames, int _rows, int _cols, int _type, uint _exposureTime_us, uint _gain) : psflive::AcquireData(AcquireType::capture, _exposureTime_us, _gain), m_meanFrame(_rows,_cols,CV_64FC1), m_disp_meanFrame(_rows,_cols,CV_16SC1), m_frames(_nFrames,ImageData(_cols,_rows,_type,cv::Scalar(0))) {
  m_meanFrame.convertTo(m_disp_meanFrame, CV_16SC1);
  psflive::function::LogMessage("CaptureData","CaptureData()","Initializing - m_images : "+psflive::function::imageInfo(m_frames));
  psflive::function::LogMessage("CaptureData","CaptureData()","Initializing - m_meanImage : "+psflive::function::imageInfo(m_meanFrame));
  setFileName();
  return;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
psflive::CaptureData::CaptureData(const std::string& _fileName) : psflive::AcquireData(AcquireType::capture, _fileName) {
  psflive::function::LogMessage("CaptureData","CaptureData()","Initializing - from file = "+_fileName);
  ReadFromFile(_fileName);
  return;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::CaptureData::ReadFromFile(const std::string& _fullPath) {
  psflive::function::LogMessage("CaptureData", "ReadFromFile()",PSFLIVE_GREEN "File = "+_fullPath+PSFLIVE_RESET);
  int beginIdx = _fullPath.rfind('/');
  setFileName(_fullPath.substr(beginIdx+1));

  std::shared_ptr<CCfits::FITS> ptrFITS(new CCfits::FITS(_fullPath,CCfits::Read));

  ptrFITS->pHDU().readAllKeys();
  
  ptrFITS->pHDU().keyWord(PSFLIVE_SOURCE_STR).value(m_source);
  m_sourceComment = ptrFITS->pHDU().keyWord(PSFLIVE_SOURCE_STR).comment();
  psflive::function::LogMessage("CaptureData","ReadFromFile()", PSFLIVE_GREEN PSFLIVE_SOURCE_STR "  = "+getSource()+" ("+getSourceComment()+")" PSFLIVE_RESET);

  std::string dataType;
  ptrFITS->pHDU().keyWord(PSFLIVE_DATATYPE_STR).value(dataType);
  m_acquireTypeComment = ptrFITS->pHDU().keyWord(PSFLIVE_DATATYPE_STR).comment();
  psflive::function::stringToAcquireDataType(dataType, m_dataType);
  psflive::function::LogMessage("CaptureData","ReadFromFile()", PSFLIVE_GREEN PSFLIVE_DATATYPE_STR "    = "+psflive::function::acquireDataTypeToString(getAcquireType())+" ("+getTypeComment()+")" PSFLIVE_RESET);

  ptrFITS->pHDU().keyWord(PSFLIVE_VERSION_STR).value(m_version);
  m_versionComment = ptrFITS->pHDU().keyWord(PSFLIVE_VERSION_STR).comment();
  psflive::function::LogMessage("CaptureData","ReadFromFile()", PSFLIVE_GREEN PSFLIVE_VERSION_STR " : "+getVersion()+" ("+getVersionComment()+")" PSFLIVE_RESET);

  std::string date, time, dateComment, timeComment;
  ptrFITS->pHDU().keyWord(PSFLIVE_DATE_STR).value(date);
  dateComment = ptrFITS->pHDU().keyWord(PSFLIVE_DATE_STR).comment();
  ptrFITS->pHDU().keyWord(PSFLIVE_TIME_STR).value(time);
  timeComment = ptrFITS->pHDU().keyWord(PSFLIVE_TIME_STR).comment();
  setTime(date, time, dateComment, timeComment);
  psflive::function::LogMessage("CaptureData","ReadFromFile()", PSFLIVE_GREEN PSFLIVE_DATE_STR "-" PSFLIVE_TIME_STR " : "+getDate()+"-"+getTime()+" ("+getTimeComment()+")" PSFLIVE_RESET);

  int dataExposureTime_us;
  std::string dataExposureTimeComment;
  ptrFITS->pHDU().keyWord(PSFLIVE_EXPOSURE_STR).value(dataExposureTime_us);
  dataExposureTimeComment = ptrFITS->pHDU().keyWord(PSFLIVE_EXPOSURE_STR).comment();
  setExposureTime_us(dataExposureTime_us, dataExposureTimeComment);
  psflive::function::LogMessage("CaptureData","ReadFromFile()", PSFLIVE_GREEN PSFLIVE_EXPOSURE_STR " : "+std::to_string(getExposureTime_us())+" us ("+getExposureTimeComment()+")" PSFLIVE_RESET);

  float dataWavelength_nm;
  std::string dataWavelengthComment;
  ptrFITS->pHDU().keyWord(PSFLIVE_WAVLENTH_STR).value(dataWavelength_nm);
  dataWavelengthComment = ptrFITS->pHDU().keyWord(PSFLIVE_WAVLENTH_STR).comment();
  setWavelength_nm(dataWavelength_nm, dataWavelengthComment);
  psflive::function::LogMessage("CaptureData","ReadFromFile()", PSFLIVE_GREEN PSFLIVE_WAVLENTH_STR " : "+std::to_string(getWavelength_nm())+" nm ("+getWavelengthComment()+")" PSFLIVE_RESET);

  int dataGain;
  std::string dataGainComment;
  ptrFITS->pHDU().keyWord(PSFLIVE_GAIN_STR).value(dataGain);
  dataGainComment = ptrFITS->pHDU().keyWord(PSFLIVE_GAIN_STR).comment();
  setGain(dataGain, dataGainComment);
  psflive::function::LogMessage("CaptureData","ReadFromFile()", PSFLIVE_GREEN PSFLIVE_GAIN_STR " : "+std::to_string(getGain())+" ("+getGainComment()+")" PSFLIVE_RESET);

  // ---- read mean frame (primary hdu) -------------------------------------------------------------------------------
  int cols = ptrFITS->pHDU().axis(0), rows = ptrFITS->pHDU().axis(1);
  int m_meanFrame_bpp = ptrFITS->pHDU().bitpix();
  m_meanFrame = ImageData(rows, cols, psflive::function::fitsBitPixToCVType(m_meanFrame_bpp), cv::Scalar(0));
  m_disp_meanFrame = ImageData(rows, cols, CV_16SC1, cv::Scalar(0));
  
  if (m_meanFrame_bpp==DOUBLE_IMG) {
    std::valarray<double> dataMeanFrame;
    ptrFITS->pHDU().read(dataMeanFrame);
    for (size_t iPx = 0; iPx < m_meanFrame.total(); iPx++)
      m_meanFrame.at<double>(iPx) = dataMeanFrame[iPx];
  } else if (m_meanFrame_bpp==FLOAT_IMG) {
    std::valarray<float> dataMeanFrame;
    ptrFITS->pHDU().read(dataMeanFrame);
    for (size_t iPx = 0; iPx < m_meanFrame.total(); iPx++)
      m_meanFrame.at<float>(iPx) = dataMeanFrame[iPx];
  } else
    psflive::function::LogMessage("MeasureData","ReadFromFile()", PSFLIVE_RED "Failed due to unexpected mean capture data type." PSFLIVE_RESET);
  
  m_meanFrame.convertTo(m_disp_meanFrame, CV_16SC1);
  m_comment = ptrFITS->pHDU().getComments();
  m_comment.pop_back();
  psflive::function::LogMessage("CaptureData","ReadFromFile()", PSFLIVE_GREEN "m_meanImage : "+psflive::function::imageInfo(m_meanFrame)+" ("+m_comment+")" PSFLIVE_RESET);
  psflive::function::LogMessage("CaptureData","ReadFromFile()", PSFLIVE_GREEN "m_disp_meanImage : "+psflive::function::imageInfo(m_disp_meanFrame)+PSFLIVE_RESET);

  // ---- read frames (extension 1) -----------------------------------------------------------------------------------
  int nFrames = ptrFITS->extension(PSFLIVE_CAPTURES_STR).axis(2);
  m_frames = std::vector<ImageData>(nFrames);
  
  for (int iFrame = 0; iFrame < nFrames; iFrame++)
    m_frames[iFrame] = ImageData(rows, cols, psflive::function::fitsBitPixToCVType(ptrFITS->extension(PSFLIVE_CAPTURES_STR).bitpix()), cv::Scalar(0));
  
  std::valarray<uint16_t> dataFrames;
  ptrFITS->extension(PSFLIVE_CAPTURES_STR).read(dataFrames);
  
  for (int iFrame = 0; iFrame < nFrames; iFrame++)
    for (size_t iPx = 0; iPx < m_frames[iFrame].total(); iPx++)
      m_frames[iFrame].at<uint16_t>(iPx) = dataFrames[iPx + iFrame*m_meanFrame.total()];
  
  m_framesComment = ptrFITS->extension(PSFLIVE_CAPTURES_STR).comment();
  psflive::function::LogMessage("CaptureData","ReadFromFile()", PSFLIVE_GREEN "m_images : "+psflive::function::imageInfo(m_frames)+" ("+m_framesComment+")" PSFLIVE_RESET);

  // ---- read mask (extension 2) -------------------------------------------------------------------------------------
  m_mask = ImageData(rows, cols, psflive::function::fitsBitPixToCVType(ptrFITS->extension(PSFLIVE_MASK_STR).bitpix()), cv::Scalar(0));
  std::valarray<uint8_t> dataMask;
  ptrFITS->extension(PSFLIVE_MASK_STR).read(dataMask);
  
  for (size_t iPx = 0; iPx < m_mask.total(); iPx++)
    m_mask.at<uint8_t>(iPx) = dataMask[iPx];
  
  m_maskComment = ptrFITS->extension(PSFLIVE_MASK_STR).comment();
  psflive::function::LogMessage("CaptureData","ReadFromFile()", PSFLIVE_GREEN "m_mask : "+psflive::function::imageInfo(m_mask)+" ("+m_maskComment+")" PSFLIVE_RESET);

  m_show = true;

  return true;
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
bool psflive::CaptureData::WriteToFile(const std::string& _fileName, bool _saveFrames) {

  setFileName(_fileName);

  psflive::function::LogMessage("CaptureData","WriteToFile()", PSFLIVE_GREEN "File "+getFileName()+PSFLIVE_RESET);

  std::shared_ptr<CCfits::FITS> ptrFITS(0);

  long primaryDims[2] = {getColumnCount(), getRowCount()};
  try {
    ptrFITS.reset(new CCfits::FITS(m_fileName, DOUBLE_IMG, 2, primaryDims));
    int tstatus=0;
    while (!tstatus)
      fits_delete_key(ptrFITS->fitsPointer(),"COMMENT",&tstatus); // remove default comment
  } catch (CCfits::FITS::CantCreate) {
    return false;
  }

  // ---- keys (primary hdu) ------------------------------------------------------------------------------------------
  ptrFITS->pHDU().addKey(PSFLIVE_SOURCE_STR, PSFLIVE_STR, "Source application");
  ptrFITS->pHDU().addKey(PSFLIVE_DATATYPE_STR, PSFLIVE_CAPTURE_TYPE_STR, "Data type");
  ptrFITS->pHDU().addKey(PSFLIVE_VERSION_STR, PSFLIVE_VERSION_NUMBER_STR, "Application version");
  ptrFITS->pHDU().addKey(PSFLIVE_DATE_STR, getDate(), "Date created");
  ptrFITS->pHDU().addKey(PSFLIVE_TIME_STR, getTime(), "Time created");
  ptrFITS->pHDU().addKey(PSFLIVE_EXPOSURE_STR, getExposureTime_us(), "Exposure time (us)");
  ptrFITS->pHDU().addKey(PSFLIVE_WAVLENTH_STR, getWavelength_nm(), "Wavelength (nm)");
  ptrFITS->pHDU().addKey(PSFLIVE_GAIN_STR, getGain(), "Gain units");

  // ---- write mean frame (primary hdu) ------------------------------------------------------------------------------
  int meanFrameType = psflive::function::cvTypeToFITSBitPix(m_meanFrame.type());
  if (meanFrameType==FLOAT_IMG)
    ptrFITS->pHDU().write(1L, getPerFramePixelCount(), std::valarray<float>((float*)m_meanFrame.data, getPerFramePixelCount()));
  else if (meanFrameType==DOUBLE_IMG)
    ptrFITS->pHDU().write(1L, getPerFramePixelCount(), std::valarray<double>((double*)m_meanFrame.data, getPerFramePixelCount()));
  else
    psflive::function::LogMessage("CaptureData","WriteToFile()", PSFLIVE_RED "Failed due to unexpected mean capture data type." PSFLIVE_RESET);

  ptrFITS->pHDU().writeComment("Mean image");
  psflive::function::LogMessage("CaptureData","WriteToFile()", PSFLIVE_GREEN "meanImage : "+psflive::function::imageInfo(m_meanFrame)+" (Mean image) as : "+CCfits::typeToString(meanFrameType)+PSFLIVE_RESET);

  // ---- write frames (extension 1) ----------------------------------------------------------------------------------
  if (_saveFrames) {
    int framesType = psflive::function::cvTypeToFITSBitPix(m_frames[0].type());
    std::vector<long> framesDims = {getColumnCount(), getRowCount(), getFrameCount()};
    CCfits::ExtHDU* framesExtension = ptrFITS->addImage(PSFLIVE_CAPTURES_STR, framesType, framesDims);
    for (size_t iFrame = 0; iFrame < getFrameCount(); iFrame++) {
      if (framesType==BYTE_IMG)
        framesExtension->write(iFrame*getPerFramePixelCount()+1L, getPerFramePixelCount(), std::valarray<uint8_t>((uint8_t*)m_frames[iFrame].data, getPerFramePixelCount()));
      else if (framesType==USHORT_IMG)
        framesExtension->write(iFrame*getPerFramePixelCount()+1L, getPerFramePixelCount(), std::valarray<uint16_t>((uint16_t*)m_frames[iFrame].data, getPerFramePixelCount()));
      else if (framesType==FLOAT_IMG)
        framesExtension->write(iFrame*getPerFramePixelCount()+1L, getPerFramePixelCount(), std::valarray<float>((float*)m_frames[iFrame].data, getPerFramePixelCount()));
      else if (framesType==DOUBLE_IMG)
        framesExtension->write(iFrame*getPerFramePixelCount()+1L, getPerFramePixelCount(), std::valarray<double>((double*)m_frames[iFrame].data, getPerFramePixelCount()));
      else
        framesExtension->write(iFrame*getPerFramePixelCount()+1L, getPerFramePixelCount(), std::valarray<uint32_t>((uint32_t*)m_frames[iFrame].data, getPerFramePixelCount()));
    }
    framesExtension->writeComment("Captured images");
    psflive::function::LogMessage("CaptureData","WriteToFile()", PSFLIVE_GREEN "frames : "+std::to_string(getFrameCount())+" frames of "+psflive::function::imageInfo(m_frames[0])+" (Captured images) as "+CCfits::typeToString(framesType)+PSFLIVE_RESET);
  }

  // ---- write mask (extension 2) ------------------------------------------------------------------------------------
  int maskType = psflive::function::cvTypeToFITSBitPix(m_mask.type());
  std::vector<long> maskDims = {getColumnCount(), getRowCount()};
  CCfits::ExtHDU* maskExtension = ptrFITS->addImage(PSFLIVE_MASK_STR, maskType, maskDims);
  assert(maskType==BYTE_IMG);
  maskExtension->write(1L, getPerFramePixelCount(), std::valarray<uint8_t>((uint8_t*)m_mask.data, getPerFramePixelCount()));
  maskExtension->writeComment("Mask");
  psflive::function::LogMessage("CaptureData","WriteToFile()", PSFLIVE_GREEN "mask : "+psflive::function::imageInfo(m_mask)+" (Mask) as "+CCfits::typeToString(maskType)+PSFLIVE_RESET);

  return 0;
}
// ====================================================================================================================
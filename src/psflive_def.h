
#ifndef __PSFLIVE_DEF_H__
#define __PSFLIVE_DEF_H__

#pragma once

#include <opencv2/opencv.hpp>
// ====================================================================================================================










// ====================================================================================================================
#define PSFLIVE_VERSION_NUMBER_STR "0.2.1"

#define PSFLIVE_STR "PSFLIVE"
#define PSFLIVE_TEST_STR "PSFLIVE_TEST"

// 8 letter FITS header field names
#define PSFLIVE_SOURCE_STR "SOURCE"
#define PSFLIVE_DATATYPE_STR "DATATYPE"
#define PSFLIVE_VERSION_STR "VERSION"
#define PSFLIVE_DATE_STR "DATE"
#define PSFLIVE_TIME_STR "TIME"
#define PSFLIVE_WAVLENTH_STR "WAVLENTH"
#define PSFLIVE_EXPOSURE_STR "EXPOSURE"
#define PSFLIVE_GAIN_STR "GAIN"

#define PSFLIVE_COMMENT_STR "COMMENT"
#define PSFLIVE_CAPTURES_STR "CAPTURES"
#define PSFLIVE_MEAN_CAPTURES_STR "MEANCAPS"
#define PSFLIVE_MASK_STR "MASK"
#define PSFLIVE_STEPS_STR "STEPS"

#define PSFLIVE_CAPTURE_STR "Capture"
#define PSFLIVE_CAPTURE_TYPE_STR "capture"
#define PSFLIVE_CAPTURE_PREFIX_STR "capture"

#define PSFLIVE_MEASURE_STR "Measurement"
#define PSFLIVE_MEASURE_TYPE_STR "measure"
#define PSFLIVE_MEASURE_PREFIX_STR "measurement"

#define PSFLIVE_CALIBRATE_STR "Calibration"
#define PSFLIVE_CALIBRATE_TYPE_STR "calibrate"
#define PSFLIVE_CALIBRATE_PREFIX_STR "calibration"

#define PSFLIVE_CONFIGURATION_FILE_STR ".psflive.config"

#define PSFLIVE_DEFAULT_LOCATION "../data/"

#define PSFLIVE_ACQUISITION_TYPE_LABELS {PSFLIVE_CAPTURE_TYPE_STR}

#define PSFLIVE_MEASURE_DEFAULT_STEP_ADU 275 // using 1100 adu as the period from calibration
#define PSFLIVE_DEFAULT_INDEX_FORMAT "%04d"
#define PSFLIVE_DEFAULT_TIMESTAMP_FORMAT "%Y%m%d.%H%M%S"

#define PSFLIVE_DEFAULT_INTERVAL_US 1000
#define PSFLIVE_DEFAULT_NFRAMES 1

#define PSFLIVE_DEFAULT_ADDRESS "tcp://127.0.0.1"
#define PSFLIVE_DEFAULT_PORT 5555

#define PSFLIVE_SHORT_SLEEP_US 1000 // 0.01 s

#define PSFLIVE_CHANNEL PSFLIVE_STR
#define PSFLIVE_OPEN_PARA "{"
#define PSFLIVE_TYPE "TYPE"
#define PSFLIVE_ACTION "ACTION"
#define PSFLIVE_MEASURE "MEASURE"
#define PSFLIVE_CAPTURE "CAPTURE"
#define PSFLIVE_CALIBRATE "CALIBRATE"
#define PSFLIVE_PARAMETER "PARAMETER"
#define PSFLIVE_CLOSE_PARA "}"

#define PSFLIVE_CORE_DEBUG 1

#define PSFLIVE_COMMENT_SIZE 256
#define PSFLIVE_MESSAGE_SIZE 256

// OtherUnwrapper defaults

// BoilerplateUnwrapper defaults


#define PSFLIVE_BLACK "\u001b[30m"
#define PSFLIVE_RED "\u001b[31m"
#define PSFLIVE_GREEN "\u001b[32m"
#define PSFLIVE_YELLOW "\u001b[33m"
#define PSFLIVE_BLUE "\u001b[34m"
#define PSFLIVE_MAGENTA "\u001b[35m"
#define PSFLIVE_CYAN "\u001b[36m"
#define PSFLIVE_WHITE "\u001b[37m"
#define PSFLIVE_RESET "\u001b[0m"
// --------------------------------------------------------------------------------------------------------------------
#define PSFLIVE_MAX(A, B) (((A) >= (B)) ? (A) : (B))
#define PSFLIVE_MIN(A, B) (((A) <= (B)) ? (A) : (B))
// ====================================================================================================================










// ====================================================================================================================
typedef cv::Mat ImageData;
// ====================================================================================================================










// ====================================================================================================================
namespace CCfits {
  // ==================================================================================================================
  const std::string typeToString(int _type);
  // ==================================================================================================================
}
// ====================================================================================================================
#endif //__PSFLIVE_DEF_H__
#ifndef __CAMERA_H__
#define __CAMERA_H__
#pragma once

#include "tcamcamera/tcamcamera.h"

typedef gsttcam::TcamCamera Camera;
typedef gsttcam::CameraInfo CameraInfo;
typedef gsttcam::FrameRate FrameRate;
typedef gsttcam::FrameSize FrameSize;
typedef gsttcam::FrameFormat FrameFormat;

typedef gsttcam::Property Property;
typedef gsttcam::IntegerProperty IntegerProperty;
typedef gsttcam::BooleanProperty BooleanProperty;
typedef gsttcam::ButtonProperty ButtonProperty;
typedef gsttcam::DoubleProperty DoubleProperty;
typedef gsttcam::EnumProperty EnumProperty;
typedef gsttcam::StringProperty StringProperty;

constexpr auto DeviceList = gsttcam::CameraInfoList;

#endif //__CAMERA_H__
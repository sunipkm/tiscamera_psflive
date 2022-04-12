#ifndef __TCAMCAMERA_H__
#define __TCAMCAMERA_H__

#include <vector>
#include <string>
#include <functional>

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/video/videooverlay.h>

#include "tcamprop.h"

#include <mutex>
#include <condition_variable>

namespace gsttcam {

  // ==== forward declarations ========================================================================================
  struct CameraInfo;
  typedef std::string FrameFormat;
  struct FrameRate;
  struct FrameSize;

  class TcamCamera;
  // ==================================================================================================================



















  // ------------------------------------------------------------------------------------------------------------------
  struct CameraInfo {
    std::string serial;
    std::string name;
    std::string identifier;
    std::string connection_type;
    std::string toString() const {
      return "serial: "+serial+", name: "+name+", identifier: "+identifier+", connection_type:"+connection_type;
    }
  };
  inline bool operator==(const CameraInfo& lhs, const CameraInfo& rhs) {
    return ((lhs.serial == rhs.serial) && (lhs.name == rhs.name));
  }
  // ------------------------------------------------------------------------------------------------------------------



















  // ------------------------------------------------------------------------------------------------------------------
  struct FrameRate {
    int numerator = 0;
    int denominator = 0;
    uint toPeriod_us() const {
      return (uint) (1000000 * ((float)denominator/(float)numerator));
    }
    std::string toString() const {
      return std::to_string(denominator) + " / " + std::to_string(numerator);
    }
  };
  inline bool operator==(const FrameRate& lhs, const FrameRate& rhs) {
    return ((lhs.numerator == rhs.numerator) && (lhs.denominator == rhs.denominator));
  }
  // ------------------------------------------------------------------------------------------------------------------



















  // ------------------------------------------------------------------------------------------------------------------
  struct FrameSize {
    int width = 0;
    int height = 0;
    std::string toString() const {
      return std::to_string(width) + " x " + std::to_string(height);
    }
  };
  inline bool operator==(const FrameSize& lhs, const FrameSize& rhs) {
    return ((lhs.height == rhs.height) && (lhs.width == rhs.width));
  }
  // ==================================================================================================================



















  // ==================================================================================================================
  class VideoFormatCaps {
  public:
    FrameSize size;
    FrameSize size_min;
    FrameSize size_max;
    std::vector<FrameFormat> formats;
    std::vector<FrameRate> framerates;
    FrameRate framerate_min;
    FrameRate framerate_max;
    std::string to_string() {
      std::string ret;
      if (size.width != 0)
        ret += "size = " + std::to_string(size.width) + " x " + std::to_string(size.height) + " ";
      else {
        ret += "size range = [" + std::to_string(size_min.width) + " x ";
        ret += std::to_string(size_min.height) + ", ";
        ret += std::to_string(size_max.width) + " x " + std::to_string(size_max.height) + "] ";
      }
      ret += "color formats = {";
      for (FrameFormat &colorfmt : formats) {
        ret += colorfmt + ",";
      }
      ret += "} ";
      if (!framerates.empty()) {
        ret += "frame rates = {";
        for(FrameRate &rate : framerates)
          ret += std::to_string(rate.numerator) + "/" + std::to_string(rate.denominator) + ",";
        ret += "}";
      } else {
        ret += "frame rate range = [";
        ret += std::to_string(framerate_min.numerator) + "/";
        ret += std::to_string(framerate_min.denominator) + ",";
        ret += std::to_string(framerate_max.numerator) + "/";
        ret += std::to_string(framerate_max.denominator) + "]";
      }
      return ret;
    }
  };





  class Property {
  public:
    std::string name;
    std::string category;
    std::string group;
    std::string type;
    virtual std::string to_string() {
      return std::string(type + " property '" + name + "': ");
    }
    // Convenience getters / setters
    virtual bool get(TcamCamera &cam, int &value) {return false;}
    virtual bool get(TcamCamera &cam, double &value) {return false;}
    virtual bool get(TcamCamera &cam, std::string &value) {return false;}
    virtual bool set(TcamCamera &cam, int value)  {return false;}
    virtual bool set(TcamCamera &cam, double value) {return false;}
    virtual bool set(TcamCamera &cam, std::string value) {return false;}
  };





  class IntegerProperty : public Property {
  public:
    int value;
    int default_value;
    int min;
    int max;
    int step_size;
    virtual std::string to_string() override {
      return Property::to_string() + "value = " + std::to_string(value) + " " + "default = " + std::to_string(default_value) + " " + "min = " + std::to_string(min) + " " + "max = " + std::to_string(max) + " " + "step_size = " + std::to_string(step_size);
    }
    virtual bool get(TcamCamera &cam, int &value) override;
    virtual bool set(TcamCamera &cam, int value) override;
  };





  class DoubleProperty : public Property {
  public:
    double value;
    double default_value;
    double min;
    double max;
    double step_size;
    virtual std::string to_string() override {
      return Property::to_string() + "value = " + std::to_string(value) + " " + "default = " + std::to_string(default_value) + " " + "min = " + std::to_string(min) + " " + "max = " + std::to_string(max) + " " + "step_size = " + std::to_string(step_size);
    }
    virtual bool get(TcamCamera &cam, double &value) override;
    virtual bool set(TcamCamera &cam, double value) override;
  };





  class StringProperty : public Property {
  public:
    std::string value;
    std::string default_value;

    virtual std::string to_string() override {
      return Property::to_string() + "value = '" + value + "' default_value = '" + default_value +"'";
    }
    virtual bool get(TcamCamera &cam, std::string &value) override;
    virtual bool set(TcamCamera &cam, std::string value) override;
  };





  class EnumProperty : public StringProperty {
  public:
    std::vector<std::string> values;

    virtual std::string to_string() override {
      std::string ret = StringProperty::to_string() + " values = {";
      for(std::string &value : values)
        ret += value + ", ";
      ret += "}";
      return ret;
    }
  };





  class BooleanProperty : public Property {
  public:
    bool value;
    bool default_value;

    virtual std::string to_string() override {
      return Property::to_string() + "value = " + std::string(value ? "true" : "false") + " default_value = " + std::string(default_value ? "true" : "false");
    };
    virtual bool get(TcamCamera &cam, int &value) override;
    virtual bool set(TcamCamera &cam, int value) override;
  };





  class ButtonProperty : public BooleanProperty {
  public:
    virtual bool set(TcamCamera &cam, int value=true) override;
  private:
    virtual bool get(TcamCamera &cam, int &value) override;
  };



























  // ==================================================================================================================
  static std::vector<CameraInfo> CameraInfoList() {
    if (!gst_is_initialized())
      throw std::runtime_error("GStreamer is not initialized! gst_init(...) needs to be called before using this function.");
    
    GstElement* ptrSource = gst_element_factory_make("tcambin", "source");
    if (!ptrSource)
      throw std::runtime_error("'tcambin' could not be initialized! Check tiscamera installation");

    GSList* serials = tcam_prop_get_device_serials(TCAM_PROP(ptrSource));

    std::vector<CameraInfo> ret;

    for (GSList* elem = serials; elem; elem = elem->next) {
      gchar *serial = reinterpret_cast<gchar *>(elem->data);
      gchar *name = nullptr;
      gchar *identifier = nullptr;
      gchar *connection_type = nullptr;
      if (tcam_prop_get_device_info(TCAM_PROP(ptrSource), serial, &name, &identifier, &connection_type)) {
        CameraInfo info;
        info.serial = serial;
        info.name = name;
        info.identifier = identifier;
        info.connection_type = connection_type;
        ret.push_back(info);
      }
      g_free(name);
      g_free(identifier);
      g_free(connection_type);
    }
    gst_object_unref(ptrSource);
    g_slist_free_full(serials, free);

    return ret;
  }
  // ------------------------------------------------------------------------------------------------------------------







  class TcamCamera {
  public:
    TcamCamera(std::string serial);
    ~TcamCamera();
    
    TcamCamera( const TcamCamera& ) = delete; // disallows copy construction: TcamCamera a = b

    TcamCamera& operator= ( const TcamCamera& ) = delete; // disallows copy assignment: a = b;

    TcamCamera( TcamCamera&& other ) : pipeline_ { other.pipeline_ } { // move construction
      other.pipeline_ = nullptr;
    };

    TcamCamera& operator= ( TcamCamera&& other) { // move assignment
      gst_object_unref( pipeline_ );
      pipeline_ = other.pipeline_;
      other.pipeline_ = nullptr;
      return *this;
    };

    /*
    * Get a list of all video formats supported by the device
    */
    std::vector<VideoFormatCaps> get_format_list();
    /*
    * Get a list of unique frame sizes supported by the device
    */
    std::vector<FrameSize> get_framesize_list();
    /*
    * Get a list of unique color format strings for a given frame size
    */
    std::vector<FrameFormat> get_frameformat_list(FrameSize &framesize);
    /*
    * Get a list of framerates for a given frame size and a color format
    */
    std::vector<FrameRate> get_framerate_list(FrameSize &framesize, FrameFormat &frameformat);
    /*
    * Get a list of all properties supported by the device
    */
    std::vector<std::shared_ptr<Property>> get_camera_property_list();
    /*
    * Get a single camera property
    */
    std::shared_ptr<Property> get_property(std::string name);
    template<typename T> std::shared_ptr<T> get_property( std::string name ) {
      return std::dynamic_pointer_cast<T>( get_property(name) );
    }

    bool set_property(std::string name, GValue &val);
    /*
    * Set the video format for capturing
    */
    void set_capture_format(FrameFormat frameformat, FrameSize framesize, FrameRate framerate);
    /*
    * Set a callback to be called for each new frame
    */
    void set_new_frame_callback(std::function<GstFlowReturn(GstAppSink *appsink, void* data)>callback, void* data);
    /*
    * Start capturing video data
    */
    bool start();
    /*
    * Stop capturing video data
    */
    bool stop();
    /*
    * Get the GStreamer pipeline used for video capture
    */
    GstElement *get_pipeline();
    /*
    * Connect a video display sink element to the capture pipeline
    */
    void enable_video_display(GstElement *displaysink);
    /*
    * Disconnect the video display sink element from the capture pipeline
    */
    void disable_video_display();

  private:
    GstElement *pipeline_ = nullptr;
    GstElement *tcambin_ = nullptr;
    GstElement *capturecapsfilter_ = nullptr;
    GstElement *tee_ = nullptr;
    GstElement *capturesink_ = nullptr;
    GstElement *displaybin_ = nullptr;
    GstElement *displaysink_ = nullptr;
    std::vector<VideoFormatCaps> videocaps_;
    std::function<GstFlowReturn(GstAppSink *appsink, void* data)>callback_;
    void* callback_data_ = nullptr;
    guintptr window_handle_ = 0;

    static GstFlowReturn new_frame_callback(GstAppSink *appsink, void* data);
    void ensure_ready_state();
    void create_pipeline();
    std::vector<VideoFormatCaps> initialize_format_list();
  };
  // ==================================================================================================================

};

#endif//__TCAMCAMERA_H__
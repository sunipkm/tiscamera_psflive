fileName = [/dev/fliusb0]
name = [MicroLine ICX694]
domain = [258]
arrayArea = [(2840,2224),(0,0)]
firmwareRevision = [515]
hardwareRevision = [256]
model = [MicroLine ICX694]
pixelSize = (4.54e-06,4.54e-06)
visibleArea = [(2799,2227),(41,19)]










// LIBFLIAPI FLICancelExposure (flidev t dev)
// LIBFLIAPI FLIClose (flidev t dev)
// LIBFLIAPI FLIGetArrayArea (flidev t dev, long* ul x, long* ul y, long* lr x, long* lr y)
LIBFLIAPI FLIFlushRow (flidev t dev, long rows, long repeat)
// LIBFLIAPI FLIGetFWRevision (flidev t dev, long* fwrev)
// LIBFLIAPI FLIGetHWRevision (flidev t dev, long* hwrev)
// LIBFLIAPI FLIGetLibVersion (char* ver, size t len)
// LIBFLIAPI FLIGetModel (flidev t dev, char* model, size t len)
// LIBFLIAPI FLIGetPixelSize (flidev t dev, double* pixel x, double* pixel y)
// LIBFLIAPI FLIGetVisibleArea (flidev t dev, long* ul x, long* ul y, long* lr x, long* lr y)
// LIBFLIAPI FLIOpen (flidev t* dev, char* name, flidomain t domain)
LIBFLIAPI FLISetDebugLevel (char* host, flidebug t level)
// LIBFLIAPI FLISetExposureTime (flidev t dev, long exptime)
// LIBFLIAPI FLISetHBin (flidev t dev, long hbin)
// LIBFLIAPI FLISetFrameType (flidev t dev, fliframe t frametype)
// LIBFLIAPI FLISetImageArea (flidev t dev, long ul x, long ul y, long lr x, long lr y)
// LIBFLIAPI FLISetVBin (flidev t dev, long vbin)
// LIBFLIAPI FLIGetExposureStatus (flidev t dev, long* timeleft)
// LIBFLIAPI FLISetTemperature (flidev t dev, double temperature)
// LIBFLIAPI FLIGetTemperature (flidev t dev, double* temperature)
// LIBFLIAPI FLIGrabRow (flidev t dev, void* buff, size t width)
// LIBFLIAPI FLIExposeFrame (flidev t dev)
LIBFLIAPI FLISetBitDepth (flidev t dev, flibitdepth t bitdepth)
// LIBFLIAPI FLISetNFlushes (flidev t dev, long nflushes)
LIBFLIAPI FLIReadIOPort (flidev t dev, long* ioportset)
LIBFLIAPI FLIWriteIOPort (flidev t dev, long ioportset)
LIBFLIAPI FLIConfigureIOPort (flidev t dev, long ioportset)
LIBFLIAPI FLILockDevice (flidev t dev)
LIBFLIAPI FLIUnlockDevice (flidev t dev)
LIBFLIAPI FLIControlShutter (flidev t dev, flishutter t shutter)
LIBFLIAPI FLIControlBackgroundFlush (flidev t dev, flibgflush t bgflush)
LIBFLIAPI FLIList (flidomain t domain, char*** names)
LIBFLIAPI FLIFreeList (char** names)
LIBFLIAPI FLISetFilterPos (flidev t dev, long filter)
LIBFLIAPI FLIGetFilterPos (flidev t dev, long* filter)
LIBFLIAPI FLIGetStepsRemaining (flidev t dev, long* steps)
LIBFLIAPI FLIGetFilterCount (flidev t dev, long* filter)
LIBFLIAPI FLIStepMotorAsync (flidev t dev, long steps)
LIBFLIAPI FLIStepMotor (flidev t dev, long steps)
LIBFLIAPI FLIGetStepperPosition (flidev t dev, long* position)
LIBFLIAPI FLIHomeFocuser (flidev t dev)
LIBFLIAPI FLIGetFocuserExtent (flidev t dev, long* extent)
LIBFLIAPI FLIReadTemperature (flidev t dev, flichannel t channel, double* temperature)
// LIBFLIAPI FLICreateList (flidomain t domain)
// LIBFLIAPI FLIDeleteList (void)
// LIBFLIAPI FLIListFirst (flidomain t* domain, char* filename, size t fnlen, char* name, size t namelen)
LIBFLIAPI FLIListNext (flidomain t* domain, char* filename, size t fnlen, char* name, size t namelen)







// LIBFLIAPI FLIOpen(flidev_t *dev, char *name, flidomain_t domain);
LIBFLIAPI FLISetDebugLevel(char *host, flidebug_t level);
// LIBFLIAPI FLIClose(flidev_t dev);
LIBFLIAPI FLIGetLibVersion(char* ver, size_t len);
// LIBFLIAPI FLIGetModel(flidev_t dev, char* model, size_t len);
// LIBFLIAPI FLIGetPixelSize(flidev_t dev, double *pixel_x, double *pixel_y);
// LIBFLIAPI FLIGetHWRevision(flidev_t dev, long *hwrev);
// LIBFLIAPI FLIGetFWRevision(flidev_t dev, long *fwrev);
// LIBFLIAPI FLIGetArrayArea(flidev_t dev, long* ul_x, long* ul_y, long* lr_x, long* lr_y);
// LIBFLIAPI FLIGetVisibleArea(flidev_t dev, long* ul_x, long* ul_y, long* lr_x, long* lr_y);
// LIBFLIAPI FLISetExposureTime(flidev_t dev, long exptime);
// LIBFLIAPI FLISetImageArea(flidev_t dev, long ul_x, long ul_y, long lr_x, long lr_y);
// LIBFLIAPI FLISetHBin(flidev_t dev, long hbin);
// LIBFLIAPI FLISetVBin(flidev_t dev, long vbin);
// LIBFLIAPI FLISetFrameType(flidev_t dev, fliframe_t frametype);
// LIBFLIAPI FLICancelExposure(flidev_t dev);
// LIBFLIAPI FLIGetExposureStatus(flidev_t dev, long *timeleft);
// LIBFLIAPI FLISetTemperature(flidev_t dev, double temperature);
// LIBFLIAPI FLIGetTemperature(flidev_t dev, double *temperature);
            LIBFLIAPI FLIGetCoolerPower(flidev_t dev, double *power);
// LIBFLIAPI FLIGrabRow(flidev_t dev, void *buff, size_t width);
// LIBFLIAPI FLIExposeFrame(flidev_t dev);
LIBFLIAPI FLIFlushRow(flidev_t dev, long rows, long repeat);
// LIBFLIAPI FLISetNFlushes(flidev_t dev, long nflushes);
// LIBFLIAPI FLISetBitDepth(flidev_t dev, flibitdepth_t bitdepth);
LIBFLIAPI FLIReadIOPort(flidev_t dev, long *ioportset);
LIBFLIAPI FLIWriteIOPort(flidev_t dev, long ioportset);
LIBFLIAPI FLIConfigureIOPort(flidev_t dev, long ioportset);
LIBFLIAPI FLILockDevice(flidev_t dev);
LIBFLIAPI FLIUnlockDevice(flidev_t dev);
LIBFLIAPI FLIControlShutter(flidev_t dev, flishutter_t shutter);
LIBFLIAPI FLIControlBackgroundFlush(flidev_t dev, flibgflush_t bgflush);
            LIBFLIAPI FLISetDAC(flidev_t dev, unsigned long dacset);
LIBFLIAPI FLIList(flidomain_t domain, char ***names);
LIBFLIAPI FLIFreeList(char **names);
            LIBFLIAPI FLIGetFilterName(flidev_t dev, long filter, char *name, size_t len);
            LIBFLIAPI FLISetActiveWheel(flidev_t dev, long wheel);
            LIBFLIAPI FLIGetActiveWheel(flidev_t dev, long *wheel);
LIBFLIAPI FLISetFilterPos(flidev_t dev, long filter);
LIBFLIAPI FLIGetFilterPos(flidev_t dev, long *filter);
LIBFLIAPI FLIGetFilterCount(flidev_t dev, long *filter);
LIBFLIAPI FLIStepMotor(flidev_t dev, long steps);
LIBFLIAPI FLIStepMotorAsync(flidev_t dev, long steps);
LIBFLIAPI FLIGetStepperPosition(flidev_t dev, long *position);
LIBFLIAPI FLIGetStepsRemaining(flidev_t dev, long *steps);
LIBFLIAPI FLIHomeFocuser(flidev_t dev);
LIBFLIAPI FLICreateList(flidomain_t domain);
LIBFLIAPI FLIDeleteList(void);
LIBFLIAPI FLIListFirst(flidomain_t *domain, char *filename, size_t fnlen, char *name, size_t namelen);
LIBFLIAPI FLIListNext(flidomain_t *domain, char *filename, size_t fnlen, char *name, size_t namelen);
LIBFLIAPI FLIReadTemperature(flidev_t dev, flichannel_t channel, double *temperature);
LIBFLIAPI FLIGetFocuserExtent(flidev_t dev, long *extent);
            LIBFLIAPI FLIUsbBulkIO(flidev_t dev, int ep, void *buf, long *len);
            LIBFLIAPI FLIGetDeviceStatus(flidev_t dev, long *status);
            LIBFLIAPI FLIGetCameraModeString(flidev_t dev, flimode_t mode_index, char *mode_string, size_t siz);
            LIBFLIAPI FLIGetCameraMode(flidev_t dev, flimode_t *mode_index);
            LIBFLIAPI FLISetCameraMode(flidev_t dev, flimode_t mode_index);
            LIBFLIAPI FLIHomeDevice(flidev_t dev);
            LIBFLIAPI FLIGrabFrame(flidev_t dev, void* buff, size_t buffsize, size_t* bytesgrabbed);
            LIBFLIAPI FLISetTDI(flidev_t dev, flitdirate_t tdi_rate, flitdiflags_t flags);
            LIBFLIAPI FLIGrabVideoFrame(flidev_t dev, void *buff, size_t size);
            LIBFLIAPI FLIStopVideoMode(flidev_t dev);
            LIBFLIAPI FLIStartVideoMode(flidev_t dev);
            LIBFLIAPI FLIGetSerialString(flidev_t dev, char* serial, size_t len);
            LIBFLIAPI FLIEndExposure(flidev_t dev);
            LIBFLIAPI FLITriggerExposure(flidev_t dev);
            LIBFLIAPI FLISetFanSpeed(flidev_t dev, long fan_speed);
            LIBFLIAPI FLISetVerticalTableEntry(flidev_t dev, long index, long height, long bin, long mode);
            LIBFLIAPI FLIGetVerticalTableEntry(flidev_t dev, long index, long *height, long *bin, long *mode);
            LIBFLIAPI FLIGetReadoutDimensions(flidev_t dev, long *width, long *hoffset, long *hbin, long *height, long *voffset, long *vbin);
            LIBFLIAPI FLIEnableVerticalTable(flidev_t dev, long width, long offset, long flags);
            LIBFLIAPI FLIReadUserEEPROM(flidev_t dev, long loc, long address, long length, void *rbuf);
            LIBFLIAPI FLIWriteUserEEPROM(flidev_t dev, long loc, long address, long length, void *wbuf);
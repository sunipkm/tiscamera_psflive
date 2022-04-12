#include <regex>
#include <unistd.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/addons/file_browser/imgui_file_browser.h"
#include "imgui/addons/implot/implot.h"

#include "psflive.h"
#include "psflive_Capture.h"
#include "psflive_ui.h"
// ====================================================================================================================




















// ~~~~ HelpMarker ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::HelpMarker(const std::string message) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(message.c_str());
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}
// ~~~~ HelpMarker ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



















// ~~~~ MainMenuBar ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::MainMenuBar(Data& _uiData, AppData& _appData) {
  { // ++++ draw main menu ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    if (ImGui::BeginMainMenuBar()) {

      if (ImGui::BeginMenu("File")) {

        if (ImGui::BeginMenu("New"/*,"Ctrl+N"*/)) {
          if (ImGui::MenuItem("Capture", "", false, _appData.ptrImager->isAvailable()))
            _uiData.showCaptureOperationWindow = true;
          ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Test"))
          _uiData.showTestWindow = true;

        ImGui::Separator(); // ========================================================================================

        if (ImGui::MenuItem("Open"/*, "Ctrl+O"*/))
          _uiData.showOpenFileDialog = true;
        
        if (ImGui::MenuItem("Save"/*, "Ctrl+S"*/))
          _uiData.showSaveFileDialog = true;

        ImGui::Separator(); // ========================================================================================

        if (ImGui::BeginMenu("Options"/*,"Ctrl+N"*/)) {
          if (ImGui::MenuItem("Device Options", "", false, _appData.ptrImager->isAvailable()))
            _uiData.showDeviceOptionsWindow = true;
          if (ImGui::MenuItem("Acquisition Options", "", false))
            _uiData.showAcquisitionOptionsWindow = true;
          if (ImGui::MenuItem("Other Options", "", false))
            _uiData.showOtherOptionsWindow = true;
          ImGui::EndMenu();
        }

        ImGui::Separator(); // ========================================================================================
        
        if (ImGui::MenuItem("Quit"/*, "Ctrl+Q"*/)) {}

        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }

    psflive::ui::OpenFileDialog(_uiData);
    psflive::ui::SaveFileDialog(_uiData);
  } // ++++ draw main menu ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}
// ~~~~ MainMenuBar ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




















// ~~~~ OpenFileDialog ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::OpenFileDialog(Data& _uiData) {

  static imgui_addons::ImGuiFileBrowser fileBrowser;
  static bool fileValid = true;
  std::string source, version;
  psflive::AcquireType acquireType;
  
  if (_uiData.showOpenFileDialog)
    ImGui::OpenPopup("Open File");
  
  if (fileBrowser.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".fits")) {

    fileValid = psflive::function::validateFile(fileBrowser.selected_path, source, version, acquireType);

    if (fileValid) {
      if (acquireType == psflive::AcquireType::capture) { // ---- capture -----------------------------------------------
        std::shared_ptr<psflive::CaptureData> ptrCaptureData(new psflive::CaptureData(fileBrowser.selected_path));
        bool found = false;
        for(std::shared_ptr<psflive::AcquireData>& ptrDocument: _uiData.ptrDocuments) {
          if(ptrDocument->getFileName() == ptrCaptureData->getFileName()) {
            found = true;
            ptrDocument->m_show = true;
            psflive::function::LogMessage("ui","OpenFileDialog()","capture already in documents, not appending...");
            break;
          }
        }
        if(!found) {
          psflive::function::LogMessage("ui","OpenFileDialog()","capture not found in documents, appending...");
          ptrCaptureData->m_show = true;
          _uiData.ptrDocuments.push_back(ptrCaptureData);
        }
      }
    }
  } else
    _uiData.showOpenFileDialog = false;

  if (!fileValid) {
    ImGui::OpenPopup("Invalid file");
    if (ImGui::BeginPopupModal("Invalid file", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
      ImGui::Text("File: %s\ndoes not appear to be a valid PSFLIVE file", fileBrowser.selected_path.c_str());
      if (ImGui::TreeNode("More info:")) {
        ImGui::Text("Source: %s", source.empty()?"Missing":source.c_str());
        ImGui::Text("Version: %s", version.empty()?"Missing":version.c_str());
        ImGui::Text("Type: %s", psflive::function::acquireDataTypeToString(acquireType));
        ImGui::TreePop();
      }
      if (ImGui::Button("Close")) {
        fileValid = true;
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }
  }

}
// ~~~~ OpenFileDialog ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~










// ~~~~ SaveFileDialog ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::SaveFileDialog(Data& _uiData) {

  static imgui_addons::ImGuiFileBrowser fileBrowser;

  if (_uiData.showSaveFileDialog)
    ImGui::OpenPopup("Save File");

  if (fileBrowser.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".fits")) {
    // save file
    // std::cout << fileBrowser.selected_path << std::endl;
  } else
    _uiData.showSaveFileDialog = false;

}
// ~~~~ SaveFileDialog ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




















// ~~~~ DeviceOptionsWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::DeviceOptionsWindow(Data& _uiData, AppData& _appData) {  
  static bool initialized = 0;

  if (_uiData.showDeviceOptionsWindow) {
    // ---- show DeviceOptionsWindow ----------------------------------------------------------------------------------

    if (!initialized) { // ---- initialize DeviceOptionsWindow --------------------------------------------------------
      psflive::function::LogMessage("ui","DeviceOptionsWindow()","Initializing options window.");
      initialized = 1;
      psflive::function::LogMessage("ui","DeviceOptionsWindow()","Options window initialized.");
    }
    
    // ---- initialize DeviceOptionsWindow ----------------------------------------------------------------------------
    if (ImGui::Begin("Device Options", &_uiData.showDeviceOptionsWindow)) { // ++++ draw DeviceOptionsWindow +++++++++++++++++
      if (ImGui::BeginTabBar("DeviceOptionsTabBar", ImGuiTabBarFlags_None)) {

        if (_appData.ptrImager->isAvailable()) {
          if (ImGui::BeginTabItem("Camera")) { // ---- camera options tab ---------------------------------------------
            psflive::ui::ImagerOptionsGroup(_uiData, _appData);
            ImGui::EndTabItem(); // ---- camera options tab -----------------------------------------------------------
          }
        }

        ImGui::EndTabBar();
      }
    }
    ImGui::End(); // ++++ draw DeviceOptionsWindow ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  } // ____ show window _______________________________________________________________________________________________

  if (initialized & !_uiData.showDeviceOptionsWindow) { // ---- un-initialize DeviceOptionsWindow ---------------------
    psflive::function::LogMessage("ui","DeviceOptionsWindow()","Un-initializing options window.");
    initialized = 0;
    psflive::function::LogMessage("ui","DeviceOptionsWindow()","Options window un-initialized.");
  } // ---- un-initialize DeviceOptionsWindow -------------------------------------------------------------------------
}
// ~~~~ DeviceOptionsWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




















// ~~~~ AcquisitionOptionsWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::AcquisitionOptionsWindow(Data& _uiData, AppData& _appData) {  
  static bool initialized = 0;

  if (_uiData.showAcquisitionOptionsWindow) {
    // ---- show AcquisitionOptionsWindow -----------------------------------------------------------------------------

    if (!initialized) { // ---- initialize AcquisitionOptionsWindow ---------------------------------------------------
      psflive::function::LogMessage("ui","AcquisitionOptionsWindow()","Initializing options window.");
      initialized = 1;
      psflive::function::LogMessage("ui","AcquisitionOptionsWindow()","Options window initialized.");
    }
    
    // ---- initialize AcquisitionOptionsWindow -----------------------------------------------------------------------
    if (ImGui::Begin("Acquisition Options", &_uiData.showAcquisitionOptionsWindow)) { // ++++ draw AcquisitionOptionsWindow +++++++
      if (ImGui::BeginTabBar("AcquisitionOptionsTabBar", ImGuiTabBarFlags_None)) {

        if (ImGui::BeginTabItem("Capture")) { // ---- capture options tab ---------------------------------------------
          psflive::ui::CaptureOptionsGroup(_uiData, _appData);
          ImGui::EndTabItem(); // ---- capture options tab ------------------------------------------------------------
        }

        ImGui::EndTabBar();
      }
    }
    ImGui::End(); // ++++ draw AcquisitionOptionsWindow +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  } // ____ show window _______________________________________________________________________________________________

  if (initialized & !_uiData.showAcquisitionOptionsWindow) { // ---- un-initialize AcquisitionOptionsWindow -----------
    psflive::function::LogMessage("ui","AcquisitionOptionsWindow()","Un-initializing options window.");
    initialized = 0;
    psflive::function::LogMessage("ui","AcquisitionOptionsWindow()","Options window un-initialized.");
  } // ---- un-initialize AcquisitionOptionsWindow --------------------------------------------------------------------
}
// ~~~~ AcquisitionOptionsWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




















// ~~~~ OtherOptionsWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::OtherOptionsWindow(Data& _uiData, AppData& _appData) {  
  static bool initialized = 0;

  if (_uiData.showOtherOptionsWindow) {
    // ---- show OtherOptionsWindow -----------------------------------------------------------------------------------

    if (!initialized) { // ---- initialize OtherOptionsWindow ---------------------------------------------------------
      psflive::function::LogMessage("ui","OtherOptionsWindow()","Initializing options window.");
      initialized = 1;
      psflive::function::LogMessage("ui","OtherOptionsWindow()","Options window initialized.");
    }
    
    // ---- initialize OtherOptionsWindow -----------------------------------------------------------------------------
    if (ImGui::Begin("Other Options", &_uiData.showOtherOptionsWindow)) { // ++++ draw OtherOptionsWindow +++++++++++++++++++
      if (ImGui::BeginTabBar("OtherOptionsTabBar", ImGuiTabBarFlags_None)) {

        if (ImGui::BeginTabItem("Application")) { // ---- application options tab -------------------------------------
          psflive::ui::ApplicationOptionsGroup(_uiData, _appData);
          ImGui::EndTabItem(); // ---- application options tab --------------------------------------------------------
        }

        if (ImGui::BeginTabItem("Connection")) { // ---- connection options tab ---------------------------------------
          psflive::ui::ConnectionOptionsGroup(_uiData, _appData);
          ImGui::EndTabItem(); // ---- connection options tab ---------------------------------------------------------
        }

        if (ImGui::BeginTabItem("Test")) { // ---- test options tab ---------------------------------------------------
          psflive::ui::TestOptionsGroup(_uiData, _appData);
          ImGui::EndTabItem(); // ---- test options tab ---------------------------------------------------------------
        }

        ImGui::EndTabBar();
      }
    }
    ImGui::End(); // ++++ draw OtherOptionsWindow +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  } // ____ show window _______________________________________________________________________________________________

  if (initialized & !_uiData.showOtherOptionsWindow) { // ---- un-initialize OtherOptionsWindow -----------------------
    psflive::function::LogMessage("ui","OtherOptionsWindow()","Un-initializing options window.");
    initialized = 0;
    psflive::function::LogMessage("ui","OtherOptionsWindow()","Options window un-initialized.");
  } // ---- un-initialize OtherOptionsWindow --------------------------------------------------------------------------
}
// ~~~~ OtherOptionsWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~










// ~~~~ CaptureOptionsGroup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::CaptureOptionsGroup(Data& _uiData, AppData& _appData) {
  static bool initialized = 0;
  static ImU32 frameInterval_us = PSFLIVE_DEFAULT_INTERVAL_US; static const ImU32 frameInterval_us_SliderStep = 100, frameInterval_us_SliderFast = 1000;
  static ImU16 nFrames = PSFLIVE_DEFAULT_NFRAMES; static const ImU16 nFramesSliderStep = 1, nFramesSliderFast = 2;
  static bool saveOnClick = false, saveFrames = false;
  static int suffixOption = 0;
  static char locationBuffer[64], prefixBuffer[64] = PSFLIVE_CAPTURE_PREFIX_STR "_", suffixBuffer[64], suffixTimeFormatBuffer[64] = PSFLIVE_DEFAULT_TIMESTAMP_FORMAT, suffixIndexFormatBuffer[64] = PSFLIVE_DEFAULT_INDEX_FORMAT;
  static std::string location, prefix = prefixBuffer, suffix, suffixTimeFormat = suffixTimeFormatBuffer, suffixIndexFormat = suffixIndexFormatBuffer;
  static char fileNameBuffer[4*64];
  static uint tempCurrentIndex;
  static std::time_t tempCurrentTime;
  static float wavelength_nm;
  
  // _uiData.showAcquisitionOptionsWindow = true;

  if (_uiData.showAcquisitionOptionsWindow) {
    // ---- capture configuration data --------------------------------------------------------------------------------
    location = psflive::AcquireOperation::GETLOCATION();
    strcpy(locationBuffer, location.c_str());

    if (!initialized) {
      psflive::function::LogMessage("ui","CaptureOptionsGroup()","Initializing capture options group.");
      // ---- get settings from the config ----------------------------------------------------------------------------
      _appData.ptrConfiguration->getNumber("Capture Options","nFrames",nFrames);
      _appData.ptrConfiguration->getNumber("Capture Options","frameInterval_us",frameInterval_us);

      _appData.ptrConfiguration->getBoolean("Capture Options","saveFrames",saveFrames);
      _appData.ptrConfiguration->getBoolean("Capture Options","saveOnClick",saveOnClick);

      _appData.ptrConfiguration->getNumber("Capture Options","wavelength_nm",wavelength_nm);

      if (_appData.ptrConfiguration->getString("Capture Options","prefix",prefix))
        strcpy(prefixBuffer, prefix.c_str());

      if (_appData.ptrConfiguration->getString("Application Options","location",location)) {
        strcpy(locationBuffer, location.c_str());
        psflive::AcquireOperation::SETLOCATION(location);
      }

      _appData.ptrConfiguration->getNumber("Capture Options","suffixOption",suffixOption);
      
      if (_appData.ptrConfiguration->getString("Capture Options","suffix",suffix))
        strcpy(suffixBuffer, suffix.c_str());

      suffixTimeFormat = suffixTimeFormatBuffer;
      suffixTimeFormat = std::regex_replace(suffixTimeFormat, std::regex("%%%%"), "%");

      suffixIndexFormat = suffixIndexFormatBuffer;
      suffixIndexFormat = std::regex_replace(suffixIndexFormat, std::regex("%%%%"), "%");
      // ---- get settings from the config ----------------------------------------------------------------------------
      initialized = 1;
      psflive::function::LogMessage("ui","CaptureOptionsGroup()","Capture options group initialized.");
    }

    { // ++++ draw ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Number of exposures to average");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(150);
      if (ImGui::InputScalar("##nFrames##CaptureOptionsGroup()", ImGuiDataType_U16, &nFrames, &nFramesSliderStep, &nFramesSliderFast, "%u"))
        if (!nFrames)
          nFrames = 1;

      ImGui::AlignTextToFramePadding();
      ImGui::Text("Interval                      ");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(150);
      ImGui::InputScalar("##interval_us##CaptureOptionsGroup()", ImGuiDataType_U32, &frameInterval_us, &frameInterval_us_SliderStep, &frameInterval_us_SliderFast, "%u us");

      ImGui::AlignTextToFramePadding();
      ImGui::Text("Save captured exposure frames ");
      ImGui::SameLine();
      ImGui::Checkbox("##saveFrames##CaptureOptionsGroup()", &saveFrames);

      ImGui::AlignTextToFramePadding();
      ImGui::Text("Auto save on click            ");
      ImGui::SameLine();
      ImGui::Checkbox("##saveOnClick##CaptureOptionsGroup()", &saveOnClick);
      if (saveOnClick) {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("File name prefix              ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150);
        if (ImGui::InputText("##prefixBuffer##CaptureOptionsGroup()", prefixBuffer, IM_ARRAYSIZE(prefixBuffer))) {
          prefix = prefixBuffer;
        }

        ImGui::AlignTextToFramePadding();
        ImGui::Text("File name suffix              ");
        ImGui::SameLine();
        ImGui::RadioButton("timestamp ##suffixOption##CaptureOptionsGroup()", &suffixOption, 0);
        if (suffixOption == 0) { // time stamp
          ImGui::SameLine();
          ImGui::SetNextItemWidth(150);
          if (ImGui::InputText("##suffixTimeFormatBuffer##CaptureOptionsGroup()", suffixTimeFormatBuffer, IM_ARRAYSIZE(suffixTimeFormatBuffer))) {
            suffixTimeFormat = suffixTimeFormatBuffer;
          }
          tempCurrentTime = std::time(nullptr);
          suffixTimeFormat = std::regex_replace(suffixTimeFormat, std::regex("%%%%"), "%");
          std::strftime(suffixBuffer, sizeof(suffixBuffer), suffixTimeFormat.c_str(), std::localtime(&tempCurrentTime));

          ImGui::SameLine();
          psflive::ui::HelpMarker("use strftime style format specifiers");
        }

        ImGui::Text("                              ");
        ImGui::SameLine();
        if (ImGui::RadioButton("index     ##suffixOption##CaptureOptionsGroup()", &suffixOption, 1)) {
          tempCurrentIndex = (uint)psflive::function::rangedRand(0,999);
        }
        if (suffixOption == 1) { // index stamp
          ImGui::SameLine();
          ImGui::SetNextItemWidth(150);
          if (ImGui::InputText("##suffixIndexFormatBuffer##CaptureOptionsGroup()", suffixIndexFormatBuffer, IM_ARRAYSIZE(suffixIndexFormatBuffer))) {
            suffixIndexFormat = suffixIndexFormatBuffer;
          }
          suffixIndexFormat = std::regex_replace(suffixIndexFormat, std::regex("%%%%"), "%");
          sprintf(suffixBuffer, suffixIndexFormat.c_str(), tempCurrentIndex);

          ImGui::SameLine();
          psflive::ui::HelpMarker("use c style format specifiers");

          ImGui::SameLine();
          if (ImGui::Button("Reset")) {
            psflive::AcquireOperation::RESETCURRENTINDEX();
          }
        }

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Example file name             ");
        ImGui::SameLine();
        sprintf(fileNameBuffer,"%s%s%s.fits", locationBuffer, prefixBuffer, suffixBuffer);
        ImGui::TextDisabled(fileNameBuffer);
      }

      ImGui::AlignTextToFramePadding();
      ImGui::Text("Wavelength                    ");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(150);
      ImGui::DragFloat("##wavelength_nm##CaptureOptionsGroup()", &wavelength_nm, 0.01f, 0.0f, 0.0f, "%.02f nm");

      ImGui::Separator(); // ==========================================================================================

      if (ImGui::Button("APPLY##CaptureOptionsGroup()", ImVec2(200,40))) {
        _appData.ptrConfiguration->putNumber("Capture Options","nFrames",nFrames);
        _appData.ptrConfiguration->putNumber("Capture Options","frameInterval_us",frameInterval_us);
        _appData.ptrConfiguration->putBoolean("Capture Options","saveFrames",saveFrames);
        _appData.ptrConfiguration->putBoolean("Capture Options","saveOnClick",saveOnClick);
        _appData.ptrConfiguration->putString("Capture Options","prefix",prefix);
        _appData.ptrConfiguration->putNumber("Capture Options","suffixOption",suffixOption);
        _appData.ptrConfiguration->putString("Capture Options","suffix",(suffixOption?suffixIndexFormat:suffixTimeFormat));
        _appData.ptrConfiguration->putNumber("Capture Options","wavelength_nm",wavelength_nm);
        _appData.ptrConfiguration->Write();
      }
    } // ++++ draw ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ---- un-initialize CaptureOptions ------------------------------------------------------------------------------
    if (initialized & !_uiData.showAcquisitionOptionsWindow) {
      psflive::function::LogMessage("ui","CaptureOptionsGroup()","Un-initializing capture options group.");
      initialized = 0;
      psflive::function::LogMessage("ui","CaptureOptionsGroup()","Capture options group un-initialized.");
    }
    // ---- un-initialize CaptureOptions ------------------------------------------------------------------------------
  }
}
// ~~~~ CaptureOptionsGroup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~










// ~~~~ ImagerOptionsGroup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::ImagerOptionsGroup(Data& _uiData, AppData& _appData) {
  static bool initialized = 0;

  static cv::Point minPxIdx, maxPxIdx;
  static double minPxVal = 0, maxPxVal = 0;

  static ImageData histMaskImage, histDataImage;

  static Shader* ptrShader;
  static ImageData* ptrMaskImage;
  
  static CameraInfo cameraInfoSelection;
  static FrameSize frameSizeSelection;
  static FrameFormat frameFormatSelection;
  static FrameRate frameRateSelection;
  
  static std::shared_ptr<BooleanProperty> ptrExpAutoBoolProp;
  static std::shared_ptr<IntegerProperty> ptrExpTimeIntProp;
  int expTimeMax_us;

  static std::shared_ptr<IntegerProperty> ptrGainIntProp;
  static std::shared_ptr<BooleanProperty> ptrGainAutoBoolProp;

  static bool roiEnableX, roiEnableY, maskEnable, updateMask;
  static ImRect imROI;
  static cv::Rect cvROI;

  // ---- show ImagerOptionsGroup -------------------------------------------------------------------------------------
  if (_uiData.showDeviceOptionsWindow) {
    static GLuint imageTexture, maskTexture, compositeTexture, renderBuffers, frameBuffers;

    static bool comboSelect = false;

    static ImVec2 mapSize, viewSize;
    static std::vector<ImGuiCanvasShape> lineShapes;

    // ---- initialize ImagerOptionsGroup -----------------------------------------------------------------------------
    if (!initialized) {
      psflive::function::LogMessage("ui","ImagerOptionsGroup()","Initializing imager options group.");
      
      // ---- setup shader --------------------------------------------------------------------------------------------
      ptrShader = new Shader(Shader::ReadFile(BASE_VERTEX_SHADER_FILE_STR), Shader::ReadFile(MASK_FRAGMENT_SHADER_FILE_STR)); // CHECKED: delete ptrShader;
      ptrShader->Initialize();
      ptrShader->BindFragDataLocation();
      ptrShader->Use();
      ptrShader->CreateTexture();

      // ---- setup Imager --------------------------------------------------------------------------------------------
      if (_appData.ptrImager != nullptr)
        delete _appData.ptrImager;

      // ---- load settings from config -------------------------------------------------------------------------------
      _appData.ptrConfiguration->pickCameraInfo(psflive::device::Imager::CAMERAINFOS, cameraInfoSelection);
      psflive::function::LogMessage("ui","ImagerOptionsGroup()","cameraInfoSelection = "+cameraInfoSelection.toString());
      try {
        _appData.ptrImager = new psflive::device::Imager(cameraInfoSelection);
      } catch(const std::exception& _exception) {
        psflive::function::LogMessage("ui","ImagerOptionsGroup()","Exception "+std::string(_exception.what()));
      }

      _appData.ptrConfiguration->pickFrameSize(_appData.ptrImager->getFrameSizes(), frameSizeSelection);
      psflive::function::LogMessage("ui","ImagerOptionsGroup()","frameSizeSelection = "+frameSizeSelection.toString());
      try {
        _appData.ptrImager->Update(frameSizeSelection);
      } catch(const std::exception& _exception) {
        psflive::function::LogMessage("ui","ImagerOptionsGroup()","Exception "+std::string(_exception.what()));
      }

      _appData.ptrConfiguration->pickFrameFormat(_appData.ptrImager->getFrameFormats(), frameFormatSelection);
      psflive::function::LogMessage("ui","ImagerOptionsGroup()","frameFormatSelection = "+frameFormatSelection);
      try {
        _appData.ptrImager->Update(frameFormatSelection);
      } catch(const std::exception& _exception) {
        psflive::function::LogMessage("ui","ImagerOptionsGroup()","Exception "+std::string(_exception.what()));
      }

      _appData.ptrConfiguration->pickFrameRate(_appData.ptrImager->getFrameRates(), frameRateSelection);
      psflive::function::LogMessage("ui","ImagerOptionsGroup()","frameRateSelection = "+frameRateSelection.toString());
      try {
        _appData.ptrImager->Update(frameRateSelection);
      } catch(const std::exception& _exception) {
        psflive::function::LogMessage("ui","ImagerOptionsGroup()","Exception "+std::string(_exception.what()));
      }

      _appData.ptrImager->Start();

      ptrExpTimeIntProp = std::dynamic_pointer_cast<IntegerProperty>(_appData.ptrImager->getProperty("Exposure Time (us)"));
      ptrExpAutoBoolProp = std::dynamic_pointer_cast<BooleanProperty>(_appData.ptrImager->getProperty("Exposure Auto"));
      _appData.ptrConfiguration->getExposureSetting(ptrExpAutoBoolProp->value, ptrExpTimeIntProp->value);
      _appData.ptrImager->setProperty(ptrExpAutoBoolProp);
      _appData.ptrImager->setProperty(ptrExpTimeIntProp);

      ptrGainIntProp = std::dynamic_pointer_cast<IntegerProperty>(_appData.ptrImager->getProperty("Gain"));
      ptrGainAutoBoolProp = std::dynamic_pointer_cast<BooleanProperty>(_appData.ptrImager->getProperty("Gain Auto"));
      _appData.ptrConfiguration->getGainSetting(ptrGainAutoBoolProp->value, ptrGainIntProp->value);
      _appData.ptrImager->setProperty(ptrGainAutoBoolProp);
      _appData.ptrImager->setProperty(ptrGainIntProp);

      mapSize = ImVec2(_appData.ptrImager->getFrameSize().width, _appData.ptrImager->getFrameSize().height);
      ptrMaskImage = new ImageData(mapSize.y, mapSize.x, CV_8UC1, cv::Scalar(0));

      lineShapes.clear();
      lineShapes.push_back(ImGuiCanvasShape("vLine##A_V_ImGuiShape", {0.0f, mapSize.y/2}, ImGuiCanvasShapeType::VLine, {0.0f, mapSize.y}, ImGuiCanvasClip::Out, true));
      lineShapes.push_back(ImGuiCanvasShape("vLine##B_V_ImGuiShape", {mapSize.x, mapSize.y/2}, ImGuiCanvasShapeType::VLine, {0.0f, mapSize.y}, ImGuiCanvasClip::In, true));
      lineShapes.push_back(ImGuiCanvasShape("hLine##A_H_ImGuiShape", {mapSize.x/2, 0.0f}, ImGuiCanvasShapeType::HLine, {0.0f, mapSize.x}, ImGuiCanvasClip::Out, true));
      lineShapes.push_back(ImGuiCanvasShape("hLine##B_H_ImGuiShape", {mapSize.x/2, mapSize.y}, ImGuiCanvasShapeType::HLine, {0.0f, mapSize.x}, ImGuiCanvasClip::In, true));
      
      // ---- load imROI from config ----------------------------------------------------------------------------------
      imROI = {lineShapes[0].m_center.position.x, lineShapes[2].m_center.position.y, lineShapes[1].m_center.position.x, lineShapes[3].m_center.position.y};
      _appData.ptrConfiguration->getROI(roiEnableX, roiEnableY, imROI);
      imROI.Min = ImClamp(imROI.Min,{0,0},mapSize);
      imROI.Max = ImClamp(imROI.Max,{0,0},mapSize);
      lineShapes[0].m_center.position.x = imROI.Min.x;
      lineShapes[1].m_center.position.x = imROI.Max.x;
      lineShapes[2].m_center.position.y = imROI.Min.y;
      lineShapes[3].m_center.position.y = imROI.Max.y;
      _appData.ptrConfiguration->getMasks(maskEnable, lineShapes);

      ptrShader->SetupTexture(GL_TEXTURE0, &imageTexture, _appData.ptrImager->getFrame());
      ptrShader->setUniform("image", 0);
      ptrShader->SetupTexture(GL_TEXTURE1, &maskTexture, *ptrMaskImage);
      ptrShader->setUniform("mask", 1);
      ptrShader->SetupTexture(GL_TEXTURE2, &compositeTexture, mapSize.x, mapSize.y, 0, GL_RGBA);
      ptrShader->BindTexture(0);
      ptrShader->SetupRenderBuffer(&renderBuffers, mapSize.x, mapSize.y, &frameBuffers, &compositeTexture);

      initialized = 1;
      psflive::function::LogMessage("ui","ImagerOptionsGroup()","Imager options group initialized.");
    }
    // ---- initialize ImagerOptionsGroup -----------------------------------------------------------------------------

    { // ++++ draw ImagerOptionsGroup +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

      // ---- camera combo --------------------------------------------------------------------------------------------
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Camera              ");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(200);
      if (ImGui::BeginCombo("##cameraInfo_Combo", _appData.ptrImager->getCameraInfo().serial.c_str())) {
        for(CameraInfo& cameraInfo : psflive::device::Imager::CAMERAINFOS) {
          comboSelect = (cameraInfoSelection == cameraInfo);
          if (ImGui::Selectable(cameraInfo.serial.c_str(), comboSelect)) {
            cameraInfoSelection = cameraInfo;
            psflive::function::LogMessage("ui","ImagerOptionsGroup()","cameraInfoSelection = "+cameraInfoSelection.toString());
            if (_appData.ptrImager->isEnabled())
              _appData.ptrImager->Stop();
            delete _appData.ptrImager;
            _appData.ptrImager = new psflive::device::Imager(cameraInfoSelection);
            _appData.ptrImager->Start();
          }
          if (comboSelect)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::SameLine();
      psflive::ui::HelpMarker(_appData.ptrImager->getCameraInfo().name + " on " + _appData.ptrImager->getCameraInfo().identifier);
      // ---- camera combo --------------------------------------------------------------------------------------------

      // ---- framesize combo -----------------------------------------------------------------------------------------
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Frame size          ");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(200);
      if (ImGui::BeginCombo("##framesize_Combo", _appData.ptrImager->getFrameSize().toString().c_str())) {
        for(FrameSize& frameSize : _appData.ptrImager->getFrameSizes()) {
          comboSelect = (frameSizeSelection == frameSize);
          if (ImGui::Selectable(frameSize.toString().c_str(), comboSelect)) {
            frameSizeSelection = frameSize;
            psflive::function::LogMessage("ui","ImagerOptionsGroup()","frameSizeSelection = "+frameSizeSelection.toString());
            if (_appData.ptrImager->isEnabled())
              _appData.ptrImager->Stop();
            _appData.ptrImager->Update(frameSizeSelection);
            _appData.ptrImager->Start();

            mapSize = ImVec2(_appData.ptrImager->getFrameSize().width, _appData.ptrImager->getFrameSize().height);
            delete ptrMaskImage;
            ptrMaskImage = new ImageData(mapSize.y, mapSize.x, CV_8UC1, cv::Scalar(0));

            lineShapes[0].m_center.position = {0,mapSize.y/2};
            lineShapes[0].m_ctrlPoints[0].position.y = 0;
            lineShapes[0].m_ctrlPoints[1].position.y = mapSize.y;

            lineShapes[1].m_center.position = {mapSize.x,mapSize.y/2};
            lineShapes[1].m_ctrlPoints[0].position.y = 0;
            lineShapes[1].m_ctrlPoints[1].position.y = mapSize.y;

            lineShapes[2].m_center.position = {mapSize.x/2,0};
            lineShapes[2].m_ctrlPoints[0].position.x = 0;
            lineShapes[2].m_ctrlPoints[1].position.x = mapSize.x;

            lineShapes[3].m_center.position = {mapSize.x/2,mapSize.y};
            lineShapes[3].m_ctrlPoints[0].position.x = 0;
            lineShapes[3].m_ctrlPoints[1].position.x = mapSize.x;
            
            imROI = {lineShapes[0].m_center.position.x, lineShapes[2].m_center.position.y, lineShapes[1].m_center.position.x, lineShapes[3].m_center.position.y};
          }
          if (comboSelect)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      // ---- framesize combo -----------------------------------------------------------------------------------------

      // ---- colorformat combo ---------------------------------------------------------------------------------------
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Color format        ");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(200);
      if (ImGui::BeginCombo("##colorformat_Combo", _appData.ptrImager->getFrameFormat().c_str())) {
        for(FrameFormat& frameFormat : _appData.ptrImager->getFrameFormats()) {
          comboSelect = (frameFormatSelection == frameFormat);
          if (ImGui::Selectable(frameFormat.c_str(), comboSelect)) {
            frameFormatSelection = frameFormat;
            psflive::function::LogMessage("ui","ImagerOptionsGroup()","frameFormatSelection = "+frameFormatSelection);
            if (_appData.ptrImager->isEnabled())
              _appData.ptrImager->Stop();
            _appData.ptrImager->Update(frameFormatSelection);
            _appData.ptrImager->Start();
          }
          if (comboSelect)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      // ---- colorformat combo ---------------------------------------------------------------------------------------

      // ---- framerate combo -----------------------------------------------------------------------------------------
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Frame rate          ");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(200);
      if (ImGui::BeginCombo("##framerate_Combo", _appData.ptrImager->getFrameRate().toString().c_str())) {
        for(FrameRate& frameRate : _appData.ptrImager->getFrameRates()) {
          comboSelect = (frameRateSelection == frameRate);
          if (ImGui::Selectable(frameRate.toString().c_str(), comboSelect)){
            frameRateSelection = frameRate;
            psflive::function::LogMessage("ui","ImagerOptionsGroup()","frameRateSelection = "+frameRateSelection.toString());
            if (_appData.ptrImager->isEnabled())
              _appData.ptrImager->Stop();
            _appData.ptrImager->Update(frameRateSelection);
            _appData.ptrImager->Start();
          }
          if (comboSelect)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      // ---- framerate combo -----------------------------------------------------------------------------------------

      // ---- exposure time slider ------------------------------------------------------------------------------------
      ptrExpTimeIntProp = std::dynamic_pointer_cast<IntegerProperty>(_appData.ptrImager->getProperty("Exposure Time (us)"));
      expTimeMax_us = _appData.ptrImager->getFrameRate().toPeriod_us();
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Exposure Time       ");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(200);
      if (ImGui::SliderScalar("##exptime_SliderScalar", ImGuiDataType_U32, &(ptrExpTimeIntProp->value), &(ptrExpTimeIntProp->min), &expTimeMax_us, "%d us"))
        _appData.ptrImager->setProperty(ptrExpTimeIntProp);
      ImGui::SameLine();
      // ---- exposure time slider ------------------------------------------------------------------------------------
      // ---- auto exposure checkbox ----------------------------------------------------------------------------------
      ptrExpAutoBoolProp = std::dynamic_pointer_cast<BooleanProperty>(_appData.ptrImager->getProperty("Exposure Auto"));
      if (ImGui::Checkbox("Auto##auto_exptime_Checkbox", &(ptrExpAutoBoolProp->value)))
        _appData.ptrImager->setProperty(ptrExpAutoBoolProp);
      // ---- auto exposure checkbox ----------------------------------------------------------------------------------

      // ---- exposure time slider ------------------------------------------------------------------------------------
      ptrGainIntProp = std::dynamic_pointer_cast<IntegerProperty>(_appData.ptrImager->getProperty("Gain"));
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Gain                ");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(200);
      if (ImGui::SliderScalar("##gain_SliderScalar", ImGuiDataType_U32, &(ptrGainIntProp->value), &(ptrGainIntProp->min), &(ptrGainIntProp->max), "%d")) 
        _appData.ptrImager->setProperty(ptrGainIntProp);
      ImGui::SameLine();
      // ---- exposure time slider ------------------------------------------------------------------------------------
      // ---- auto gain checkbox --------------------------------------------------------------------------------------
      ptrGainAutoBoolProp = std::dynamic_pointer_cast<BooleanProperty>(_appData.ptrImager->getProperty("Gain Auto"));
      if (ImGui::Checkbox("Auto##auto_gain_Checkbox", &(ptrGainAutoBoolProp->value)))
        _appData.ptrImager->setProperty(ptrGainAutoBoolProp);
      // ---- auto gain checkbox --------------------------------------------------------------------------------------

      // ---- roi select x --------------------------------------------------------------------------------------------
      ImGui::AlignTextToFramePadding();
      ImGui::Text("ROI X               ");
      ImGui::SameLine();
      if (ImGui::Checkbox("##roi_enable_x_Checkbox", &roiEnableX)) {
        static int x1 = (lineShapes[0].m_center.position.x==0) ? psflive::function::rangedRand(0,mapSize.x) : lineShapes[0].m_center.position.x;
        static int x2 = (lineShapes[1].m_center.position.x==mapSize.x-1) ? psflive::function::rangedRand(x1,mapSize.x) : lineShapes[1].m_center.position.x;
        if (roiEnableX) {
          lineShapes[0].m_center.position.x = x1;
          lineShapes[1].m_center.position.x = x2;
        } else {
          lineShapes[0].m_center.position.x = 0;
          lineShapes[1].m_center.position.x = mapSize.x-1;
        }
        ImGui::UpdateMask(ptrMaskImage->data, mapSize, lineShapes);
      }
      if (roiEnableX) {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        if(ImGui::DragFloatRange2("##roi_x_DragFloatRange2", &(lineShapes[0].m_center.position.x), &(lineShapes[1].m_center.position.x), 1.0, 0, mapSize.x, "%.0f", "%.0f"))
          ImGui::UpdateMask(ptrMaskImage->data, mapSize, lineShapes);
        ImGui::SameLine();
        ImGui::Text("width: %.0f", imROI.GetSize().x);
      }
      // ---- roi select x --------------------------------------------------------------------------------------------

      // ---- roi select y --------------------------------------------------------------------------------------------
      ImGui::AlignTextToFramePadding();
      ImGui::Text("ROI Y               ");
      ImGui::SameLine();
      if (ImGui::Checkbox("##roi_enable_y_Checkbox", &roiEnableY)) {
        static int y1 = (lineShapes[2].m_center.position.y==0) ? psflive::function::rangedRand(0,mapSize.y) : lineShapes[2].m_center.position.y;
        static int y2 = (lineShapes[3].m_center.position.y==mapSize.y-1) ? psflive::function::rangedRand(y1,mapSize.y) : lineShapes[3].m_center.position.y;
        if (roiEnableY) {
          lineShapes[2].m_center.position.y = y1;
          lineShapes[3].m_center.position.y = y2;
        } else {
          lineShapes[2].m_center.position.y = 0;
          lineShapes[3].m_center.position.y = mapSize.y-1;
        }
        ImGui::UpdateMask(ptrMaskImage->data, mapSize, lineShapes);
      }
      if (roiEnableY) {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        if (ImGui::DragFloatRange2("##roi_y_DragFloatRange2", &(lineShapes[2].m_center.position.y), &(lineShapes[3].m_center.position.y), 1.0, 0, mapSize.y, "%.0f", "%.0f"))
          ImGui::UpdateMask(ptrMaskImage->data, mapSize, lineShapes);
        ImGui::SameLine();
        ImGui::Text("height: %.0f", imROI.GetSize().y);
      }
      // ---- roi select y --------------------------------------------------------------------------------------------

      // ---- mask ----------------------------------------------------------------------------------------------------
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Mask                ");
      ImGui::SameLine();
      if (ImGui::Checkbox("##mask_enable_Checkbox", &maskEnable)) {
        updateMask = true;
        for (ImGuiCanvasShape& shape: lineShapes)
          shape.setVisible();
      }

      ImGui::SameLine();

      if (!maskEnable) {
        for (std::vector<ImGuiCanvasShape>::iterator shapeIter = lineShapes.begin()+4; shapeIter != lineShapes.end(); ++shapeIter)
          shapeIter->setVisible(false);
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
      }
      
      if (ImGui::Button("Square##square_Button"))
        lineShapes.push_back(ImGuiCanvasShape("square##square_ImGuiShape", {(float)psflive::function::rangedRand(lineShapes[0].m_center.position.x,lineShapes[1].m_center.position.x), (float)psflive::function::rangedRand(lineShapes[2].m_center.position.y,lineShapes[3].m_center.position.y)}, ImGuiCanvasShapeType::Square, {50.0f}, ImGuiCanvasClip::Out, true));

      ImGui::SameLine();
      if (ImGui::Button("Rectangle##rectangle_Button"))
        lineShapes.push_back(ImGuiCanvasShape("rectangle##rectangle_ImGuiShape", {(float)psflive::function::rangedRand(lineShapes[0].m_center.position.x,lineShapes[1].m_center.position.x), (float)psflive::function::rangedRand(lineShapes[2].m_center.position.y,lineShapes[3].m_center.position.y)}, ImGuiCanvasShapeType::Rectangle, {100.0f, 50.0f}, ImGuiCanvasClip::Out, true));

      ImGui::SameLine();
      if (ImGui::Button("Circle##circle_Button"))
        lineShapes.push_back(ImGuiCanvasShape("circle##circle_ImGuiShape", {(float)psflive::function::rangedRand(lineShapes[0].m_center.position.x,lineShapes[1].m_center.position.x), (float)psflive::function::rangedRand(lineShapes[2].m_center.position.y,lineShapes[3].m_center.position.y)}, ImGuiCanvasShapeType::Circle, {50.0f}, ImGuiCanvasClip::Out, true));

      ImGui::SameLine();
      if (ImGui::Button("Ellipse##ellipse_Button"))
        lineShapes.push_back(ImGuiCanvasShape("ellipse##ellipse_ImGuiShape", {(float)psflive::function::rangedRand(lineShapes[0].m_center.position.x,lineShapes[1].m_center.position.x), (float)psflive::function::rangedRand(lineShapes[2].m_center.position.y,lineShapes[3].m_center.position.y)}, ImGuiCanvasShapeType::Ellipse, {100.0f, 50.0f}, ImGuiCanvasClip::Out, true));
      
      if (!maskEnable) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
      }
      // ---- mask ----------------------------------------------------------------------------------------------------

      if (ImGui::CollapsingHeader("Frame Statistics")) { // ==== INFO =================================================
        cv::minMaxLoc(histDataImage, &minPxVal, &maxPxVal, &minPxIdx, &maxPxIdx);
        cv::minMaxLoc(histDataImage, &minPxVal, &maxPxVal, &minPxIdx, &maxPxIdx, histMaskImage);
        ImGui::Text("[ROI] Min Px     : [%d adu]", (int)minPxVal);
        ImGui::Text("[ROI] Max Px     : [%d adu]", (int)maxPxVal);
        ImGui::Text("[Region] Min Px  : [%d adu]", (int)minPxVal);
        ImGui::Text("[Region] Max Px  : [%d adu]", (int)maxPxVal);
        if (_uiData.showHistogramPlotWindow) {
          if (ImGui::Button("Close Plot##Histogram")) {
            _uiData.showHistogramPlotWindow = false;
          }
        } else {
          if (ImGui::Button("Open Plot##Histogram")) {
            _uiData.showHistogramPlotWindow = true;
          }
        }
        ImGui::Separator(); // ________________________________________________________________________________________
      } // ==== INFO ==================================================================================================

      psflive::ui::HistogramWindow(_uiData, histDataImage, histMaskImage);

      ptrShader->UpdateTexture(GL_TEXTURE0, &imageTexture, _appData.ptrImager->getFrame());
      ptrShader->setUniform("image", 0);
      ptrShader->UpdateTexture(GL_TEXTURE1, &maskTexture, *ptrMaskImage);
      ptrShader->setUniform("mask", 1);
      ptrShader->UpdateTexture(GL_TEXTURE2, &compositeTexture, mapSize.x, mapSize.y, 0, GL_RGBA);
      ptrShader->BindTexture(0);
      ptrShader->UpdateRenderBuffer(&renderBuffers, mapSize.x, mapSize.y, &frameBuffers, &compositeTexture);

      viewSize.x = ImGui::GetContentRegionAvailWidth();
      viewSize.y = (int)(viewSize.x*_appData.ptrImager->getAspectRatio());

      ImGui::DrawCanvas("canvas", viewSize, mapSize, lineShapes, (ImTextureID)(intptr_t)compositeTexture, ptrMaskImage->data, updateMask);

      ImGui::Separator(); // ==========================================================================================

      histMaskImage = (*ptrMaskImage)(cvROI);
      histDataImage = (_appData.ptrImager->getFrame())(cvROI);

      imROI = {IM_ROUND(lineShapes[0].m_center.position.x), IM_ROUND(lineShapes[2].m_center.position.y), IM_ROUND(lineShapes[1].m_center.position.x), IM_ROUND(lineShapes[3].m_center.position.y)};
      cvROI = cv::Rect(imROI.Min.x, imROI.Min.y, imROI.GetWidth(), imROI.GetHeight());

      if (ImGui::Button("APPLY##ImagerOptions_APPLY_Button", ImVec2(200,40))) {
        // ---- put settings in config --------------------------------------------------------------------------------
        _appData.ptrConfiguration->putCameraInfo(_appData.ptrImager->getCameraInfo());
        _appData.ptrConfiguration->putFrameSize(_appData.ptrImager->getFrameSize());
        _appData.ptrConfiguration->putFrameFormat(_appData.ptrImager->getFrameFormat());
        _appData.ptrConfiguration->putFrameRate(_appData.ptrImager->getFrameRate());
        _appData.ptrConfiguration->putExposureSetting(ptrExpAutoBoolProp->value, ptrExpTimeIntProp->value); 
        _appData.ptrConfiguration->putGainSetting(ptrGainAutoBoolProp->value, ptrGainIntProp->value);
        _appData.ptrConfiguration->putROI(roiEnableX, roiEnableY, imROI);
        _appData.ptrConfiguration->putMasks(maskEnable, lineShapes);
        _appData.ptrConfiguration->Write();
        // ---- put settings in config --------------------------------------------------------------------------------
      }

    } // ++++ draw ImagerOptionsGroup +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  }
  // ---- un-initialize ImagerOptionsGroup ----------------------------------------------------------------------------
  if (initialized & !_uiData.showDeviceOptionsWindow) {
    psflive::function::LogMessage("ui","ImagerOptionsGroup()","Un-initializing imager options group.");
    delete ptrShader;
    delete ptrMaskImage;

    if (_appData.ptrImager->isEnabled())
      _appData.ptrImager->Stop();
    delete _appData.ptrImager;
    _appData.ptrImager = nullptr;

    initialized = 0;
    psflive::function::LogMessage("ui","ImagerOptionsGroup()","Imager options group uninitialized.");
  }
  // ---- un-initialize ImagerOptionsGroup ----------------------------------------------------------------------------
}
// ~~~~ ImagerOptionsGroup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~










// ~~~~ HistogramWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::HistogramWindow(Data& _uiData, ImageData& _histDataImage, ImageData& _histMaskImage) {

  static cv::Mat histDataROI, histDataRegion;
  static int histNBins = 128;
  static int histNTicks = 8;
  static std::vector<double> tickPositions(histNTicks+1);
  static std::vector<const char *> tickLabels(histNTicks+1);
  static std::vector<std::string> tickLabelsStrings(histNTicks+1);
  static float histRangeMinPxVal = 0, histRangeMaxPxVal = pow(2,8);
  static float histRangeValue[] = {histRangeMinPxVal, histRangeMaxPxVal};
  static const float* histRange[] = {histRangeValue};
  static bool histUniform = true, histAccumulate = false;

  if (_uiData.showHistogramPlotWindow) {
    histRangeMinPxVal = 0, histRangeMaxPxVal = pow(2,psflive::function::cvTypeToSize(_histDataImage.type()));
    histRangeValue[0] = histRangeMinPxVal, histRangeValue[1] = histRangeMaxPxVal;
    histRange[0] = histRangeValue;
    tickPositions.resize(histNTicks+1);
    tickLabels.resize(histNTicks+1);
    tickLabelsStrings.resize(histNTicks+1);
    for (size_t histITick = 0; histITick < tickPositions.size(); ++histITick) {
      tickPositions[histITick] = histITick*histNBins/histNTicks;
      tickLabelsStrings[histITick] = std::to_string((int)(histITick*histRangeMaxPxVal/histNTicks));
      tickLabels[histITick] = tickLabelsStrings[histITick].c_str();
    }
    ImGui::Begin("Pixel histogram");
    {
      ImPlot::SetNextPlotLimits(0.1, 100, 0, 10);
      cv::calcHist(&_histDataImage, 1, 0, _histMaskImage, histDataRegion, 1, &histNBins, histRange, histUniform, histAccumulate);
      cv::calcHist(&_histDataImage, 1, 0, cv::Mat(), histDataROI, 1, &histNBins, histRange, histUniform, histAccumulate);
      ImPlot::SetNextPlotTicksX(tickPositions.data(), histNTicks+1, tickLabels.data());
      ImPlot::SetNextPlotLimits(histRangeMinPxVal, histNBins, 0.0, 100.0, ImGuiCond_Once);
      if (ImPlot::BeginPlot("##Histogram","Pixel value","Pixel count")) {
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        ImPlot::PlotBars(" ROI Pixels", &histDataROI.at<float>(), histNBins, 1.0);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        ImPlot::PlotBars(" Region Pixels", &histDataRegion.at<float>(), histNBins, 1.0);
        ImPlot::EndPlot();
      }
    }
    ImGui::End();
  }
}
// ~~~~ HistogramWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~










// ~~~~ ApplicationOptionsGroup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::ApplicationOptionsGroup(Data& _uiData, AppData& _appData) {
  static bool initialized = 0;

  static char locationBuffer[256];
  static std::string location;

  if (_uiData.showOtherOptionsWindow) {
    location = psflive::AcquireOperation::GETLOCATION();
    strcpy(locationBuffer, location.c_str());
    
    if (!initialized) {
      psflive::function::LogMessage("ui","ApplicationOptionsGroup()","Initializing application options group.");
 
      if (_appData.ptrConfiguration->getString("Application Options","location",location)) {
        strcpy(locationBuffer, location.c_str());
        psflive::AcquireOperation::SETLOCATION(location);
      }

      initialized = 1;
      psflive::function::LogMessage("ui","ApplicationOptionsGroup()","Application options group initialized.");
    }

    { // ++++ draw ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Data location                 ");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(300);
      
      if (ImGui::InputText("##locationBuffer_InputText", locationBuffer, IM_ARRAYSIZE(locationBuffer))) {
        location = locationBuffer;
        psflive::AcquireOperation::SETLOCATION(location);
      }

      ImGui::Separator(); // ==========================================================================================

      if (ImGui::Button("APPLY##ApplicationOptions_APPLY_Button", ImVec2(200,40))) {
        _appData.ptrConfiguration->putString("Application Options","location",location);
        _appData.ptrConfiguration->Write();
      }

    } // ++++ draw ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ---- un-initialize ApplicationOptionsGroup ---------------------------------------------------------------------
    if (initialized & !_uiData.showOtherOptionsWindow) {
      psflive::function::LogMessage("ui","ApplicationOptionsGroup()","Un-initializing application options group.");
      initialized = 0;
      psflive::function::LogMessage("ui","ApplicationOptionsGroup()","Application options group un-initialized.");
    }
    // ---- un-initialize ApplicationOptionsGroup ---------------------------------------------------------------------
  }
}
// ~~~~ ApplicationOptionsGroup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~










// ~~~~ ConnectionOptionsGroup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::ConnectionOptionsGroup(Data& _uiData, AppData& _appData) {
  static bool initialized = 0;

  static std::string address;
  static char addressBuffer[64] = PSFLIVE_DEFAULT_ADDRESS;
  static ImU16 portNumber = PSFLIVE_DEFAULT_PORT; static const ImU16 port_number_SliderStep = 1, port_number_SliderFast = 1;

  if (_uiData.showOtherOptionsWindow) {
    address = _appData.ptrConnection->getAddres();
    strcpy(addressBuffer, address.c_str());

    if (!initialized) {
      psflive::function::LogMessage("ui","ConnectionOptionsGroup()","Initializing connection options group.");

      if (_appData.ptrConfiguration->getNumber("Connection Options","port",portNumber))
        _appData.ptrConnection->setPort(portNumber);

      if (_appData.ptrConfiguration->getString("Connection Options","address",address)) {
        strcpy(addressBuffer, address.c_str());
        _appData.ptrConnection->setAddress(address);
      }

      initialized = 1;
      psflive::function::LogMessage("ui","ConnectionOptionsGroup()","Connection options group initialized.");
    }

    { // ++++ draw ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

      ImGui::AlignTextToFramePadding();
      ImGui::Text("Host address                  ");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(150);
      if (ImGui::InputText("##prefixBuffer##MeasureOptionsGroup()", addressBuffer, IM_ARRAYSIZE(addressBuffer))) {
        address = addressBuffer;
      }
      
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Port                          ");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(150);
      ImGui::InputScalar("##port_InputScalar", ImGuiDataType_U16, &portNumber, &port_number_SliderStep, &port_number_SliderFast, "%u");

      ImGui::Separator(); // ==========================================================================================

      if (ImGui::Button("APPLY##ConnectionOptions_APPLY_Button", ImVec2(200,40))) {
        _appData.ptrConfiguration->putNumber("Connection Options","port",portNumber);
        _appData.ptrConfiguration->putString("Connection Options","address",address);
        _appData.ptrConfiguration->Write();
      }

      ImGui::SameLine();

      if (_appData.ptrConnection->isListening()) {
        if (ImGui::Button("STOP LISTENING", ImVec2(200,40))) {
          _appData.ptrConnection->setListen(false);
          if(_appData.ptrConnection->m_threadId.joinable())
            _appData.ptrConnection->m_threadId.join();
        }
      } else {
        if (ImGui::Button("LISTEN", ImVec2(200,40))) {
          delete _appData.ptrConnection;
          _appData.ptrConnection = new psflive::Connection(std::string(address), portNumber);
          _appData.ptrConnection->m_threadId = std::thread(psflive::ui::ZMQListener, std::ref(_uiData), std::ref(_appData));          
        }
      }

    } // ++++ draw ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ---- un-initialize ConnectionOptionsGroup ----------------------------------------------------------------------
    if (initialized & !_uiData.showOtherOptionsWindow) {
      psflive::function::LogMessage("ui","ConnectionOptionsGroup()","Un-initializing connection options group.");
      initialized = 0;
      psflive::function::LogMessage("ui","ConnectionOptionsGroup()","Connection options group un-initialized.");
    }
    // ---- un-initialize ConnectionOptionsGroup ----------------------------------------------------------------------
  }
}
// ~~~~ ConnectionOptionsGroup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~










// ~~~~ TestOptionsGroup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::TestOptionsGroup(Data& _uiData, AppData& _appData) {
  static bool initialized = 0;
  if (_uiData.showOtherOptionsWindow) {
    if (!initialized) {
      psflive::function::LogMessage("ui","TestOptionsGroup()","Initializing test options group.");
      initialized = 1;
      psflive::function::LogMessage("ui","TestOptionsGroup()","Test options group initialized.");
    }

    { // ++++ draw ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    } // ++++ draw ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ---- un-initialize TestOptionsGroup ----------------------------------------------------------------------------
    if (initialized & !_uiData.showOtherOptionsWindow) {
      psflive::function::LogMessage("ui","TestOptionsGroup()","Un-initializing test options group.");
      initialized = 0;
      psflive::function::LogMessage("ui","TestOptionsGroup()","Test options group un-initialized.");
    }
    // ---- un-initialize TestOptionsGroup ----------------------------------------------------------------------------
  }
}
// ~~~~ TestOptionsGroup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




















// ~~~~ CaptureOperationWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::CaptureOperationWindow(Data& _uiData, AppData& _appData) {
  static bool initialized = 0;

  static cv::Point minPxIdx, maxPxIdx;
  static double minPxVal = 0, maxPxVal = 0;

  static ImageData histMaskImage, histDataImage;

  static Shader* ptrShader;
  static ImageData* ptrMaskImage;

  static psflive::CaptureOperation* ptrCaptureOperation;

  static CameraInfo cameraInfo;
  static FrameSize frameSize;
  static FrameFormat frameFormat;
  static FrameRate frameRate;

  static std::shared_ptr<BooleanProperty> ptrExpAutoBoolProp;
  static std::shared_ptr<IntegerProperty> ptrExpTimeIntProp;

  static std::shared_ptr<IntegerProperty> ptrGainIntProp;
  static std::shared_ptr<BooleanProperty> ptrGainAutoBoolProp;

  static bool roiEnableX, roiEnableY, maskEnable;
  static ImRect imROI;
  static cv::Rect cvROI;

  static float wavelength_nm;

  static std::string address;
  static ImU16 portNumber = PSFLIVE_DEFAULT_PORT;

  // ---- show CaptureOperationWindow ---------------------------------------------------------------------------------
  if (_uiData.showCaptureOperationWindow) {
    static GLuint imageTexture, maskTexture, compositeTexture, renderBuffers, frameBuffers;

    static ImVec2 mapSize, viewSize;
    static std::vector<ImGuiCanvasShape> shapes;

    static ImU16 nFrames = 1;
    static ImU32 frameInterval_us = 0;
    static bool saveOnClick = false, saveFrames = false;
    static int suffixOption = 0;
    static char prefixBuffer[64], suffixBuffer[64], suffixTimeFormatBuffer[64], suffixIndexFormatBuffer[64];
    static char buttonLabel[64];
    static std::string prefix, suffix, suffixIndexFormat, suffixTimeFormat, location;
    static char fileNameBuffer[3*64];

    // ---- initialize CaptureOperationWindow -------------------------------------------------------------------------
    if (!initialized) {
      psflive::function::LogMessage("ui","CaptureOperationWindow()","Initializing new capture window.");

      // ---- setup shader --------------------------------------------------------------------------------------------
      ptrShader = new Shader(Shader::ReadFile(BASE_VERTEX_SHADER_FILE_STR), Shader::ReadFile(MASK_FRAGMENT_SHADER_FILE_STR)); // CHECKED: delete ptrShader;
      ptrShader->Initialize();
      ptrShader->BindFragDataLocation();
      ptrShader->Use();

      ptrShader->CreateTexture();

      // ---- setup Imager --------------------------------------------------------------------------------------------
      if (_appData.ptrImager != nullptr)
        delete _appData.ptrImager;

      _appData.ptrConfiguration->pickCameraInfo(psflive::device::Imager::CAMERAINFOS, cameraInfo);
      psflive::function::LogMessage("ui","CaptureOperationWindow()","CameraInfo = "+cameraInfo.toString());
      try {
        _appData.ptrImager = new psflive::device::Imager(cameraInfo);
      } catch(const std::exception& _exception) {
        psflive::function::LogMessage("ui", "CaptureOperationWindow()","Exception = "+std::string(_exception.what()));
      }

      _appData.ptrConfiguration->pickFrameSize(_appData.ptrImager->getFrameSizes(), frameSize);
      psflive::function::LogMessage("ui", "CaptureOperationWindow()","FrameSize = "+frameSize.toString());
      try {
        _appData.ptrImager->Update(frameSize);
      } catch(const std::exception& _exception) {
        psflive::function::LogMessage("ui", "CaptureOperationWindow()","Exception = "+std::string(_exception.what()));
      }

      _appData.ptrConfiguration->pickFrameFormat(_appData.ptrImager->getFrameFormats(), frameFormat);
      psflive::function::LogMessage("ui", "CaptureOperationWindow()","FrameFormat = "+frameFormat);
      try {
        _appData.ptrImager->Update(frameFormat);
      } catch(const std::exception& _exception) {
        psflive::function::LogMessage("ui", "CaptureOperationWindow()","Exception = "+std::string(_exception.what()));
      }

      _appData.ptrConfiguration->pickFrameRate(_appData.ptrImager->getFrameRates(), frameRate);
      psflive::function::LogMessage("ui", "CaptureOperationWindow()","FrameRate = "+frameRate.toString());
      try {
        _appData.ptrImager->Update(frameRate);
      } catch(const std::exception& _exception) {
        psflive::function::LogMessage("ui", "CaptureOperationWindow()","Exception = "+std::string(_exception.what()));
      }

      _appData.ptrImager->Start();
      // ---- setup Imager --------------------------------------------------------------------------------------------

      ptrExpTimeIntProp = std::dynamic_pointer_cast<IntegerProperty>(_appData.ptrImager->getProperty("Exposure Time (us)"));
      ptrExpAutoBoolProp = std::dynamic_pointer_cast<BooleanProperty>(_appData.ptrImager->getProperty("Exposure Auto"));
      _appData.ptrConfiguration->getExposureSetting(ptrExpAutoBoolProp->value, ptrExpTimeIntProp->value);
      _appData.ptrImager->setProperty(ptrExpAutoBoolProp);
      _appData.ptrImager->setProperty(ptrExpTimeIntProp);

      ptrGainIntProp = std::dynamic_pointer_cast<IntegerProperty>(_appData.ptrImager->getProperty("Gain"));
      ptrGainAutoBoolProp = std::dynamic_pointer_cast<BooleanProperty>(_appData.ptrImager->getProperty("Gain Auto"));
      _appData.ptrConfiguration->getGainSetting(ptrGainAutoBoolProp->value, ptrGainIntProp->value);
      _appData.ptrImager->setProperty(ptrGainAutoBoolProp);
      _appData.ptrImager->setProperty(ptrGainIntProp);

      _appData.ptrConfiguration->getROI(roiEnableX, roiEnableY, imROI);
      imROI.Min = ImClamp(imROI.Min,{0,0},{(float)_appData.ptrImager->getFrameSize().width, (float)_appData.ptrImager->getFrameSize().height});
      imROI.Max = ImClamp(imROI.Max,{0,0},{(float)_appData.ptrImager->getFrameSize().width, (float)_appData.ptrImager->getFrameSize().height});
      _appData.ptrImager->setRoi({(int)imROI.Min.x,(int)imROI.Min.y,(int)imROI.GetWidth(),(int)imROI.GetHeight()});

      mapSize = ImVec2(_appData.ptrImager->getRoi().width, _appData.ptrImager->getRoi().height);
      ptrMaskImage = new ImageData(mapSize.y, mapSize.x, CV_8UC1, cv::Scalar(0));

      shapes.clear();
      _appData.ptrConfiguration->getMasks(maskEnable, shapes);
      if (maskEnable) {
        for (size_t iShape = 0; iShape < shapes.size(); iShape++) {
          shapes[iShape].setDrawCtrls(false);
          shapes[iShape].select();
          shapes[iShape].MoveCenter(ImVec2(-1,-1)*imROI.Min, mapSize);
          shapes[iShape].deselect();
        }
      }

      ptrShader->SetupTexture(GL_TEXTURE0, &imageTexture, _appData.ptrImager->getROIFrame());
      ptrShader->setUniform("image", 0);
      
      ptrShader->SetupTexture(GL_TEXTURE1, &maskTexture, *ptrMaskImage);
      ptrShader->setUniform("mask", 1);

      ptrShader->SetupTexture(GL_TEXTURE2, &compositeTexture, mapSize.x, mapSize.y, 0, GL_RGBA);
      ptrShader->BindTexture(0);

      ptrShader->SetupRenderBuffer(&renderBuffers, mapSize.x, mapSize.y, &frameBuffers, &compositeTexture);

      _appData.ptrConfiguration->getNumber("Capture Options","nFrames",nFrames);
      _appData.ptrConfiguration->getNumber("Capture Options","frameInterval_us",frameInterval_us);

      _appData.ptrConfiguration->getBoolean("Capture Options","saveFrames",saveFrames);
      _appData.ptrConfiguration->getBoolean("Capture Options","saveOnClick",saveOnClick);
      
      _appData.ptrConfiguration->getString("Application Options","location",location);
      _appData.ptrConfiguration->getNumber("Calibration Options","wavelength_nm",wavelength_nm);
  
      _appData.ptrConfiguration->getNumber("Connection Options","port",portNumber);
      _appData.ptrConfiguration->getString("Connection Options","address",address);

      ptrCaptureOperation = new psflive::CaptureOperation(_appData.ptrImager, nFrames, frameInterval_us); // CHECKED: delete ptrShader;
      ptrCaptureOperation->setMask(*ptrMaskImage);
      ptrCaptureOperation->setSaveFrames(saveFrames);
      ptrCaptureOperation->setSaveOnClick(saveOnClick);
      ptrCaptureOperation->SETLOCATION(location);
      ptrCaptureOperation->setWavelength_nm(wavelength_nm);

      _appData.ptrConfiguration->getString("Capture Options","prefix", prefix);
      strcpy(prefixBuffer, prefix.c_str());
      _appData.ptrConfiguration->getNumber("Capture Options","suffixOption", suffixOption);
      _appData.ptrConfiguration->getString("Capture Options","suffix", suffix);
      if (suffixOption == 0) { // time stamp
        strcpy(suffixTimeFormatBuffer, suffix.c_str());
        suffixTimeFormat = suffixTimeFormatBuffer;
        suffixTimeFormat = std::regex_replace(suffixTimeFormat, std::regex("%%%%"), "%"); // TODO: this does not work properly when the corresponding config entry is empty
      }
      if (suffixOption == 1) { // index stamp
        strcpy(suffixIndexFormatBuffer, suffix.c_str());
        suffixIndexFormat = suffixIndexFormatBuffer;
        suffixIndexFormat = std::regex_replace(suffixIndexFormat, std::regex("%%%%"), "%");
      }

      initialized = 1;
      _uiData.executeCapture = false;
      psflive::function::LogMessage("ui","CaptureOperationWindow()","New capture window initialized.");
    }
    // ---- initialize CaptureOperationWindow -------------------------------------------------------------------------

    if (_uiData.showCaptureOperationWindow) {
      if (ImGui::Begin("New Capture", &_uiData.showCaptureOperationWindow)) { // ++++ CaptureOperationWindow ++++++++++

        if (ImGui::CollapsingHeader("Frame Statistics")) { // ==== INFO ===============================================
          cv::minMaxLoc(histDataImage, &minPxVal, &maxPxVal, &minPxIdx, &maxPxIdx);
          cv::minMaxLoc(histDataImage, &minPxVal, &maxPxVal, &minPxIdx, &maxPxIdx, histMaskImage);
          ImGui::Text("[ROI] Min Px     : [%d adu]", (int)minPxVal);
          ImGui::Text("[ROI] Max Px     : [%d adu]", (int)maxPxVal);
          ImGui::Text("[Region] Min Px  : [%d adu]", (int)minPxVal);
          ImGui::Text("[Region] Max Px  : [%d adu]", (int)maxPxVal);
          if (_uiData.showHistogramPlotWindow) {
            if (ImGui::Button("Close Plot##Histogram")) {
              _uiData.showHistogramPlotWindow = false;
            }
          } else {
            if (ImGui::Button("Open Plot##Histogram")) {
              _uiData.showHistogramPlotWindow = true;
            }
          }
          ImGui::Separator(); // ______________________________________________________________________________________
        } // ==== INFO ================================================================================================

        psflive::ui::HistogramWindow(_uiData, histDataImage, histMaskImage);

        ptrShader->UpdateTexture(GL_TEXTURE0, &imageTexture, _appData.ptrImager->getROIFrame());
        ptrShader->setUniform("image", 0);

        ptrShader->UpdateTexture(GL_TEXTURE1, &maskTexture, *ptrMaskImage);
        ptrShader->setUniform("mask", 1);

        ptrShader->UpdateTexture(GL_TEXTURE2, &compositeTexture, mapSize.x, mapSize.y, 0, GL_RGBA);
        ptrShader->BindTexture(0);

        ptrShader->UpdateRenderBuffer(&renderBuffers, mapSize.x, mapSize.y, &frameBuffers, &compositeTexture);

        viewSize.x = ImGui::GetContentRegionAvailWidth();
        viewSize.y = (int)(viewSize.x*_appData.ptrImager->getROIAspectRatio());
        ImGui::DrawCanvas("canvas", viewSize, mapSize, shapes, (ImTextureID)(intptr_t)compositeTexture, ptrMaskImage->data);

        ImGui::Separator(); // ========================================================================================

        histMaskImage = *ptrMaskImage;
        histDataImage = _appData.ptrImager->getROIFrame();

        if (psflive::CaptureOperation::GETBUSY()) {
          sprintf(buttonLabel, "Capturing frame %d of %d\n        CANCEL        ##CANCEL_Button", ptrCaptureOperation->getIframe(), ptrCaptureOperation->getFrameCount());
          if (ImGui::Button(buttonLabel, ImVec2(200,40)))
            psflive::CaptureOperation::SETBUSY(false);
        } else {
          if (ptrCaptureOperation->m_operationThread.joinable())
            ptrCaptureOperation->m_operationThread.join();
          if (ImGui::Button("CAPTURE##CAPTURE_Button", ImVec2(200,40)) || _uiData.executeCapture) {
            if (saveOnClick) {
              psflive::CaptureOperation::UPDATECURRENTTIME();
              psflive::CaptureOperation::INCREMENTCURRENTINDEX();
              if (suffixOption == 0) // time stamp
                std::strftime(suffixBuffer, sizeof(suffixBuffer), suffixTimeFormat.c_str(), psflive::CaptureOperation::GETLOCALTIME());
              if (suffixOption == 1) // index stamp
                sprintf(suffixBuffer, suffixIndexFormat.c_str(), psflive::CaptureOperation::GETCURRENTINDEX());
              sprintf(fileNameBuffer, "%s%s.fits", prefixBuffer, suffixBuffer);
              ptrCaptureOperation->setTime();
              ptrCaptureOperation->setFileName(fileNameBuffer);
              _uiData.fileName = ptrCaptureOperation->getFileName();
            }
            ptrCaptureOperation->Execute();
            _uiData.executeCapture = false;
          }
        }

        ImGui::SameLine();

        if (_appData.ptrConnection->isListening()) {
          if (ImGui::Button("STOP LISTENING", ImVec2(200,40))) {
            _appData.ptrConnection->setListen(false);
            if(_appData.ptrConnection->m_threadId.joinable())
              _appData.ptrConnection->m_threadId.join();
          }
        } else {
          if (ImGui::Button("LISTEN", ImVec2(200,40))) {
            delete _appData.ptrConnection;
            _appData.ptrConnection = new psflive::Connection(std::string(address), portNumber);
            _appData.ptrConnection->m_threadId = std::thread(psflive::ui::ZMQListener, std::ref(_uiData), std::ref(_appData));          
          }
        }

      }
      ImGui::End(); // ++++ CaptureOperationWindow ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }
  }
  // ---- show CaptureOperationWindow ---------------------------------------------------------------------------------
  // ---- un-initialize CaptureOperationWindow ------------------------------------------------------------------------
  if (initialized & !_uiData.showCaptureOperationWindow) {
    psflive::function::LogMessage("ui","CaptureOperationWindow()","Un-initializing new capture window.");

    if (ptrCaptureOperation->m_operationThread.joinable())
      ptrCaptureOperation->m_operationThread.join();

    delete ptrShader;
    delete ptrMaskImage;
    delete ptrCaptureOperation;

    if (_appData.ptrImager->isEnabled())
      _appData.ptrImager->Stop();
    delete _appData.ptrImager;
    _appData.ptrImager = nullptr;

    initialized = 0;

    psflive::function::LogMessage("ui","CaptureOperationWindow()","New capture window un-initialized.");
  }
  // ---- un-initialize CaptureOperationWindow ------------------------------------------------------------------------
}
// ~~~~ CaptureOperationWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~










// ~~~~ CaptureDataWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::CaptureDataWindow(CaptureData& _captureData) {

  static ImU8 frameNumber=0, frameNumberMin=0, frameNumberMax=_captureData.getFrameCount()-1;

  if (_captureData.m_show) {
    // ---- initialize window -----------------------------------------------------------------------------------------
    if (!_captureData.m_initWindow) {
      psflive::function::LogMessage("ui","CaptureDataWindow()","Initializing capture window.");
      _captureData.m_mapSize = ImVec2(_captureData.getRowCount(), _captureData.getColumnCount());
      // --------------------------------------------------------------------------------------------------------------
      _captureData.m_ptrShader = new Shader(Shader::ReadFile(BASE_VERTEX_SHADER_FILE_STR), Shader::ReadFile(MASK_FRAGMENT_SHADER_FILE_STR)); // CHECKED: delete ptrShader;
      _captureData.m_ptrShader->Initialize();
      _captureData.m_ptrShader->BindFragDataLocation();
      _captureData.m_ptrShader->Use();
      _captureData.m_ptrShader->CreateTexture();
      // --------------------------------------------------------------------------------------------------------------
      _captureData.m_ptrShader->SetupTexture(GL_TEXTURE0, &_captureData.m_imageTexture, _captureData.getFrame(0));
      _captureData.m_ptrShader->setUniform("image", 0);
      _captureData.m_ptrShader->SetupTexture(GL_TEXTURE1, &_captureData.m_maskTexture, _captureData.getMask());
      _captureData.m_ptrShader->setUniform("mask", 1);
      _captureData.m_ptrShader->SetupTexture(GL_TEXTURE2, &_captureData.m_compositeTexture, _captureData.m_mapSize.x, _captureData.m_mapSize.y, 0, GL_RGBA);
      _captureData.m_ptrShader->BindTexture(0);
      _captureData.m_ptrShader->SetupRenderBuffer(&_captureData.m_renderBuffers, _captureData.m_mapSize.x, _captureData.m_mapSize.y, &_captureData.m_frameBuffers, &_captureData.m_compositeTexture);
      // --------------------------------------------------------------------------------------------------------------
      _captureData.m_initWindow = 1;
      psflive::function::LogMessage("ui","CaptureDataWindow()","Capture window initialized.");
    }
    
    // ---- initialize window -----------------------------------------------------------------------------------------
    if (ImGui::Begin(_captureData.getWindowTitle().c_str(), &_captureData.m_show)) { // ++++ CaptureData window begin +

      if (ImGui::BeginTabBar("CaptureDataTabs", ImGuiTabBarFlags_None)) {

        if (ImGui::BeginTabItem("Mean Frame##capture")) { // ---- mean frame ------------------------------------------
          // ----------------------------------------------------------------------------------------------------------
          _captureData.m_ptrShader->UpdateTexture(GL_TEXTURE0, &_captureData.m_imageTexture, _captureData.getDispMeanFrame());
          _captureData.m_ptrShader->setUniform("image", 0);
          _captureData.m_ptrShader->UpdateTexture(GL_TEXTURE2, &_captureData.m_compositeTexture, _captureData.m_mapSize.x, _captureData.m_mapSize.y, 0, GL_RGBA);
          _captureData.m_ptrShader->BindTexture(0);
          _captureData.m_ptrShader->UpdateRenderBuffer(&_captureData.m_renderBuffers, _captureData.m_mapSize.x, _captureData.m_mapSize.y, &_captureData.m_frameBuffers, &_captureData.m_compositeTexture);
          // ----------------------------------------------------------------------------------------------------------
          _captureData.m_viewSize.x = ImGui::GetContentRegionAvailWidth();
          _captureData.m_viewSize.y = (int)(_captureData.m_viewSize.x*_captureData.getAspectRatio());
          ImGui::DrawCanvas("canvas", _captureData.m_viewSize, _captureData.m_mapSize, (ImTextureID)(intptr_t)_captureData.m_compositeTexture);
          // ----------------------------------------------------------------------------------------------------------
          ImGui::EndTabItem(); // ---- mean frame ---------------------------------------------------------------------
        }

        if (ImGui::BeginTabItem("Captured Frames##capture")) { // ---- captured frames --------------------------------
          // ----------------------------------------------------------------------------------------------------------
          _captureData.m_ptrShader->UpdateTexture(GL_TEXTURE0, &_captureData.m_imageTexture, _captureData.getFrame(frameNumber));
          _captureData.m_ptrShader->setUniform("image", 0);
          _captureData.m_ptrShader->UpdateTexture(GL_TEXTURE2, &_captureData.m_compositeTexture, _captureData.m_mapSize.x, _captureData.m_mapSize.y, 0, GL_RGBA);
          _captureData.m_ptrShader->BindTexture(0);
          _captureData.m_ptrShader->UpdateRenderBuffer(&_captureData.m_renderBuffers, _captureData.m_mapSize.x, _captureData.m_mapSize.y, &_captureData.m_frameBuffers, &_captureData.m_compositeTexture);
          // ----------------------------------------------------------------------------------------------------------
          _captureData.m_viewSize.x = ImGui::GetContentRegionAvailWidth();
          _captureData.m_viewSize.y = (int)(_captureData.m_viewSize.x*_captureData.getAspectRatio());
          ImGui::DrawCanvas("canvas", _captureData.m_viewSize, _captureData.m_mapSize, (ImTextureID)(intptr_t)_captureData.m_compositeTexture);
          ImGui::Separator(); // ======================================================================================

          ImGui::AlignTextToFramePadding();
          ImGui::Text("Frame number        ");
          ImGui::SameLine();
          ImGui::SetNextItemWidth(200);
          ImGui::SliderScalar("##frameNumber", ImGuiDataType_U8, &frameNumber, &frameNumberMin, &frameNumberMax, "%d");

          ImGui::EndTabItem(); // ---- captured frames ----------------------------------------------------------------
        }

        if (ImGui::BeginTabItem("Info##capture")) {
          ImGui::Text("Source           : %s", _captureData.getSource().c_str());
          ImGui::SameLine();
          psflive::ui::HelpMarker( _captureData.getSourceComment());

          ImGui::Text("Version          : %s", _captureData.getVersion().c_str());
          ImGui::SameLine();
          psflive::ui::HelpMarker( _captureData.getVersionComment());

          ImGui::Text("Type             : %s", psflive::function::acquireDataTypeToString(_captureData.getAcquireType()).c_str());
          ImGui::SameLine();
          psflive::ui::HelpMarker( _captureData.getTypeComment());

          ImGui::Text("Size             : [%d x %d x %d frames]", _captureData.getRowCount(), _captureData.getColumnCount(), _captureData.getFrames());

          ImGui::Text("Exposure time    : %d (us)", _captureData.getExposureTime_us());
          ImGui::SameLine();
          psflive::ui::HelpMarker( _captureData.getExposureTimeComment());

          ImGui::Text("Gain             : %d", _captureData.getGain());
          ImGui::SameLine();
          psflive::ui::HelpMarker( _captureData.getGainComment());

          ImGui::Text("Wavelength       : %f (nm)", _captureData.getWavelength_nm());
          ImGui::SameLine();
          psflive::ui::HelpMarker( _captureData.getWavelengthComment());

          ImGui::Text("Date             : %s", _captureData.getDate().c_str());
          ImGui::SameLine();
          psflive::ui::HelpMarker( _captureData.getDateComment());

          ImGui::Text("Time             : %s", _captureData.getTime().c_str());
          ImGui::SameLine();
          psflive::ui::HelpMarker( _captureData.getTimeComment());

          ImGui::Text("Comment          : %s", _captureData.getComment().c_str());
          ImGui::Text("Number of Frames : %d", _captureData.getFrames());
          ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
      }
      
    }
    ImGui::End(); // ++++ CaptureData window end ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  }
  // ---- un-initialize window ----------------------------------------------------------------------------------------
  if (_captureData.m_initWindow & !_captureData.m_show) {
    psflive::function::LogMessage("ui","CaptureDataWindow()","un-initializing capture window.");
    delete _captureData.m_ptrShader;
    _captureData.m_initWindow = 0;
    psflive::function::LogMessage("ui","CaptureDataWindow()","Capture window un-initialized.");
  }
  // ---- un-initialize window ----------------------------------------------------------------------------------------
}
// ~~~~ CaptureDataWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~










// ~~~~ TestWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::TestWindow(Data& _uiData, AppData& _appData) {
  static bool initWindow = 0;

  if (_uiData.showTestWindow) {
    // static char buttonLabel[64];

    // ---- initialize window -----------------------------------------------------------------------------------------
    if (!initWindow){
      psflive::function::LogMessage("ui","TestWindow()","Initializing test window.");
      initWindow = 1;
      psflive::function::LogMessage("ui","TestWindow()","Test window initialized.");
    }
    // ---- initialize window -----------------------------------------------------------------------------------------
    if (ImGui::Begin("Test", &_uiData.showTestWindow)) { // ++++ test window begin ++++++++++++++++++++++++++++++++++++++

      // ImGui::Separator(); // ==========================================================================================

      // if (_appData.ptrConnection->getState() == psflive::Connection::State::connected) {
      //   if (ImGui::Button("DISCONNECT##TestWindow_DISCONNECT_Button", ImVec2(200,40))) {
      //     _appData.ptrConnection->Disconnecting();
      //   }
      //   _appData.ptrConnection->Communicate();
      // } else if (_appData.ptrConnection->getState() == psflive::Connection::State::connecting) {
      //   // TODO: show attempt on button
      //   // sprintf(buttonLabel, "CONNECTING... %d of %d\n        CANCEL        ##CANCEL_Button", _app.ptrConnection->getAttempt(), 100);
      //   _appData.ptrConnection->Establish(100,100);
      //   if (ImGui::Button("CANCEL##CANCEL_Button", ImVec2(200,40))) {
      //     _appData.ptrConnection->Disconnecting();
      //   }
      // } else if (_appData.ptrConnection->getState() == psflive::Connection::State::disconnected) {
      //   if (ImGui::Button("CONNECT##TestWindow_CONNECT_Button", ImVec2(200,40))) {
      //     _appData.ptrConnection->Connect();
      //   }
      // } else if (_appData.ptrConnection->getState() == psflive::Connection::State::disconnecting) {
      //   ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      //   ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
      //   ImGui::Button("DISCONNECTING...##TestWindow_DISCONNECTING_Button", ImVec2(200,40));
      //   ImGui::PopItemFlag();
      //   ImGui::PopStyleVar();
      //   _appData.ptrConnection->Disconnect();
      // } else if (_appData.ptrConnection->getState() == psflive::Connection::State::busy) {
      //   ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      //   ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
      //   ImGui::Button("BUSY...##TestWindow_BUSY_Button", ImVec2(200,40));
      //   ImGui::PopItemFlag();
      //   ImGui::PopStyleVar();
      //   // consider counting attempts
      // } else {
        
      // }

      // ImGui::Separator(); // ==========================================================================================

    }
    ImGui::End(); // ++++ test window end +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  }
  // ---- un-initialize window ----------------------------------------------------------------------------------------
  if (initWindow & !_uiData.showTestWindow){
    psflive::function::LogMessage("ui","TestWindow()","un-initializing test window.");
    initWindow = 0;
    psflive::function::LogMessage("ui","TestWindow()","Test window un-initialized.");
  }
  // ---- un-initialize window ----------------------------------------------------------------------------------------
}
// ~~~~ TestWindow ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~













// ~~~~ ZMQListener ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::ZMQListener(Data& _uiData, AppData& _appData) {
  psflive::function::LogMessage("ui","ZMQListener()","Listening on : "+_appData.ptrConnection->getAddressPort());

  static Json::Reader jsonReader;
  static Json::Value rxJsonData;
  static std::string rxMessage;
  static Json::StreamWriterBuilder jsonBuilder;
  jsonBuilder["commentStyle"] = "None";
  jsonBuilder["indentation"] = "";

  _appData.ptrConnection->setListen();

  while (_appData.ptrConnection->isListening()) {
    usleep(PSFLIVE_SHORT_SLEEP_US);
    rxMessage = _appData.ptrConnection->Receive();
    if (rxMessage.size() > 0) {
      psflive::function::LogMessage("ui","ZMQListener()","Received : "+rxMessage);
      jsonReader.parse(rxMessage, rxJsonData);

      _uiData.executeCapture = _uiData.showCaptureOperationWindow && (rxJsonData[PSFLIVE_ACTION].asString().compare(PSFLIVE_CAPTURE) == 0);

      usleep(PSFLIVE_SHORT_SLEEP_US);

      if (_uiData.executeCapture) {
        while (_uiData.executeCapture || psflive::CaptureOperation::GETBUSY())
          usleep(PSFLIVE_SHORT_SLEEP_US); // hold in loop till aquisition finishes

        rxJsonData[PSFLIVE_PARAMETER] = _uiData.fileName;
      } else {
        RunAction(rxJsonData, _uiData, _appData);
      }

      _appData.ptrConnection->Send(Json::writeString(jsonBuilder, rxJsonData));

    } else {
      // psflive::function::LogMessage("ui","ZMQListener()","Received 0 bytes");
    }
  }

  _appData.ptrConnection->setListen(false);
  psflive::function::LogMessage("ui","ZMQListener()","Stop listening");
  return;
}
// ~~~~ ZMQListener ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~













// ~~~~ runAction ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void psflive::ui::RunAction(Json::Value& _actionJson, Data& _data, AppData& _appData) {
  if (_actionJson[PSFLIVE_ACTION].asString().compare("getImagerSerialNumber") == 0) {
    _actionJson[PSFLIVE_PARAMETER] = _appData.ptrImager->getCameraInfo().serial;
  } else {
    _actionJson[PSFLIVE_PARAMETER] = "invalid";
    psflive::function::LogMessage("ui","RunAction()","Command not implemented.");
  }
}
// ~~~~ runAction ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~













// ~~~~ NotificationOverlay ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void psflive::ui::NotificationOverlay(Data& _uiData, AppData& _appData) {
  static int corner = 0;
  ImGuiIO& io = ImGui::GetIO();
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
  if (corner != -1) {
    const float PAD = 10.0f;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
    window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
    window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
    window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    window_flags |= ImGuiWindowFlags_NoMove;
  }
  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
  // if (ImGui::Begin("Example: Simple overlay", p_open, window_flags)) {
  //   ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
  //   ImGui::Separator();
  //   if (ImGui::IsMousePosValid())
  //     ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
  //   else
  //     ImGui::Text("Mouse Position: <invalid>");
  //   if (ImGui::BeginPopupContextWindow()) {
  //     if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
  //     if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
  //     if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
  //     if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
  //     if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
  //     if (p_open && ImGui::MenuItem("Close")) *p_open = false;
  //     ImGui::EndPopup();
  //   }
  // }
  // ImGui::End();
}
// ~~~~ NotificationOverlay ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

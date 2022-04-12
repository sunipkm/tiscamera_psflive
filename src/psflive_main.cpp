#include <stdio.h>
#include <string>
#include <iostream>
#include <iterator>
#include <unistd.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui/addons/implot/implot.h"

#include "psflive_device_Imager.h"
#include "psflive_Acquire.h"
#include "psflive_Capture.h"
#include "psflive_Connection.h"
#include "psflive_Configuration.h"
#include "psflive_function.h"
#include "psflive_ui.h"
#include "psflive.h"
// ====================================================================================================================










// ====================================================================================================================
static void glfw_error_callback (int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
// ====================================================================================================================










// ====================================================================================================================
std::vector<CameraInfo> psflive::device::Imager::CAMERAINFOS = {};

bool psflive::AcquireOperation::BUSY = false;
std::time_t psflive::AcquireOperation::CURRENTTIME = std::time(nullptr);
uint psflive::AcquireOperation::CURRENTINDEX = 0;
std::string psflive::AcquireOperation::LOCATION = PSFLIVE_DEFAULT_LOCATION;

zmq::context_t psflive::Connection::CONTEXT = zmq::context_t(1);
// ====================================================================================================================










// ====================================================================================================================
int main (int argc, char** argv) {

  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

  // Decide GL+GLSL versions
  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create window with graphics context
  GLFWwindow* window = glfwCreateWindow(1920, 1200, "PSICo V0.2.0", NULL, NULL);
  if (window == NULL)
    return 1;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return 1;
  }


  psflive::function::LogMessage("Drawing","main()","Starting...");

  // ---- setup gst ---------------------------------------------------------------------------------------------------
  psflive::device::Imager::Initialize(&argc, &argv); // populates psflive::device::Imager::CAMERAINFOS
  // ------------------------------------------------------------------------------------------------------------------

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();

  ImGui::StyleColorsDark(); // Setup Dear ImGui style

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  static psflive::ui::Data uiData;
  static psflive::AppData appData;

  // ---- load configuration ------------------------------------------------------------------------------------------
  appData.ptrConfiguration = new psflive::Configuration();
  appData.ptrConnection = new psflive::Connection();
  // ------------------------------------------------------------------------------------------------------------------

  static std::shared_ptr<psflive::CaptureData> ptrCaptureData;

  // Main loop
  while (!glfwWindowShouldClose(window)) {

    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
 
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
    psflive::ui::MainMenuBar(uiData, appData);

    psflive::ui::DeviceOptionsWindow(uiData, appData);
    psflive::ui::AcquisitionOptionsWindow(uiData, appData);
    psflive::ui::OtherOptionsWindow(uiData, appData);

    psflive::ui::CaptureOperationWindow(uiData, appData);

    for (size_t iDocument = 0; iDocument < uiData.ptrDocuments.size(); iDocument++) {
      if (uiData.ptrDocuments[iDocument]->getAcquireType() == psflive::AcquireType::capture) { // capture
        ptrCaptureData = std::dynamic_pointer_cast<psflive::CaptureData>(uiData.ptrDocuments[iDocument]);
        psflive::ui::CaptureDataWindow(*ptrCaptureData);
      }
      if (!uiData.ptrDocuments[iDocument]->m_show)
        uiData.ptrDocuments.erase(uiData.ptrDocuments.begin()+iDocument);
    }
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  {
    // ==== cleaning up ===============================================================================================
    if (appData.ptrImager != nullptr) {
      appData.ptrImager->Stop();
      delete appData.ptrImager;
    }
    // ----------------------------------------------------------------------------------------------------------------
    if (appData.ptrConfiguration != nullptr) {
      appData.ptrConfiguration->Write();
      delete appData.ptrConfiguration;
    }
    // ----------------------------------------------------------------------------------------------------------------
    if (appData.ptrConnection->isListening())
      appData.ptrConnection->setListen(false);
    if(appData.ptrConnection->m_threadId.joinable())
      appData.ptrConnection->m_threadId.join();
    // ----------------------------------------------------------------------------------------------------------------
    if (appData.ptrConnection != nullptr)
      delete appData.ptrConnection;
    // ----------------------------------------------------------------------------------------------------------------
    for (std::shared_ptr<psflive::AcquireData>& ptrDocument: uiData.ptrDocuments)
      delete &ptrDocument;
    // ----------------------------------------------------------------------------------------------------------------
    psflive::function::LogMessage("Drawing","main()","Exitting...");
    // ================================================================================================================
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();

  ImPlot::DestroyContext();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
// ====================================================================================================================
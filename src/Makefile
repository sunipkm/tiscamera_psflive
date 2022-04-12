CXX = g++
CC = gcc

EXE = psflive
SOURCES = psflive_main.cpp

IMGUI_DIR = imgui
IMGUI_ADDONS_DIR = $(IMGUI_DIR)/addons

SOURCES += psflive_device_Imager.cpp

# abstract class : AcquireData
SOURCES += psflive_CaptureData.cpp

# abstract class : AcquireOperation
SOURCES += psflive_CaptureOperation.cpp

SOURCES += psflive_Configuration.cpp

SOURCES += psflive_Connection.cpp

SOURCES += psflive_function.cpp

SOURCES += psflive_ui.cpp

SOURCES += psflive_main.cpp

# tcamcamera
SOURCES += cameras/tcamcamera/tcamcamera.cpp
# # flicamera
# SOURCES += cameras/flicamera/flicamera.cpp

# imgui
SOURCES += $(IMGUI_ADDONS_DIR)/file_browser/imgui_file_browser.cpp
SOURCES += $(IMGUI_ADDONS_DIR)/canvas/shader/shader.cpp $(IMGUI_ADDONS_DIR)/canvas/imgui_canvas.cpp
SOURCES += $(IMGUI_ADDONS_DIR)/implot/implot.cpp $(IMGUI_ADDONS_DIR)/implot/implot_items.cpp
SOURCES += $(IMGUI_DIR)/backend/imgui_impl_glfw.cpp $(IMGUI_DIR)/backend/imgui_impl_opengl3.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_widgets.cpp $(IMGUI_DIR)/imgui_tables.cpp

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -I$(IMGUI_DIR)/backend/ -I$(IMGUI_DIR)/
CXXFLAGS += -Wall -Wformat -O3
LIBS = -lpthread

# ---- GLAD OPENGL LOADER ---------------------------------------------------------------------------------------------
SOURCES += imgui/backend/libs/glad/src/glad.c
CXXFLAGS += -Iimgui/backend/libs/glad/include -DIMGUI_IMPL_OPENGL_LOADER_GLAD

# ---- LINUX ----------------------------------------------------------------------------------------------------------
ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL `pkg-config --static --libs glfw3` `pkg-config --libs gstreamer-app-1.0` `pkg-config --libs tcam` `pkg-config --libs opencv4` `pkg-config --libs jsoncpp` `pkg-config --static --libs CCfits` `pkg-config --libs libzmq`
	CXXFLAGS += `pkg-config --cflags glfw3` `pkg-config --cflags gstreamer-app-1.0` `pkg-config --cflags tcam` `pkg-config --cflags opencv4` `pkg-config --cflags jsoncpp` `pkg-config --cflags CCfits` `pkg-config --cflags libzmq`
	CFLAGS = $(CXXFLAGS)
endif

CFLAGS_STD = $(CFLAGS) -std=c11
CXXFLAGS_STD = $(CXXFLAGS) -std=c++11

# ---- BUILD RULES ----------------------------------------------------------------------------------------------------
%.o:%.cpp
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

%.o:$(IMGUI_ADDONS_DIR)/canvas/shader/%.cpp $(IMGUI_ADDONS_DIR)/canvas/shader/*.h
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

%.o:$(IMGUI_ADDONS_DIR)/canvas/%.cpp $(IMGUI_ADDONS_DIR)/canvas/*.h
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

%.o:$(IMGUI_ADDONS_DIR)/file_browser/%.cpp $(IMGUI_ADDONS_DIR)/file_browser/*.h
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

%.o:$(IMGUI_ADDONS_DIR)/implot/%.cpp $(IMGUI_ADDONS_DIR)/implot/*.h
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

%.o:cameras/tcamcamera/%.cpp cameras/tcamcamera/*.h
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

%.o:cameras/flicamera/%.cpp cameras/flicamera/*.h
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

%.o:$(IMGUI_DIR)/backend/%.cpp
	$(CXX) $(CXXFLAGS_STD) -c -o $@ $<

%.o:$(IMGUI_DIR)/backend/libs/glad/src/%.c
	$(CC) $(CFLAGS_STD) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)

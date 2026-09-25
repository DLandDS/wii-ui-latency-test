#include <coreinit/time.h>
#include <imgui/imgui.h>
#include <imgui_impl_gx2.h>
#include <imgui_impl_wiiu.h>
#include <whb/log.h>
#include <whb/log_cafe.h>
#include <whb/proc.h>
#include <whb/gfx.h>

#include <cstdint>

struct FrameSnapshot
{
   double fps;
   std::uint64_t framecount;
   std::uint64_t timestamp_ms;
};

int
main()
{
   WHBProcInit();
   WHBLogCafeInit();

   if (!WHBGfxInit()) {
      WHBLogPrintf("Failed to initialize GX2 renderer");
      WHBLogCafeDeinit();
      WHBProcShutdown();
      return 1;
   }

   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO &io = ImGui::GetIO();
   io.IniFilename = nullptr;
   io.DisplaySize = ImVec2(854.0f, 480.0f);
   io.FontGlobalScale = 2.0f;
   ImGui::StyleColorsDark();
   ImGui::GetStyle().ScaleAllSizes(2.0f);

   if (!ImGui_ImplWiiU_Init() || !ImGui_ImplGX2_Init()) {
      WHBLogPrintf("Failed to initialize ImGui Wii U/GX2 backend");
      if (io.BackendPlatformUserData) {
         ImGui_ImplWiiU_Shutdown();
      }
      ImGui::DestroyContext();
      WHBGfxShutdown();
      WHBLogCafeDeinit();
      WHBProcShutdown();
      return 1;
   }

   const OSTime startTime = OSGetSystemTime();
   OSTime measurementStart = startTime;
   std::uint64_t measurementFrame = 0;
   std::uint64_t framecount = 0;
   double fps = 0.0;

   while (WHBProcIsRunning()) {
      const OSTime now = OSGetSystemTime();
      const OSTime measurementTicks = now - measurementStart;

      if (measurementTicks >= static_cast<OSTime>(OSSecondsToTicks(1))) {
         fps = static_cast<double>(framecount - measurementFrame) *
               static_cast<double>(OSTimerClockSpeed) /
               static_cast<double>(measurementTicks);
         measurementStart = now;
         measurementFrame = framecount;
      }

      const FrameSnapshot snapshot{
         fps,
         framecount,
         OSTicksToMilliseconds(now - startTime),
      };
      OSCalendarTime calendarTime;
      OSTicksToCalendarTime(OSGetTime(), &calendarTime);

      const std::uint64_t milliseconds = snapshot.timestamp_ms % 1000;
      const std::uint64_t totalSeconds = snapshot.timestamp_ms / 1000;
      const std::uint64_t seconds = totalSeconds % 60;
      const std::uint64_t totalMinutes = totalSeconds / 60;
      const std::uint64_t minutes = totalMinutes % 60;
      const std::uint64_t hours = totalMinutes / 60;

      ImGui_ImplGX2_NewFrame();
      ImGui::NewFrame();
      ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);

      constexpr ImGuiWindowFlags windowFlags =
         ImGuiWindowFlags_NoTitleBar |
         ImGuiWindowFlags_NoResize |
         ImGuiWindowFlags_NoMove |
         ImGuiWindowFlags_NoScrollbar |
         ImGuiWindowFlags_NoScrollWithMouse |
         ImGuiWindowFlags_NoSavedSettings |
         ImGuiWindowFlags_AlwaysAutoResize;

      ImGui::Begin("latency", nullptr, windowFlags);
      ImGui::Text("fps: %.0f fps", snapshot.fps);
      ImGui::Text("frame count: %llu",
                  static_cast<unsigned long long>(snapshot.framecount));
      ImGui::Text("timestamp: %02llu:%02llu:%02llu.%03llu",
                  static_cast<unsigned long long>(hours),
                  static_cast<unsigned long long>(minutes),
                  static_cast<unsigned long long>(seconds),
                  static_cast<unsigned long long>(milliseconds));
      ImGui::End();
      ImGui::Render();

      WHBGfxBeginRender();

      WHBGfxBeginRenderTV();
      WHBGfxClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      ImGui_ImplGX2_RenderDrawData(ImGui::GetDrawData());
      WHBGfxFinishRenderTV();

      WHBGfxBeginRenderDRC();
      WHBGfxClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      ImGui_ImplGX2_RenderDrawData(ImGui::GetDrawData());
      WHBGfxFinishRenderDRC();

      WHBGfxFinishRender();

      ++framecount;
   }

   ImGui_ImplGX2_Shutdown();
   ImGui_ImplWiiU_Shutdown();
   ImGui::DestroyContext();
   WHBGfxShutdown();
   WHBLogCafeDeinit();
   WHBProcShutdown();
   return 0;
}

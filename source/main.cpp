#include <coreinit/time.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/proc.h>

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
   WHBLogConsoleInit();

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

      WHBLogPrintf("Date: %04d-%02d-%02d %02d:%02d:%02d.%03d",
                   calendarTime.tm_year,
                   calendarTime.tm_mon + 1,
                   calendarTime.tm_mday,
                   calendarTime.tm_hour,
                   calendarTime.tm_min,
                   calendarTime.tm_sec,
                   calendarTime.tm_msec);
      WHBLogPrintf("fps: %.2f | framecount: %llu | timestamp_ms: %llu",
                   snapshot.fps,
                   static_cast<unsigned long long>(snapshot.framecount),
                   static_cast<unsigned long long>(snapshot.timestamp_ms));
      WHBLogConsoleDraw();

      ++framecount;
   }

   WHBLogConsoleFree();
   WHBProcShutdown();
   return 0;
}

with Ada.Real_Time;
with Buffer;
with Buffer.Reader;
with Buffer.Writer;
with Monitor;
with Monitor.MyTestMonitor;

package Test is
   package Buffer_1 is new Buffer(Event_Info_Type => Integer,Buffer_Length => 5);
   package Reader_1 is new Buffer_1.Reader;
   package Writer_1 is new Buffer_1.Writer;

   package Mon_1 is new Monitor(Priority => 10,
                               Period   => Ada.Real_Time.Milliseconds(100));
   package Monitor_Proc_1 is new Mon_1.MyTestMonitor;
end Test;

with Ada.Text_Io;
with Ada.Real_Time;

with Circular_Arrays;
with Buffer;
with Buffer.Reader;
with Buffer.Writer;
with Buffer_Declarations;

with Ada.Task_Identification;

with Monitor;
with Monitor.Order;
with Debug;
--with Monitor.Mon_2;
--with Monitor.Mon_3;

procedure Main_Ex1 is
   package Monitor1 is new Monitor(Period => 1000, Priority => 61);
   package Monitor_One is new Monitor1.Order;

   --package Monitor_Two is new Monitor.Mon_2;
   --package Monitor_Three is new Monitor.Mon_3;
   -- Writer task T1
   task T1 with Priority => 58;
   task body T1 is
      Period : Integer := 1000;
      package T1_Writer is new Buffer_Declarations.Buffer_1.Writer;
      Next_Time : Ada.Real_Time.Time := Ada.Real_Time.Clock;
      Release_Time : constant Ada.Real_Time.Time_Span := Ada.Real_Time.Milliseconds (Period);
      K : Ada.Real_Time.Time;
   begin
      loop
         K := Ada.Real_Time.Clock;
         T1_Writer.Write(Data => 1, TimeStamp => K);
         Ada.Text_Io.Put_Line("Monitor 1 just wrote (" & Debug.Rt_To_String(K) &")");
         Next_Time := Ada.Real_Time."+"(Next_Time,Release_Time);
         delay until Next_Time;
      end loop;
   end T1;

   -- Writer task T1
   task T2 with Priority => 59;
   task body T2 is
      Period : Integer := 1200;
      package T2_Writer is new Buffer_Declarations.Buffer_1.Writer;
      Next_Time : Ada.Real_Time.Time := Ada.Real_Time.Clock;
      Release_Time : constant Ada.Real_Time.Time_Span := Ada.Real_Time.Milliseconds (Period);
      K : Ada.Real_Time.Time;
   begin
      loop
         K := Ada.Real_Time.Clock;
         T2_Writer.Write(Data => 2, TimeStamp => K);
         Ada.Text_Io.Put_Line("Monitor 2 just wrote (" & Debug.Rt_To_String(K) & ")");
         Next_Time := Ada.Real_Time."+"(Next_Time,Release_Time);
         delay until Next_Time;
      end loop;
   end T2;
   -- Writer task T1
   task T3 with Priority => 60;
   task body T3 is
      Period : Integer := 1500;
      package T3_Writer is new Buffer_Declarations.Buffer_1.Writer;
      Next_Time : Ada.Real_Time.Time := Ada.Real_Time.Clock;
      Release_Time : constant Ada.Real_Time.Time_Span := Ada.Real_Time.Milliseconds (Period);
      K : Ada.Real_Time.Time;
   begin
      loop
         K := Ada.Real_Time.Clock;
         T3_Writer.Write(Data => 3, TimeStamp => K);
         Ada.Text_Io.Put_Line("Monitor 3 just wrote (" & Debug.Rt_To_String(K) & ")");
         Next_Time := Ada.Real_Time."+"(Next_Time,Release_Time);
         delay until Next_Time;
      end loop;
   end T3;
begin
   null;
end Main_Ex1;

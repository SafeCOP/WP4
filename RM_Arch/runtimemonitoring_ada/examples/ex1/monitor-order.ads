with Ada.Text_IO;

with Buffer_Declarations;
with Buffer.Reader;
with Debug;
with Monitor;

generic
package Monitor.Order is

--   package tr is new External_Interface;

private
   procedure Mark_Presence;

   --package M2_Reader is new Event_Reader(InputBuffer => Buffer_Declarations.Buffer_2);
   --package M3_Reader is new Event_Reader(InputBuffer => Buffer_Declarations.Buffer_3);
   Cnt : Integer := 0;
   Last : Integer := 0;
   --Cnt2 : Integer := 0;
   --Cnt3 : Integer := 0;

end Monitor.Order;

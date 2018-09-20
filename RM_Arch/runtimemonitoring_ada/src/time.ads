with Ada.Real_Time;

package Time is
   Zero : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(0), Ada.Real_Time.Milliseconds(0));
end Time;

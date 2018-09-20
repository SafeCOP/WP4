with Ada.Real_Time;
with Time;

generic
package Buffer.Reader is

   procedure Read(Index :in Buffer_Index; Data :out Event_Info_Type; Time_Stamp :out Ada.Real_Time.Time);
   procedure Get_Last_Written(Index :out Buffer_Index; Time_Stamp :out Ada.Real_Time.Time);
   procedure Get_Last_Overwritten(Time_Stamp:out Ada.Real_Time.Time);
   procedure Get(Data :out Event_Info_Type; Time_Stamp :out Ada.Real_Time.Time; Is_Empty : out Boolean; Has_Gap : out Boolean);
   procedure Pop(Data :out Event_Info_Type; Time_Stamp :out Ada.Real_Time.Time; Is_Empty : out Boolean; Has_Gap : out Boolean);

private
   Current_Index : Buffer_Index := Buffer_Index'First;
   Last_Read : Ada.Real_Time.Time := Time.Zero;

end Buffer.Reader;

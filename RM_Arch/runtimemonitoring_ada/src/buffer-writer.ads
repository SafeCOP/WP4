with Ada.Real_Time;

generic
package Buffer.Writer is

   Writer_Creation_Exception : exception;

   procedure Write(Data :in Event_Info_Type; Time_Stamp :in Ada.Real_Time.Time);
   procedure Write(Data :in Event_Info_Type);
end Buffer.Writer;

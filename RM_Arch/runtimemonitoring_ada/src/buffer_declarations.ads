--with Ada.Task_Identification;
with Buffer;

package Buffer_Declarations is
   --   package Tid renames Ada.Task_Identification;

   function Taskid_To_String(I :in Integer) return String;
   package Buffer_1 is new Buffer(Event_Info_Type => Integer,Buffer_Length => 100);
   --package Buffer_2 is new Buffer(Event_Info_Type => Integer,Buffer_Length => 100,Lt_Print => Taskid_To_String);
   --package Buffer_3 is new Buffer(Event_Info_Type => Integer,Buffer_Length => 1000,Lt_Print => Taskid_To_String);

end Buffer_Declarations;

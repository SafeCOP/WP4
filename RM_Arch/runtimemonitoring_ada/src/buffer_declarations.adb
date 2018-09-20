package body Buffer_Declarations is

   function Taskid_To_String(I :in Integer) return String is
   begin
      return Integer'Image(I);
   end Taskid_To_String;

end Buffer_Declarations;

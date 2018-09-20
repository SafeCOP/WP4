package body Buffer.Writer is

   procedure Write(Data :in Event_Info_Type; Time_Stamp :in Ada.Real_Time.Time) is
   begin
      -- protect against concurrent accesses
--      Semaphore.Lock;
      -- Push the new data into the buffer
      Push(Data, Time_Stamp);
--      Semaphore.Unlock;
   end Write;

   procedure Write(Data :in Event_Info_Type) is
      Time_Stamp : Ada.Real_Time.Time := Ada.Real_Time.Clock;
   begin
      -- protect against concurrent accesses
--      Semaphore.Lock;
      -- Push the new data into the buffer
      Push(Data, Time_Stamp);
--      Semaphore.Unlock;
   end Write;

begin
   -- check that there is only one writer per buffer
--   if Limited_Write then
   --pragma Assert(Writer_Exists=False);
   if Writer_Exists=True then
      raise Writer_Creation_Exception with "More than one writer associated with this buffer.";
   end if;
   Writer_Exists := True;
--   end if;

end Buffer.Writer;

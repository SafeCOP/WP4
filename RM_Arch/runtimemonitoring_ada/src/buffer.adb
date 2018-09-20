package body Buffer is
   
--     protected body Locker is
--        entry Lock when Is_Locked = False is
--        begin
--           Is_Locked := True;
--        end Lock;
--        entry Unlock when Is_Locked = True is
--        begin
--           Is_Locked := False;
--        end Unlock;
--     end Locker;
   
   
--     function Total_Elements return Natural is
--     begin
--        return Circular_Array.Total_Elements;
--     end Total_Elements;

--     procedure Increment(Index : in out Buffer_Index) is
--        NextIndex : Natural := Natural(Index);
--     begin
--        NextIndex := (NextIndex + 1) mod (Natural(Buffer_Index'Last)+1);
--        Index := Buffer_Index(NextIndex);
--     end Increment;
   
   procedure Increment(Index : in out Buffer_Index) is
   begin
      if Index = Buffer_Index'Last then
         Index := Buffer_Index'First;
      else
         Index := Buffer_Index(Natural(Index) + 1);
      end if;
   end Increment;
   
   function "+"( Index_1 , Index_2 : in Buffer_Index ) return Buffer_Index is
      NewIndex : Natural;
   begin
      NewIndex := (Natural(Index_1) + Natural(Index_2)) mod (Natural(Buffer_Index'Last)+1);
      return Buffer_Index(NewIndex);
   end "+";

   function "-"( Index_1 , Index_2 : in Buffer_Index ) return Buffer_Index is
      NewIndex : Natural := Natural(Index_1);
   begin
      NewIndex := (Natural(Index_1) + Natural(Buffer_Index'Last) + 1 - Natural(Index_2) ) mod (Natural(Buffer_Index'Last)+1);
      return Buffer_Index(NewIndex);
   end "-";


   procedure Update(Flag : in out Status_Flag_Type) is
   begin
      if Flag = Status_Flag_Type'Last then
         Flag := Status_Flag_Type'First;
      else
         Flag := Flag + 1;
      end if;
   end Update;
      
   procedure Read(Index :in Buffer_Index; Data :out Event_Info_Type; Time_Stamp :out Ada.Real_Time.Time) is
      Elem : Buffer_Elements.Pair;
      Status_Flag_Save : Status_Flag_Type;
   begin
      loop
         -- busy wait until no task is writing in the buffer
         while Write_Lock = True loop
            null;
         end loop;

         Status_Flag_Save := Status_Flag;
         Circular_Array.Get(Index => Circular_Array.Buffer_Range(Index), Elem => Elem);

         -- check if there was no writing during the reading that could have 
         -- corrupted the read data
         exit when Status_Flag_Save = Status_Flag and Write_Lock = False;
      end loop;
      
      Data := Buffer_Elements.Get_Data(Elem);
      Time_Stamp := Buffer_Elements.Get_Time(Elem);
   end Read;

   procedure Push(Data : in Event_Info_Type; Time_Stamp : in Ada.Real_Time.Time) is
      Elem : Buffer_Elements.Pair := (Data => Data, Time => Time_Stamp);
      Overwritten_Element : Buffer_Elements.Pair;
   begin
      -- avoid another task to read when we are writing
      Write_Lock := True;
      
      -- Update status variables
      Last_Written_Index := Buffer_Index(Circular_Array.Get_Next_Writing_Position);
      Circular_Array.Get(Circular_Array.Buffer_range(Last_Written_Index), Overwritten_Element);
      Last_Overwritten_Time_Stamp := Overwritten_Element.Time;
      Last_Written_Time_Stamp := Time_Stamp;

      -- Push the new event into the buffer
      Circular_Array.Push(Elem => Elem);      
      
      -- notify that something was written in the buffer
      Update(Status_Flag);
      -- allow reading again
      Write_Lock := False;
   end Push;
   
end Buffer;

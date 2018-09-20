package body Buffer.Reader is

   use Ada.Real_Time;
   
   procedure Read(Index :in Buffer_Index; Data :out Event_Info_Type; Time_Stamp :out Ada.Real_Time.Time) is
      Elem : Buffer_Elements.Pair;
   begin
      Circular_Array.Get(Index => Circular_Array.Buffer_Range(Index), Elem => Elem);
      Data := Buffer_Elements.Get_Data(Elem);
      Time_Stamp := Buffer_Elements.Get_Time(Elem);
   end Read;

   procedure Get_Last_Written(Index :out Buffer_Index; Time_Stamp :out Ada.Real_Time.Time) is
   begin
      Index := Last_Written_Index;
      Time_Stamp := Last_Written_Time_Stamp;
   end Get_Last_Written;

   procedure Get_Last_Overwritten(Time_Stamp:out Ada.Real_Time.Time) is
   begin
      Time_Stamp := Last_Overwritten_Time_Stamp;
   end Get_Last_Overwritten;

   procedure Get(Data :out Event_Info_Type; Time_Stamp :out Ada.Real_Time.Time; Is_Empty : out Boolean; Has_Gap : out Boolean) is     
      Time_Stamp_Save : Ada.Real_Time.Time;
   begin
      Read(Current_Index, Data, Time_Stamp);
      
      if Last_Overwritten_Time_Stamp > Last_Read then
         Has_Gap := True;
         
         -- find the oldest element remaining in the buffer
         Current_Index := Last_Written_Index+1;
         Time_Stamp_Save := Time_Stamp;
         Read(Current_Index, Data, Time_Stamp);
         while Time_Stamp >= Time_Stamp_Save loop
            Time_Stamp_Save := Time_Stamp;
            Increment(Current_Index);
            Read( Current_Index, Data, Time_Stamp);
         end loop;

         -- if there is a gap that means that new elements were written in the 
         -- buffer hence it cannot be empty
         Is_Empty := False; 
         
      else
         Has_Gap := False; 
         if( Last_Read >= Time_Stamp) then
            Is_Empty := True;
         else
            Is_Empty := False;
         end if;
      end if;
   end Get;
   
   procedure Pop(Data :out Event_Info_Type; Time_Stamp :out Ada.Real_Time.Time; Is_Empty : out Boolean; Has_Gap : out Boolean) is
      Time_Stamp_Save : Ada.Real_Time.Time;
   begin
      Read(Current_Index, Data, Time_Stamp);

      if Last_Overwritten_Time_Stamp > Last_Read then
         Has_Gap := True;

         -- find the oldest element remaining in the buffer
         Current_Index := Last_Written_Index+1;
         Time_Stamp_Save := Time_Stamp;
         Read(Current_Index, Data, Time_Stamp);
         while Time_Stamp >= Time_Stamp_Save loop
            Time_Stamp_Save := Time_Stamp;
            Increment(Current_Index);
            Read( Current_Index, Data, Time_Stamp);
         end loop;

         -- if there is a gap that means that new elements were written in the 
         -- buffer hence it cannot be empty
         Is_Empty := False;
         
         Increment(Current_Index);
         Last_Read := Time_Stamp;
      else
         Has_Gap := False; 
         
         if( Last_Read >= Time_Stamp) then
            Is_Empty := True;
         else
            Is_Empty := False;
            Increment(Current_Index);
            Last_Read := Time_Stamp;
         end if;
      end if;
            
   end Pop;

end Buffer.Reader;

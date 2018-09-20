package body Circular_Arrays is
   
   procedure Reset is
   begin
      Current_Index := 0;
   end Reset;

   --
   -- Pushes a new element into the writing position of the buffer. The 
   -- current count of pushed is updated by one unit. 
   --
   procedure Push(Elem :in Element_Type) is
   begin      
      Carrier(Current_Index) := Elem;
      Increment(Current_Index);
      --Count := Count + 1;      
   end Push;
   
   --
   -- Gets the value at position indicated by P
   --
   procedure Get(Index :in Buffer_Range; Elem :out Element_Type) is
   begin
      Elem := Carrier(Index);
   end Get;

   function Get_Next_Writing_Position return Buffer_Range is
   begin
      return Current_Index;
   end Get_Next_Writing_Position;


   procedure Increment(Index : in out Buffer_Range) is
   begin
      if Index = Buffer_Range'Last then
         Index := Buffer_Range'First;
      else
         Index := Index + 1;
      end if;
   end Increment;

end Circular_Arrays;

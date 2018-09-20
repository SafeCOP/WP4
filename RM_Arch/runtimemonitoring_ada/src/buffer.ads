with Ada.Real_Time;
with Pairs;
with Circular_Arrays;
with Ada.Text_IO;
with Time;

-- @summary
-- Generic package instanciating an event buffer
-- @description
-- Each instanciation of this generic package generates a buffer of length 
-- Buffer_Length and storing data of type Event_Info_Type. Each data stored in
-- the buffer is saved with the time stamp at which it was produced. The buffer
-- is a circular array, which means that it is never blocking full. This package
-- assumes that there is only one write attempt in the buffer at any time, i.e.,
-- no concurrent and non-synchronised writing in the buffer.
--
generic
   type Event_Info_Type is private; -- Type of the data that will be saved in the buffer
   Buffer_Length : Positive; -- Length of the circular array used by the buffer
package Buffer is
   --
   -- Type of valid indexes used to access the elements saved in the underlying 
   -- circular array.
   --
   type Buffer_Index is new Natural range 0..(Buffer_Length-1);
   

   -- Increment a Buffer_Index, i.e., Index := Index + 1.
   -- If Index = Buffer_Index'Last when calling the prodcedure then Index is 
   -- set to 0.
   procedure Increment(Index : in out Buffer_Index);
   pragma Inline(Increment);
   
   -- Addition between two Buffer_Index type variables.
   function "+"( Index_1 , Index_2 : in Buffer_Index ) return Buffer_Index;
   pragma Inline("+");
   
   -- Subtraction between two Buffer_Index type variables.
   function "-"( Index_1 , Index_2 : in Buffer_Index ) return Buffer_Index;
   pragma Inline("-");
   
private

   -- Reads the element saved in the buffer at the specified index.
   procedure Read(Index :in Buffer_Index; Data :out Event_Info_Type; Time_Stamp :out Ada.Real_Time.Time);
   
   -- Pushes a new element in the circular buffer. The element is composed of 
   -- Data and the associated time stamp.
   procedure Push(Data : in Event_Info_Type; Time_Stamp : in Ada.Real_Time.Time);
   
   --
   -- Instantiation of pairs (Event_Info_Type, Time_Stamp)
   --
   package Buffer_Elements is new Pairs(Data_Type => Event_Info_Type);
   
   --
   -- Instantiation of the circular array that will serve as circular buffer.
   --
   package Circular_Array is new Circular_Arrays(Element_Type => Buffer_Elements.Pair , Max_Size => Buffer_Length);
   
   --
   -- Last written and last overwritten positions in the circular array
   --
   Last_Written_Index : Buffer_Index := Buffer_Index'Last;
   Last_Written_Time_Stamp : Ada.Real_Time.Time := Time.Zero;
   Last_Overwritten_Time_Stamp : Ada.Real_Time.Time := Time.Zero;

   --
   -- Flags used to ensure an atomic read
   --
   type Status_Flag_Type is new Natural;
   Status_Flag : Status_Flag_Type := Status_Flag_Type'First;
   Write_Lock : Boolean := False;
   pragma Atomic(Write_Lock);
   pragma Atomic(Status_Flag);
   
   procedure Update(Flag : in out Status_Flag_Type);

   --
   -- used to limit the number of writers associated with the buffer to one  
   --
   Writer_Exists : Boolean := False;
   
end Buffer;

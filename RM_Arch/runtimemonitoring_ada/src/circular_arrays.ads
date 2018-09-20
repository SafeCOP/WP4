with Ada.Text_IO;

generic
   -- Generic type of elements
   type Element_Type is private;
   -- Maximum size allowed array size
   Max_Size : in Positive;

package Circular_Arrays is
   -- Type for valid indexes of the buffer
   type Buffer_Range is new Natural range 0..(Max_Size-1);
   -- Public operations
   procedure Reset;
   procedure Push(Elem :in Element_Type);
   procedure Get(Index :in Buffer_Range; Elem :out Element_Type);
   function Get_Next_Writing_Position return Buffer_Range;
   pragma Inline(Get_Next_Writing_Position);

   procedure Increment(Index : in out Buffer_Range);
   pragma Inline(Increment);

   -- Exception
   Abstract_Buffer_Exception : exception;
private
   -- Type of the array of valid buffer indices, of generic element Element_Type
   type Container is array(Buffer_Range) of Element_Type;

   -- Complete (hidden) declaration of the type of circular arrays
   Current_Index : Buffer_Range := 0;
   Carrier : Container;

end Circular_Arrays;

with Ada.Text_Io;
with Ada.Real_Time;
with Time;
use Time;

generic
   type Data_Type is private;
package Pairs is
   
   type Pair is 
      record
         Data : Data_Type;
         Time : Ada.Real_Time.Time := Zero;
      end record;
   
   
   procedure Set_Data(X:out Pair;V:in Data_Type);
   procedure Set_Time(X:out Pair;V:in Ada.Real_Time.Time);
   function Get_Data(X:in Pair) return Data_Type;
   function Get_Time(X:in Pair) return Ada.Real_Time.Time;

--   procedure Print(X:in Pair);
   
end Pairs;

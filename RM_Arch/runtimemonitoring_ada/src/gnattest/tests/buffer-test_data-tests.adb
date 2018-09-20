--  This package has been generated automatically by GNATtest.
--  You are allowed to add your code to the bodies of test routines.
--  Such changes will be kept during further regeneration of this file.
--  All code placed outside of test routine bodies will be lost. The
--  code intended to set up and tear down the test environment should be
--  placed into Buffer.Test_Data.

with AUnit.Assertions; use AUnit.Assertions;

package body Buffer.Test_Data.Tests is


--  begin read only
   procedure Test_Increment (Gnattest_T : in out Test);
   procedure Test_Increment_73b073 (Gnattest_T : in out Test) renames Test_Increment;
--  id:2.2/73b07390e1a3efe0/Increment/1/0/
   procedure Test_Increment (Gnattest_T : in out Test) is
   --  buffer.ads:31:4:Increment
--  end read only

      pragma Unreferenced (Gnattest_T);

      Index : Buffer_Index;

      procedure simple is
      begin
         Index := 0;
         Increment(Index);
         AUnit.Assertions.Assert
           (Condition =>  Index = 1,
            Message   => "Simple increment of buffer_index failed.");
      end simple;

      procedure limit_of_range is
      begin
         Index := Buffer_Index'Last;
         Increment(Index);
         AUnit.Assertions.Assert
           (Condition => Index = 0,
            Message   => "Increment failed when at the limit of range of buffer_index.");
      end limit_of_range;

   begin

         simple;
         limit_of_range;

--  begin read only
   end Test_Increment;
--  end read only


--  begin read only
   procedure Test_Plus (Gnattest_T : in out Test);
   procedure Test_Plus_6c7b64 (Gnattest_T : in out Test) renames Test_Plus;
--  id:2.2/6c7b64214eafd510/Plus/1/0/
   procedure Test_Plus (Gnattest_T : in out Test) is
   --  buffer.ads:35:4:"+"
--  end read only

      pragma Unreferenced (Gnattest_T);

      Input1, Input2, Output : Buffer_Index;

      procedure simple is
      begin
         Input1 := 1;
         Input2 := 2;
         Output := Input1 + Input2;
         AUnit.Assertions.Assert
           (Condition => Output = 3,
            Message   => "Simple addition of two buffer_index failed.");
      end simple;

      procedure limit_of_range is
      begin
         Input1 := Buffer_Index'Last;
         Input2 := 1;
         Output := Input1 + Input2;
         AUnit.Assertions.Assert
           (Condition => Output = 0,
            Message   => "Addition of two buffer_index failed when at the limit of range.");
      end limit_of_range;

      procedure limit_of_range2 is
      begin
         Input1 := Buffer_Index'Last;
         Input2 := Buffer_Index'Last;
         Output := Input1 + Input2;
         AUnit.Assertions.Assert
           (Condition => Output = Buffer_Index'Last - 1,
            Message   => "Addition of two buffer_index failed when passed the limit of range.");
      end limit_of_range2;

   begin

      simple;
      limit_of_range;
      limit_of_range2;

--  begin read only
   end Test_Plus;
--  end read only


--  begin read only
   procedure Test_Minus (Gnattest_T : in out Test);
   procedure Test_Minus_a4e864 (Gnattest_T : in out Test) renames Test_Minus;
--  id:2.2/a4e8640c6d826d76/Minus/1/0/
   procedure Test_Minus (Gnattest_T : in out Test) is
   --  buffer.ads:39:4:"-"
--  end read only

      pragma Unreferenced (Gnattest_T);

      Input1, Input2, Output : Buffer_Index;

      procedure simple is
      begin
         Input1 := 2;
         Input2 := 1;
         Output := Input1 - Input2;
         AUnit.Assertions.Assert
           (Condition => Output = 1,
            Message   => "Simple subtraction of two buffer_index failed.");
      end simple;

      procedure limit_of_range is
      begin
         Input1 := 0;
         Input2 := 1;
         Output := Input1 - Input2;
         AUnit.Assertions.Assert
           (Condition => Output = Buffer_Index'Last,
            Message   => "Addition of two buffer_index failed when at the limit of range.");
      end limit_of_range;

      procedure limit_of_range2 is
      begin
         Input1 := 1;
         Input2 := Buffer_Index'Last;
         Output := Input1 - Input2;
         AUnit.Assertions.Assert
           (Condition => Output = 2,
            Message   => "Addition of two buffer_index failed when passed the limit of range.");
      end limit_of_range2;

   begin

      simple;
      limit_of_range;
      limit_of_range2;

--  begin read only
   end Test_Minus;
--  end read only


--  begin read only
   procedure Test_Read (Gnattest_T : in out Test);
   procedure Test_Read_412d0f (Gnattest_T : in out Test) renames Test_Read;
--  id:2.2/412d0f6e45aa9505/Read/1/0/
   procedure Test_Read (Gnattest_T : in out Test) is
   --  buffer.ads:45:4:Read
--  end read only

      pragma Unreferenced (Gnattest_T);

      use Ada.Real_Time;

      DataInserted : Buffer_Elements.Pair;
      Data, D1 : Event_Info_Type;
      Time_Stamp : Ada.Real_Time.Time;

      T1 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(0), Ada.Real_Time.Milliseconds(100));
      T2 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(0), Ada.Real_Time.Milliseconds(212));
      T3 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(0), Ada.Real_Time.Milliseconds(363));

      procedure simple is
      begin
         DataInserted.Time := T1;
         D1 := DataInserted.Data;
         Circular_Array.Push(Elem => DataInserted);
         DataInserted.Time := T2;
         Circular_Array.Push(Elem => DataInserted);
         DataInserted.Time := T3;
         Circular_Array.Push(Elem => DataInserted);
         Read(Index      => 1,
              Data       => Data,
              Time_Stamp => Time_Stamp);
         AUnit.Assertions.Assert
           (Condition => D1 = Data and Time_Stamp = T2,
            Message => "Buffer failed to read an element in the cisruclar array.");
         Read(Index      => 0,
              Data       => Data,
              Time_Stamp => Time_Stamp);
         AUnit.Assertions.Assert
           (Condition => D1 = Data and Time_Stamp = T1,
            Message => "Buffer failed to read an element in the cisruclar array.");
         Read(Index      => 2,
              Data       => Data,
              Time_Stamp => Time_Stamp);
         AUnit.Assertions.Assert
           (Condition => D1 = Data and Time_Stamp = T3,
            Message => "Buffer failed to read an element in the cisruclar array.");
      end simple;


   begin

      simple;

--  begin read only
   end Test_Read;
--  end read only


--  begin read only
   procedure Test_Push (Gnattest_T : in out Test);
   procedure Test_Push_b0cb78 (Gnattest_T : in out Test) renames Test_Push;
--  id:2.2/b0cb78e875f524d1/Push/1/0/
   procedure Test_Push (Gnattest_T : in out Test) is
   --  buffer.ads:49:4:Push
--  end read only

      pragma Unreferenced (Gnattest_T);

      use Ada.Real_Time;

      Data_In : Event_Info_Type;
      Time_Stamp_In : Ada.Real_Time.Time;
      T1 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(0), Ada.Real_Time.Milliseconds(525));
      T2 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(10), Ada.Real_Time.Milliseconds(754));

      Elem : Buffer_Elements.Pair;

      procedure simple is
      begin
         Time_Stamp_In := T1;
         Push(Data_In, Time_Stamp_In);

         AUnit.Assertions.Assert
           (Last_Written_Index = 0,
            "Push wrongly updated the last_written_index. Last_Written_Index is = " & Buffer_Index'Image(Last_Written_Index) & " instead of 0.");
         AUnit.Assertions.Assert
           (Last_Written_Time_Stamp = T1,
            "Push wrongly updated the last_written_time_stamp.");

         Circular_Array.Get(0, Elem);
         AUnit.Assertions.Assert
           (Elem.Time = T1 and Elem.Data = Data_In,
            "Push did not write properly in the circular buffer.");

      end simple;

      procedure full_round is
      begin
         Time_Stamp_In := T1;
         -- we make a full turn, hence the last place we should write is at index 0
         for I in 0..Buffer_Index'Last loop
            Push(Data_In, Time_Stamp_In);
         end loop;

         Time_Stamp_In := T2;
         Push(Data_In, Time_Stamp_In);
         -- we should be at index 1 now

         AUnit.Assertions.Assert
           (Last_Written_Index = 1,
            "Push wrongly updated the last_written_index after full round. Last_Written_Index is = " & Buffer_Index'Image(Last_Written_Index) & " instead of 1.");
         AUnit.Assertions.Assert
           (Last_Written_Time_Stamp = T2,
            "Push wrongly updated the last_written_time_stamp after full round.");

         Circular_Array.Get(0, Elem);
         AUnit.Assertions.Assert
           (Elem.Time = T1  and Elem.Data = Data_In,
            "Push did not write properly in the circular buffer after full round.");

         Circular_Array.Get(1, Elem);
         AUnit.Assertions.Assert
           (Elem.Time = T2  and Elem.Data = Data_In,
            "Push did not write properly in the circular buffer after full round.");

         for I in 2..Buffer_Index'Last - 1 loop
            Circular_Array.Get(Circular_Array.Buffer_Range(I), Elem);
            AUnit.Assertions.Assert
              (Elem.Time = T1 and Elem.Data = Data_In,
               "Push did not write properly in the circular buffer after full round.");
         end loop;

      end full_round;

   begin

      simple;
      full_round;

--  begin read only
   end Test_Push;
--  end read only


--  begin read only
   procedure Test_Update (Gnattest_T : in out Test);
   procedure Test_Update_7f75ad (Gnattest_T : in out Test) renames Test_Update;
--  id:2.2/7f75ad9633841ccc/Update/1/0/
   procedure Test_Update (Gnattest_T : in out Test) is
   --  buffer.ads:89:4:Update
--  end read only

      pragma Unreferenced (Gnattest_T);

      flag : Status_Flag_Type;

      procedure simple is
      begin
         flag := 0;
         Update(Flag);
         AUnit.Assertions.Assert
           (Flag = 1,
            "Failed to update Flag.");
      end simple;

      procedure limit_of_range is
      begin
         flag := Status_Flag_Type'Last;
         Update(Flag);
         AUnit.Assertions.Assert
           (Flag = 0,
            "Failed to update Flag.");
      end limit_of_range;

   begin

      simple;
      limit_of_range;

--  begin read only
   end Test_Update;
--  end read only

end Buffer.Test_Data.Tests;

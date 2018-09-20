--  This package has been generated automatically by GNATtest.
--  You are allowed to add your code to the bodies of test routines.
--  Such changes will be kept during further regeneration of this file.
--  All code placed outside of test routine bodies will be lost. The
--  code intended to set up and tear down the test environment should be
--  placed into Buffer.Writer.Test_Data.

with AUnit.Assertions; use AUnit.Assertions;

package body Buffer.Writer.Test_Data.Tests is


--  begin read only
   procedure Test_1_Write (Gnattest_T : in out Test);
   procedure Test_Write_ebc478 (Gnattest_T : in out Test) renames Test_1_Write;
--  id:2.2/ebc478db02553508/Write/1/0/
   procedure Test_1_Write (Gnattest_T : in out Test) is
   --  buffer-writer.ads:8:4:Write
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
         Write(Data_In, Time_Stamp_In);

         AUnit.Assertions.Assert
           (Last_Written_Index = 0,
            "Write(2 args) wrongly updated the last_written_index. Last_Written_Index is = " & Buffer_Index'Image(Last_Written_Index) & " instead of 0.");
         AUnit.Assertions.Assert
           (Last_Written_Time_Stamp = T1,
            "Write(2 args) wrongly updated the last_written_time_stamp.");

         Circular_Array.Get(0, Elem);
         AUnit.Assertions.Assert
           (Elem.Time = T1 and Elem.Data = Data_In,
            "Write(2 args) did not write properly in the circular buffer.");

      end simple;

      procedure full_round is
      begin
         Time_Stamp_In := T1;
         -- we make a full turn, hence the last place we should write is at index 0
         for I in 0..Buffer_Index'Last loop
            Write(Data_In, Time_Stamp_In);
         end loop;

         Time_Stamp_In := T2;
         Write(Data_In, Time_Stamp_In);
         -- we should be at index 1 now

         AUnit.Assertions.Assert
           (Last_Written_Index = 1,
            "Write(2 args) wrongly updated the last_written_index after full round. Last_Written_Index is = " & Buffer_Index'Image(Last_Written_Index) & " instead of 1.");
         AUnit.Assertions.Assert
           (Last_Written_Time_Stamp = T2,
            "Write(2 args) wrongly updated the last_written_time_stamp after full round.");

         Circular_Array.Get(0, Elem);
         AUnit.Assertions.Assert
           (Elem.Time = T1  and Elem.Data = Data_In,
            "Write(2 args) did not write properly in the circular buffer after full round.");

         Circular_Array.Get(1, Elem);
         AUnit.Assertions.Assert
           (Elem.Time = T2  and Elem.Data = Data_In,
            "Write(2 args) did not write properly in the circular buffer after full round.");

         for I in 2..Buffer_Index'Last - 1 loop
            Circular_Array.Get(Circular_Array.Buffer_Range(I), Elem);
            AUnit.Assertions.Assert
              (Elem.Time = T1 and Elem.Data = Data_In,
               "Write(2 args) did not write properly in the circular buffer after full round.");
         end loop;

      end full_round;

   begin

      simple;
      full_round;

--  begin read only
   end Test_1_Write;
--  end read only


--  begin read only
   procedure Test_2_Write (Gnattest_T : in out Test);
   procedure Test_Write_3e9010 (Gnattest_T : in out Test) renames Test_2_Write;
--  id:2.2/3e9010a5045bfc33/Write/0/0/
   procedure Test_2_Write (Gnattest_T : in out Test) is
   --  buffer-writer.ads:9:4:Write
--  end read only

      pragma Unreferenced (Gnattest_T);

      use Ada.Real_Time;

      Data_In : Event_Info_Type;
      Time_Stamp_In, Time_Stamp_1, Time_Stamp_2 : Ada.Real_Time.Time;
      T1 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(0), Ada.Real_Time.Milliseconds(100));

      Elem : Buffer_Elements.Pair;

      procedure simple is
      begin
         Time_Stamp_1 := Ada.Real_Time.Clock;
         Write(Data_In);
         Time_Stamp_2 := Ada.Real_Time.Clock;

         AUnit.Assertions.Assert
           (Last_Written_Index = 0,
            "Write(1 arg) wrongly updated the last_written_index. Last_Written_Index is = " & Buffer_Index'Image(Last_Written_Index) & " instead of 0.");
         AUnit.Assertions.Assert
           (Last_Written_Time_Stamp >= Time_Stamp_1 and Last_Written_Time_Stamp <= Time_Stamp_2,
            "Write(1 arg) wrongly updated the last_written_time_stamp.");

         Circular_Array.Get(0, Elem);
         AUnit.Assertions.Assert
           (Elem.Time >= Time_Stamp_1 and Elem.Time <= Time_Stamp_2 and Elem.Data = Data_In,
            "Write(1 arg) did not write properly in the circular buffer.");

      end simple;

      procedure full_round is
      begin
         Time_Stamp_In := T1;
         -- we make a full turn, hence the last place we should write is at index 0
         for I in 0..Buffer_Index'Last loop
            Write(Data_In, Time_Stamp_In);
         end loop;

         Time_Stamp_1 := Ada.Real_Time.Clock;
         Write(Data_In);
         Time_Stamp_2 := Ada.Real_Time.Clock;
         -- we should be at index 1 now

         AUnit.Assertions.Assert
           (Last_Written_Index = 1,
            "Write(1 arg) wrongly updated the last_written_index after full round. Last_Written_Index is = " & Buffer_Index'Image(Last_Written_Index) & " instead of 1.");
         AUnit.Assertions.Assert
           (Last_Written_Time_Stamp >= Time_Stamp_1 and Last_Written_Time_Stamp <= Time_Stamp_2,
            "Write(1 arg) wrongly updated the last_written_time_stamp after full round.");

         Circular_Array.Get(0, Elem);
         AUnit.Assertions.Assert
           (Elem.Time = T1  and Elem.Data = Data_In,
            "Write(1 arg) did not write properly in the circular buffer after full round.");

         Circular_Array.Get(1, Elem);
         AUnit.Assertions.Assert
           (Elem.Time >= Time_Stamp_1 and Elem.Time <= Time_Stamp_2  and Elem.Data = Data_In,
            "Write(1 arg) did not write properly in the circular buffer after full round.");

         for I in 2..Buffer_Index'Last - 1 loop
            Circular_Array.Get(Circular_Array.Buffer_Range(I), Elem);
            AUnit.Assertions.Assert
              (Elem.Time = T1 and Elem.Data = Data_In,
               "Write(1 arg) did not write properly in the circular buffer after full round.");
         end loop;

      end full_round;

   begin

      simple;
      full_round;

--  begin read only
   end Test_2_Write;
--  end read only

end Buffer.Writer.Test_Data.Tests;

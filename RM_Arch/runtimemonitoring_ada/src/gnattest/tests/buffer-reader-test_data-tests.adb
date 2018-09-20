--  This package has been generated automatically by GNATtest.
--  You are allowed to add your code to the bodies of test routines.
--  Such changes will be kept during further regeneration of this file.
--  All code placed outside of test routine bodies will be lost. The
--  code intended to set up and tear down the test environment should be
--  placed into Buffer.Reader.Test_Data.

with AUnit.Assertions; use AUnit.Assertions;

package body Buffer.Reader.Test_Data.Tests is


--  begin read only
   procedure Test_Read (Gnattest_T : in out Test);
   procedure Test_Read_412d0f (Gnattest_T : in out Test) renames Test_Read;
--  id:2.2/412d0f6e45aa9505/Read/1/0/
   procedure Test_Read (Gnattest_T : in out Test) is
   --  buffer-reader.ads:7:4:Read
--  end read only

      pragma Unreferenced (Gnattest_T);

      use Ada.Real_Time;

      Element_To_Insert : Buffer_Elements.Pair;
      Data, Data_Inserted : Event_Info_Type;
      Time_Stamp : Ada.Real_Time.Time;

      T1 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(0), Ada.Real_Time.Milliseconds(100));
      T2 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(0), Ada.Real_Time.Milliseconds(212));
      T3 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(0), Ada.Real_Time.Milliseconds(363));

      procedure simple is
      begin
         Element_To_Insert.Time := T1;
         Data_Inserted := Element_To_Insert.Data;
         Circular_Array.Push(Elem => Element_To_Insert);
         Element_To_Insert.Time := T2;
         Circular_Array.Push(Elem => Element_To_Insert);
         Element_To_Insert.Time := T3;
         Circular_Array.Push(Elem => Element_To_Insert);

         Read(Index      => 1,
              Data       => Data,
              Time_Stamp => Time_Stamp);
         AUnit.Assertions.Assert
           (Condition => Data = Data_Inserted and Time_Stamp = T2,
            Message => "Reader failed to read an element in the buffer.");
         Read(Index      => 0,
              Data       => Data,
              Time_Stamp => Time_Stamp);
         AUnit.Assertions.Assert
           (Condition => Data = Data_Inserted and Time_Stamp = T1,
            Message => "Reader failed to read an element in the buffer.");
         Read(Index      => 2,
              Data       => Data,
              Time_Stamp => Time_Stamp);
         AUnit.Assertions.Assert
           (Condition => Data = Data_Inserted and Time_Stamp = T3,
            Message => "Reader failed to read an element in the buffer.");
      end simple;

   begin

      simple;

--  begin read only
   end Test_Read;
--  end read only


--  begin read only
   procedure Test_Get_Last_Written (Gnattest_T : in out Test);
   procedure Test_Get_Last_Written_40186d (Gnattest_T : in out Test) renames Test_Get_Last_Written;
--  id:2.2/40186d8c2352141d/Get_Last_Written/1/0/
   procedure Test_Get_Last_Written (Gnattest_T : in out Test) is
   --  buffer-reader.ads:8:4:Get_Last_Written
--  end read only

      pragma Unreferenced (Gnattest_T);

      use Ada.Real_Time;

      Data_Inserted : Event_Info_Type;
      Index : Buffer_Index;
      Time_Stamp : Ada.Real_Time.Time;
      T1 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(23), Ada.Real_Time.Milliseconds(657));
      T2 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(42), Ada.Real_Time.Milliseconds(123));
      T3 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(1), Ada.Real_Time.Milliseconds(001));

      procedure simple is
      begin
         Push(Data       => Data_Inserted,
              Time_Stamp => T1);
         Get_Last_Written(Index, Time_Stamp);
         AUnit.Assertions.Assert
           ( Index = 0 and Time_Stamp = T1,
             "Reader failed at sending the last written time stamp and the associated buffer index. Sends index " & Buffer_Index'Image(Index) & " instead of 0.");

         Push(Data       => Data_Inserted,
              Time_Stamp => T2);
         Get_Last_Written(Index, Time_Stamp);
         AUnit.Assertions.Assert
           ( Index = 1 and Time_Stamp = T2,
             "Reader failed at sending the last written time stamp and the associated buffer index. Sends index " & Buffer_Index'Image(Index) & " instead of 1.");
      end simple;

      procedure turn_around is
      begin
         for I in 0..Buffer_Index'Last-1 loop
            Push(Data       => Data_Inserted,
                 Time_Stamp => T1);
         end loop;
         Push(Data       => Data_Inserted,
              Time_Stamp => T3);

         Get_Last_Written(Index, Time_Stamp);
         AUnit.Assertions.Assert
           ( Index = 1 and Time_Stamp = T3,
            "Reader failed at sending the last written time stamp and the associated buffer index when turning around the buffer. Sends index " & Buffer_Index'Image(Index) & " instead of 1.");

      end turn_around;

   begin

      simple;
      turn_around;

--  begin read only
   end Test_Get_Last_Written;
--  end read only


--  begin read only
   procedure Test_Get_Last_Overwritten (Gnattest_T : in out Test);
   procedure Test_Get_Last_Overwritten_419e17 (Gnattest_T : in out Test) renames Test_Get_Last_Overwritten;
--  id:2.2/419e1759b9349b99/Get_Last_Overwritten/1/0/
   procedure Test_Get_Last_Overwritten (Gnattest_T : in out Test) is
   --  buffer-reader.ads:9:4:Get_Last_Overwritten
--  end read only

      pragma Unreferenced (Gnattest_T);

      use Debug;
      use Ada.Real_Time;

      Data_Inserted : Event_Info_Type;
      Time_Stamp : Ada.Real_Time.Time;
      T1 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(45), Ada.Real_Time.Milliseconds(111));
      T2 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(0), Ada.Real_Time.Milliseconds(567));
      T3 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(6), Ada.Real_Time.Milliseconds(999));

      procedure turn_around is
      begin
         for I in 0..Buffer_Index'Last loop
            Push(Data       => Data_Inserted,
                 Time_Stamp => T1);
         end loop;
         Push(Data       => Data_Inserted,
              Time_Stamp => T2);

         Get_Last_Overwritten(Time_Stamp);
         AUnit.Assertions.Assert
           ( Time_Stamp = T1,
            "Reader failed at sending the last overwritten time stamp (test 1). Sends " & Rt_To_String(Time_Stamp) & " instead of " & Rt_To_String(T1));

         for I in 0..Buffer_Index'Last loop
            Push(Data       => Data_Inserted,
                 Time_Stamp => T3);
         end loop;

         Get_Last_Overwritten(Time_Stamp);
         AUnit.Assertions.Assert
           ( Time_Stamp = T2,
            "Reader failed at sending the last overwritten time stamp (test 2). Sends " & Rt_To_String(Time_Stamp) & " instead of " & Rt_To_String(T2));

      end turn_around;

   begin

      turn_around;

--  begin read only
   end Test_Get_Last_Overwritten;
--  end read only


--  begin read only
   procedure Test_Get (Gnattest_T : in out Test);
   procedure Test_Get_b58e38 (Gnattest_T : in out Test) renames Test_Get;
--  id:2.2/b58e38ad8ab6ea87/Get/1/0/
   procedure Test_Get (Gnattest_T : in out Test) is
   --  buffer-reader.ads:10:4:Get
--  end read only

      pragma Unreferenced (Gnattest_T);

      use Ada.Real_Time;

      Data_Inserted, Data : Event_Info_Type;
      Time_Stamp : Ada.Real_Time.Time;
      Is_Empty : Boolean;
      Has_Gap : Boolean;

      T1 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(1), Ada.Real_Time.Milliseconds(0));
      T2 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(2), Ada.Real_Time.Milliseconds(67));
      T3 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(66), Ada.Real_Time.Milliseconds(098));
      T4 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(77), Ada.Real_Time.Milliseconds(098));
      T5 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(78), Ada.Real_Time.Milliseconds(098));

      procedure empty is
      begin
         Get(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = True,
             "Get failed at detecting an empty buffer (test 1).");
      end empty;

      procedure complete is
      begin
         Push(Data       => Data_Inserted,
              Time_Stamp => T1);
         Push(Data       => Data_Inserted,
              Time_Stamp => T2);

         Get(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = False and Has_Gap = False and Data = Data_Inserted and Time_Stamp = T1,
             "Get failed at getting the first element in the buffer (test 2).");

         Get(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = False and Has_Gap = False and Data = Data_Inserted and Time_Stamp = T1,
             "Get failed at getting a second time the first element in the buffer (test 2).");

         Pop(Data, Time_Stamp, Is_Empty, Has_Gap);
         Get(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = False and Has_Gap = False and Data = Data_Inserted and Time_Stamp = T2,
             "Get failed at getting the second element in the buffer (test 2).");

         Get(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = False and Has_Gap = False and Data = Data_Inserted and Time_Stamp = T2,
             "Get failed at getting a second time the second element in the buffer (test 2).");

         Pop(Data, Time_Stamp, Is_Empty, Has_Gap);
         Get(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = True,
             "Get failed at detecting an empty buffer (test 2).");

         Push(Data       => Data_Inserted,
              Time_Stamp => T3);

         Get(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = False and Has_Gap = False and Data = Data_Inserted and Time_Stamp = T3,
             "Get failed at getting the third element in the buffer (test 2).");

         Push(Data       => Data_Inserted,
              Time_Stamp => T4);
         for I in 0..Buffer_Index'Last-1 loop
            Push(Data       => Data_Inserted,
                 Time_Stamp => T5);
         end loop;
         Get(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Has_Gap = True,
             "Get failed at detecting a gap (test 2).");
         AUnit.Assertions.Assert
           ( Is_Empty = False and Has_Gap = True and Data = Data_Inserted and Time_Stamp = T4,
             "Get failed at getting the first element after the one that was overwritten in the buffer (test 2).");

      end complete;


   begin

      empty;
      complete;

--  begin read only
   end Test_Get;
--  end read only


--  begin read only
   procedure Test_Pop (Gnattest_T : in out Test);
   procedure Test_Pop_2a7df9 (Gnattest_T : in out Test) renames Test_Pop;
--  id:2.2/2a7df91223e9eec6/Pop/1/0/
   procedure Test_Pop (Gnattest_T : in out Test) is
   --  buffer-reader.ads:11:4:Pop
--  end read only

      pragma Unreferenced (Gnattest_T);

      use Ada.Real_Time;

      Data_Inserted, Data : Event_Info_Type;
      Time_Stamp : Ada.Real_Time.Time;
      Is_Empty : Boolean;
      Has_Gap : Boolean;

      T1 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(5), Ada.Real_Time.Milliseconds(0));
      T2 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(6), Ada.Real_Time.Milliseconds(67));
      T3 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(23), Ada.Real_Time.Milliseconds(098));
      T4 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(32), Ada.Real_Time.Milliseconds(098));
      T5 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(33), Ada.Real_Time.Milliseconds(098));
      T6 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(34), Ada.Real_Time.Milliseconds(098));
      T7 : constant Ada.Real_Time.Time := Ada.Real_Time.Time_Of(Ada.Real_Time.Seconds_Count(36), Ada.Real_Time.Milliseconds(098));

      procedure empty is
      begin
         Pop(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = True,
             "Pop failed at detecting an empty buffer (test 1).");
      end empty;

      procedure complete is
      begin
         Push(Data       => Data_Inserted,
              Time_Stamp => T1);
         Push(Data       => Data_Inserted,
              Time_Stamp => T2);

         Pop(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = False and Has_Gap = False and Data = Data_Inserted and Time_Stamp = T1,
             "Pop failed at getting the first element in the buffer (test 2).");

         Pop(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = False and Has_Gap = False and Data = Data_Inserted and Time_Stamp = T2,
             "Pop failed at getting the second element in the buffer (test 2).");

         Pop(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = True,
             "Pop failed at detecting an empty buffer (test 2).");

         Push(Data       => Data_Inserted,
              Time_Stamp => T3);

         Pop(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = False and Has_Gap = False and Data = Data_Inserted and Time_Stamp = T3,
             "Pop failed at getting the third element in the buffer (test 2).");

         for I in 0..Buffer_Index'Last loop
            Push(Data       => Data_Inserted,
                 Time_Stamp => T4);
         end loop;
         Pop(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Is_Empty = False and Has_Gap = False and Data = Data_Inserted and Time_Stamp = T4,
           "Pop failed at getting the fourth element in the buffer (test 2).");

         Push(Data       => Data_Inserted,
              Time_Stamp => T5);
         Push(Data       => Data_Inserted,
              Time_Stamp => T6);
         for I in 0..Buffer_Index'Last-1 loop
            Push(Data       => Data_Inserted,
                 Time_Stamp => T7);
         end loop;
         Pop(Data, Time_Stamp, Is_Empty, Has_Gap);
         AUnit.Assertions.Assert
           ( Has_Gap = True,
             "Pop failed at detecting a gap (test 2).");
         AUnit.Assertions.Assert
           ( Is_Empty = False and Has_Gap = True and Data = Data_Inserted and Time_Stamp = T6,
           "Pop failed at getting the first element after the overwritten one in the buffer (test 2).");

      end complete;


   begin

      empty;
      complete;

--  begin read only
   end Test_Pop;
--  end read only

end Buffer.Reader.Test_Data.Tests;

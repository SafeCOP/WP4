--  This package is intended to set up and tear down  the test environment.
--  Once created by GNATtest, this package will never be overwritten
--  automatically. Contents of this package can be modified in any way
--  except for sections surrounded by a 'read only' marker.

package body Buffer.Reader.Test_Data is

   procedure Set_Up (Gnattest_T : in out Test) is
      X : Test'Class renames Test'Class (Gnattest_T);
   begin
      Circular_Array.Reset;
      Current_Index := Buffer_Index'First;
      Last_Read := Time.Zero;
      Last_Overwritten_Time_Stamp := Time.Zero;
      X.User_Set_Up;
   end Set_Up;

   procedure Tear_Down (Gnattest_T : in out Test) is
      X : Test'Class renames Test'Class (Gnattest_T);
      Data : Event_Info_Type;
   begin
      for I in 0..Buffer_Index'Last loop
         Push
           ( Data       => Data,
             Time_Stamp => Time.Zero);
      end loop;
      X.User_Tear_Down;
   end Tear_Down;

end Buffer.Reader.Test_Data;

--  This package has been generated automatically by GNATtest.
--  You are allowed to add your code to the bodies of test routines.
--  Such changes will be kept during further regeneration of this file.
--  All code placed outside of test routine bodies will be lost. The
--  code intended to set up and tear down the test environment should be
--  placed into Debug.Test_Data.

with AUnit.Assertions; use AUnit.Assertions;

package body Debug.Test_Data.Tests is


--  begin read only
   procedure Test_Rt_To_String (Gnattest_T : in out Test);
   procedure Test_Rt_To_String_8d15be (Gnattest_T : in out Test) renames Test_Rt_To_String;
--  id:2.2/8d15be8ecd519ae0/Rt_To_String/1/0/
   procedure Test_Rt_To_String (Gnattest_T : in out Test) is
   --  debug.ads:6:4:Rt_To_String
--  end read only

      pragma Unreferenced (Gnattest_T);

   begin

      AUnit.Assertions.Assert
        (True,
         "Test not implemented.");

--  begin read only
   end Test_Rt_To_String;
--  end read only

end Debug.Test_Data.Tests;

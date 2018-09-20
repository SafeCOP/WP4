--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body Test.Test_Data.Tests.Writer_1.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.Test.Test_Data.Tests.Writer_1.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_Write_ebc478 : aliased Runner_1.Test_Case;
   Case_2_1_Test_Write_3e9010 : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_Write_ebc478,
         "buffer-writer.ads:8:4 instance at test.ads:11:4:",
         Test_Write_ebc478'Access);
      Runner_1.Create
        (Case_2_1_Test_Write_3e9010,
         "buffer-writer.ads:9:4 instance at test.ads:11:4:",
         Test_Write_3e9010'Access);

      Result.Add_Test (Case_1_1_Test_Write_ebc478'Access);
      Result.Add_Test (Case_2_1_Test_Write_3e9010'Access);

      return Result'Access;

   end Suite;

end Test.Test_Data.Tests.Writer_1.Test_Data.Tests.Suite;
--  end read only

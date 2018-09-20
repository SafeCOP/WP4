--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body Test.Test_Data.Tests.Buffer_1.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.Test.Test_Data.Tests.Buffer_1.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_Increment_73b073 : aliased Runner_1.Test_Case;
   Case_2_1_Test_Plus_6c7b64 : aliased Runner_1.Test_Case;
   Case_3_1_Test_Minus_a4e864 : aliased Runner_1.Test_Case;
   Case_4_1_Test_Read_412d0f : aliased Runner_1.Test_Case;
   Case_5_1_Test_Push_b0cb78 : aliased Runner_1.Test_Case;
   Case_6_1_Test_Update_7f75ad : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_Increment_73b073,
         "buffer.ads:31:4 instance at test.ads:9:4:",
         Test_Increment_73b073'Access);
      Runner_1.Create
        (Case_2_1_Test_Plus_6c7b64,
         "buffer.ads:35:4 instance at test.ads:9:4:",
         Test_Plus_6c7b64'Access);
      Runner_1.Create
        (Case_3_1_Test_Minus_a4e864,
         "buffer.ads:39:4 instance at test.ads:9:4:",
         Test_Minus_a4e864'Access);
      Runner_1.Create
        (Case_4_1_Test_Read_412d0f,
         "buffer.ads:45:4 instance at test.ads:9:4:",
         Test_Read_412d0f'Access);
      Runner_1.Create
        (Case_5_1_Test_Push_b0cb78,
         "buffer.ads:49:4 instance at test.ads:9:4:",
         Test_Push_b0cb78'Access);
      Runner_1.Create
        (Case_6_1_Test_Update_7f75ad,
         "buffer.ads:89:4 instance at test.ads:9:4:",
         Test_Update_7f75ad'Access);

      Result.Add_Test (Case_1_1_Test_Increment_73b073'Access);
      Result.Add_Test (Case_2_1_Test_Plus_6c7b64'Access);
      Result.Add_Test (Case_3_1_Test_Minus_a4e864'Access);
      Result.Add_Test (Case_4_1_Test_Read_412d0f'Access);
      Result.Add_Test (Case_5_1_Test_Push_b0cb78'Access);
      Result.Add_Test (Case_6_1_Test_Update_7f75ad'Access);

      return Result'Access;

   end Suite;

end Test.Test_Data.Tests.Buffer_1.Test_Data.Tests.Suite;
--  end read only

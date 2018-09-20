--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body Test.Test_Data.Tests.Reader_1.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.Test.Test_Data.Tests.Reader_1.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_Read_412d0f : aliased Runner_1.Test_Case;
   Case_2_1_Test_Get_Last_Written_40186d : aliased Runner_1.Test_Case;
   Case_3_1_Test_Get_Last_Overwritten_419e17 : aliased Runner_1.Test_Case;
   Case_4_1_Test_Get_b58e38 : aliased Runner_1.Test_Case;
   Case_5_1_Test_Pop_2a7df9 : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_Read_412d0f,
         "buffer-reader.ads:7:4 instance at test.ads:10:4:",
         Test_Read_412d0f'Access);
      Runner_1.Create
        (Case_2_1_Test_Get_Last_Written_40186d,
         "buffer-reader.ads:8:4 instance at test.ads:10:4:",
         Test_Get_Last_Written_40186d'Access);
      Runner_1.Create
        (Case_3_1_Test_Get_Last_Overwritten_419e17,
         "buffer-reader.ads:9:4 instance at test.ads:10:4:",
         Test_Get_Last_Overwritten_419e17'Access);
      Runner_1.Create
        (Case_4_1_Test_Get_b58e38,
         "buffer-reader.ads:10:4 instance at test.ads:10:4:",
         Test_Get_b58e38'Access);
      Runner_1.Create
        (Case_5_1_Test_Pop_2a7df9,
         "buffer-reader.ads:11:4 instance at test.ads:10:4:",
         Test_Pop_2a7df9'Access);

      Result.Add_Test (Case_1_1_Test_Read_412d0f'Access);
      Result.Add_Test (Case_2_1_Test_Get_Last_Written_40186d'Access);
      Result.Add_Test (Case_3_1_Test_Get_Last_Overwritten_419e17'Access);
      Result.Add_Test (Case_4_1_Test_Get_b58e38'Access);
      Result.Add_Test (Case_5_1_Test_Pop_2a7df9'Access);

      return Result'Access;

   end Suite;

end Test.Test_Data.Tests.Reader_1.Test_Data.Tests.Suite;
--  end read only

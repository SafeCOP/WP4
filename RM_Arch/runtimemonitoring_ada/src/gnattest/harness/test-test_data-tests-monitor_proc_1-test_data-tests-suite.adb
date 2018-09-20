--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body Test.Test_Data.Tests.Monitor_Proc_1.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.Test.Test_Data.Tests.Monitor_Proc_1.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_My_Test_Monitoring_Procedure_f52863 : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_My_Test_Monitoring_Procedure_f52863,
         "monitor-mytestmonitor.ads:9:4 instance at test.ads:15:4:",
         Test_My_Test_Monitoring_Procedure_f52863'Access);

      Result.Add_Test (Case_1_1_Test_My_Test_Monitoring_Procedure_f52863'Access);

      return Result'Access;

   end Suite;

end Test.Test_Data.Tests.Monitor_Proc_1.Test_Data.Tests.Suite;
--  end read only

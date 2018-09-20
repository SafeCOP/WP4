--  This package has been generated automatically by GNATtest.
--  You are allowed to add your code to the bodies of test routines.
--  Such changes will be kept during further regeneration of this file.
--  All code placed outside of test routine bodies will be lost. The
--  code intended to set up and tear down the test environment should be
--  placed into Monitor.MyTestMonitor.Test_Data.

with AUnit.Assertions; use AUnit.Assertions;

package body Monitor.MyTestMonitor.Test_Data.Tests is


--  begin read only
   procedure Test_My_Test_Monitoring_Procedure (Gnattest_T : in out Test);
   procedure Test_My_Test_Monitoring_Procedure_f52863 (Gnattest_T : in out Test) renames Test_My_Test_Monitoring_Procedure;
--  id:2.2/f52863dbc0168200/My_Test_Monitoring_Procedure/1/0/
   procedure Test_My_Test_Monitoring_Procedure (Gnattest_T : in out Test) is
   --  monitor-mytestmonitor.ads:9:4:My_Test_Monitoring_Procedure
--  end read only

      pragma Unreferenced (Gnattest_T);

      package Buffer_Test_1 is new Buffer(Event_Info_Type => Integer,
                                        Buffer_Length   => 5);
      package Buffer_Test_2 is new Buffer(Event_Info_Type => Integer,
                                        Buffer_Length   => 3);
      package Synch_Reader_1 is new Synchronised_Reader(Buffer_Test_1);
      package Synch_Reader_2 is new Synchronised_Reader(Buffer_Test_1);
      package Synch_Reader_3 is new Synchronised_Reader(Buffer_Test_2);

      procedure simple_synch_reader_test is
      begin
         null;
      end simple_synch_reader_test;

      procedure combined_synch_reader_test is
      begin
         null;
      end combined_synch_reader_test;

   begin

      simple_synch_reader_test;
      combined_synch_reader_test;

--  begin read only
   end Test_My_Test_Monitoring_Procedure;
--  end read only

end Monitor.MyTestMonitor.Test_Data.Tests;

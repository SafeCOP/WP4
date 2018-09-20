pragma Ada_95;
pragma Source_File_Name (ada_main, Spec_File_Name => "b__test_runner.ads");
pragma Source_File_Name (ada_main, Body_File_Name => "b__test_runner.adb");
pragma Suppress (Overflow_Check);
with Ada.Exceptions;

package body ada_main is
   pragma Warnings (Off);

   E161 : Short_Integer; pragma Import (Ada, E161, "system__os_lib_E");
   E013 : Short_Integer; pragma Import (Ada, E013, "system__soft_links_E");
   E019 : Short_Integer; pragma Import (Ada, E019, "system__exception_table_E");
   E096 : Short_Integer; pragma Import (Ada, E096, "ada__io_exceptions_E");
   E132 : Short_Integer; pragma Import (Ada, E132, "ada__strings_E");
   E134 : Short_Integer; pragma Import (Ada, E134, "ada__strings__maps_E");
   E137 : Short_Integer; pragma Import (Ada, E137, "ada__strings__maps__constants_E");
   E050 : Short_Integer; pragma Import (Ada, E050, "ada__tags_E");
   E095 : Short_Integer; pragma Import (Ada, E095, "ada__streams_E");
   E080 : Short_Integer; pragma Import (Ada, E080, "interfaces__c_E");
   E021 : Short_Integer; pragma Import (Ada, E021, "system__exceptions_E");
   E164 : Short_Integer; pragma Import (Ada, E164, "system__file_control_block_E");
   E159 : Short_Integer; pragma Import (Ada, E159, "system__file_io_E");
   E098 : Short_Integer; pragma Import (Ada, E098, "system__finalization_root_E");
   E093 : Short_Integer; pragma Import (Ada, E093, "ada__finalization_E");
   E100 : Short_Integer; pragma Import (Ada, E100, "system__storage_pools_E");
   E085 : Short_Integer; pragma Import (Ada, E085, "system__finalization_masters_E");
   E147 : Short_Integer; pragma Import (Ada, E147, "system__storage_pools__subpools_E");
   E078 : Short_Integer; pragma Import (Ada, E078, "ada__calendar_E");
   E166 : Short_Integer; pragma Import (Ada, E166, "gnat__directory_operations_E");
   E102 : Short_Integer; pragma Import (Ada, E102, "system__pool_global_E");
   E009 : Short_Integer; pragma Import (Ada, E009, "system__secondary_stack_E");
   E139 : Short_Integer; pragma Import (Ada, E139, "ada__strings__unbounded_E");
   E176 : Short_Integer; pragma Import (Ada, E176, "system__regexp_E");
   E127 : Short_Integer; pragma Import (Ada, E127, "gnat__command_line_E");
   E190 : Short_Integer; pragma Import (Ada, E190, "ada__real_time_E");
   E155 : Short_Integer; pragma Import (Ada, E155, "ada__text_io_E");
   E070 : Short_Integer; pragma Import (Ada, E070, "ada_containers__aunit_lists_E");
   E045 : Short_Integer; pragma Import (Ada, E045, "aunit_E");
   E047 : Short_Integer; pragma Import (Ada, E047, "aunit__memory_E");
   E072 : Short_Integer; pragma Import (Ada, E072, "aunit__memory__utils_E");
   E083 : Short_Integer; pragma Import (Ada, E083, "aunit__tests_E");
   E063 : Short_Integer; pragma Import (Ada, E063, "aunit__test_filters_E");
   E076 : Short_Integer; pragma Import (Ada, E076, "aunit__time_measure_E");
   E074 : Short_Integer; pragma Import (Ada, E074, "aunit__test_results_E");
   E067 : Short_Integer; pragma Import (Ada, E067, "aunit__assertions_E");
   E115 : Short_Integer; pragma Import (Ada, E115, "aunit__reporter_E");
   E117 : Short_Integer; pragma Import (Ada, E117, "aunit__reporter__gnattest_E");
   E065 : Short_Integer; pragma Import (Ada, E065, "aunit__simple_test_cases_E");
   E221 : Short_Integer; pragma Import (Ada, E221, "aunit__test_fixtures_E");
   E227 : Short_Integer; pragma Import (Ada, E227, "aunit__test_caller_E");
   E125 : Short_Integer; pragma Import (Ada, E125, "aunit__test_suites_E");
   E123 : Short_Integer; pragma Import (Ada, E123, "aunit__run_E");
   E232 : Short_Integer; pragma Import (Ada, E232, "circular_arrays_E");
   E182 : Short_Integer; pragma Import (Ada, E182, "debug_E");
   E219 : Short_Integer; pragma Import (Ada, E219, "debug__test_data_E");
   E223 : Short_Integer; pragma Import (Ada, E223, "debug__test_data__tests_E");
   E225 : Short_Integer; pragma Import (Ada, E225, "debug__test_data__tests__suite_E");
   E180 : Short_Integer; pragma Import (Ada, E180, "gnattest_main_suite_E");
   E235 : Short_Integer; pragma Import (Ada, E235, "time_E");
   E234 : Short_Integer; pragma Import (Ada, E234, "pairs_E");
   E230 : Short_Integer; pragma Import (Ada, E230, "buffer_E");
   E237 : Short_Integer; pragma Import (Ada, E237, "buffer__reader_E");
   E271 : Short_Integer; pragma Import (Ada, E271, "buffer__reader__test_data_E");
   E273 : Short_Integer; pragma Import (Ada, E273, "buffer__reader__test_data__tests_E");
   E251 : Short_Integer; pragma Import (Ada, E251, "buffer__test_data_E");
   E253 : Short_Integer; pragma Import (Ada, E253, "buffer__test_data__tests_E");
   E239 : Short_Integer; pragma Import (Ada, E239, "buffer__writer_E");
   E281 : Short_Integer; pragma Import (Ada, E281, "buffer__writer__test_data_E");
   E283 : Short_Integer; pragma Import (Ada, E283, "buffer__writer__test_data__tests_E");
   E241 : Short_Integer; pragma Import (Ada, E241, "monitor_E");
   E243 : Short_Integer; pragma Import (Ada, E243, "monitor__mytestmonitor_E");
   E261 : Short_Integer; pragma Import (Ada, E261, "monitor__mytestmonitor__test_data_E");
   E263 : Short_Integer; pragma Import (Ada, E263, "monitor__mytestmonitor__test_data__tests_E");
   E228 : Short_Integer; pragma Import (Ada, E228, "test_E");
   E245 : Short_Integer; pragma Import (Ada, E245, "test__test_data_E");
   E246 : Short_Integer; pragma Import (Ada, E246, "test__test_data__tests_E");
   E249 : Short_Integer; pragma Import (Ada, E249, "test__test_data__tests__buffer_1__test_data_E");
   E254 : Short_Integer; pragma Import (Ada, E254, "test__test_data__tests__buffer_1__test_data__tests_E");
   E256 : Short_Integer; pragma Import (Ada, E256, "test__test_data__tests__buffer_1__test_data__tests__suite_E");
   E259 : Short_Integer; pragma Import (Ada, E259, "test__test_data__tests__monitor_proc_1__test_data_E");
   E264 : Short_Integer; pragma Import (Ada, E264, "test__test_data__tests__monitor_proc_1__test_data__tests_E");
   E266 : Short_Integer; pragma Import (Ada, E266, "test__test_data__tests__monitor_proc_1__test_data__tests__suite_E");
   E269 : Short_Integer; pragma Import (Ada, E269, "test__test_data__tests__reader_1__test_data_E");
   E274 : Short_Integer; pragma Import (Ada, E274, "test__test_data__tests__reader_1__test_data__tests_E");
   E276 : Short_Integer; pragma Import (Ada, E276, "test__test_data__tests__reader_1__test_data__tests__suite_E");
   E279 : Short_Integer; pragma Import (Ada, E279, "test__test_data__tests__writer_1__test_data_E");
   E284 : Short_Integer; pragma Import (Ada, E284, "test__test_data__tests__writer_1__test_data__tests_E");
   E286 : Short_Integer; pragma Import (Ada, E286, "test__test_data__tests__writer_1__test_data__tests__suite_E");

   Local_Priority_Specific_Dispatching : constant String := "";
   Local_Interrupt_States : constant String := "";

   Is_Elaborated : Boolean := False;

   procedure finalize_library is
   begin
      declare
         procedure F1;
         pragma Import (Ada, F1, "test__test_data__tests__writer_1__test_data__tests__suite__finalize_body");
      begin
         E286 := E286 - 1;
         F1;
      end;
      declare
         procedure F2;
         pragma Import (Ada, F2, "test__test_data__tests__writer_1__test_data__tests__finalize_spec");
      begin
         E284 := E284 - 1;
         F2;
      end;
      E279 := E279 - 1;
      declare
         procedure F3;
         pragma Import (Ada, F3, "test__test_data__tests__writer_1__test_data__finalize_spec");
      begin
         F3;
      end;
      declare
         procedure F4;
         pragma Import (Ada, F4, "test__test_data__tests__reader_1__test_data__tests__suite__finalize_body");
      begin
         E276 := E276 - 1;
         F4;
      end;
      declare
         procedure F5;
         pragma Import (Ada, F5, "test__test_data__tests__reader_1__test_data__tests__finalize_spec");
      begin
         E274 := E274 - 1;
         F5;
      end;
      E269 := E269 - 1;
      declare
         procedure F6;
         pragma Import (Ada, F6, "test__test_data__tests__reader_1__test_data__finalize_spec");
      begin
         F6;
      end;
      declare
         procedure F7;
         pragma Import (Ada, F7, "test__test_data__tests__monitor_proc_1__test_data__tests__suite__finalize_body");
      begin
         E266 := E266 - 1;
         F7;
      end;
      declare
         procedure F8;
         pragma Import (Ada, F8, "test__test_data__tests__monitor_proc_1__test_data__tests__finalize_spec");
      begin
         E264 := E264 - 1;
         F8;
      end;
      E259 := E259 - 1;
      declare
         procedure F9;
         pragma Import (Ada, F9, "test__test_data__tests__monitor_proc_1__test_data__finalize_spec");
      begin
         F9;
      end;
      declare
         procedure F10;
         pragma Import (Ada, F10, "test__test_data__tests__buffer_1__test_data__tests__suite__finalize_body");
      begin
         E256 := E256 - 1;
         F10;
      end;
      declare
         procedure F11;
         pragma Import (Ada, F11, "test__test_data__tests__buffer_1__test_data__tests__finalize_spec");
      begin
         E254 := E254 - 1;
         F11;
      end;
      E249 := E249 - 1;
      declare
         procedure F12;
         pragma Import (Ada, F12, "test__test_data__tests__buffer_1__test_data__finalize_spec");
      begin
         F12;
      end;
      declare
         procedure F13;
         pragma Import (Ada, F13, "test__test_data__tests__finalize_spec");
      begin
         E246 := E246 - 1;
         F13;
      end;
      E245 := E245 - 1;
      declare
         procedure F14;
         pragma Import (Ada, F14, "test__test_data__finalize_spec");
      begin
         F14;
      end;
      declare
         procedure F15;
         pragma Import (Ada, F15, "debug__test_data__tests__suite__finalize_body");
      begin
         E225 := E225 - 1;
         F15;
      end;
      E223 := E223 - 1;
      declare
         procedure F16;
         pragma Import (Ada, F16, "debug__test_data__tests__finalize_spec");
      begin
         F16;
      end;
      E219 := E219 - 1;
      declare
         procedure F17;
         pragma Import (Ada, F17, "debug__test_data__finalize_spec");
      begin
         F17;
      end;
      E125 := E125 - 1;
      declare
         procedure F18;
         pragma Import (Ada, F18, "aunit__test_suites__finalize_spec");
      begin
         F18;
      end;
      E221 := E221 - 1;
      declare
         procedure F19;
         pragma Import (Ada, F19, "aunit__test_fixtures__finalize_spec");
      begin
         F19;
      end;
      E063 := E063 - 1;
      E065 := E065 - 1;
      declare
         procedure F20;
         pragma Import (Ada, F20, "aunit__simple_test_cases__finalize_spec");
      begin
         F20;
      end;
      E117 := E117 - 1;
      declare
         procedure F21;
         pragma Import (Ada, F21, "aunit__reporter__gnattest__finalize_spec");
      begin
         F21;
      end;
      E067 := E067 - 1;
      declare
         procedure F22;
         pragma Import (Ada, F22, "aunit__assertions__finalize_spec");
      begin
         F22;
      end;
      E074 := E074 - 1;
      declare
         procedure F23;
         pragma Import (Ada, F23, "aunit__test_results__finalize_spec");
      begin
         F23;
      end;
      declare
         procedure F24;
         pragma Import (Ada, F24, "aunit__test_filters__finalize_spec");
      begin
         F24;
      end;
      declare
         procedure F25;
         pragma Import (Ada, F25, "aunit__tests__finalize_spec");
      begin
         E083 := E083 - 1;
         F25;
      end;
      E155 := E155 - 1;
      declare
         procedure F26;
         pragma Import (Ada, F26, "ada__text_io__finalize_spec");
      begin
         F26;
      end;
      E176 := E176 - 1;
      declare
         procedure F27;
         pragma Import (Ada, F27, "system__regexp__finalize_spec");
      begin
         F27;
      end;
      E139 := E139 - 1;
      declare
         procedure F28;
         pragma Import (Ada, F28, "ada__strings__unbounded__finalize_spec");
      begin
         F28;
      end;
      declare
         procedure F29;
         pragma Import (Ada, F29, "system__file_io__finalize_body");
      begin
         E159 := E159 - 1;
         F29;
      end;
      E085 := E085 - 1;
      E147 := E147 - 1;
      E102 := E102 - 1;
      declare
         procedure F30;
         pragma Import (Ada, F30, "system__pool_global__finalize_spec");
      begin
         F30;
      end;
      declare
         procedure F31;
         pragma Import (Ada, F31, "system__storage_pools__subpools__finalize_spec");
      begin
         F31;
      end;
      declare
         procedure F32;
         pragma Import (Ada, F32, "system__finalization_masters__finalize_spec");
      begin
         F32;
      end;
      declare
         procedure Reraise_Library_Exception_If_Any;
            pragma Import (Ada, Reraise_Library_Exception_If_Any, "__gnat_reraise_library_exception_if_any");
      begin
         Reraise_Library_Exception_If_Any;
      end;
   end finalize_library;

   procedure adafinal is
      procedure s_stalib_adafinal;
      pragma Import (C, s_stalib_adafinal, "system__standard_library__adafinal");

      procedure Runtime_Finalize;
      pragma Import (C, Runtime_Finalize, "__gnat_runtime_finalize");

   begin
      if not Is_Elaborated then
         return;
      end if;
      Is_Elaborated := False;
      Runtime_Finalize;
      s_stalib_adafinal;
   end adafinal;

   type No_Param_Proc is access procedure;

   procedure adainit is
      Main_Priority : Integer;
      pragma Import (C, Main_Priority, "__gl_main_priority");
      Time_Slice_Value : Integer;
      pragma Import (C, Time_Slice_Value, "__gl_time_slice_val");
      WC_Encoding : Character;
      pragma Import (C, WC_Encoding, "__gl_wc_encoding");
      Locking_Policy : Character;
      pragma Import (C, Locking_Policy, "__gl_locking_policy");
      Queuing_Policy : Character;
      pragma Import (C, Queuing_Policy, "__gl_queuing_policy");
      Task_Dispatching_Policy : Character;
      pragma Import (C, Task_Dispatching_Policy, "__gl_task_dispatching_policy");
      Priority_Specific_Dispatching : System.Address;
      pragma Import (C, Priority_Specific_Dispatching, "__gl_priority_specific_dispatching");
      Num_Specific_Dispatching : Integer;
      pragma Import (C, Num_Specific_Dispatching, "__gl_num_specific_dispatching");
      Main_CPU : Integer;
      pragma Import (C, Main_CPU, "__gl_main_cpu");
      Interrupt_States : System.Address;
      pragma Import (C, Interrupt_States, "__gl_interrupt_states");
      Num_Interrupt_States : Integer;
      pragma Import (C, Num_Interrupt_States, "__gl_num_interrupt_states");
      Unreserve_All_Interrupts : Integer;
      pragma Import (C, Unreserve_All_Interrupts, "__gl_unreserve_all_interrupts");
      Exception_Tracebacks : Integer;
      pragma Import (C, Exception_Tracebacks, "__gl_exception_tracebacks");
      Detect_Blocking : Integer;
      pragma Import (C, Detect_Blocking, "__gl_detect_blocking");
      Default_Stack_Size : Integer;
      pragma Import (C, Default_Stack_Size, "__gl_default_stack_size");
      Leap_Seconds_Support : Integer;
      pragma Import (C, Leap_Seconds_Support, "__gl_leap_seconds_support");

      procedure Runtime_Initialize (Install_Handler : Integer);
      pragma Import (C, Runtime_Initialize, "__gnat_runtime_initialize");

      Finalize_Library_Objects : No_Param_Proc;
      pragma Import (C, Finalize_Library_Objects, "__gnat_finalize_library_objects");
   begin
      if Is_Elaborated then
         return;
      end if;
      Is_Elaborated := True;
      Main_Priority := -1;
      Time_Slice_Value := -1;
      WC_Encoding := 'b';
      Locking_Policy := ' ';
      Queuing_Policy := ' ';
      Task_Dispatching_Policy := ' ';
      Priority_Specific_Dispatching :=
        Local_Priority_Specific_Dispatching'Address;
      Num_Specific_Dispatching := 0;
      Main_CPU := -1;
      Interrupt_States := Local_Interrupt_States'Address;
      Num_Interrupt_States := 0;
      Unreserve_All_Interrupts := 0;
      Exception_Tracebacks := 1;
      Detect_Blocking := 0;
      Default_Stack_Size := -1;
      Leap_Seconds_Support := 0;

      Runtime_Initialize (1);

      Finalize_Library_Objects := finalize_library'access;

      System.Soft_Links'Elab_Spec;
      System.Exception_Table'Elab_Body;
      E019 := E019 + 1;
      Ada.Io_Exceptions'Elab_Spec;
      E096 := E096 + 1;
      Ada.Strings'Elab_Spec;
      E132 := E132 + 1;
      Ada.Strings.Maps'Elab_Spec;
      Ada.Strings.Maps.Constants'Elab_Spec;
      E137 := E137 + 1;
      Ada.Tags'Elab_Spec;
      Ada.Streams'Elab_Spec;
      E095 := E095 + 1;
      Interfaces.C'Elab_Spec;
      System.Exceptions'Elab_Spec;
      E021 := E021 + 1;
      System.File_Control_Block'Elab_Spec;
      E164 := E164 + 1;
      System.Finalization_Root'Elab_Spec;
      E098 := E098 + 1;
      Ada.Finalization'Elab_Spec;
      E093 := E093 + 1;
      System.Storage_Pools'Elab_Spec;
      E100 := E100 + 1;
      System.Finalization_Masters'Elab_Spec;
      System.Storage_Pools.Subpools'Elab_Spec;
      Ada.Calendar'Elab_Spec;
      Ada.Calendar'Elab_Body;
      E078 := E078 + 1;
      Gnat.Directory_Operations'Elab_Spec;
      System.Pool_Global'Elab_Spec;
      E102 := E102 + 1;
      E147 := E147 + 1;
      System.Finalization_Masters'Elab_Body;
      E085 := E085 + 1;
      System.File_Io'Elab_Body;
      E159 := E159 + 1;
      E080 := E080 + 1;
      Ada.Tags'Elab_Body;
      E050 := E050 + 1;
      E134 := E134 + 1;
      System.Soft_Links'Elab_Body;
      E013 := E013 + 1;
      System.Os_Lib'Elab_Body;
      E161 := E161 + 1;
      System.Secondary_Stack'Elab_Body;
      E009 := E009 + 1;
      Gnat.Directory_Operations'Elab_Body;
      E166 := E166 + 1;
      Ada.Strings.Unbounded'Elab_Spec;
      E139 := E139 + 1;
      System.Regexp'Elab_Spec;
      E176 := E176 + 1;
      Gnat.Command_Line'Elab_Spec;
      Ada.Real_Time'Elab_Spec;
      Ada.Real_Time'Elab_Body;
      E190 := E190 + 1;
      Ada.Text_Io'Elab_Spec;
      Ada.Text_Io'Elab_Body;
      E155 := E155 + 1;
      Gnat.Command_Line'Elab_Body;
      E127 := E127 + 1;
      E047 := E047 + 1;
      E045 := E045 + 1;
      E072 := E072 + 1;
      E070 := E070 + 1;
      Aunit.Tests'Elab_Spec;
      E083 := E083 + 1;
      Aunit.Test_Filters'Elab_Spec;
      Aunit.Time_Measure'Elab_Spec;
      E076 := E076 + 1;
      Aunit.Test_Results'Elab_Spec;
      Aunit.Test_Results'Elab_Body;
      E074 := E074 + 1;
      Aunit.Assertions'Elab_Spec;
      Aunit.Assertions'Elab_Body;
      E067 := E067 + 1;
      Aunit.Reporter'Elab_Spec;
      E115 := E115 + 1;
      Aunit.Reporter.Gnattest'Elab_Spec;
      Aunit.Reporter.Gnattest'Elab_Body;
      E117 := E117 + 1;
      Aunit.Simple_Test_Cases'Elab_Spec;
      E065 := E065 + 1;
      E063 := E063 + 1;
      Aunit.Test_Fixtures'Elab_Spec;
      E221 := E221 + 1;
      E227 := E227 + 1;
      Aunit.Test_Suites'Elab_Spec;
      E125 := E125 + 1;
      E123 := E123 + 1;
      E232 := E232 + 1;
      E182 := E182 + 1;
      Debug.Test_Data'Elab_Spec;
      E219 := E219 + 1;
      Debug.Test_Data.Tests'Elab_Spec;
      E223 := E223 + 1;
      Debug.Test_Data.Tests.Suite'Elab_Body;
      E225 := E225 + 1;
      Time'Elab_Spec;
      E235 := E235 + 1;
      E234 := E234 + 1;
      E230 := E230 + 1;
      E237 := E237 + 1;
      E271 := E271 + 1;
      E273 := E273 + 1;
      E251 := E251 + 1;
      E253 := E253 + 1;
      E239 := E239 + 1;
      E281 := E281 + 1;
      E283 := E283 + 1;
      E241 := E241 + 1;
      E243 := E243 + 1;
      E261 := E261 + 1;
      E263 := E263 + 1;
      Test'Elab_Spec;
      E228 := E228 + 1;
      Test.Test_Data'Elab_Spec;
      E245 := E245 + 1;
      Test.Test_Data.Tests'Elab_Spec;
      E246 := E246 + 1;
      Test.Test_Data.Tests.Buffer_1.Test_Data'Elab_Spec;
      E249 := E249 + 1;
      Test.Test_Data.Tests.Buffer_1.Test_Data.Tests'Elab_Spec;
      E254 := E254 + 1;
      Test.Test_Data.Tests.Buffer_1.Test_Data.Tests.Suite'Elab_Body;
      E256 := E256 + 1;
      Test.Test_Data.Tests.Monitor_Proc_1.Test_Data'Elab_Spec;
      E259 := E259 + 1;
      Test.Test_Data.Tests.Monitor_Proc_1.Test_Data.Tests'Elab_Spec;
      E264 := E264 + 1;
      Test.Test_Data.Tests.Monitor_Proc_1.Test_Data.Tests.Suite'Elab_Body;
      E266 := E266 + 1;
      Test.Test_Data.Tests.Reader_1.Test_Data'Elab_Spec;
      E269 := E269 + 1;
      Test.Test_Data.Tests.Reader_1.Test_Data.Tests'Elab_Spec;
      E274 := E274 + 1;
      Test.Test_Data.Tests.Reader_1.Test_Data.Tests.Suite'Elab_Body;
      E276 := E276 + 1;
      Test.Test_Data.Tests.Writer_1.Test_Data'Elab_Spec;
      E279 := E279 + 1;
      Test.Test_Data.Tests.Writer_1.Test_Data.Tests'Elab_Spec;
      E284 := E284 + 1;
      Test.Test_Data.Tests.Writer_1.Test_Data.Tests.Suite'Elab_Body;
      E286 := E286 + 1;
      Gnattest_Main_Suite'Elab_Body;
      E180 := E180 + 1;
   end adainit;

   procedure Ada_Main_Program;
   pragma Import (Ada, Ada_Main_Program, "_ada_test_runner");

   function main
     (argc : Integer;
      argv : System.Address;
      envp : System.Address)
      return Integer
   is
      procedure Initialize (Addr : System.Address);
      pragma Import (C, Initialize, "__gnat_initialize");

      procedure Finalize;
      pragma Import (C, Finalize, "__gnat_finalize");
      SEH : aliased array (1 .. 2) of Integer;

      Ensure_Reference : aliased System.Address := Ada_Main_Program_Name'Address;
      pragma Volatile (Ensure_Reference);

   begin
      gnat_argc := argc;
      gnat_argv := argv;
      gnat_envp := envp;

      Initialize (SEH'Address);
      adainit;
      Ada_Main_Program;
      adafinal;
      Finalize;
      return (gnat_exit_status);
   end;

--  BEGIN Object file/option list
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/debug-test_data.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/gnattest_generated.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/debug-test_data-tests.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/debug-test_data-tests-suite.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test_runner.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/buffer-reader-test_data.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/buffer-reader-test_data-tests.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/buffer-test_data.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/buffer-test_data-tests.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/buffer-writer-test_data.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/buffer-writer-test_data-tests.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/monitor-mytestmonitor-test_data.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/monitor-mytestmonitor-test_data-tests.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-buffer_1.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-buffer_1-test_data.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-buffer_1-test_data-tests.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-buffer_1-test_data-tests-suite.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-monitor_proc_1.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-monitor_proc_1-test_data.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-monitor_proc_1-test_data-tests.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-monitor_proc_1-test_data-tests-suite.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-reader_1.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-reader_1-test_data.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-reader_1-test_data-tests.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-reader_1-test_data-tests-suite.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-writer_1.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-writer_1-test_data.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-writer_1-test_data-tests.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/test-test_data-tests-writer_1-test_data-tests-suite.o
   --   /Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/gnattest_main_suite.o
   --   -L/Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/
   --   -L/Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/src/gnattest/harness/
   --   -L/usr/local/gnat/lib/aunit/
   --   -L/Users/cister/Dropbox/Research/Runtime_Monitoring/Ada_Reference_Architecture/runtimemonitoring_ada/Library/
   --   -L/usr/local/gnat/lib/gcc/x86_64-apple-darwin13.4.0/4.9.3/adalib/
   --   -static
   --   -lgnarl
   --   -lgnat
--  END Object file/option list   

end ada_main;

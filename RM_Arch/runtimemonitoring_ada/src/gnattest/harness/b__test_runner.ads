pragma Ada_95;
with System;
package ada_main is
   pragma Warnings (Off);

   gnat_argc : Integer;
   gnat_argv : System.Address;
   gnat_envp : System.Address;

   pragma Import (C, gnat_argc);
   pragma Import (C, gnat_argv);
   pragma Import (C, gnat_envp);

   gnat_exit_status : Integer;
   pragma Import (C, gnat_exit_status);

   GNAT_Version : constant String :=
                    "GNAT Version: GPL 2015 (20150428-49)" & ASCII.NUL;
   pragma Export (C, GNAT_Version, "__gnat_version");

   Ada_Main_Program_Name : constant String := "_ada_test_runner" & ASCII.NUL;
   pragma Export (C, Ada_Main_Program_Name, "__gnat_ada_main_program_name");

   procedure adainit;
   pragma Export (C, adainit, "adainit");

   procedure adafinal;
   pragma Export (C, adafinal, "adafinal");

   function main
     (argc : Integer;
      argv : System.Address;
      envp : System.Address)
      return Integer;
   pragma Export (C, main, "main");

   type Version_32 is mod 2 ** 32;
   u00001 : constant Version_32 := 16#ca2a7db3#;
   pragma Export (C, u00001, "test_runnerB");
   u00002 : constant Version_32 := 16#fbff4c67#;
   pragma Export (C, u00002, "system__standard_libraryB");
   u00003 : constant Version_32 := 16#13e2b8e6#;
   pragma Export (C, u00003, "system__standard_libraryS");
   u00004 : constant Version_32 := 16#3ffc8e18#;
   pragma Export (C, u00004, "adaS");
   u00005 : constant Version_32 := 16#9630f6da#;
   pragma Export (C, u00005, "ada__command_lineB");
   u00006 : constant Version_32 := 16#d59e21a4#;
   pragma Export (C, u00006, "ada__command_lineS");
   u00007 : constant Version_32 := 16#100301f7#;
   pragma Export (C, u00007, "systemS");
   u00008 : constant Version_32 := 16#b19b6653#;
   pragma Export (C, u00008, "system__secondary_stackB");
   u00009 : constant Version_32 := 16#bb62ce9e#;
   pragma Export (C, u00009, "system__secondary_stackS");
   u00010 : constant Version_32 := 16#b01dad17#;
   pragma Export (C, u00010, "system__parametersB");
   u00011 : constant Version_32 := 16#6e290c88#;
   pragma Export (C, u00011, "system__parametersS");
   u00012 : constant Version_32 := 16#a207fefe#;
   pragma Export (C, u00012, "system__soft_linksB");
   u00013 : constant Version_32 := 16#4b59d020#;
   pragma Export (C, u00013, "system__soft_linksS");
   u00014 : constant Version_32 := 16#2130723f#;
   pragma Export (C, u00014, "ada__exceptionsB");
   u00015 : constant Version_32 := 16#f9d4899e#;
   pragma Export (C, u00015, "ada__exceptionsS");
   u00016 : constant Version_32 := 16#a46739c0#;
   pragma Export (C, u00016, "ada__exceptions__last_chance_handlerB");
   u00017 : constant Version_32 := 16#3aac8c92#;
   pragma Export (C, u00017, "ada__exceptions__last_chance_handlerS");
   u00018 : constant Version_32 := 16#393398c1#;
   pragma Export (C, u00018, "system__exception_tableB");
   u00019 : constant Version_32 := 16#be1a67e2#;
   pragma Export (C, u00019, "system__exception_tableS");
   u00020 : constant Version_32 := 16#ce4af020#;
   pragma Export (C, u00020, "system__exceptionsB");
   u00021 : constant Version_32 := 16#78606c01#;
   pragma Export (C, u00021, "system__exceptionsS");
   u00022 : constant Version_32 := 16#37d758f1#;
   pragma Export (C, u00022, "system__exceptions__machineS");
   u00023 : constant Version_32 := 16#b895431d#;
   pragma Export (C, u00023, "system__exceptions_debugB");
   u00024 : constant Version_32 := 16#a3e11849#;
   pragma Export (C, u00024, "system__exceptions_debugS");
   u00025 : constant Version_32 := 16#570325c8#;
   pragma Export (C, u00025, "system__img_intB");
   u00026 : constant Version_32 := 16#12d8e135#;
   pragma Export (C, u00026, "system__img_intS");
   u00027 : constant Version_32 := 16#39a03df9#;
   pragma Export (C, u00027, "system__storage_elementsB");
   u00028 : constant Version_32 := 16#3dc04bf3#;
   pragma Export (C, u00028, "system__storage_elementsS");
   u00029 : constant Version_32 := 16#b98c3e16#;
   pragma Export (C, u00029, "system__tracebackB");
   u00030 : constant Version_32 := 16#8e3ed82c#;
   pragma Export (C, u00030, "system__tracebackS");
   u00031 : constant Version_32 := 16#9ed49525#;
   pragma Export (C, u00031, "system__traceback_entriesB");
   u00032 : constant Version_32 := 16#1058f787#;
   pragma Export (C, u00032, "system__traceback_entriesS");
   u00033 : constant Version_32 := 16#8c33a517#;
   pragma Export (C, u00033, "system__wch_conB");
   u00034 : constant Version_32 := 16#0b7e2325#;
   pragma Export (C, u00034, "system__wch_conS");
   u00035 : constant Version_32 := 16#9721e840#;
   pragma Export (C, u00035, "system__wch_stwB");
   u00036 : constant Version_32 := 16#266f0f24#;
   pragma Export (C, u00036, "system__wch_stwS");
   u00037 : constant Version_32 := 16#92b797cb#;
   pragma Export (C, u00037, "system__wch_cnvB");
   u00038 : constant Version_32 := 16#04c999d6#;
   pragma Export (C, u00038, "system__wch_cnvS");
   u00039 : constant Version_32 := 16#6033a23f#;
   pragma Export (C, u00039, "interfacesS");
   u00040 : constant Version_32 := 16#ece6fdb6#;
   pragma Export (C, u00040, "system__wch_jisB");
   u00041 : constant Version_32 := 16#84b980f7#;
   pragma Export (C, u00041, "system__wch_jisS");
   u00042 : constant Version_32 := 16#41837d1e#;
   pragma Export (C, u00042, "system__stack_checkingB");
   u00043 : constant Version_32 := 16#9ebc6a1f#;
   pragma Export (C, u00043, "system__stack_checkingS");
   u00044 : constant Version_32 := 16#fa79b673#;
   pragma Export (C, u00044, "aunitB");
   u00045 : constant Version_32 := 16#76cdf7c6#;
   pragma Export (C, u00045, "aunitS");
   u00046 : constant Version_32 := 16#b6c145a2#;
   pragma Export (C, u00046, "aunit__memoryB");
   u00047 : constant Version_32 := 16#c6ffa777#;
   pragma Export (C, u00047, "aunit__memoryS");
   u00048 : constant Version_32 := 16#e99cd447#;
   pragma Export (C, u00048, "aunit__optionsS");
   u00049 : constant Version_32 := 16#12c8cd7d#;
   pragma Export (C, u00049, "ada__tagsB");
   u00050 : constant Version_32 := 16#ce72c228#;
   pragma Export (C, u00050, "ada__tagsS");
   u00051 : constant Version_32 := 16#c3335bfd#;
   pragma Export (C, u00051, "system__htableB");
   u00052 : constant Version_32 := 16#94c1b21d#;
   pragma Export (C, u00052, "system__htableS");
   u00053 : constant Version_32 := 16#089f5cd0#;
   pragma Export (C, u00053, "system__string_hashB");
   u00054 : constant Version_32 := 16#369fd963#;
   pragma Export (C, u00054, "system__string_hashS");
   u00055 : constant Version_32 := 16#8d5ba537#;
   pragma Export (C, u00055, "system__unsigned_typesS");
   u00056 : constant Version_32 := 16#06052bd0#;
   pragma Export (C, u00056, "system__val_lluB");
   u00057 : constant Version_32 := 16#f7a9f245#;
   pragma Export (C, u00057, "system__val_lluS");
   u00058 : constant Version_32 := 16#27b600b2#;
   pragma Export (C, u00058, "system__val_utilB");
   u00059 : constant Version_32 := 16#bca3b709#;
   pragma Export (C, u00059, "system__val_utilS");
   u00060 : constant Version_32 := 16#d1060688#;
   pragma Export (C, u00060, "system__case_utilB");
   u00061 : constant Version_32 := 16#340a6820#;
   pragma Export (C, u00061, "system__case_utilS");
   u00062 : constant Version_32 := 16#e9d6512d#;
   pragma Export (C, u00062, "aunit__test_filtersB");
   u00063 : constant Version_32 := 16#9a67cba8#;
   pragma Export (C, u00063, "aunit__test_filtersS");
   u00064 : constant Version_32 := 16#6e9501f4#;
   pragma Export (C, u00064, "aunit__simple_test_casesB");
   u00065 : constant Version_32 := 16#f9679d50#;
   pragma Export (C, u00065, "aunit__simple_test_casesS");
   u00066 : constant Version_32 := 16#8872fb1a#;
   pragma Export (C, u00066, "aunit__assertionsB");
   u00067 : constant Version_32 := 16#6db0f35e#;
   pragma Export (C, u00067, "aunit__assertionsS");
   u00068 : constant Version_32 := 16#11329e00#;
   pragma Export (C, u00068, "ada_containersS");
   u00069 : constant Version_32 := 16#bc28bbc0#;
   pragma Export (C, u00069, "ada_containers__aunit_listsB");
   u00070 : constant Version_32 := 16#c8d9569a#;
   pragma Export (C, u00070, "ada_containers__aunit_listsS");
   u00071 : constant Version_32 := 16#9b1c7ff2#;
   pragma Export (C, u00071, "aunit__memory__utilsB");
   u00072 : constant Version_32 := 16#fb2f6c57#;
   pragma Export (C, u00072, "aunit__memory__utilsS");
   u00073 : constant Version_32 := 16#01adf261#;
   pragma Export (C, u00073, "aunit__test_resultsB");
   u00074 : constant Version_32 := 16#1087836e#;
   pragma Export (C, u00074, "aunit__test_resultsS");
   u00075 : constant Version_32 := 16#9df5edcf#;
   pragma Export (C, u00075, "aunit__time_measureB");
   u00076 : constant Version_32 := 16#07f2df2f#;
   pragma Export (C, u00076, "aunit__time_measureS");
   u00077 : constant Version_32 := 16#649a98f6#;
   pragma Export (C, u00077, "ada__calendarB");
   u00078 : constant Version_32 := 16#e67a5d0a#;
   pragma Export (C, u00078, "ada__calendarS");
   u00079 : constant Version_32 := 16#769e25e6#;
   pragma Export (C, u00079, "interfaces__cB");
   u00080 : constant Version_32 := 16#4a38bedb#;
   pragma Export (C, u00080, "interfaces__cS");
   u00081 : constant Version_32 := 16#820eb304#;
   pragma Export (C, u00081, "system__os_primitivesB");
   u00082 : constant Version_32 := 16#a0d28dde#;
   pragma Export (C, u00082, "system__os_primitivesS");
   u00083 : constant Version_32 := 16#6b6cea8f#;
   pragma Export (C, u00083, "aunit__testsS");
   u00084 : constant Version_32 := 16#b5b2aca1#;
   pragma Export (C, u00084, "system__finalization_mastersB");
   u00085 : constant Version_32 := 16#641528b7#;
   pragma Export (C, u00085, "system__finalization_mastersS");
   u00086 : constant Version_32 := 16#57a37a42#;
   pragma Export (C, u00086, "system__address_imageB");
   u00087 : constant Version_32 := 16#b1ef9ccd#;
   pragma Export (C, u00087, "system__address_imageS");
   u00088 : constant Version_32 := 16#7268f812#;
   pragma Export (C, u00088, "system__img_boolB");
   u00089 : constant Version_32 := 16#e5da701c#;
   pragma Export (C, u00089, "system__img_boolS");
   u00090 : constant Version_32 := 16#d7aac20c#;
   pragma Export (C, u00090, "system__ioB");
   u00091 : constant Version_32 := 16#8e41f6b8#;
   pragma Export (C, u00091, "system__ioS");
   u00092 : constant Version_32 := 16#b7ab275c#;
   pragma Export (C, u00092, "ada__finalizationB");
   u00093 : constant Version_32 := 16#19f764ca#;
   pragma Export (C, u00093, "ada__finalizationS");
   u00094 : constant Version_32 := 16#10558b11#;
   pragma Export (C, u00094, "ada__streamsB");
   u00095 : constant Version_32 := 16#2e6701ab#;
   pragma Export (C, u00095, "ada__streamsS");
   u00096 : constant Version_32 := 16#db5c917c#;
   pragma Export (C, u00096, "ada__io_exceptionsS");
   u00097 : constant Version_32 := 16#95817ed8#;
   pragma Export (C, u00097, "system__finalization_rootB");
   u00098 : constant Version_32 := 16#5ff17267#;
   pragma Export (C, u00098, "system__finalization_rootS");
   u00099 : constant Version_32 := 16#6d4d969a#;
   pragma Export (C, u00099, "system__storage_poolsB");
   u00100 : constant Version_32 := 16#e5588673#;
   pragma Export (C, u00100, "system__storage_poolsS");
   u00101 : constant Version_32 := 16#e34550ca#;
   pragma Export (C, u00101, "system__pool_globalB");
   u00102 : constant Version_32 := 16#c88d2d16#;
   pragma Export (C, u00102, "system__pool_globalS");
   u00103 : constant Version_32 := 16#9d39c675#;
   pragma Export (C, u00103, "system__memoryB");
   u00104 : constant Version_32 := 16#497e67c3#;
   pragma Export (C, u00104, "system__memoryS");
   u00105 : constant Version_32 := 16#6092d7f9#;
   pragma Export (C, u00105, "system__crtlS");
   u00106 : constant Version_32 := 16#fd2ad2f1#;
   pragma Export (C, u00106, "gnatS");
   u00107 : constant Version_32 := 16#82fca8a5#;
   pragma Export (C, u00107, "gnat__source_infoS");
   u00108 : constant Version_32 := 16#6cae85f3#;
   pragma Export (C, u00108, "gnat__tracebackB");
   u00109 : constant Version_32 := 16#3aa76275#;
   pragma Export (C, u00109, "gnat__tracebackS");
   u00110 : constant Version_32 := 16#701f9d88#;
   pragma Export (C, u00110, "ada__exceptions__tracebackB");
   u00111 : constant Version_32 := 16#5b6d87c9#;
   pragma Export (C, u00111, "ada__exceptions__tracebackS");
   u00112 : constant Version_32 := 16#561b585b#;
   pragma Export (C, u00112, "gnat__traceback__symbolicS");
   u00113 : constant Version_32 := 16#3b95750f#;
   pragma Export (C, u00113, "system__traceback__symbolicB");
   u00114 : constant Version_32 := 16#b5e41f97#;
   pragma Export (C, u00114, "system__traceback__symbolicS");
   u00115 : constant Version_32 := 16#17d955ab#;
   pragma Export (C, u00115, "aunit__reporterS");
   u00116 : constant Version_32 := 16#1c0d4e63#;
   pragma Export (C, u00116, "aunit__reporter__gnattestB");
   u00117 : constant Version_32 := 16#9cd34a09#;
   pragma Export (C, u00117, "aunit__reporter__gnattestS");
   u00118 : constant Version_32 := 16#b48102f5#;
   pragma Export (C, u00118, "gnat__ioB");
   u00119 : constant Version_32 := 16#6227e843#;
   pragma Export (C, u00119, "gnat__ioS");
   u00120 : constant Version_32 := 16#b602a99c#;
   pragma Export (C, u00120, "system__exn_intB");
   u00121 : constant Version_32 := 16#528f1dab#;
   pragma Export (C, u00121, "system__exn_intS");
   u00122 : constant Version_32 := 16#e11af2d7#;
   pragma Export (C, u00122, "aunit__runB");
   u00123 : constant Version_32 := 16#4b2a8016#;
   pragma Export (C, u00123, "aunit__runS");
   u00124 : constant Version_32 := 16#d01edb9b#;
   pragma Export (C, u00124, "aunit__test_suitesB");
   u00125 : constant Version_32 := 16#f3c7e671#;
   pragma Export (C, u00125, "aunit__test_suitesS");
   u00126 : constant Version_32 := 16#a8b4ea59#;
   pragma Export (C, u00126, "gnat__command_lineB");
   u00127 : constant Version_32 := 16#c4427fe3#;
   pragma Export (C, u00127, "gnat__command_lineS");
   u00128 : constant Version_32 := 16#12c24a43#;
   pragma Export (C, u00128, "ada__charactersS");
   u00129 : constant Version_32 := 16#8f637df8#;
   pragma Export (C, u00129, "ada__characters__handlingB");
   u00130 : constant Version_32 := 16#3b3f6154#;
   pragma Export (C, u00130, "ada__characters__handlingS");
   u00131 : constant Version_32 := 16#4b7bb96a#;
   pragma Export (C, u00131, "ada__characters__latin_1S");
   u00132 : constant Version_32 := 16#af50e98f#;
   pragma Export (C, u00132, "ada__stringsS");
   u00133 : constant Version_32 := 16#e2ea8656#;
   pragma Export (C, u00133, "ada__strings__mapsB");
   u00134 : constant Version_32 := 16#1e526bec#;
   pragma Export (C, u00134, "ada__strings__mapsS");
   u00135 : constant Version_32 := 16#a55efc94#;
   pragma Export (C, u00135, "system__bit_opsB");
   u00136 : constant Version_32 := 16#0765e3a3#;
   pragma Export (C, u00136, "system__bit_opsS");
   u00137 : constant Version_32 := 16#92f05f13#;
   pragma Export (C, u00137, "ada__strings__maps__constantsS");
   u00138 : constant Version_32 := 16#f78329ae#;
   pragma Export (C, u00138, "ada__strings__unboundedB");
   u00139 : constant Version_32 := 16#e303cf90#;
   pragma Export (C, u00139, "ada__strings__unboundedS");
   u00140 : constant Version_32 := 16#36ece461#;
   pragma Export (C, u00140, "ada__strings__searchB");
   u00141 : constant Version_32 := 16#c1ab8667#;
   pragma Export (C, u00141, "ada__strings__searchS");
   u00142 : constant Version_32 := 16#5b9edcc4#;
   pragma Export (C, u00142, "system__compare_array_unsigned_8B");
   u00143 : constant Version_32 := 16#b900707a#;
   pragma Export (C, u00143, "system__compare_array_unsigned_8S");
   u00144 : constant Version_32 := 16#5f72f755#;
   pragma Export (C, u00144, "system__address_operationsB");
   u00145 : constant Version_32 := 16#030fbfc4#;
   pragma Export (C, u00145, "system__address_operationsS");
   u00146 : constant Version_32 := 16#6a859064#;
   pragma Export (C, u00146, "system__storage_pools__subpoolsB");
   u00147 : constant Version_32 := 16#e3b008dc#;
   pragma Export (C, u00147, "system__storage_pools__subpoolsS");
   u00148 : constant Version_32 := 16#63f11652#;
   pragma Export (C, u00148, "system__storage_pools__subpools__finalizationB");
   u00149 : constant Version_32 := 16#fe2f4b3a#;
   pragma Export (C, u00149, "system__storage_pools__subpools__finalizationS");
   u00150 : constant Version_32 := 16#afc64758#;
   pragma Export (C, u00150, "system__atomic_countersB");
   u00151 : constant Version_32 := 16#dd7f953d#;
   pragma Export (C, u00151, "system__atomic_countersS");
   u00152 : constant Version_32 := 16#f4e1c091#;
   pragma Export (C, u00152, "system__stream_attributesB");
   u00153 : constant Version_32 := 16#221dd20d#;
   pragma Export (C, u00153, "system__stream_attributesS");
   u00154 : constant Version_32 := 16#28f088c2#;
   pragma Export (C, u00154, "ada__text_ioB");
   u00155 : constant Version_32 := 16#fe568dda#;
   pragma Export (C, u00155, "ada__text_ioS");
   u00156 : constant Version_32 := 16#84a27f0d#;
   pragma Export (C, u00156, "interfaces__c_streamsB");
   u00157 : constant Version_32 := 16#8bb5f2c0#;
   pragma Export (C, u00157, "interfaces__c_streamsS");
   u00158 : constant Version_32 := 16#431faf3c#;
   pragma Export (C, u00158, "system__file_ioB");
   u00159 : constant Version_32 := 16#b772e092#;
   pragma Export (C, u00159, "system__file_ioS");
   u00160 : constant Version_32 := 16#0ac2ab10#;
   pragma Export (C, u00160, "system__os_libB");
   u00161 : constant Version_32 := 16#d7b69782#;
   pragma Export (C, u00161, "system__os_libS");
   u00162 : constant Version_32 := 16#1a817b8e#;
   pragma Export (C, u00162, "system__stringsB");
   u00163 : constant Version_32 := 16#6ebc1091#;
   pragma Export (C, u00163, "system__stringsS");
   u00164 : constant Version_32 := 16#ed9c9b5f#;
   pragma Export (C, u00164, "system__file_control_blockS");
   u00165 : constant Version_32 := 16#bfcb6e33#;
   pragma Export (C, u00165, "gnat__directory_operationsB");
   u00166 : constant Version_32 := 16#919ee674#;
   pragma Export (C, u00166, "gnat__directory_operationsS");
   u00167 : constant Version_32 := 16#e5480ede#;
   pragma Export (C, u00167, "ada__strings__fixedB");
   u00168 : constant Version_32 := 16#a86b22b3#;
   pragma Export (C, u00168, "ada__strings__fixedS");
   u00169 : constant Version_32 := 16#a8ce4fe7#;
   pragma Export (C, u00169, "gnat__os_libS");
   u00170 : constant Version_32 := 16#7ebd8839#;
   pragma Export (C, u00170, "system__val_intB");
   u00171 : constant Version_32 := 16#58a62bc8#;
   pragma Export (C, u00171, "system__val_intS");
   u00172 : constant Version_32 := 16#b44f9ae7#;
   pragma Export (C, u00172, "system__val_unsB");
   u00173 : constant Version_32 := 16#9df3480c#;
   pragma Export (C, u00173, "system__val_unsS");
   u00174 : constant Version_32 := 16#084c16d0#;
   pragma Export (C, u00174, "gnat__regexpS");
   u00175 : constant Version_32 := 16#933fac2f#;
   pragma Export (C, u00175, "system__regexpB");
   u00176 : constant Version_32 := 16#e587bf12#;
   pragma Export (C, u00176, "system__regexpS");
   u00177 : constant Version_32 := 16#b4645806#;
   pragma Export (C, u00177, "gnat__stringsS");
   u00178 : constant Version_32 := 16#33489ddc#;
   pragma Export (C, u00178, "gnattest_generatedS");
   u00179 : constant Version_32 := 16#85ed49bf#;
   pragma Export (C, u00179, "gnattest_main_suiteB");
   u00180 : constant Version_32 := 16#eea1e993#;
   pragma Export (C, u00180, "gnattest_main_suiteS");
   u00181 : constant Version_32 := 16#a0b427ab#;
   pragma Export (C, u00181, "debugB");
   u00182 : constant Version_32 := 16#ce6ec09b#;
   pragma Export (C, u00182, "debugS");
   u00183 : constant Version_32 := 16#276453b7#;
   pragma Export (C, u00183, "system__img_lldB");
   u00184 : constant Version_32 := 16#e321089e#;
   pragma Export (C, u00184, "system__img_lldS");
   u00185 : constant Version_32 := 16#bd3715ff#;
   pragma Export (C, u00185, "system__img_decB");
   u00186 : constant Version_32 := 16#be2e082c#;
   pragma Export (C, u00186, "system__img_decS");
   u00187 : constant Version_32 := 16#9777733a#;
   pragma Export (C, u00187, "system__img_lliB");
   u00188 : constant Version_32 := 16#014c5426#;
   pragma Export (C, u00188, "system__img_lliS");
   u00189 : constant Version_32 := 16#306b4691#;
   pragma Export (C, u00189, "ada__real_timeB");
   u00190 : constant Version_32 := 16#87ade2f4#;
   pragma Export (C, u00190, "ada__real_timeS");
   u00191 : constant Version_32 := 16#1f99af62#;
   pragma Export (C, u00191, "system__arith_64B");
   u00192 : constant Version_32 := 16#307d063a#;
   pragma Export (C, u00192, "system__arith_64S");
   u00193 : constant Version_32 := 16#91b1145a#;
   pragma Export (C, u00193, "system__taskingB");
   u00194 : constant Version_32 := 16#0614561b#;
   pragma Export (C, u00194, "system__taskingS");
   u00195 : constant Version_32 := 16#4ef49049#;
   pragma Export (C, u00195, "system__task_primitivesS");
   u00196 : constant Version_32 := 16#b498031e#;
   pragma Export (C, u00196, "system__os_interfaceB");
   u00197 : constant Version_32 := 16#fe105d7f#;
   pragma Export (C, u00197, "system__os_interfaceS");
   u00198 : constant Version_32 := 16#f71cb626#;
   pragma Export (C, u00198, "interfaces__c__extensionsS");
   u00199 : constant Version_32 := 16#434c88e8#;
   pragma Export (C, u00199, "system__os_constantsS");
   u00200 : constant Version_32 := 16#c920fb2e#;
   pragma Export (C, u00200, "system__task_primitives__operationsB");
   u00201 : constant Version_32 := 16#0e983487#;
   pragma Export (C, u00201, "system__task_primitives__operationsS");
   u00202 : constant Version_32 := 16#89b55e64#;
   pragma Export (C, u00202, "system__interrupt_managementB");
   u00203 : constant Version_32 := 16#bec4dbfd#;
   pragma Export (C, u00203, "system__interrupt_managementS");
   u00204 : constant Version_32 := 16#f65595cf#;
   pragma Export (C, u00204, "system__multiprocessorsB");
   u00205 : constant Version_32 := 16#28af9e84#;
   pragma Export (C, u00205, "system__multiprocessorsS");
   u00206 : constant Version_32 := 16#e0fce7f8#;
   pragma Export (C, u00206, "system__task_infoB");
   u00207 : constant Version_32 := 16#9019bcf6#;
   pragma Export (C, u00207, "system__task_infoS");
   u00208 : constant Version_32 := 16#85267e7e#;
   pragma Export (C, u00208, "system__tasking__debugB");
   u00209 : constant Version_32 := 16#62f5b061#;
   pragma Export (C, u00209, "system__tasking__debugS");
   u00210 : constant Version_32 := 16#fd83e873#;
   pragma Export (C, u00210, "system__concat_2B");
   u00211 : constant Version_32 := 16#12a3d627#;
   pragma Export (C, u00211, "system__concat_2S");
   u00212 : constant Version_32 := 16#2b70b149#;
   pragma Export (C, u00212, "system__concat_3B");
   u00213 : constant Version_32 := 16#1b735d52#;
   pragma Export (C, u00213, "system__concat_3S");
   u00214 : constant Version_32 := 16#d0432c8d#;
   pragma Export (C, u00214, "system__img_enum_newB");
   u00215 : constant Version_32 := 16#714f0737#;
   pragma Export (C, u00215, "system__img_enum_newS");
   u00216 : constant Version_32 := 16#118e865d#;
   pragma Export (C, u00216, "system__stack_usageB");
   u00217 : constant Version_32 := 16#00bc3311#;
   pragma Export (C, u00217, "system__stack_usageS");
   u00218 : constant Version_32 := 16#c3f62490#;
   pragma Export (C, u00218, "debug__test_dataB");
   u00219 : constant Version_32 := 16#b3edac99#;
   pragma Export (C, u00219, "debug__test_dataS");
   u00220 : constant Version_32 := 16#269b1972#;
   pragma Export (C, u00220, "aunit__test_fixturesB");
   u00221 : constant Version_32 := 16#3b99f1a5#;
   pragma Export (C, u00221, "aunit__test_fixturesS");
   u00222 : constant Version_32 := 16#9dc9d9e1#;
   pragma Export (C, u00222, "debug__test_data__testsB");
   u00223 : constant Version_32 := 16#8ff91856#;
   pragma Export (C, u00223, "debug__test_data__testsS");
   u00224 : constant Version_32 := 16#d865bb77#;
   pragma Export (C, u00224, "debug__test_data__tests__suiteB");
   u00225 : constant Version_32 := 16#c9c253aa#;
   pragma Export (C, u00225, "debug__test_data__tests__suiteS");
   u00226 : constant Version_32 := 16#8f189ee8#;
   pragma Export (C, u00226, "aunit__test_callerB");
   u00227 : constant Version_32 := 16#581d22b8#;
   pragma Export (C, u00227, "aunit__test_callerS");
   u00228 : constant Version_32 := 16#d18018ae#;
   pragma Export (C, u00228, "testS");
   u00229 : constant Version_32 := 16#e9243836#;
   pragma Export (C, u00229, "bufferB");
   u00230 : constant Version_32 := 16#434db180#;
   pragma Export (C, u00230, "bufferS");
   u00231 : constant Version_32 := 16#e4226b57#;
   pragma Export (C, u00231, "circular_arraysB");
   u00232 : constant Version_32 := 16#a7737125#;
   pragma Export (C, u00232, "circular_arraysS");
   u00233 : constant Version_32 := 16#0ebdc065#;
   pragma Export (C, u00233, "pairsB");
   u00234 : constant Version_32 := 16#2b2e159f#;
   pragma Export (C, u00234, "pairsS");
   u00235 : constant Version_32 := 16#f2573306#;
   pragma Export (C, u00235, "timeS");
   u00236 : constant Version_32 := 16#51ef9209#;
   pragma Export (C, u00236, "buffer__readerB");
   u00237 : constant Version_32 := 16#e619abeb#;
   pragma Export (C, u00237, "buffer__readerS");
   u00238 : constant Version_32 := 16#615250d4#;
   pragma Export (C, u00238, "buffer__writerB");
   u00239 : constant Version_32 := 16#7384590c#;
   pragma Export (C, u00239, "buffer__writerS");
   u00240 : constant Version_32 := 16#0bc1456e#;
   pragma Export (C, u00240, "monitorB");
   u00241 : constant Version_32 := 16#e1b711da#;
   pragma Export (C, u00241, "monitorS");
   u00242 : constant Version_32 := 16#28ea4664#;
   pragma Export (C, u00242, "monitor__mytestmonitorB");
   u00243 : constant Version_32 := 16#089bf42b#;
   pragma Export (C, u00243, "monitor__mytestmonitorS");
   u00244 : constant Version_32 := 16#308a8d5a#;
   pragma Export (C, u00244, "test__test_dataB");
   u00245 : constant Version_32 := 16#89828373#;
   pragma Export (C, u00245, "test__test_dataS");
   u00246 : constant Version_32 := 16#b4869c86#;
   pragma Export (C, u00246, "test__test_data__testsS");
   u00247 : constant Version_32 := 16#494132e6#;
   pragma Export (C, u00247, "test__test_data__tests__buffer_1S");
   u00248 : constant Version_32 := 16#65731234#;
   pragma Export (C, u00248, "test__test_data__tests__buffer_1__test_dataB");
   u00249 : constant Version_32 := 16#e2298139#;
   pragma Export (C, u00249, "test__test_data__tests__buffer_1__test_dataS");
   u00250 : constant Version_32 := 16#1f087f25#;
   pragma Export (C, u00250, "buffer__test_dataB");
   u00251 : constant Version_32 := 16#97305146#;
   pragma Export (C, u00251, "buffer__test_dataS");
   u00252 : constant Version_32 := 16#74ab567d#;
   pragma Export (C, u00252, "buffer__test_data__testsB");
   u00253 : constant Version_32 := 16#f0bcc6b1#;
   pragma Export (C, u00253, "buffer__test_data__testsS");
   u00254 : constant Version_32 := 16#f14953e7#;
   pragma Export (C, u00254, "test__test_data__tests__buffer_1__test_data__testsS");
   u00255 : constant Version_32 := 16#c01e011a#;
   pragma Export (C, u00255, "test__test_data__tests__buffer_1__test_data__tests__suiteB");
   u00256 : constant Version_32 := 16#6c76e0b6#;
   pragma Export (C, u00256, "test__test_data__tests__buffer_1__test_data__tests__suiteS");
   u00257 : constant Version_32 := 16#84754d7f#;
   pragma Export (C, u00257, "test__test_data__tests__monitor_proc_1S");
   u00258 : constant Version_32 := 16#919a4b62#;
   pragma Export (C, u00258, "test__test_data__tests__monitor_proc_1__test_dataB");
   u00259 : constant Version_32 := 16#74ab41ca#;
   pragma Export (C, u00259, "test__test_data__tests__monitor_proc_1__test_dataS");
   u00260 : constant Version_32 := 16#7531fc8b#;
   pragma Export (C, u00260, "monitor__mytestmonitor__test_dataB");
   u00261 : constant Version_32 := 16#f0bc8b16#;
   pragma Export (C, u00261, "monitor__mytestmonitor__test_dataS");
   u00262 : constant Version_32 := 16#0a909061#;
   pragma Export (C, u00262, "monitor__mytestmonitor__test_data__testsB");
   u00263 : constant Version_32 := 16#e7a9c626#;
   pragma Export (C, u00263, "monitor__mytestmonitor__test_data__testsS");
   u00264 : constant Version_32 := 16#7ece2871#;
   pragma Export (C, u00264, "test__test_data__tests__monitor_proc_1__test_data__testsS");
   u00265 : constant Version_32 := 16#acb7bc0f#;
   pragma Export (C, u00265, "test__test_data__tests__monitor_proc_1__test_data__tests__suiteB");
   u00266 : constant Version_32 := 16#b0788215#;
   pragma Export (C, u00266, "test__test_data__tests__monitor_proc_1__test_data__tests__suiteS");
   u00267 : constant Version_32 := 16#8b0117b1#;
   pragma Export (C, u00267, "test__test_data__tests__reader_1S");
   u00268 : constant Version_32 := 16#c69db10f#;
   pragma Export (C, u00268, "test__test_data__tests__reader_1__test_dataB");
   u00269 : constant Version_32 := 16#53b33842#;
   pragma Export (C, u00269, "test__test_data__tests__reader_1__test_dataS");
   u00270 : constant Version_32 := 16#c25835b8#;
   pragma Export (C, u00270, "buffer__reader__test_dataB");
   u00271 : constant Version_32 := 16#c992a9b9#;
   pragma Export (C, u00271, "buffer__reader__test_dataS");
   u00272 : constant Version_32 := 16#2ede6392#;
   pragma Export (C, u00272, "buffer__reader__test_data__testsB");
   u00273 : constant Version_32 := 16#2f57d3aa#;
   pragma Export (C, u00273, "buffer__reader__test_data__testsS");
   u00274 : constant Version_32 := 16#061873ac#;
   pragma Export (C, u00274, "test__test_data__tests__reader_1__test_data__testsS");
   u00275 : constant Version_32 := 16#cadf0ae5#;
   pragma Export (C, u00275, "test__test_data__tests__reader_1__test_data__tests__suiteB");
   u00276 : constant Version_32 := 16#8acd0e77#;
   pragma Export (C, u00276, "test__test_data__tests__reader_1__test_data__tests__suiteS");
   u00277 : constant Version_32 := 16#7ddad4f2#;
   pragma Export (C, u00277, "test__test_data__tests__writer_1S");
   u00278 : constant Version_32 := 16#4626a41c#;
   pragma Export (C, u00278, "test__test_data__tests__writer_1__test_dataB");
   u00279 : constant Version_32 := 16#9b16cf2f#;
   pragma Export (C, u00279, "test__test_data__tests__writer_1__test_dataS");
   u00280 : constant Version_32 := 16#7b28311d#;
   pragma Export (C, u00280, "buffer__writer__test_dataB");
   u00281 : constant Version_32 := 16#a16799f5#;
   pragma Export (C, u00281, "buffer__writer__test_dataS");
   u00282 : constant Version_32 := 16#ff6c4c2f#;
   pragma Export (C, u00282, "buffer__writer__test_data__testsB");
   u00283 : constant Version_32 := 16#a463e3c6#;
   pragma Export (C, u00283, "buffer__writer__test_data__testsS");
   u00284 : constant Version_32 := 16#f53bc9b3#;
   pragma Export (C, u00284, "test__test_data__tests__writer_1__test_data__testsS");
   u00285 : constant Version_32 := 16#38161ce7#;
   pragma Export (C, u00285, "test__test_data__tests__writer_1__test_data__tests__suiteB");
   u00286 : constant Version_32 := 16#70e4db0f#;
   pragma Export (C, u00286, "test__test_data__tests__writer_1__test_data__tests__suiteS");
   --  BEGIN ELABORATION ORDER
   --  ada%s
   --  ada.characters%s
   --  ada.characters.handling%s
   --  ada.characters.latin_1%s
   --  ada.command_line%s
   --  gnat%s
   --  gnat.io%s
   --  gnat.io%b
   --  gnat.source_info%s
   --  interfaces%s
   --  system%s
   --  system.address_operations%s
   --  system.address_operations%b
   --  system.arith_64%s
   --  system.atomic_counters%s
   --  system.atomic_counters%b
   --  system.case_util%s
   --  system.case_util%b
   --  system.exn_int%s
   --  system.exn_int%b
   --  system.htable%s
   --  system.img_bool%s
   --  system.img_bool%b
   --  system.img_dec%s
   --  system.img_enum_new%s
   --  system.img_enum_new%b
   --  system.img_int%s
   --  system.img_int%b
   --  system.img_dec%b
   --  system.img_lld%s
   --  system.img_lli%s
   --  system.img_lli%b
   --  system.img_lld%b
   --  system.io%s
   --  system.io%b
   --  system.multiprocessors%s
   --  system.os_primitives%s
   --  system.os_primitives%b
   --  system.parameters%s
   --  system.parameters%b
   --  system.crtl%s
   --  interfaces.c_streams%s
   --  interfaces.c_streams%b
   --  system.standard_library%s
   --  system.exceptions_debug%s
   --  system.exceptions_debug%b
   --  system.storage_elements%s
   --  system.storage_elements%b
   --  system.stack_checking%s
   --  system.stack_checking%b
   --  system.stack_usage%s
   --  system.stack_usage%b
   --  system.string_hash%s
   --  system.string_hash%b
   --  system.htable%b
   --  system.strings%s
   --  system.strings%b
   --  gnat.strings%s
   --  system.os_lib%s
   --  gnat.os_lib%s
   --  system.task_info%s
   --  system.task_info%b
   --  system.traceback_entries%s
   --  system.traceback_entries%b
   --  ada.exceptions%s
   --  system.arith_64%b
   --  system.soft_links%s
   --  system.unsigned_types%s
   --  system.val_int%s
   --  system.val_llu%s
   --  system.val_uns%s
   --  system.val_util%s
   --  system.val_util%b
   --  system.val_uns%b
   --  system.val_llu%b
   --  system.val_int%b
   --  system.wch_con%s
   --  system.wch_con%b
   --  system.wch_cnv%s
   --  system.wch_jis%s
   --  system.wch_jis%b
   --  system.wch_cnv%b
   --  system.wch_stw%s
   --  system.wch_stw%b
   --  ada.exceptions.last_chance_handler%s
   --  ada.exceptions.last_chance_handler%b
   --  ada.exceptions.traceback%s
   --  system.address_image%s
   --  system.bit_ops%s
   --  system.bit_ops%b
   --  system.compare_array_unsigned_8%s
   --  system.compare_array_unsigned_8%b
   --  system.concat_2%s
   --  system.concat_2%b
   --  system.concat_3%s
   --  system.concat_3%b
   --  system.exception_table%s
   --  system.exception_table%b
   --  ada.io_exceptions%s
   --  ada.strings%s
   --  ada.strings.maps%s
   --  ada.strings.fixed%s
   --  ada.strings.maps.constants%s
   --  ada.strings.search%s
   --  ada.strings.search%b
   --  ada.tags%s
   --  ada.streams%s
   --  ada.streams%b
   --  interfaces.c%s
   --  system.multiprocessors%b
   --  interfaces.c.extensions%s
   --  system.exceptions%s
   --  system.exceptions%b
   --  system.exceptions.machine%s
   --  system.file_control_block%s
   --  system.file_io%s
   --  system.finalization_root%s
   --  system.finalization_root%b
   --  ada.finalization%s
   --  ada.finalization%b
   --  system.os_constants%s
   --  system.os_interface%s
   --  system.os_interface%b
   --  system.interrupt_management%s
   --  system.interrupt_management%b
   --  system.storage_pools%s
   --  system.storage_pools%b
   --  system.finalization_masters%s
   --  system.storage_pools.subpools%s
   --  system.storage_pools.subpools.finalization%s
   --  system.storage_pools.subpools.finalization%b
   --  system.stream_attributes%s
   --  system.stream_attributes%b
   --  system.task_primitives%s
   --  system.tasking%s
   --  system.task_primitives.operations%s
   --  system.tasking%b
   --  system.tasking.debug%s
   --  system.tasking.debug%b
   --  system.task_primitives.operations%b
   --  ada.calendar%s
   --  ada.calendar%b
   --  gnat.directory_operations%s
   --  system.memory%s
   --  system.memory%b
   --  system.standard_library%b
   --  system.pool_global%s
   --  system.pool_global%b
   --  system.secondary_stack%s
   --  system.storage_pools.subpools%b
   --  system.finalization_masters%b
   --  system.file_io%b
   --  interfaces.c%b
   --  ada.tags%b
   --  ada.strings.fixed%b
   --  ada.strings.maps%b
   --  system.soft_links%b
   --  system.os_lib%b
   --  ada.command_line%b
   --  ada.characters.handling%b
   --  system.secondary_stack%b
   --  gnat.directory_operations%b
   --  system.address_image%b
   --  ada.exceptions.traceback%b
   --  ada.strings.unbounded%s
   --  ada.strings.unbounded%b
   --  system.regexp%s
   --  system.regexp%b
   --  gnat.regexp%s
   --  gnat.command_line%s
   --  system.traceback%s
   --  ada.exceptions%b
   --  system.traceback%b
   --  ada.real_time%s
   --  ada.real_time%b
   --  ada.text_io%s
   --  ada.text_io%b
   --  gnat.command_line%b
   --  gnat.traceback%s
   --  gnat.traceback%b
   --  system.traceback.symbolic%s
   --  system.traceback.symbolic%b
   --  gnat.traceback.symbolic%s
   --  ada_containers%s
   --  ada_containers.aunit_lists%s
   --  aunit%s
   --  aunit.memory%s
   --  aunit.memory%b
   --  aunit%b
   --  aunit.memory.utils%s
   --  aunit.memory.utils%b
   --  ada_containers.aunit_lists%b
   --  aunit.tests%s
   --  aunit.test_filters%s
   --  aunit.options%s
   --  aunit.time_measure%s
   --  aunit.time_measure%b
   --  aunit.test_results%s
   --  aunit.test_results%b
   --  aunit.assertions%s
   --  aunit.assertions%b
   --  aunit.reporter%s
   --  aunit.reporter.gnattest%s
   --  aunit.reporter.gnattest%b
   --  aunit.simple_test_cases%s
   --  aunit.simple_test_cases%b
   --  aunit.test_filters%b
   --  aunit.test_fixtures%s
   --  aunit.test_fixtures%b
   --  aunit.test_caller%s
   --  aunit.test_caller%b
   --  aunit.test_suites%s
   --  aunit.test_suites%b
   --  aunit.run%s
   --  aunit.run%b
   --  circular_arrays%s
   --  circular_arrays%b
   --  debug%s
   --  debug%b
   --  debug.test_data%s
   --  debug.test_data%b
   --  gnattest_generated%s
   --  debug.test_data.tests%s
   --  debug.test_data.tests%b
   --  debug.test_data.tests.suite%s
   --  debug.test_data.tests.suite%b
   --  gnattest_main_suite%s
   --  test_runner%b
   --  time%s
   --  pairs%s
   --  pairs%b
   --  buffer%s
   --  buffer%b
   --  buffer.reader%s
   --  buffer.reader%b
   --  buffer.reader.test_data%s
   --  buffer.reader.test_data%b
   --  buffer.reader.test_data.tests%s
   --  buffer.reader.test_data.tests%b
   --  buffer.test_data%s
   --  buffer.test_data%b
   --  buffer.test_data.tests%s
   --  buffer.test_data.tests%b
   --  buffer.writer%s
   --  buffer.writer%b
   --  buffer.writer.test_data%s
   --  buffer.writer.test_data%b
   --  buffer.writer.test_data.tests%s
   --  buffer.writer.test_data.tests%b
   --  monitor%s
   --  monitor%b
   --  monitor.mytestmonitor%s
   --  monitor.mytestmonitor%b
   --  monitor.mytestmonitor.test_data%s
   --  monitor.mytestmonitor.test_data%b
   --  monitor.mytestmonitor.test_data.tests%s
   --  monitor.mytestmonitor.test_data.tests%b
   --  test%s
   --  test.test_data%s
   --  test.test_data%b
   --  test.test_data.tests%s
   --  test.test_data.tests.buffer_1%s
   --  test.test_data.tests.buffer_1.test_data%s
   --  test.test_data.tests.buffer_1.test_data%b
   --  test.test_data.tests.buffer_1.test_data.tests%s
   --  test.test_data.tests.buffer_1.test_data.tests.suite%s
   --  test.test_data.tests.buffer_1.test_data.tests.suite%b
   --  test.test_data.tests.monitor_proc_1%s
   --  test.test_data.tests.monitor_proc_1.test_data%s
   --  test.test_data.tests.monitor_proc_1.test_data%b
   --  test.test_data.tests.monitor_proc_1.test_data.tests%s
   --  test.test_data.tests.monitor_proc_1.test_data.tests.suite%s
   --  test.test_data.tests.monitor_proc_1.test_data.tests.suite%b
   --  test.test_data.tests.reader_1%s
   --  test.test_data.tests.reader_1.test_data%s
   --  test.test_data.tests.reader_1.test_data%b
   --  test.test_data.tests.reader_1.test_data.tests%s
   --  test.test_data.tests.reader_1.test_data.tests.suite%s
   --  test.test_data.tests.reader_1.test_data.tests.suite%b
   --  test.test_data.tests.writer_1%s
   --  test.test_data.tests.writer_1.test_data%s
   --  test.test_data.tests.writer_1.test_data%b
   --  test.test_data.tests.writer_1.test_data.tests%s
   --  test.test_data.tests.writer_1.test_data.tests.suite%s
   --  test.test_data.tests.writer_1.test_data.tests.suite%b
   --  gnattest_main_suite%b
   --  END ELABORATION ORDER


end ada_main;

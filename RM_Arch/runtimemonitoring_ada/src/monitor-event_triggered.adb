package body Monitor.Event_Triggered is

   package body External_Interface is
      procedure Call is
      begin
         -- trigger the execution of the monitor
         Monitoring_Flags.Signal;
         -- wait for the monitoring procedure to start
         Monitoring_Flags.WaitForStart;
         -- wait for the monitoring procedure to complete
         Monitoring_Flags.WaitForCompletion;
      end Call;

      procedure Signal is
      begin
         -- trigger the execution of the monitor
         Monitoring_Flags.Signal;
         -- ... but do not wait for its completion
      end Signal;

      procedure Wait is
      begin
         -- wait for the monitoring procedure to start
         Monitoring_Flags.WaitForStart;
         -- wait for the monitoring procedure to complete
         Monitoring_Flags.WaitForCompletion;
      end Wait;
   end External_Interface;

end Monitor.Event_Triggered;

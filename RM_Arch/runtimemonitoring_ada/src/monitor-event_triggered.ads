package Monitor.Event_Triggered is

   -- An interface with the external world can be instanciated to externally
   -- trigger the execution of the monitor, either in a blocking (Call) or
   -- non-blocking (Signal) manner. This interface can also be used to simply
   -- wait for the monitor to start and then complet its execution.
   -- => Note that Call is equivalent to Signal followed by Wait.
   generic
   package External_Interface is
      procedure Signal; -- non-blocking trigger of the monitoring procedure
      procedure Call; -- trigger and waits for the completion of the execution of the monitoring procedure
      procedure Wait; -- wait for the monitoring procedure to executed
   end External_Interface;

end package Monitor.Event_Triggered

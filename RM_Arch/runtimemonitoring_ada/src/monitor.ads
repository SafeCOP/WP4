with Ada.Real_Time;
with Buffer.Reader;
with Time;

-- @summary
-- Basic periodic monitor.
-- @description
-- This package provides the required infrastructure to generate a periodic task
-- calling a monitoring procedure. The specific monitoring procedure associated
-- to each monitor should be defined in a child package in which the periodic
-- monitoring task should also be instanciated (see the documentation of the nested
-- package Monitoring_Task).
--
generic
   Priority : Integer; -- Priority of the task executing the monitoring procedure
   Period : Ada.Real_Time.Time_Span ; -- Period of activation of the monitoring procedure
package Monitor is

private
   Monitor_Creation_Exception : exception;

   -- @summary
   -- The periodic task activating the monitoring procedure.
   -- @description
   -- The actual periodic task activating the monitoring procedure passed as a
   -- generic. Only one such monitoring task can be instanciated per monitor.
   -- It is usually instanciated in the body of a child package which also
   -- defines the monitoring procedure.
   --
   generic
      with procedure Monitoring_Procedure; -- The procedure that will periodically be called by the monitoring task.
   package Monitoring_Task is

   private
      -- Instance of the periodic task calling the monitoring procedure;
      task type Periodic_Monitor with Priority => Priority;
   end Monitoring_Task;

   -- @summary
   -- Reader synchronised with all other Synchronised_Reader instanciated in
   -- the monitor.
   -- @description
   -- Synchronised_Reader is a reader providing a Pop and Get operation similarly
   -- to the simple Buffer.Reader. The difference lies in the fact that all the
   -- Synchronised_Readers instanciated in a same monitor are synchronised together.
   -- That is, when calling a Pop or Get operation from a synchronised_reader, all the
   -- elements saved in the associated buffer with a time stamp earlier than the
   -- last time stamp popped from any buffer by any synchronised_reader instanciated
   -- in the monitor, are discarded. Hence, synchronised_readers somewhat reorder
   -- the elements saved in different buffers so that they are read in an increasing
   -- time of occurence order.
   -- Example: we have two buffers used by the monitor. Buffer_1 = < 1 , 2 , 4 , 7 >
   --          and Buffer_2 = < 3 , 5 , 8 >, where the numbers refer to time stamps.
   --          If we first use a Pop operation on Buffer_2 using a synchronised_reader
   --          we get 3 (earliest element in the buffer). Then, if we call a Pop or Get
   --          operation using another synchronised_reader on Buffer_1, we receive
   --          4. The elements with the time stamps 1 and 2 are ignored.
   --          Note 1: if we had started with a Pop on Buffer_1 rather than Buffer_2
   --          we would have received 1.
   --          Note 2: simple Buffer.Readers instanciated in the monitor do not
   --          affect the order in which elements are read by synchronised_readers.
   --          Note 3: synchronised_readers instanciated in the monitor do not
   --          affect the order in which elements are read by simple Buffer.Readers
   --          instanciated in the same monitor.
   generic
      with package InputBuffer is new Buffer(<>); -- Buffer to be read by the synchronised_reader
   package Synchronised_Reader is

      -- Send back and remove from the buffer the first element of the buffer
      -- with a time stamp not earlier than the last time stamp popped from any
      -- buffer by any synchronised_reader instanciated in the same monitor.
      -- @param Data Takes the value of the data associated with the earliest element saved in the buffer that respect the constraint defined above.
      -- @param Time_Stamp Takes the value of the time stamp associated with the earliest element saved in the buffer that respect the constraint defined above.
      -- @param Is_Empty Is True when there is no element in the buffer respecting the constraint defined above.
      -- @param Has_Gap Is true when some relevant element in the buffer have been overwritten before to be read, i.e., when there is a gap in the trace.
      procedure Pop(Data :out InputBuffer.Event_Info_Type; Time_Stamp :out Ada.Real_Time.Time; Is_Empty : out Boolean; Has_Gap : out Boolean);

      -- Send back the first element of the buffer with a time stamp not earlier
      -- than the last time stamp popped from any buffer by any synchronised_reader
      -- instanciated in the same monitor.
      -- @param Data Takes the value of the data associated with the earliest element saved in the buffer that respect the constraint defined above.
      -- @param Time_Stamp Takes the value of the time stamp associated with the earliest element saved in the buffer that respect the constraint defined above.
      -- @param Is_Empty Is True when there is no element in the buffer respecting the constraint defined above.
      -- @param Has_Gap Is true when some relevant element in the buffer have been overwritten before to be read, i.e., when there is a gap in the trace.
      procedure Get(Data :out InputBuffer.Event_Info_Type; Time_Stamp :out Ada.Real_Time.Time; Is_Empty : out Boolean; Has_Gap : out Boolean);

   private
      -- Hidden buffer_reader on the input buffer sent as a generic.
      package Reader is new InputBuffer.Reader;

      -- Expected index of the next element to be read in the buffer.
      Current_Index : InputBuffer.Buffer_Index := 0;

      -- Variable used to detect when the element pointed by Current_Index has
      -- been overwritten between two calls to Pop or Get.
      Time_Current_Index_Save : Ada.Real_Time.Time := Time.Zero;
   end Synchronised_Reader;

end Monitor;

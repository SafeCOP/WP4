package body Monitor is

   use Ada.Real_Time;
   
   Monitor_Exists : Boolean := False;

   package body Monitoring_Task is
      
      use Ada.Real_Time;

      task body Periodic_Monitor is
         -- Variables for real-time task
         Next_Time : Ada.Real_Time.Time := Ada.Real_Time.Clock;         
      begin      
         Next_Time := Next_Time + Period;
         loop
            delay until Next_Time;   
            Monitoring_Procedure;
            Next_Time := Next_Time + Period;                
         end loop;
      end Periodic_Monitor;

      -- Creation of the actual instance of the task
      Monitor_Task: Periodic_Monitor;
      
   begin
      -- check that there is only one monitor instanciated
      if Monitor_Exists=True then
         raise Monitor_Creation_Exception with "More than one monitoring task associated with this monitor.";  
      end if;
      Monitor_Exists := True;
   end Monitoring_Task;
   
   -- Time of last read made by any of the buffers being used by the monitor
   Last_Read_Time_Stamp : Ada.Real_Time.Time:= Time.Zero; 

   package body Synchronised_Reader is
      
      use InputBuffer;
      
      procedure Pop(Data :out InputBuffer.Event_Info_Type; Time_Stamp :out Ada.Real_Time.Time; Is_Empty : out Boolean; Has_Gap : out Boolean) is
         Position_Last_Written : InputBuffer.Buffer_Index;
         Time_Last_Overwritten, Time_Last_Written, Time_Current_Read : Ada.Real_Time.Time;
         Data_Current_Read : InputBuffer.Event_Info_Type;
         Gap_In_Trace : Boolean := False;
      begin      
         Reader.Get_Last_Written(Position_Last_Written,Time_Last_Written); 
	 
	 if (Time_Last_Written = Time.Zero) or (Time_Last_Written < Last_Read_Time_Stamp) then
            -- there is no event that happend after Last_Read
            -- => we reposition the Current_Index on the end of the buffer
            --    and we write that there is nothing to read in the status 
            --    variable
            declare
               Buffer_End : InputBuffer.Buffer_Index := Position_Last_Written + 1; 
            begin
               Current_Index := Buffer_End;
               Time_Current_Index_Save := Time.Zero;
            end;            
            Is_Empty := True;
            Has_Gap := False;
            
         else	 
            
            Reader.Read(Current_Index, Data_Current_Read, Time_Current_Read); 
            
            if Time_Current_Read <= Last_Read_Time_Stamp then
               loop                  
                  Increment(Current_Index);                 
                  -- no need to check if ( Current_Index /= Buffer_End ) since
                  -- we know that at least Time_Last_Written >= Last_Read_Time_Stamp
                  exit when Time_Current_Read >= Last_Read_Time_Stamp;
                  
                  Reader.Read(Current_Index,Data_Current_Read,Time_Current_Read);
               end loop;
                              
            elsif( Time_Current_Read /= Time_Current_Index_Save ) then
               -- the current_index has been ovewritten since our last read
               -- => we may be too far in the trace 
               -- => we go backward

               declare
                  J : InputBuffer.Buffer_Index := Current_Index - 1; 
                  Ji : InputBuffer.Event_Info_Type;
                  Jt : Ada.Real_Time.Time;
               begin
                  loop                     
                     Reader.Read(J,Ji,Jt);             
                     exit when Jt = Time.Zero or Jt < Last_Read_Time_Stamp or Jt > Time_Current_Read;
                     
                     Data_Current_Read := Ji;
                     Time_Current_Read := Jt;
                     J := J - 1; --Inputbuffer.Buffer_Index((Natural(J) + InputBuffer.Buffer_Index'Last) mod (InputBuffer.Buffer_Index'Last+1));
                  end loop; 
                  
                  Current_Index := J + 2; --Inputbuffer.Buffer_Index((Natural(J)+2) mod (InputBuffer.Buffer_Index'Last+1));
               end; 
            else
               -- current_index is the next event to be read
               -- => we send back the info releated to current_index and 
               --    we update current_index 
               Increment(Current_Index);  
               
            end if;
            
            Data := Data_Current_Read;
            Time_Stamp := Time_Current_Read;
            Is_Empty := False;
            
            -- save the time stamp of the new Current_Index
            Reader.Read(Current_Index,Data_Current_Read,Time_Current_Index_Save);
            if Time_Current_Index_Save < Time_Current_Read then
               -- we are at the end of the buffer
               Time_Current_Index_Save := Time.Zero;
            end if;
            
            -- Check if there is a gap in the trace; that is, Last_Read is smaller
            -- than the timestamp of the last ovewritten value (note that it must 
            -- be the last thing checked if we want to be sure to detect any gap
            -- even with race conditions between readers and writers in the buffer)
            Reader.Get_Last_Overwritten(Time_Last_Overwritten);
            if Last_Read_Time_Stamp < Time_Last_Overwritten then
               Has_Gap := True;
               if Time_Last_Overwritten > Time_Current_Read then
                  -- due to race conditions between the reader and the writer,
                  -- the element we just read and the one after were overwritten. 
                  -- Hence, we do not know where we are in the trace.
                  Time_Current_Index_Save := Time.Zero;
               end if;
            else
               Has_Gap := False;
            end if;
            
            -- update last_read_time_stamp
            Last_Read_Time_Stamp := Time_Current_Read;
            
         end if;
         

      end Pop;
      
      
      procedure Get(Data :out InputBuffer.Event_Info_Type; Time_Stamp :out Ada.Real_Time.Time; Is_Empty : out Boolean; Has_Gap : out Boolean) is
         Position_Last_Written : InputBuffer.Buffer_Index;
         Time_Last_Overwritten, Time_Last_Written, Time_Current_Read : Ada.Real_Time.Time;
         Data_Current_Read : InputBuffer.Event_Info_Type;
         Gap_In_Trace : Boolean := False;
      begin      
         Reader.Get_Last_Written(Position_Last_Written,Time_Last_Written); 
	 
	 if (Time_Last_Written = Time.Zero) or (Time_Last_Written < Last_Read_Time_Stamp) then
            -- there is no event that happend after Last_Read
            -- => we reposition the Current_Index on the end of the buffer
            --    and we write that there is nothing to read in the status 
            --    variable
            declare
               Buffer_End : InputBuffer.Buffer_Index := Position_Last_Written + 1; 
            begin
               Current_Index := Buffer_End;
               Time_Current_Index_Save := Time.Zero;
            end;            
            
            Is_Empty := True;
            Has_Gap := False;
            
         else	 
            
            Reader.Read(Current_Index, Data_Current_Read, Time_Current_Read); 
            
            if Time_Current_Read <= Last_Read_Time_Stamp then
                                   
               -- no need to check if ( Current_Index /= Buffer_End ) since
               -- we know that at least Time_Last_Written >= Last_Read_Time_Stamp
               while Time_Current_Read < Last_Read_Time_Stamp loop
                  Increment(Current_Index);  
                  Reader.Read(Current_Index, Data_Current_Read, Time_Current_Read);
               end loop;
                              
            elsif( Time_Current_Read /= Time_Current_Index_Save ) then
               -- the current_index has been ovewritten since our last read
               -- => we may be too far in the trace 
               -- => we go backward
               declare
                  J : InputBuffer.Buffer_Index := Current_Index - 1; 
                  Ji : InputBuffer.Event_Info_Type;
                  Jt : Ada.Real_Time.Time;
               begin
                  loop                     
                     Reader.Read(J,Ji,Jt);             
                     exit when Jt = Time.Zero or Jt < Last_Read_Time_Stamp or Jt > Time_Current_Read;
                     
                     Data_Current_Read := Ji;
                     Time_Current_Read := Jt;
                     J := J - 1; 
                  end loop; 
                  
                  Current_Index := J + 1;
               end; 
            --else
               -- current_index is the next event to be read
               -- => we simply send back the info releated to current_index
            end if;
            
            Data := Data_Current_Read;
            Time_Stamp := Time_Current_Read;
            Is_Empty := False;     
                                    
            -- save the time stamp of the new Current_Index
            Time_Current_Index_Save := Time_Current_Read;
                        
            -- Check if there is a gap in the trace; that is, Last_Read is smaller
            -- than the timestamp of the last ovewritten value (note that it must 
            -- be the last thing checked if we want to be sure to detect any gap
            -- even with race conditions between readers and writers in the buffer)
            Reader.Get_Last_Overwritten(Time_Last_Overwritten);
            if Last_Read_Time_Stamp < Time_Last_Overwritten then
               Has_Gap := True;
            else
               Has_Gap := False;
            end if;
         end if;
         
      end Get;
            

   end Synchronised_Reader;

begin
   
   Last_Read_Time_Stamp := Time.Zero; 

end Monitor;

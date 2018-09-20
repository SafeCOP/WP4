package body Monitor.Order is

   procedure Mark_Presence is
      V : Integer;
      T : Ada.Real_Time.Time;
      --S : Read_State;
      Empty, Gap : Boolean;
      I : Integer := 0;
      N : Integer := 0;
      --N2 : Integer := 0;
      --N3 : Integer := 0;
      package M1_Reader is new Synchronised_Reader(InputBuffer => Buffer_Declarations.Buffer_1);
   begin
      M1_Reader.Pop(V,T,Empty,Gap);
      --if S = Read_Gap then
      Ada.Text_Io.Put_Line("(" & V'Img & "," & Debug.Rt_To_String(T) & ")");
      --end if;
      --case S is
      --when Read_Gap => Ada.Text_IO.Put("Gap ");
      -- when Read_Empty => Ada.Text_IO.Put("Empty ");
      --when Read_Fail => Ada.Text_IO.Put("Fail ");
      --	 when Read_Success => Ada.Text_IO.Put("Success!!!");
      --end case;

      --if V /= 0 then
      --Ada.Text_IO.Put_Line("Yuppy");
      --end if;
      --Ada.Text_IO.Put_Line("("&V'Img&","&Debug.Rt_To_String(T)&")");
   end Mark_Presence;

   package M1 is new Monitoring_Task(Mark_Presence);
end Monitor.Order;

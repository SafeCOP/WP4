package body Debug is
   function Rt_To_String(T :in Ada.Real_Time.Time) return String is
      TS : Ada.Real_Time.Time_Span;
      Sc : Ada.Real_Time.Seconds_Count;
   begin
      Ada.Real_Time.Split(T,Sc,Ts);
      return (Ada.Real_Time.Seconds_Count'Image(Sc) & " [ " & Duration'Image(Ada.Real_Time.To_Duration(Ts)) & " ] ");
   end Rt_To_String;

end Debug;

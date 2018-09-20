package body Pairs is
   
   procedure Set_Data(X:out Pair;V:in Data_Type) is
   begin
      X.Data := V;
   end Set_Data;
   
   procedure Set_Time(X:out Pair;V:in Ada.Real_Time.Time) is
   begin
      X.Time := V;
   end Set_Time;
   
   function Get_Data(X:in Pair) return Data_Type is
   begin
      return X.Data;
   end Get_Data;
   
   function Get_Time(X:in Pair) return Ada.Real_Time.Time is
   begin
      return X.Time;
   end Get_Time;
   
--     procedure Print(X:in Pair) is
--        T : Ada.Real_Time.Time := X.Time;
--        S : Ada.Real_Time.Seconds_Count;
--        D : Ada.Real_Time.Time_Span;
--     begin
--        Ada.Real_Time.Split(T,S,D);
--        Ada.Text_Io.Put_Line("Event: (" & Data_Type_Print(X.Data) & "," & Duration'Image(Ada.Real_Time.To_Duration(D)) & ")");
--     end Print;
   
end Pairs;

import java.awt.*;

public interface Operations  {

	public Color [] Colors = {Color.RED, Color.ORANGE, Color.YELLOW, Color.GREEN, Color.CYAN, Color.BLUE, Color.MAGENTA};
	
	public default String Get_Type() {
		return "";
	}
	
	public default void Set_Type(String Type) {
	}
	public default Coords Get_Coords() {
		return new Coords();
	}
	
	public default void Set_Coords (int X, int Y) {
	}
	
	public default void Change_Coords (Coords Screen_Size, long time) {
    }
	
	public default void Set_Size(int Size) {
	}

	public default int Get_Size() {
		return 0;
	}

	public default void Change_Size(double k) {
	}
	
	public default void Set_Speed(int X, int Y) {
	}

	public default Coords Get_Speed() {
		return new Coords();
	}

	public default void Change_Speed(double k) {
	}
	
	public default void Set_Keys(Coords[] Keys) {
	}
	
	public default Coords[] Get_Keys() {
		return new Coords[1];
	}
	
	public default void Set_Points () {
	}
	 
    public default int Get_Color() {
    	return 0;
    }

	public default void Set_Color() {
	}

	public default void Change_Color() {
	}
    
	public default boolean Check_Key_Point (Coords[] Points) {
		return false;
	}
	
	public default boolean Crossing(Object Obj){
		return false;
	}

}

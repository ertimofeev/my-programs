public class Coords {
	public int X;
	public int Y;
	
	public void Set_Coords(int X, int Y) {
		this.X = X;
		this.Y = Y;
	}
	
	public int Distance(Coords point) {
		return (int)Math.round(Math.pow(Math.pow(this.X-point.X, 2) + Math.pow(this.Y-point.Y, 2), 0.5));
	}
}

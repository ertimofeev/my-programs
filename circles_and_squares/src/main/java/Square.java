public class Square extends Figure {

	public void Set_Points () {
		Coords[] Points = new Coords[4];
		Points[0] = new Coords();
		Points[0].Set_Coords(this.Get_Coords().X, this.Get_Coords().Y);
		Points[1] = new Coords();
		Points[1].Set_Coords(this.Get_Coords().X + 2*this.Get_Size(), this.Get_Coords().Y);
		Points[2] = new Coords();
		Points[2].Set_Coords(this.Get_Coords().X + 2*this.Get_Size(), this.Get_Coords().Y + 2*this.Get_Size());
		Points[3] = new Coords();
		Points[3].Set_Coords(this.Get_Coords().X, this.Get_Coords().Y + 2*this.Get_Size());
		this.Set_Keys(Points);
	}
	
	public boolean Check_Key_Point (Coords[] Points) {
		boolean res = false;
		for(int i = 0; i < Points.length; i++) {
			if((this.Get_Coords().X < Points[i].X) && (this.Get_Coords().X + this.Get_Size() > Points[i].X)) {
				if((this.Get_Coords().Y < Points[i].Y) && (this.Get_Coords().Y + this.Get_Size() > Points[i].Y)) {
					res = true;
					break;
				}
			}
		}
		return res;
	}
	
}

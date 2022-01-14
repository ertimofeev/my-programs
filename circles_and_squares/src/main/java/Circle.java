public class Circle extends Figure {

	public void Set_Points () {
		Coords[] Points = new Coords[4];
		Points[0] = new Coords();
		Points[0].Set_Coords(this.Get_Coords().X, this.Get_Coords().Y + this.Get_Size());
		Points[1] = new Coords();
		Points[1].Set_Coords(this.Get_Coords().X + this.Get_Size(), this.Get_Coords().Y);
		Points[2] = new Coords();
		Points[2].Set_Coords(this.Get_Coords().X + 2*this.Get_Size(), this.Get_Coords().Y + this.Get_Size());
		Points[3] = new Coords();
		Points[3].Set_Coords(this.Get_Coords().X + this.Get_Size(), this.Get_Coords().Y + 2*this.Get_Size());
		this.Set_Keys(Points);
	}
	
	public boolean Check_Key_Point (Coords[] Points) {
		boolean res = false;
		int Distance = 0;
		Coords O = new Coords();
		O.Set_Coords(this.Get_Coords().X + (int)(this.Get_Size()), this.Get_Coords().Y + (int)(this.Get_Size()));
		for(int i = 0; i < Points.length;i++) {
			Distance = (int)Math.round(Math.pow(Math.pow(O.X - Points[i].X, 2) + Math.pow(O.Y - Points[i].Y, 2), 0.5));
			if(Distance < this.Get_Size()) {
				res = true;
				break;
			}
		}
		if(!res) {
			Coords O1 = new Coords();
			O1.Set_Coords((Points[0].X + Points[2].X)/2, (Points[0].Y + Points[2].Y)/2);
			Distance = (int)Math.round(Math.pow(Math.pow(O.X - O1.X, 2) + Math.pow(O.Y - O1.Y, 2), 0.5));
			if(Distance < this.Get_Size()) {
				res = true;
			}
			O1 = null;
		}
		O = null;
		return res;
	}

}

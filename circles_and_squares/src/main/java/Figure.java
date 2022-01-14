public class Figure implements Operations {
	protected String Type;
	protected int Size = 0;
	protected Coords coord;
	protected Coords Speed;
	protected int Color;
	protected Coords[] Keys;

	public String Get_Type() {
		return this.Type;
	}
	
	public void Set_Type(String Type) {
		this.Type = null;
		this.Type = Type;
	}

	public int Get_Size() {
		return this.Size;
	}
	
	public void Change_Size(double k) {
		this.Size = (int)(k * this.Size);
	}

	public void Set_Coords(int X, int Y) {
		this.coord = null;
		this.coord = new Coords();
		this.coord.X = X;
		this.coord.Y = Y;
	}
	
	public Coords Get_Coords() {
		return this.coord;
	}
	
	public void Set_Size(int Size) {
		this.Size = Size;
	}

	public void Set_Speed(int X, int Y) {
		this.Speed = null;
		this.Speed = new Coords();
		this.Speed.X = X;
		this.Speed.Y = Y;
	}

	public Coords Get_Speed() {
		return this.Speed;
	}

	public void Change_Speed(double k) {
		this.Speed.X = (int)(this.Speed.X * k);
		this.Speed.Y = (int)(this.Speed.Y * k);
	}

	public void Set_Color() {
		this.Color = (int)(Math.random() * Colors.length);
		System.out.println(this.Color);
	}

    public int Get_Color() {
    	return this.Color;
    }
    
	public void Change_Color() {
		this.Color = (this.Color + 1) % Colors.length;
	}
    
	
	public void Set_Keys(Coords[] Keys) {
		this.Keys = null;
		this.Keys = Keys;
	}
	
	public Coords[] Get_Keys() {
		return this.Keys;
	}

	public void Change_Coords (Coords Screen_Size, long time) {
		this.Get_Coords().X += (int)(this.Get_Speed().X * time*0.001);
		this.Get_Coords().Y += (int)(this.Get_Speed().Y * time*0.001);
		
		if (this.Get_Coords().X <= 0) {
			this.Get_Coords().X = -this.Get_Coords().X;
			this.Get_Speed().X = -this.Get_Speed().X;
		}
		if ((this.Get_Coords().X + 2 * this.Get_Size()) >= Screen_Size.X ) {
			this.Get_Coords().X = 2 * Screen_Size.X - (this.Get_Coords().X + 4 * this.Get_Size());
			this.Get_Speed().X = -this.Get_Speed().X;
		}
		
		if (this.Get_Coords().Y <= 0) {
			this.Get_Coords().Y = -this.Get_Coords().Y;
			this.Get_Speed().Y = -this.Get_Speed().Y;
		}
		if ((this.Get_Coords().Y + 2 * this.Get_Size()) >= Screen_Size.Y ) {
			this.Get_Coords().Y = 2 * Screen_Size.Y - (this.Get_Coords().Y + 4 * this.Get_Size());
			this.Get_Speed().Y = -this.Get_Speed().Y;
		}
		
    }
}

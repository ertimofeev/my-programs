import javax.swing.*;
import java.awt.*;


public class Canvas extends JComponent {
	public Color [] Colors = {Color.RED, Color.ORANGE, Color.YELLOW, Color.GREEN, Color.CYAN, Color.BLUE, Color.MAGENTA};
	private Operations Object;
	public void paintComponent(Graphics g) {

		Graphics2D g2 = (Graphics2D) g;
		g2.setPaint(Colors[Object.Get_Color()]);  
		switch(Object.Get_Type()) {
		case("Circle"):
			g2.fillOval(Object.Get_Coords().X, Object.Get_Coords().Y, 2*Object.Get_Size(), 2*Object.Get_Size());
		break;
		case("Square"):
			g2.fillRect(Object.Get_Coords().X, Object.Get_Coords().Y, 2*Object.Get_Size(), 2*Object.Get_Size());
		break;
		}	  
	}
	public void SetObject(Operations Object) {
		this.Object = null;
		this.Object = Object;
	}
	
}

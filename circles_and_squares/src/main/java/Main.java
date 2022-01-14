import javax.swing.*;
import java.awt.*;


public class Main extends JFrame {
    public int[][] Crossing;
    public Coords Screen_Size;

    public static void main(String[] args) {
        Coords Screen_Size = new Coords();
        Dimension Size = Toolkit.getDefaultToolkit().getScreenSize();
        Screen_Size.Set_Coords ( Size.width-50, Size.height-50 );
        JFrame frame = new JFrame ( "Yeah its work BUT java sucks" );
        frame.setBounds(0, 0, Screen_Size.X, Screen_Size.Y);
        frame.setDefaultCloseOperation ( JFrame.EXIT_ON_CLOSE );

        int FPS = 100;

        int min_size = Math.min(Screen_Size.X, Screen_Size.Y)/20;
        int max_size = Math.min(Screen_Size.X, Screen_Size.Y)/10;
        Coords min_speed = new Coords();
        min_speed.Set_Coords((int)(0.1*Screen_Size.X),((int)(0.1*Screen_Size.Y)));
        Coords max_speed = new Coords();
        max_speed.Set_Coords((int)(0.4*Screen_Size.X),((int)(0.4*Screen_Size.Y)));

        ////////////
        int n = 10;
        ////////////

        Operations[] Objects = new Operations[n];

        for (int i = 0; i < n; i++){
            if (Math.random() < 0.5) {
                Objects[i] = new Circle();
                Objects[i].Set_Type("Circle");
            } else {
                Objects[i] = new Square();
                Objects[i].Set_Type("Square");
            }

            Objects[i].Set_Size((int)(Math.random()*(max_size - min_size)) + min_size);

            Objects[i].Set_Coords((int)(Math.random()*(Screen_Size.X - 2*Objects[i].Get_Size())), (int)(Math.random()*(Screen_Size.Y - 2*Objects[i].Get_Size())));

            Objects[i].Set_Speed( ((int)(Math.random()) * 2 - 1) * ((int)(Math.random() * (max_speed.X - min_speed.X) + min_speed.X)), ((int)(Math.random()) * 2 - 1) * ((int)(Math.random() * (max_speed.Y - min_speed.Y) + min_speed.Y)));

            Objects[i].Set_Color();

        }

        for (int i = 0; i < n; i++){

            Objects[i].Set_Size((int)(Math.random()*(max_size - min_size)) + min_size);

            Objects[i].Set_Coords((int)(Math.random()*(Screen_Size.X - 2*Objects[i].Get_Size())), (int)(Math.random()*(Screen_Size.Y - 2*Objects[i].Get_Size())));

            Objects[i].Set_Speed( ((int)(Math.random()) * 2 - 1) * ((int)(Math.random() * (max_speed.X - min_speed.X) + min_speed.X)), ((int)(Math.random()) * 2 - 1) * ((int)(Math.random() * (max_speed.Y - min_speed.Y) + min_speed.Y)));
            //Objects[i].Set_Speed(min_speed.X, min_speed.Y);

            Objects[i].Set_Color();


        }
        boolean[][] Crossing = new boolean[n][n];
        Canvas canv;
        for (int i = 0; i < Objects.length; i++) {
            canv = null;
            canv = new Canvas();
            canv.SetObject(Objects[i]);
            frame.add(canv);
            frame.setVisible(true);
        }

        long PrevTime = System.currentTimeMillis();
        long CurTime = System.currentTimeMillis();

        do {
            if (CurTime - PrevTime >= (int)(1000/FPS)) {
                //System.out.println(CurTime - PrevTime);
                for (int i = 0; i < n; i++) {
                    Objects[i].Change_Coords(Screen_Size, CurTime - PrevTime);
                    Objects[i].Set_Points();
                }

                for (int i = 0; i < n; i++) {
                    for(int j = i + 1; j < n; j++) {
                        if (Objects[i].Check_Key_Point(Objects[j].Get_Keys())) {
                            if (!Crossing[i][j]) {
                                Objects[i].Set_Size((int)(Math.random()*(max_size - min_size)) + min_size);
                                //Objects[i].Change_Size( K( min_size, max_size, Objects[i].Get_Size () ) );
                                //Objects[i].Change_Speed(1.0);`
                                Objects[i].Change_Color();

                                Objects[j].Set_Size((int)(Math.random()*(max_size - min_size)) + min_size);
                                //Objects[j].Change_Size(K(min_size, max_size, Objects[i].Get_Size()));
                                //Objects[j].Change_Speed(1.0);
                                Objects[j].Change_Color();

                                Crossing[i][j] = true;
                                Crossing[j][i] = true;
                            }
                        } else if ( Objects[j].Check_Key_Point ( Objects[i].Get_Keys () ) ) {
                            if (!Crossing[i][j]) {
                                Objects[i].Set_Size((int)(Math.random()*(max_size - min_size)) + min_size);
                                //Objects[i].Change_Size( K( min_size, max_size, Objects[i].Get_Size () ) );
                                //Objects[i].Change_Speed(1.0);
                                Objects[i].Change_Color();

                                Objects[j].Set_Size((int)(Math.random()*(max_size - min_size)) + min_size);
                                //Objects[j].Change_Size(K(min_size, max_size, Objects[i].Get_Size()));
                                //Objects[j].Change_Speed(1.0);
                                Objects[j].Change_Color();

                                Crossing[i][j] = true;
                                Crossing[j][i] = true;
                            }
                        } else {
                            Crossing[i][j] = false;
                            Crossing[j][i] = false;
                        }
                    }
                }
                for (int i = 0; i < Objects.length; i++) {
                    canv = new Canvas();
                    canv.SetObject(Objects[i]);
                    frame.add(canv);
                    frame.setVisible(true);
                    frame.remove(canv);
                    canv = null;
                }

                frame.repaint();
                frame.revalidate();
                PrevTime = CurTime;
            }
            CurTime = System.currentTimeMillis();
        } while (true);
    }
}
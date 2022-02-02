import java.util.ArrayList;
import java.util.Iterator;

public class Production {
    String productionName = null;
    double totalWeight = 0;
    ArrayList <Integer> cells = new ArrayList<>();
    double prime = -1;

    void displayInfo (){
        System.out.println("productionName: " + this.productionName);
        System.out.println(" totalWeight: " + this.totalWeight);
        System.out.println(" cells: ");
        Iterator<Integer> iterator = this.cells.iterator();
        while (iterator.hasNext()){
            System.out.println("  cellId: " + iterator.next());
        }
        System.out.println(" prime " + this.prime);
    }
}

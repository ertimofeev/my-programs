import java.util.ArrayList;
import java.util.Iterator;

public class Exit {
    Integer exitId = -1;
    ArrayList<Long>processes = new ArrayList<>();
    String company = null;

    Exit(int index){
        this.exitId = index;
    }

    void displayInfo (){
        System.out.println("exitId: " + this.exitId);
        System.out.println(" processes: ");
        Iterator<Long> iterator = this.processes.iterator();
        while (iterator.hasNext()){
            System.out.println("  processId: " + iterator.next());
        }
        System.out.println(" company: " + this.company);
    }
}

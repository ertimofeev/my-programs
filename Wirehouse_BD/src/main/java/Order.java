import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class Order {
    static long ordersNumber = 0;
    long orderId = -1;
    String status = "pending";
    String company = null;
    HashMap<String, Double> productionList = new HashMap<>();
    Integer exitId = -1;
    ArrayList<Integer> employees = new ArrayList<>();
    Double price = 0.0;

    Order(){
        this.orderId = this.ordersNumber++;
    }

    void displayInfo (){
        System.out.println("orderId: " + this.orderId);
        System.out.println(" status: " + this.status);
        System.out.println(" company: " + this.company);
        System.out.println(" productionList: ");
        for(Map.Entry<String, Double> pair : this.productionList.entrySet())
        {
            System.out.print("  Name: " + pair.getKey());
            System.out.println(" weight: " + pair.getValue());
        }
        System.out.println(" exitId: " + this.exitId);
        System.out.println(" employees " );
        Iterator<Integer> iterator = this.employees.iterator();
        while (iterator.hasNext()){
            System.out.println("  EmployeeId: " + iterator.next());
        }
        System.out.println(" price: " + this.price);
    }
}

import java.util.Map;

public class Main {


    public static void main(String[] args) {
	// write your code here

        Warehouse warehouse = new Warehouse();
        warehouse.readInfo();
        warehouse.displayAllInfo();

        EmployeeThread[] processes = new EmployeeThread[warehouse.employees.size()];
        int count = 0;
        for(Map.Entry<Integer, Employee> pair : warehouse.employees.entrySet())
        {
            processes[count] = new EmployeeThread(warehouse, pair.getKey());
            count++;
        }
        СommandReader comandReader = new СommandReader(warehouse, processes);
        comandReader.start();

        for (int i = 0; i< processes.length; i++){
            processes[i].addMainThread(comandReader);
            processes[i].start();
        }
        try {
            comandReader.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        warehouse.writeInfo();
    }
}

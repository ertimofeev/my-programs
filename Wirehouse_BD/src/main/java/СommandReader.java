import java.util.Scanner;

public class СommandReader extends Thread {
    EmployeeThread[] processes;
    Warehouse warehouse;

    СommandReader(Warehouse warehouse, EmployeeThread[] processes){
        this.warehouse = warehouse;
        this.processes = processes;
    }
    @Override
    public void run() {
        super.run();
        Scanner scanner = new Scanner(System.in);

        while (true){
            String command = scanner.nextLine();
            switch (command){
                case ("add order"):
                    warehouse.addOrder();
                    break;
                case ("change employee status"):
                    System.out.println("Enter employeeId");
                    int employeeId = Integer.valueOf(scanner.nextLine());
                    if(this.warehouse.employees.containsKey(employeeId)){
                        System.out.println("Enter new status");
                        this.warehouse.employees.get(employeeId).status = scanner.nextLine();
                    }else {
                        System.out.println("no employee with this Id");
                        warehouse.displayEmployees();
                    }
                    break;
                case ("add production"):
                    warehouse.addProduction();
                    break;
                case ("display all"):
                    warehouse.displayAllInfo();
                    break;
                case ("display cells"):
                    warehouse.displayCells();
                    break;
                case ("display employees"):
                    warehouse.displayEmployees();
                    break;
                case ("display orders"):
                    warehouse.displayOrders();
                    break;
                case ("display processes"):
                    warehouse.displayProcesses();
                    break;
                case ("display productions"):
                    warehouse.displayProduction();
                    break;
                case ("display exits"):
                    warehouse.displayExits();
                    break;
            }

            if (command.equals("finish work")){
                this.warehouse.employees.clear();
                break;
            }
        }
    }
}

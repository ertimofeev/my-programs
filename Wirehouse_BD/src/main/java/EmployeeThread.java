public class EmployeeThread extends Thread{
    Warehouse warehouse;
    int employeeId = -1;
    СommandReader mainThread;
    EmployeeThread(Warehouse warehouse, int employeeId){
        this.warehouse = warehouse;
        this.employeeId = employeeId;
    }
    void addMainThread(СommandReader mainThread){
        this.mainThread = mainThread;
    }
    @Override
    public void run() {
        super.run();
        while ((this.warehouse.orders.size() > 0)&&(this.warehouse.employees.containsKey(this.employeeId))) {
            if (this.warehouse.employees.get(this.employeeId).status.equals("free")) {
                if (this.warehouse.orders.size() > 0) {

                    warehouse.executeOrder66(this.warehouse.employees.get(this.employeeId));
                }
            }
        }
    }
}

public class Employee {
    Integer employeeId = -1;
    String surnameName = null;
    long phoneNumber = -1;
    long processId = -1;
    String status = "free";

    Employee() {}
    Employee(int employeeId, String surnameName, long phoneNumber){
        this.employeeId = employeeId;
        this.surnameName = surnameName;
        this.phoneNumber = phoneNumber;
    }

    void displayInfo (){
        System.out.println("employeeId: " + this.employeeId);
        System.out.println(" surnameName: " + this.surnameName);
        System.out.println(" phoneNumber: " + this.phoneNumber);
        System.out.println(" processId " + this.processId);
        System.out.println(" status: " + this.status);
    }
}

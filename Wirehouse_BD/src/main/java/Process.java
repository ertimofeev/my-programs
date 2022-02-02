public class Process {
    static long processesNumber = 0;
    long processId = -1;
    int cellId = -1;
    int exitId = -1;
    double weight = -1;

    Process(int cellId, int exitId, double weight){
        this.processId = processesNumber;
        this.processesNumber += 1;
        this.cellId = cellId;
        this.exitId = exitId;
        this.weight = weight;
    }

    void displayInfo (){
        System.out.println("processId: " + this.processId);
        System.out.println(" cellId: " + this.cellId);
        System.out.println(" exitId: " + this.exitId);
        System.out.println(" weight " + this.weight);
    }
}

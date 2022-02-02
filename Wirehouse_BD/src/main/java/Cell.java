public class Cell {
    Integer cellId = -1;
    double capacity = 50;
    double fullness = -1;
    String production = null;

    Cell() {}
    Cell(int cellId, double capacity, double fullness, String production){
        this.cellId = cellId;
        this.capacity = capacity;
        this.fullness = fullness;
        this.production = production;
    }

    void displayInfo (){
        System.out.println("cellID: " + this.cellId);
        System.out.println(" capacity: " + this.capacity);
        System.out.println(" fullness: " + this.fullness);
        System.out.println(" production " + this.production);
    }
}

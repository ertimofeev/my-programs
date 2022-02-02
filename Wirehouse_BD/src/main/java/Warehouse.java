import java.io.*;
import java.util.*;

import static java.lang.Math.min;

public class Warehouse {
    Cell[] cells = new Cell[50];
    HashMap<String, Production> production = new HashMap<>();
    HashMap<String, Order>orders = new HashMap<>();
    HashMap <Integer, Employee> employees = new HashMap<>();
    Exit[] exits = new Exit[10];
    HashMap<Long, Process>processes = new HashMap<>();

    //добавление продукции на склад
    void addProduction(){
        Scanner scanner = new Scanner(System.in);
        Production newProduction;

        System.out.println("Enter Name");
        String productionName = scanner.nextLine();
        //проверяем будет пополнение запасов или завоз новой продукции
        if (this.production.containsKey(productionName)){
            System.out.println("Such a product is already in stock");
            newProduction = production.get(productionName);
        }else {
            newProduction = new Production();
            newProduction.productionName = productionName;
        }

        //узнаем вес поставки
        System.out.println("Enter how much you want to add");
        double productionWeight = Double.valueOf(scanner.nextLine());
        if (this.production.containsKey(productionName)){
            newProduction.totalWeight += productionWeight;
        }else {
            newProduction.totalWeight = productionWeight;
        }

        //если это пополнение то сперва распределяем продукцию по ячейкам в которых уже находится данная продукция
        if (this.production.containsKey(productionName)){
            Iterator<Integer> iterator = this.production.get(newProduction.productionName).cells.iterator();
            while (iterator.hasNext()&&(productionWeight > 0)){
                Cell curCell = this.cells[iterator.next()];
                double addWeight = min(productionWeight, curCell.capacity-curCell.fullness);
                curCell.fullness += addWeight;
                productionWeight -= addWeight;
            }
        }
        //распределяем продукцию по пустым ячейкам
        for (int i = 0; i<this.cells.length; i++) {
            if (this.cells[i] == null) {
                Cell newCell = new Cell();
                newCell.production = newProduction.productionName;
                newCell.cellId = i;
                newCell.fullness = min(productionWeight, newCell.capacity);
                productionWeight -= newCell.fullness;
                this.cells[i] = newCell;

                newProduction.cells.add(newCell.cellId);
            }
            if (productionWeight == 0) {
                break;
            }
        }

        //если это новый тип продукции узнаем её стоимость
        if (!this.production.containsKey(productionName)){
            System.out.println("Enter the cost of new production");
            double productonCost = Double.valueOf(scanner.nextLine());
            newProduction.prime = productonCost;
        }

        this.production.put(newProduction.productionName, newProduction);
    }

    //получение предзаказов
    void addOrder (){
        Order newOrder = new Order();
        Scanner scanner = new Scanner(System.in);

        System.out.println("Enter your company name");
        newOrder.company = scanner.nextLine();

        double price = 0;
        //добавляем предзаказы
        while (true) {
            System.out.println("Enter the name of the product you want to order");
            String productionName = scanner.nextLine();

            //проверяем наличие продукции на складе
            if (this.production.containsKey(productionName)){
                if (this.production.get(productionName).totalWeight != 0) {
                    System.out.println("Enter how much of this product you want to receive");
                    double takenWeight = Double.valueOf(scanner.nextLine());

                    if (takenWeight > this.production.get(productionName).totalWeight) {
                        takenWeight = this.production.get(productionName).totalWeight;
                        System.out.println("Sorry, we do not have enough of these products in stock");
                        System.out.println("We will give you " + takenWeight);
                    }

                    //изменяем доступное для заказа количество продукции
                    this.production.get(productionName).totalWeight -= takenWeight;
                    //добавляем стоимость заказа в счёт
                    price += this.production.get(productionName).prime * takenWeight;
                    //добавляем новый пункт в заказ или дополняем существующую позицию
                    if (newOrder.productionList.containsKey(productionName)){
                        takenWeight += newOrder.productionList.get(productionName);
                    }
                    newOrder.productionList.put(productionName, takenWeight);

                    System.out.println("Would you like to order more products");
                }else {
                    System.out.println("This product is not in stock");
                    System.out.println("Would you like to order other products");
                }
            }else {
                System.out.println("This product is not in stock");
                System.out.println("Would you like to order other products");
            }

            //интересуемся хочет ли клиент продолжить заказ
            String choise = scanner.nextLine();
            if (Objects.equals(choise, "no")){
                break;
            }
        }
        System.out.println("The cost of your order = " + price);
        newOrder.price = price;
        //если заказ не пуст добавляем его в список предзаказов
        if (newOrder.price > 0){
            this.orders.put(newOrder.company, newOrder);
        }else {
            System.out.println("What a pity that we don’t take money for watching");
        }
    }


    //если есть пустые места выдачи запускаем обработку запросов новоприбывшей компании
    void processesCreation (){
        for (int i = 0; i < this.exits.length; i++){
            if (this.exits[i].company == null) {
                //считываем запросы компании которую еще не обслуживает склад
                for(Map.Entry<String, Order> orderPair : this.orders.entrySet()) {
                    if (orderPair.getValue().status.equals("pending")){
                        this.orders.get(orderPair.getKey()).status = "in progress";
                        System.out.println("newStatus " + orderPair.getValue().status);
                        this.exits[i].company = orderPair.getKey();
                        //для каждой продукции из запроса выделяем необходимое количество со склада
                        for(Map.Entry<String, Double> productionPair : orderPair.getValue().productionList.entrySet()) {
                            Iterator<Integer> curCellId = this.production.get(productionPair.getKey()).cells.iterator();
                            //забирае из каждой ячейки максимум продукции
                            while (curCellId.hasNext()&&(productionPair.getValue() > 0)){
                                Cell curCell = this.cells[curCellId.next()];
                                if (curCell.fullness > 0) {
                                    double takenWeight = min(productionPair.getValue(), curCell.fullness);

                                    curCell.fullness -= takenWeight;
                                    orderPair.getValue().productionList.put(productionPair.getKey(), productionPair.getValue() - takenWeight);

                                    Process newProcess = new Process(curCell.cellId, this.exits[i].exitId, takenWeight);
                                    this.exits[i].processes.add(newProcess.processId);
                                    this.processes.put(newProcess.processId, newProcess);
                                }
                            }
                        }
                        break;
                    }
                }
            }

        }
    }

    //описание рабочегоп роцесса для каждого работника после выполнения необходимо синхронизировать данные
    void executeOrder66(Employee employee) {
        if (this.orders.size() != 0) {
            this.processesCreation();
            //обработка самого раннего процесса
            if (this.processes.size() > 0) {

                Process curProcess = this.processes.get(Process.processesNumber - this.processes.size());
                Cell curCell = this.cells[curProcess.cellId];
                Exit curExit = this.exits[curProcess.exitId];
                Order curOrder = this.orders.get(curExit.company);

                employee.status = "in progress";
                employee.processId = curProcess.processId;
                curOrder.employees.add(employee.employeeId);

                //если ячейка пуста отображаем её как пустую для системы
                if (curCell != null) {
                    if (curCell.fullness == 0) {
                        this.production.get(curCell.production).cells.remove(curCell.cellId);
                        this.cells[curProcess.cellId] = null;
                    }
                }


                employee.status = "free";
                employee.processId = -1;
                curOrder.employees.remove(employee.employeeId);
                this.processes.remove(curProcess.processId);

                //если на данном пункте выдачи больше нету невыполненых процессов, то заказ выполнен
                if (curExit.processes.size() == 0) {
                    this.orders.remove(curExit.company);
                }
            }
        }

    }

    void displayAllInfo(){
        displayProduction();
        displayCells();
        displayExits();
        displayOrders();
        displayEmployees();
        displayProcesses();
        System.out.println("");
    }

    void displayCells(){
        for (Cell cell : this.cells) {
            if (cell != null) {
                cell.displayInfo();
            }
        }
        System.out.println("");
    }
    void displayProduction(){
        for(Map.Entry<String, Production> pair : this.production.entrySet())
        {
            pair.getValue().displayInfo();
        }
        System.out.println("");
    }
    void displayOrders(){
        for(Map.Entry<String, Order> pair : this.orders.entrySet())
        {
            pair.getValue().displayInfo();
        }
        System.out.println("");
    }
    void displayEmployees(){
        for(Map.Entry<Integer, Employee> pair : this.employees.entrySet())
        {
            pair.getValue().displayInfo();
        }
        System.out.println("");
    }
    void displayExits(){
        for (Exit exit : this.exits) {
            exit.displayInfo();
        }
        System.out.println("");
    }
    void displayProcesses(){
        for(Map.Entry<Long, Process> pair : this.processes.entrySet())
        {
            pair.getValue().displayInfo();
        }
        System.out.println("");
    }

    //считывание информации о складе после возобновления работы склада
    void readInfo() {
        File fCells = new File(Main.class.getResource("cells.txt").getFile());
        File fProduction = new File(Main.class.getResource("production.txt").getFile());
        File fEmployees = new File(Main.class.getResource("employees.txt").getFile());


        try(FileReader fileReader = new FileReader(fCells))
        {
            BufferedReader reader = new BufferedReader(fileReader);
            String val;
            while((val = reader.readLine())!=null){
                int index = Integer.valueOf(val);
                Cell newCell = new Cell();
                newCell.cellId = index;
                newCell.capacity = Double.valueOf(reader.readLine());
                newCell.fullness = Double.valueOf(reader.readLine());
                newCell.production = reader.readLine();
                this.cells[index] = newCell;
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        try(FileReader fileReader = new FileReader(fProduction))
        {
            BufferedReader reader = new BufferedReader(fileReader);
            String val;
            while((val = reader.readLine())!=null){
                String productionName = val;
                Production newProduction = new Production();
                newProduction.productionName = productionName;
                newProduction.totalWeight = Double.valueOf(reader.readLine());
                //считываем количество ячеек
                int cellsNumber = Integer.valueOf(reader.readLine());
                for (int i = 0; i < cellsNumber; i++){
                    newProduction.cells.add(Integer.valueOf(reader.readLine()));
                }
                newProduction.prime = Double.valueOf(reader.readLine());
                this.production.put(productionName, newProduction);
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        try(FileReader fileReader = new FileReader(fEmployees))
        {
            BufferedReader reader = new BufferedReader(fileReader);
            String val;
            while((val = reader.readLine())!=null){
                Integer employeeId = Integer.valueOf(val);
                Employee newEmployee = new Employee();
                newEmployee.employeeId = employeeId;
                newEmployee.surnameName = reader.readLine();
                newEmployee.phoneNumber = Long.valueOf(reader.readLine());
                newEmployee.processId = Long.valueOf(reader.readLine());
                newEmployee.status = reader.readLine();
                this.employees.put(employeeId, newEmployee);
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        for (int i = 0; i < this.exits.length; i++){
            this.exits[i] = new Exit(i);
        }
    }

    //запись состояния склада перед его закрытием или перерывом

    void writeInfo() {
        File fCells = new File(Main.class.getResource("cells.txt").getFile());
        File fProduction = new File(Main.class.getResource("production.txt").getFile());
        File fEmployees = new File(Main.class.getResource("employees.txt").getFile());

        try(FileWriter writer = new FileWriter(fCells, false))
        {
            for (Cell cell : this.cells) {
                if (cell != null) {
                    writer.write(Integer.toString(cell.cellId) + "\n");
                    writer.write(Double.toString(cell.capacity) + "\n");
                    writer.write(Double.toString(cell.fullness) + "\n");
                    writer.write(cell.production + "\n");
                }
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        try(FileWriter writer = new FileWriter(fProduction, false))
        {
            for(Map.Entry<String, Production> pair : this.production.entrySet())
            {
                Production curProduction = pair.getValue();
                writer.write(curProduction.productionName + "\n");
                writer.write(Double.toString(curProduction.totalWeight) + "\n");
                writer.write(Integer.toString(curProduction.cells.size()) + "\n");
                Iterator<Integer> iterator = curProduction.cells.iterator();
                while (iterator.hasNext()){
                    writer.write(Integer.toString(iterator.next()) + "\n");
                }
                writer.write(Double.toString(curProduction.prime) + "\n");
            }
        }catch (IOException ex) {
            ex.printStackTrace();
        }

        try(FileWriter writer = new FileWriter(fEmployees, false))
        {
            for(Map.Entry<Integer, Employee> pair : this.employees.entrySet())
            {
                Employee curEmployee = pair.getValue();
                writer.write(Integer.toString(curEmployee.employeeId) + "\n");
                writer.write(curEmployee.surnameName + "\n");
                writer.write(Long.toString(curEmployee.phoneNumber) + "\n");
                writer.write(Long.toString(curEmployee.processId) + "\n");
                writer.write(curEmployee.status + "\n");
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
}

#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>

class FleetVehicle {
private:
    int vehicleID;
    float fuelLevel;
    double distanceTravelled;
    char status;
    bool isAvailable;
    std::string driverName;

public:
    FleetVehicle()
    : vehicleID(0), fuelLevel(50.0f), distanceTravelled(0.0),
      status('A'), isAvailable(true), driverName("Unassigned") {}

    FleetVehicle(int id, float fuel, double dist, char st, bool avail, const std::string &driver)
    : vehicleID(id), fuelLevel(fuel), distanceTravelled(dist),
      status(st), isAvailable(avail), driverName(driver) {}

    ~FleetVehicle() {
        std::cout << "[Destructor] Vehicle ID " << vehicleID << " removed." << std::endl;
    }

    int getVehicleID() const { return vehicleID; }
    float getFuelLevel() const { return fuelLevel; }
    double getDistanceTravelled() const { return distanceTravelled; }
    char getStatus() const { return status; }
    bool getIsAvailable() const { return isAvailable; }
    std::string getDriverName() const { return driverName; }

    void setVehicleID(int v) { vehicleID = v; }
    void setFuelLevel(float f) { fuelLevel = f; }
    void setDistanceTravelled(double d) { distanceTravelled = d; }
    void setStatus(char s) { status = s; }
    void setIsAvailable(bool a) { isAvailable = a; }
    void setDriverName(const std::string &name) { driverName = name; }

    void updateStatus() {
        if (fuelLevel < 10.0f || !isAvailable) status = 'I';
        else status = 'A';
    }

    void displayInfo() const {
        std::cout << "Vehicle ID: " << vehicleID << std::endl
                  << "Fuel Level: " << fuelLevel << std::endl
                  << "Distance Travelled: " << distanceTravelled << std::endl
                  << "Status: " << status << " (" << (status=='A'?"Active":"Inactive") << ")" << std::endl
                  << "Is Available: " << (isAvailable?"Yes":"No") << std::endl
                  << "Driver Name: " << driverName << std::endl
                  << "------------------------------------" << std::endl;
    }
};

void assignDriver(FleetVehicle &vehicle, const std::string name) {
    vehicle.setDriverName(name);
    std::cout << "Driver " << vehicle.getDriverName()
              << " assigned to vehicle ID " << vehicle.getVehicleID() << std::endl;
}

void refuelVehicle(FleetVehicle &vehicle, float fuelAmount) {
    if (fuelAmount <= 0.0f) throw std::invalid_argument("Fuel amount must be > 0");
    vehicle.setFuelLevel(vehicle.getFuelLevel() + fuelAmount);
    std::cout << "Vehicle ID " << vehicle.getVehicleID()
              << " refueled. New fuel: " << vehicle.getFuelLevel() << std::endl;
}

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    std::cout << "This program manages an array of 3 FleetVehicle objects." << std::endl;
    std::cout << "Use the index (0,1,2) to select which vehicle in the array you want to work with." << std::endl;

    FleetVehicle vehicles[3] = {
        FleetVehicle(101, 45.5f, 120.0, 'A', true, "Raj"),
        FleetVehicle(102, 8.0f, 300.0, 'A', true, "Priya"),
        FleetVehicle()
    };

    bool running = true;
    while (running) {
        std::cout << "\n===== Fleet Vehicle Monitoring Menu =====" << std::endl
                  << "1. Add/Replace vehicle" << std::endl
                  << "2. Assign driver" << std::endl
                  << "3. Refuel" << std::endl
                  << "4. Update status" << std::endl
                  << "5. Display info" << std::endl
                  << "6. Exit" << std::endl
                  << "Choose option: ";

        int choice;
        if (!(std::cin >> choice)) { std::cin.clear(); clearInputBuffer(); continue; }

        switch (choice) {
            case 1: {
                int idx;
                std::cout << "Enter index (0-2) to select which vehicle in the array you want to modify: ";
                std::cin >> idx;
                if (idx<0||idx>2) { std::cout << "Invalid index!" << std::endl; break; }

                int id; float fuel; double dist; char st; int availInt; std::string name;

                std::cout << "Enter details in one line (ID Fuel Distance Status(A/I) Available(1/0) DriverName):" << std::endl;
                std::cin >> id >> fuel >> dist >> st >> availInt;
                clearInputBuffer();
                std::getline(std::cin, name);

                vehicles[idx] = FleetVehicle(id, fuel, dist, st, (availInt == 1), name);
                std::cout << "Vehicle details updated successfully!" << std::endl;
                break;
            }
            case 2: {
                int idx;
                std::cout<<"Enter index (0-2) to select which vehicle in the array you want to assign a driver: ";
                std::cin>>idx;
                clearInputBuffer();
                std::string d;
                std::cout << "Enter driver name: ";
                std::getline(std::cin,d);
                assignDriver(vehicles[idx],d);
                break;
            }
            case 3: {
                int idx;
                std::cout<<"Enter index (0-2) to select which vehicle you want to refuel: ";
                std::cin>>idx;
                float amt;
                std::cout<<"Enter fuel amount: ";
                std::cin>>amt;
                try{ refuelVehicle(vehicles[idx],amt);}catch(std::exception&e){std::cout<<e.what()<<std::endl;}
                break;
            }
            case 4: {
                int idx;
                std::cout<<"Enter index (0-2) to select which vehicle you want to update status for: ";
                std::cin>>idx;
                vehicles[idx].updateStatus();
                std::cout<<"Vehicle status updated!"<<std::endl;
                break;
            }
            case 5: {
                int idx;
                std::cout<<"Enter index (0-2) to display one vehicle, or -1 to display all: ";
                std::cin>>idx;
                if(idx==-1) for(auto&v:vehicles)v.displayInfo(); else vehicles[idx].displayInfo();
                break;
            }
            case 6: running=false; break;
        }
    }
    std::cout<<"Exiting Fleet Vehicle Monitoring System..."<<std::endl;
    return 0;
}

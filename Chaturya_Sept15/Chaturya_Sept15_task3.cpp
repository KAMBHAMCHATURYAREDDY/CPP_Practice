#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <sstream>
#include <algorithm>

enum ComponentType { ENGINE, AVIONICS, LANDING_GEAR, FUEL_SYSTEM };
enum ComponentStatus { OPERATIONAL, MAINTENANCE_REQUIRED, FAILED };

class AerospaceComponent {
private:
    int id; 
    double efficiency; 
    std::string manufacturer;
    ComponentType type; 
    ComponentStatus status;

public:
    AerospaceComponent()
        : id(0), efficiency(100), manufacturer("Unknown"), type(ENGINE), status(OPERATIONAL) {}

    AerospaceComponent(int i,double e,std::string m,ComponentType t,ComponentStatus s)
        : id(i), efficiency(e), manufacturer(m), type(t), status(s) {}

    ~AerospaceComponent() { std::cout << "[Destructor] Component " << id << " destroyed.\n"; }

    int getId() const { return id; }
    double getEfficiency() const { return efficiency; }
    std::string getManufacturer() const { return manufacturer; }
    ComponentType getType() const { return type; }
    ComponentStatus getStatus() const { return status; }

    void setId(int v) { id = v; }
    void setEfficiency(double v) { efficiency = v < 0 ? 0 : (v > 100 ? 100 : v); }
    void setManufacturer(const std::string &m) { manufacturer = m; }
    void setType(ComponentType t) { type = t; }
    void setStatus(ComponentStatus s) { status = s; }

    void UpdateStatus() {
        if(efficiency < 50) status = FAILED;
        else if(efficiency < 80) status = MAINTENANCE_REQUIRED;
        else status = OPERATIONAL;
    }

    void SimulateUsage(int h) {
        for(int i = 0; i < h; i++) {
            efficiency -= 0.5;
            if(efficiency < 0) { efficiency = 0; break; }
        }
        UpdateStatus();
    }

    void PerformMaintenanceCheck() const {
        switch(status){
            case OPERATIONAL: std::cout << "Operational\n"; break;
            case MAINTENANCE_REQUIRED: std::cout << "Needs maintenance\n"; break;
            case FAILED: std::cout << "Failed!\n"; break;
        }
    }

    void BoostEfficiency() {
        while(efficiency < 100) {
            efficiency += 1;
            if(efficiency > 100) efficiency = 100;
        }
        UpdateStatus();
    }

    void DisplayDetails() const {
        static const char* typeStr[] = {"ENGINE","AVIONICS","LANDING_GEAR","FUEL_SYSTEM"};
        static const char* statusStr[] = {"OPERATIONAL","MAINTENANCE_REQUIRED","FAILED"};
        std::cout << "ID:" << id 
                  << " Eff:" << efficiency 
                  << " Manu:" << manufacturer 
                  << " Type:" << typeStr[type] 
                  << " Status:" << statusStr[status] << "\n";
    }
};

void AssignManufacturer(AerospaceComponent &c,const std::string &m){
    c.setManufacturer(m);
    std::cout << "Manufacturer updated\n\n";
}

bool IsSameType(const AerospaceComponent &a,const AerospaceComponent &b){ return a.getType() == b.getType(); }
bool IsSameStatus(const AerospaceComponent &a,const AerospaceComponent &b){ return a.getStatus() == b.getStatus(); }

void PrintFormattedComponentList(const AerospaceComponent arr[],int n){
    static const char* typeStr[] = {"ENGINE","AVIONICS","LANDING_GEAR","FUEL_SYSTEM"};
    static const char* statusStr[] = {"OPERATIONAL","MAINTENANCE_REQUIRED","FAILED"};

    std::cout << std::left << std::setw(6) << "ID"
              << std::setw(10) << "Eff"
              << std::setw(20) << "Manu"
              << std::setw(15) << "Type"
              << std::setw(20) << "Status" << std::endl;

    for(int i=0;i<n;i++){
        std::cout << std::setw(6) << arr[i].getId()
                  << std::setw(10) << arr[i].getEfficiency()
                  << std::setw(20) << arr[i].getManufacturer()
                  << std::setw(15) << typeStr[arr[i].getType()]
                  << std::setw(20) << statusStr[arr[i].getStatus()] << "\n";
    }
    std::cout << std::endl;
}

int SearchComponentByIdentifier(const AerospaceComponent arr[],int n,int sid){
    for(int i=0;i<n;i++)
        if(arr[i].getId() == sid) return i;
    return -1;
}

void clearInputBuffer(){ std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); }

ComponentType parseType(const std::string &str){
    if(str=="COMPONENT_TYPE_ENGINE") return ENGINE;
    if(str=="COMPONENT_TYPE_AVIONICS") return AVIONICS;
    if(str=="COMPONENT_TYPE_LANDING_GEAR") return LANDING_GEAR;
    return FUEL_SYSTEM;
}

ComponentStatus parseStatus(const std::string &str){
    if(str=="COMPONENT_STATUS_OPERATIONAL") return OPERATIONAL;
    if(str=="COMPONENT_STATUS_MAINTENANCE_REQUIRED") return MAINTENANCE_REQUIRED;
    return FAILED;
}

bool parseComponentInput(const std::string &line, int &id, double &eff, std::string &manu, ComponentType &type, ComponentStatus &status){
    std::string temp = line;
    // Remove quotes
    temp.erase(std::remove(temp.begin(), temp.end(), '\"'), temp.end());
    // Replace commas with spaces
    std::replace(temp.begin(), temp.end(), ',', ' ');

    std::istringstream iss(temp);
    std::string typeStr, statusStr;

    if(!(iss >> id >> eff >> manu >> typeStr >> statusStr)) return false;

    type = parseType(typeStr);
    status = parseStatus(statusStr);
    return true;
}

int main(){
    std::cout << "This program manages an array of 3 AerospaceComponent objects.\nUse index (0-2) to select component.\n\n";

    AerospaceComponent comps[3] = {
        AerospaceComponent(101,92.5,"GE",ENGINE,OPERATIONAL),
        AerospaceComponent(102,45,"Honeywell",AVIONICS,FAILED),
        AerospaceComponent()
    };

    bool run=true;
    while(run){
        std::cout<<"\n===== Aerospace Component Menu =====\n"
                 <<"1. Add Component\n"
                 <<"2. Assign Manufacturer\n"
                 <<"3. Simulate Usage\n"
                 <<"4. Boost Efficiency\n"
                 <<"5. Maintenance Check\n"
                 <<"6. Compare Components\n"
                 <<"7. Search Component\n"
                 <<"8. Display All\n"
                 <<"9. Exit\n"
                 <<"Choose option: ";
        int c; std::cin >> c;
        clearInputBuffer();
        std::cout << std::endl;

        switch(c){
            case 1:{
                int i;
                std::cout << "Enter index (0-2) to select which component in the array you want to modify: ";
                std::cin >> i;
                clearInputBuffer();

                std::string line;
                std::cout << "Enter details in one line (ID, Efficiency, Manufacturer, COMPONENT_TYPE_..., COMPONENT_STATUS_...):\n";
                std::getline(std::cin, line);

                int id; double eff; std::string manu; ComponentType type; ComponentStatus status;
                if(parseComponentInput(line, id, eff, manu, type, status)){
                    comps[i] = AerospaceComponent(id, eff, manu, type, status);
                    std::cout << "Component updated!\n\n";
                } else {
                    std::cout << "Invalid input format. Try again.\n\n";
                }
                break;
            }

            case 2:{
                int i; 
                std::cout << "Enter index (0-2) to select which component to assign manufacturer: ";
                std::cin >> i; 
                clearInputBuffer();
                std::string m; 
                std::cout << "Enter manufacturer: "; 
                std::getline(std::cin, m);
                AssignManufacturer(comps[i], m);
                break;
            }

            case 3:{
                int i,h; 
                std::cout << "Enter index (0-2) to select which component: "; 
                std::cin >> i;
                std::cout << "Enter hours: "; std::cin >> h;
                comps[i].SimulateUsage(h);
                std::cout << "Usage simulated!\n\n";
                break;
            }

            case 4:{
                int i; std::cout << "Enter index (0-2) to boost efficiency of component: "; std::cin >> i;
                comps[i].BoostEfficiency();
                std::cout << "Efficiency boosted!\n\n";
                break;
            }

            case 5:{
                int i; std::cout << "Enter index (0-2) to check maintenance: "; std::cin >> i;
                comps[i].PerformMaintenanceCheck();
                std::cout << std::endl;
                break;
            }

            case 6:{
                int a,b; std::cout << "Enter two indices (0-2): "; std::cin >> a >> b;
                std::cout << "Same Type? " << (IsSameType(comps[a], comps[b])?"Yes":"No") << std::endl;
                std::cout << "Same Status? " << (IsSameStatus(comps[a], comps[b])?"Yes":"No") << "\n\n";
                break;
            }

            case 7:{
                int sid; std::cout << "Enter ID to search: "; std::cin >> sid;
                int res = SearchComponentByIdentifier(comps,3,sid);
                if(res==-1) std::cout << "Not found\n\n";
                else { comps[res].DisplayDetails(); std::cout << std::endl; }
                break;
            }

            case 8:
                PrintFormattedComponentList(comps,3);
                break;

            case 9:
                run=false;
                break;

            default:
                std::cout << "Invalid option. Try again.\n\n";
        }
    }

    std::cout << "Exiting Aerospace Component System...\n";
    return 0;
}

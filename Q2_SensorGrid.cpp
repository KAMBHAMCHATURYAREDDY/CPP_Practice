#include <iostream>
#include <string>
using namespace std;

namespace SensorGrid {

class Sensor {
    int id;
    float value;
    int priority;
    string status;
public:
    Sensor(int i, float v, int p) : id(i), value(v), priority(p), status("Healthy") {}
    void updateValue(float v) { value = v; }
    int getId() const { return id; }
    float getValue() const { return value; }
    int getPriority() const { return priority; }
    string getStatus() const { return status; }
    void setStatus(const string& s) { status = s; }
};

class SensorDiagnostics {
public:
    string checkHealth(Sensor& s) {
        return (s.getValue() > 80.0 ? "Warning" : "OK");
    }
};

void rebindCriticalSensor(Sensor*& ref, Sensor* list[], int size) {
    Sensor* best = list[0];
    for (int i = 1; i < size; i++) {
        if (list[i]->getPriority() > best->getPriority())
            best = list[i];
    }
    ref = best;
    cout << "[Global] Critical sensor rebound to ID=" << ref->getId() << "\n";
}

void printSensorMap(Sensor* list[], int size) {
    cout << "---- Sensor Map ----\n";
    for (int i = 0; i < size; i++) {
        cout << "Sensor " << list[i]->getId()
             << " Value=" << list[i]->getValue()
             << " Priority=" << list[i]->getPriority() << "\n";
    }
}

class Controller {
    Sensor* sensorList[10];
    Sensor& criticalSensor;
    int count;
public:
    Controller(Sensor* sensors[], int size)
        : criticalSensor(*sensors[0]), count(size) {
        for (int i = 0; i < size; i++) sensorList[i] = sensors[i];
        bindCriticalSensor();
    }

    void bindCriticalSensor() {
        Sensor* best = sensorList[0];
        for (int i = 1; i < count; i++) {
            if (sensorList[i]->getPriority() > best->getPriority())
                best = sensorList[i];
        }
        cout << "[Controller] Initially bound to Sensor " << best->getId() << "\n";
    }

    void updateCriticalSensor() {
        Sensor* ref = &criticalSensor;
        rebindCriticalSensor(ref, sensorList, count);
    }

    void printStatus() {
        cout << "[Controller] Monitoring Sensor " << criticalSensor.getId()
             << " Value=" << criticalSensor.getValue() << "\n";
    }
};

}

// ----------------- Main -----------------
int main() {
    using namespace SensorGrid;

    // Sample Data
    Sensor* s1 = new Sensor(1, 75.0, 3);
    Sensor* s2 = new Sensor(2, 60.0, 5);
    Sensor* s3 = new Sensor(3, 90.0, 2);
    Sensor* sensorList[3] = {s1, s2, s3};

    Sensor* sensors[10];
    for (int i = 0; i < 10; i++) {
        sensors[i] = new Sensor(i+1, 50 + i*5, (i % 5) + 1);
    }

    Controller ctrl(sensors, 10);
    ctrl.printStatus();

    SensorDiagnostics diag;
    cout << "[Diagnostics] Health of Sensor 2: " << diag.checkHealth(*s2) << "\n";

    s2->updateValue(95.0);
    cout << "Sensor 2 updated Value=" << s2->getValue() << "\n";

    ctrl.updateCriticalSensor();
    printSensorMap(sensors, 10);

    delete s1; delete s2; delete s3;
    for (int i = 0; i < 10; i++) delete sensors[i];
    return 0;
}

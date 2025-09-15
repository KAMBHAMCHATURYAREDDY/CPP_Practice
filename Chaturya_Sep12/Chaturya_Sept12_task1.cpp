#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
using namespace std;

class Sensor {
protected:
    int id;
    float value;
    float threshold;
    string status;
public:
    Sensor(int i, float v, float t) : id(i), value(v), threshold(t), status("OK") {}
    virtual ~Sensor() {}
    virtual void simulateFault() = 0;
    bool isFaulty() { return value > threshold; }
    int getId() const { return id; }
    float getValue() const { return value; }
    float getThreshold() const { return threshold; }
    string getStatus() const { return status; }
    void setThreshold(float t) { threshold = t; }
    void setValue(float v) { value = v; }
    void setStatus(const string &s) { status = s; }
};

static random_device rd;
static mt19937 gen(rd());

class TemperatureSensor : public Sensor {
public:
    TemperatureSensor(int i, float v, float t) : Sensor(i, v, t) {}
    void simulateFault() override {
        uniform_real_distribution<> dist(10.0, 30.0); // spike between +10 and +30
        value += dist(gen);
        status = "Temperature Fault Injected (overheat)";
    }
};

class PressureSensor : public Sensor {
public:
    PressureSensor(int i, float v, float t) : Sensor(i, v, t) {}
    void simulateFault() override {
        uniform_real_distribution<> dist(10.0, 40.0); // drop between –10 and –40
        value -= dist(gen);
        status = "Pressure Fault Injected (drop)";
    }
};

class Actuator {
    int id;
    string state;
    int responseTime;
public:
    Actuator(int i, const string &s, int r) : id(i), state(s), responseTime(r) {}
    void triggerAction() {
        state = "Active";
        cout << "[Actuator " << id << "] Triggered, state=" << state
             << " responseTime=" << responseTime << "ms\n";
    }
    void injectDelay() {
        uniform_int_distribution<> dist(20, 100); 
        responseTime += dist(gen);
        cout << "[Actuator " << id << "] Delay injected, new responseTime=" << responseTime << "ms\n";
    }
    int getId() const { return id; }
    string getState() const { return state; }
    int getResponseTime() const { return responseTime; }
};

class ConfigManager {
public:
    void updateThreshold(Sensor& s, float newThreshold) {
        s.setThreshold(newThreshold);
        cout << "[ConfigManager] Sensor " << s.getId() << " threshold updated to " << newThreshold << "\n";
    }
};

class FaultLogger {
    ofstream ofs;
public:
    FaultLogger(const string &filename) { ofs.open(filename); }
    ~FaultLogger() { if (ofs.is_open()) ofs.close(); }
    void log(Sensor& s) {
        if (s.isFaulty() && ofs.is_open()) {
            ofs << "Fault: Sensor " << s.getId()
                << " value=" << s.getValue()
                << " threshold=" << s.getThreshold() << "\n";
        }
    }
};

class FaultInjector {
    vector<Sensor*> sensors;
    vector<Actuator*> actuators;
public:
    void addSensor(Sensor* s) { sensors.push_back(s); }
    void addActuator(Actuator* a) { actuators.push_back(a); }
    void injectSensorFault(Sensor* s) { if (s) s->simulateFault(); }
    void injectActuatorDelay(Actuator& a) { a.injectDelay(); }
    void injectAllFaults() {
        for (auto* s : sensors) if (s) s->simulateFault();
        for (auto* a : actuators) if (a) a->injectDelay();
    }
    void reportStatus() {
        cout << "---- System Report ----\n";
        for (auto* s : sensors) {
            cout << "Sensor " << s->getId() << " status=" << s->getStatus()
                 << " value=" << s->getValue() << " threshold=" << s->getThreshold() << "\n";
        }
    }
    vector<Sensor*>& getSensors() { return sensors; }
    vector<Actuator*>& getActuators() { return actuators; }
};

void logFault(Sensor* s) {
    if (s && s->isFaulty()) {
        cout << "[Global] Sensor " << s->getId() << " Fault Detected\n";
    }
}

void analyzeSystem(FaultInjector* fi) {
    cout << "[Global] Analyzing system...\n";
    for (auto* s : fi->getSensors()) {
        if (s->isFaulty()) {
            cout << " -> Sensor " << s->getId() << " requires maintenance\n";
        }
    }
}

// ----------------- SimulationEngine -----------------
class SimulationEngine {
    ConfigManager& cfg;
    FaultLogger* logger;
    FaultInjector injector;
public:
    SimulationEngine(ConfigManager& c, FaultLogger* l) : cfg(c), logger(l) {}
    void run() {
        // Sample Data
        TemperatureSensor* t1 = new TemperatureSensor(101, 95.0f, 90.0f);
        PressureSensor* p1    = new PressureSensor(102, 45.0f, 50.0f);
        TemperatureSensor* t2 = new TemperatureSensor(103, 70.0f, 65.0f);
        Actuator* a1 = new Actuator(201, "Idle", 120);
        Actuator* a2 = new Actuator(202, "Idle", 150);

        injector.addSensor(t1);
        injector.addSensor(p1);
        injector.addSensor(t2);
        injector.addActuator(a1);
        injector.addActuator(a2);

        cfg.updateThreshold(*t2, 75.0f);
        injector.injectAllFaults();
        injector.reportStatus();
        analyzeSystem(&injector);

        for (auto* s : injector.getSensors()) {
            logger->log(*s);
            logFault(s);
        }

        delete t1; delete p1; delete t2;
        delete a1; delete a2;
    }
};

int main() {
    ConfigManager cm;
    FaultLogger logger("faults.log");
    SimulationEngine engine(cm, &logger);
    engine.run();
    return 0;
}

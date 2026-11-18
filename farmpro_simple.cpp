// FarmPro - Simple C++ OOP version (beginner friendly)
// Compile:  g++ farmpro_simple.cpp -o farmpro
#include <iostream>
#include <string>
using namespace std;

// ---------- Base class: Crop ----------
class Crop {
protected:
    string name;
    int minMoisture;   // soil moisture (%) the crop needs
    int maxTemp;       // temperature (C) above which crop gets stressed
public:
    Crop(string n, int m, int t) {
        name = n;
        minMoisture = m;
        maxTemp = t;
    }
    virtual ~Crop() {}   // needed so 'delete' works properly with derived classes

    string getName()    { return name; }
    int getMinMoisture() { return minMoisture; }
    int getMaxTemp()     { return maxTemp; }

    // virtual = each crop can calculate yield in its own way
    virtual double estimateYield(double area, double health) {
        return 4.0 * area * health / 100;
    }
};

// ---------- Derived classes (inheritance) ----------
class Wheat : public Crop {
public:
    Wheat() : Crop("Wheat", 35, 30) {}
    double estimateYield(double area, double health) {
        return 4.5 * area * health / 100;   // wheat gives 4.5 tonnes per hectare
    }
};

class Rice : public Crop {
public:
    Rice() : Crop("Rice", 60, 35) {}
    double estimateYield(double area, double health) {
        return 5.5 * area * health / 100;   // rice gives 5.5 tonnes per hectare
    }
};

class Maize : public Crop {
public:
    Maize() : Crop("Maize", 40, 34) {}
    double estimateYield(double area, double health) {
        return 6.0 * area * health / 100;   // maize gives 6 tonnes per hectare
    }
};

// ---------- Soil class (stores sensor readings) ----------
class Soil {
private:                       // private = data is protected (encapsulation)
    double moisture;
    double temperature;
public:
    Soil() {
        moisture = 0;
        temperature = 0;
    }
    void setReadings(double m, double t) {
        moisture = m;
        temperature = t;
    }
    double getMoisture()    { return moisture; }
    double getTemperature() { return temperature; }
};

// ---------- Field class: a crop + soil together ----------
class Field {
private:
    Crop* crop;     // pointer so it can be Wheat, Rice or Maize
    Soil soil;
    double area;    // in hectares
public:
    Field(Crop* c, double a) {
        crop = c;
        area = a;
    }

    void setSensorData(double moisture, double temp) {
        soil.setReadings(moisture, temp);
    }

    // Decide if irrigation is needed
    void checkIrrigation() {
        cout << "\n--- Irrigation Advice ---\n";
        if (soil.getMoisture() < crop->getMinMoisture()) {
            cout << "Irrigation recommended (moisture is too low)\n";
        } else if (soil.getMoisture() > 90) {
            cout << "Waterlogged - stop irrigation\n";
        } else if (soil.getTemperature() > crop->getMaxTemp()) {
            cout << "Heat stress - consider light irrigation\n";
        } else {
            cout << "No irrigation needed\n";
        }
    }

    // Health score from 0 to 100
    double calculateHealth() {
        double health = 100;
        if (soil.getMoisture() < crop->getMinMoisture())
            health -= (crop->getMinMoisture() - soil.getMoisture()) * 1.5;
        if (soil.getTemperature() > crop->getMaxTemp())
            health -= (soil.getTemperature() - crop->getMaxTemp()) * 3;
        if (health < 0) health = 0;
        return health;
    }

    void showReport() {
        double health = calculateHealth();
        cout << "\n===== FIELD REPORT =====\n";
        cout << "Crop: " << crop->getName() << "\n";
        cout << "Area: " << area << " hectares\n";
        cout << "Soil moisture: " << soil.getMoisture() << " %\n";
        cout << "Temperature: " << soil.getTemperature() << " C\n";
        cout << "Crop health: " << health << " %\n";
        cout << "Estimated yield: " << crop->estimateYield(area, health) << " tonnes\n";
        checkIrrigation();
    }
};

// ---------- Main program ----------
int main() {
    int choice;
    double area, moisture, temp;

    cout << "=== FARMPRO ===\n";
    cout << "Choose crop: 1) Wheat  2) Rice  3) Maize\n";
    cin >> choice;

    Crop* crop;
    if (choice == 1) crop = new Wheat();
    else if (choice == 2) crop = new Rice();
    else crop = new Maize();

    cout << "Enter field area (hectares): ";
    cin >> area;

    Field field(crop, area);

    int option;
    do {
        cout << "\n1. Enter sensor readings (from dashboard)\n";
        cout << "2. Show report\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> option;

        if (option == 1) {
            cout << "Soil moisture (%): ";
            cin >> moisture;
            cout << "Temperature (C): ";
            cin >> temp;
            field.setSensorData(moisture, temp);
        } else if (option == 2) {
            field.showReport();
        }
    } while (option != 0);

    delete crop;   // free memory
    return 0;
}

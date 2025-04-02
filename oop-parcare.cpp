#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

// clasa pentru un singur loc de parcare
class Pspot {
private:
    string license;
    int entryH;
    int entryM;
    int exitH;
    int exitM;
    float hourlyRate;

public:
    // constructor pentru loc de parcare
    Pspot(string license, int entryH, int entryM, float hourlyRate)
        : license(license), entryH(entryH), entryM(entryM), hourlyRate(hourlyRate) {
    }

    // metoda pentru ora de iesire
    void setetime(int exitH, int exitM) {
        this->exitH = exitH;
        this->exitM = exitM;
    }

    // metoda pentru pret virtuala pentru a fi suprascrisa in clasa derivata
    virtual float getprice() const {
        int time = (exitH - entryH) * 60 + (exitM - entryM);
        if (time < 0) {
            time += 24 * 60; // daca ora de iesire este inainte de ora de intrare, adaugam 24 de ore    
        }
        return time * hourlyRate / 60;
    }

    // getter pentru numarul de inmatriculare
    string getlicense() const {
        return license;
    }

    // destructor
    ~Pspot() {
    }

    // clasa prieten
    friend class PManager;
};

// clasa derivata din Pspot pentru locuri VIP
class VIPspot : public Pspot {
public:
    // constructor pentru loc VIP
    VIPspot(string license, int entryH, int entryM, float hourlyRate)
        : Pspot(license, entryH, entryM, hourlyRate) {
    }

    // suprascrierea metodei pentru pret
    float getprice() const override {
        return Pspot::getprice() * 2; // locul VIP costa dublu
    }
};

// clasa pentru manager
class PManager {
public:
    // metoda pentru afisarea detalii loc parcare
    void displayDet(const Pspot& spot) const {
        cout << "----------------------------------------" << endl;
        cout << "License Plate: " << spot.license << endl;
        cout << "Entry Time: " << (spot.entryH < 10 ? "0" : "") << spot.entryH << ":" << (spot.entryM < 10 ? "0" : "") << spot.entryM << endl;
        cout << "----------------------------------------" << endl;
    } // folosim operatorul ternar pentru a adauga 0 in fata cifrelor mai mici decat 10
};

// clasa pentru gestionare parcarii
class PLot {
private:
    vector<Pspot*> s; // stocam locurile de parcare
public:
    // metoda pentru adaugarea unui loc de parcare
    void addPspot(Pspot* spot) {
        s.push_back(spot);
    }

    // metoda pentru afisarea tarifelor pentru toate masinile parcate
    void displayPrices() const {
        cout << "----------------------------------------" << endl;
        cout << "Parking Fees:" << endl;
        for (const auto& spot : s) {
            cout << "License Plate: " << spot->getlicense() << " - Price: $" << spot->getprice() << endl;
        }
        cout << "----------------------------------------" << endl;
    }

    // destructor pentru a elibare memoria dinamica folosita la adaugarea locurilor de parcare
    ~PLot() {
        for (auto spot : s) {
            delete spot;
        }
    }
};

int main() {
    int availableS;
    cout << "----------------------------------------" << endl;
    cout << "Welcome to Bacau Parking!" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Enter the number of available spots: ";
    cin >> availableS;
    cin.ignore(); // pentru a evita citirea unui newline

    PLot lot; // creem un lot de parcare
    PManager manager; // creem un manager pentru lotul de parcare

    // citim locurile de parcare
    for (int i = 0; i < availableS; ++i) {
        string license;
        int entryH, entryM, exitH, exitM;
        float hourlyR;
        char isVIP;

        cout << "----------------------------------------" << endl;
        cout << "Enter details for parking spot " << i + 1 << ":" << endl;

        // citim numarul de inmatriculare
        cout << "Enter the license plate: ";
        getline(cin, license); // citim linie intreaga, permite spatii

        // citim ora de intrare
        cout << "Enter the entry time (HH MM): ";
        cin >> entryH >> entryM;
        while (entryH < 0 || entryH > 23 || entryM < 0 || entryM > 59) {
            cout << "Invalid time. Enter the entry time (HH MM): ";
            cin >> entryH >> entryM;
        }

        // citim tariful pe ora
        cout << "Enter the hourly rate: ";
        cin >> hourlyR;

        // citim daca locul este VIP
        cout << "Is the spot VIP? (y/n): ";
        cin >> isVIP;

        Pspot* spot;
        if (isVIP == 'y' || isVIP == 'Y') {
            spot = new VIPspot(license, entryH, entryM, hourlyR);
        }
        else {
            spot = new Pspot(license, entryH, entryM, hourlyR);
        }

        // citim ora de iesire
        cout << "Enter the exit time (HH MM): ";
        cin >> exitH >> exitM;
        while (exitH < 0 || exitH > 23 || exitM < 0 || exitM > 59) {
            cout << "Invalid time. Enter the exit time (HH MM): ";
            cin >> exitH >> exitM;
        }
        spot->setetime(exitH, exitM);

        // adaugam locul de parcare in lot
        lot.addPspot(spot);
        manager.displayDet(*spot); // afisarea detaliilor folosing clasa prieten
        cin.ignore(); // ignoram iar newline-ul ramas in buffer dupa ora de iesire
    }

    // asteptam 2 secunde
    this_thread::sleep_for(chrono::seconds(2));

    // stergem consola
    system("cls");

    // afisarea tarifelor pentru masinile parcate
    lot.displayPrices();

    return 0;
}
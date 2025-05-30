#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    vector<string> inventory;
    string route;
    string input;
    bool hasTorch = false;
    bool hasKey = false;
    bool hasKeycard = false;

    cout << "Welkom in het doolhof!" << endl;

start:
    cout << "\nJe bent in kamer 1." << endl;
    cout << "Beschikbare richtingen: noord, oost" << endl;
    cout << "Typ een richting: ";
    cin >> input;
    route += input + " ";
    if (input == "noord") goto kamer2;
    if (input == "oost") goto kamer3;
    cout << "Ongeldige richting." << endl;
    goto start;

kamer2:
    cout << "\nJe bent in kamer 2." << endl;
    cout << "Je vindt hier een fakkel!" << endl;
    if (!hasTorch) {
        inventory.push_back("fakkel");
        hasTorch = true;
    }
    cout << "Beschikbare richtingen: zuid, oost" << endl;
    cout << "Typ een richting: ";
    cin >> input;
    route += input + " ";
    if (input == "zuid") goto start;
    if (input == "oost") goto kamer4;
    cout << "Ongeldige richting." << endl;
    goto kamer2;

kamer3:
    cout << "\nJe bent in kamer 3 (donker)." << endl;
    if (!hasTorch) {
        cout << "Het is hier te donker. Je hebt een fakkel nodig om verder te gaan." << endl;
        cout << "Je gaat automatisch terug." << endl;
        goto start;
    }
    cout << "Je gebruikt de fakkel om verder te komen." << endl;
    cout << "Beschikbare richtingen: west, noord" << endl;
    cout << "Typ een richting: ";
    cin >> input;
    route += input + " ";
    if (input == "west") goto start;
    if (input == "noord") goto kamer5;
    cout << "Ongeldige richting." << endl;
    goto kamer3;

kamer4:
    cout << "\nJe bent in kamer 4." << endl;
    cout << "Je vindt hier een sleutel!" << endl;
    if (!hasKey) {
        inventory.push_back("sleutel");
        hasKey = true;
    }
    cout << "Beschikbare richtingen: west, noord" << endl;
    cout << "Typ een richting: ";
    cin >> input;
    route += input + " ";
    if (input == "west") goto kamer2;
    if (input == "noord") goto kamer6;
    cout << "Ongeldige richting." << endl;
    goto kamer4;

kamer5:
    cout << "\nJe bent in kamer 5." << endl;
    cout << "Beschikbare richtingen: zuid, oost" << endl;
    cout << "Typ een richting: ";
    cin >> input;
    route += input + " ";
    if (input == "zuid") goto kamer3;
    if (input == "oost") goto kamer7;
    cout << "Ongeldige richting." << endl;
    goto kamer5;

kamer6:
    cout << "\nJe bent in kamer 6 (deur met slot)." << endl;
    if (!hasKey) {
        cout << "De deur is op slot. Je hebt een sleutel nodig." << endl;
        cout << "Je gaat automatisch terug." << endl;
        goto kamer4;
    }
    cout << "Je gebruikt de sleutel om de deur te openen." << endl;
    cout << "Beschikbare richtingen: zuid, oost" << endl;
    cout << "Typ een richting: ";
    cin >> input;
    route += input + " ";
    if (input == "zuid") goto kamer4;
    if (input == "oost") goto kamer8;
    cout << "Ongeldige richting." << endl;
    goto kamer6;

kamer7:
    cout << "\nJe bent in kamer 7 (sleutelkaart kamer)." << endl;
    cout << "Je vindt hier een sleutelkaart!" << endl;
    if (!hasKeycard) {
        inventory.push_back("sleutelkaart");
        hasKeycard = true;
    }
    cout << "Beschikbare richtingen: west, noord" << endl;
    cout << "Typ een richting: ";
    cin >> input;
    route += input + " ";
    if (input == "west") goto kamer5;
    if (input == "noord") goto start;
    cout << "Ongeldige richting." << endl;
    goto kamer7;

kamer8:
    cout << "\nJe bent in kamer 8 (high security)." << endl;
    if (!hasKeycard) {
        cout << "De deur vereist een sleutelkaart. Je gaat automatisch terug." << endl;
        goto kamer6;
    }
    cout << "Je gebruikt de sleutelkaart en bereikt de uitgang!" << endl;
    goto einde;

einde:
    cout << "\nGefeliciteerd! Je hebt het doolhof verlaten." << endl;
    cout << "Jouw afgelegde route: " << route << endl;
    cout << "Je inventory: ";
    for (const auto& item : inventory) {
        cout << item << " ";
    }
    cout << endl;

    return 0;
}

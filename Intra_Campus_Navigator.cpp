#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <climits>
using namespace std;

const int MAXN = 30;
const int MAXE = 10;
const int NAME_LEN = 30;

struct Edge {
    int to;
    int cost;
};

class Location {
public:
    char name[NAME_LEN];
    Edge edges[MAXE];
    int edgeCount;

    Location() {
        name[0] = '\0';
        edgeCount = 0;
    }
};



    bool edgeExists(int a, int b) {
        for (int i = 0; i < graph[a].edgeCount; i++) {
            if (graph[a].edges[i].to == b)
                return true;
        }
        return false;
    }

public:
    CampusNavigator() {
        totalLocations = 0;
    }

    void addLocation(const char name[], bool quiet = false) {

        if (name[0] == '\0') {
            cout << "A location needs a name.\n";
            return;
        }

        if (totalLocations >= MAXN) {
            cout << "Sorry, the campus map is full (" << MAXN
                 << " locations maximum).\n";
            return;
        }

        if (findByName(name) != -1) {
            cout << "\"" << name << "\" is already on the map.\n";
            return;
        }

        strncpy(graph[totalLocations].name, name, NAME_LEN - 1);
        graph[totalLocations].name[NAME_LEN - 1] = '\0';
        graph[totalLocations].edgeCount = 0;
        totalLocations++;

        if (!quiet)
            cout << "Done! \"" << name << "\" was added as location number "
                 << totalLocations << ".\n";
    }

    void addEdge(const char u[], const char v[], int cost, bool quiet = false) {

        int a = findLocation(u);
        int b = findLocation(v);

        if (a == -1) {
            cout << "I couldn't find \"" << u
                 << "\". Choose option 1 to see all locations.\n";
            return;
        }
        if (b == -1) {
            cout << "I couldn't find \"" << v
                 << "\". Choose option 1 to see all locations.\n";
            return;
        }

        if (a == b) {
            cout << "A place can't be connected to itself.\n";
            return;
        }

        if (cost <= 0) {
            cout << "The distance must be greater than zero.\n";
            return;
        }

        if (edgeExists(a, b)) {
            cout << graph[a].name << " and " << graph[b].name
                 << " are already connected.\n";
            return;
        }

        if (graph[a].edgeCount >= MAXE || graph[b].edgeCount >= MAXE) {
            cout << "One of these places already has the maximum of "
                 << MAXE << " connections.\n";
            return;
        }

        graph[a].edges[graph[a].edgeCount].to = b;
        graph[a].edges[graph[a].edgeCount].cost = cost;
        graph[a].edgeCount++;

        graph[b].edges[graph[b].edgeCount].to = a;
        graph[b].edges[graph[b].edgeCount].cost = cost;
        graph[b].edgeCount++;

        if (!quiet)
            cout << "Connected " << graph[a].name << " <-> " << graph[b].name
                 << " (distance " << cost << ").\n";
    }

    void displayLocations() {

        cout << "\n===== CAMPUS LOCATIONS =====\n";

        if (totalLocations == 0) {
            cout << "No locations yet. Use option 4 to add one.\n";
            return;
        }

        for (int i = 0; i < totalLocations; i++) {
            if (i + 1 < 10)
                cout << " ";
            cout << i + 1 << ". " << graph[i].name << endl;
        }

        cout << "\n(Tip: you can type a number instead of a name.)\n";
    }

    void displayConnections() {

        cout << "\n===== CAMPUS CONNECTIONS =====\n";

        for (int i = 0; i < totalLocations; i++) {

            cout << "\n" << graph[i].name << " connects to:\n";

            if (graph[i].edgeCount == 0) {
                cout << "   (nothing yet)\n";
                continue;
            }

            for (int j = 0; j < graph[i].edgeCount; j++) {
                int next = graph[i].edges[j].to;
                cout << "   - " << graph[next].name
                     << " (distance " << graph[i].edges[j].cost << ")\n";
            }
        }
    }

    void searchLocation(const char name[]) {

        int index = findLocation(name);

        if (index == -1) {
            cout << "Sorry, I couldn't find \"" << name << "\".\n";
            cout << "Choose option 1 to see every location on campus.\n";
            return;
        }

        cout << "\nFound: " << graph[index].name << endl;

        if (graph[index].edgeCount == 0) {
            cout << "It isn't connected to any other place yet.\n";
            return;
        }

        cout << "You can walk directly to:\n";

        for (int i = 0; i < graph[index].edgeCount; i++) {
            int next = graph[index].edges[i].to;
            cout << "   - " << graph[next].name
                 << " (distance " << graph[index].edges[i].cost << ")\n";
        }
    }

    void shortestPath(const char start[], const char destination[]) {

        int source = findLocation(start);
        int dest = findLocation(destination);

        if (source == -1) {
            cout << "I couldn't find the starting point \"" << start
                 << "\". Choose option 1 to see all locations.\n";
            return;
        }
        if (dest == -1) {
            cout << "I couldn't find the destination \"" << destination
                 << "\". Choose option 1 to see all locations.\n";
            return;
        }

        if (source == dest) {
            cout << "You're already at " << graph[source].name << "!\n";
            return;
        }

        int distance[MAXN];
        int previous[MAXN];
        int visited[MAXN];

        for (int i = 0; i < totalLocations; i++) {
            distance[i] = INT_MAX;
            previous[i] = -1;
            visited[i] = 0;
        }

        distance[source] = 0;

        for (int count = 0; count < totalLocations; count++) {

            int current = -1;
            int minDistance = INT_MAX;

            for (int i = 0; i < totalLocations; i++) {
                if (!visited[i] && distance[i] < minDistance) {
                    minDistance = distance[i];
                    current = i;
                }
            }

            if (current == -1)
                break;

            visited[current] = 1;

            if (current == dest)
                break;

            for (int i = 0; i < graph[current].edgeCount; i++) {

                int next = graph[current].edges[i].to;
                int cost = graph[current].edges[i].cost;

                if (!visited[next] &&
                    distance[current] + cost < distance[next]) {

                    distance[next] = distance[current] + cost;
                    previous[next] = current;
                }
            }
        }

        if (distance[dest] == INT_MAX) {
            cout << "Sorry, there is no walking route from "
                 << graph[source].name << " to " << graph[dest].name << ".\n";
            return;
        }

        int path[MAXN];
        int pathSize = 0;
        int current = dest;

        while (current != -1 && pathSize < MAXN) {
            path[pathSize++] = current;
            current = previous[current];
        }

        cout << "\n===== SHORTEST ROUTE =====\n";

        for (int i = pathSize - 1; i >= 0; i--) {
            cout << graph[path[i]].name;

            if (i > 0)
                cout << " -> ";
        }

        cout << "\n\nTotal distance: " << distance[dest] << " units\n";
    }
};

void pause() {
    char dummy[10];
    readText("\nPress Enter to go back to the menu...", dummy, 10);
}

void loadDefaultCampus(CampusNavigator &campus) {

    const char *places[] = {
        "Gate1", "Entry", "MacLab", "ERPCell", "Library",
        "GirlsWashroom1", "Hall1", "OfficeRoom1", "Lab2",
        "GirlsWashroom2", "BoysWashroom1", "Hall2", "Office2",
        "Lab1", "BoysWashroom2", "Out", "LiftStair",
        "Class1", "Class2", "Class3", "Class4", "Class5",
        "SportsGround", "Gate2", "SeatingArea", "Canteen"
    };

    int count = sizeof(places) / sizeof(places[0]);
    for (int i = 0; i < count; i++)
        campus.addLocation(places[i], true);

    campus.addEdge("Gate1", "Entry", 3, true);
    campus.addEdge("Entry", "MacLab", 2, true);
    campus.addEdge("MacLab", "ERPCell", 3, true);
    campus.addEdge("ERPCell", "Library", 3, true);
    campus.addEdge("ERPCell", "GirlsWashroom1", 2, true);
    campus.addEdge("GirlsWashroom1", "Hall1", 4, true);
    campus.addEdge("Hall1", "OfficeRoom1", 2, true);
    campus.addEdge("OfficeRoom1", "Lab2", 4, true);
    campus.addEdge("Lab2", "GirlsWashroom2", 2, true);
    campus.addEdge("OfficeRoom1", "LiftStair", 3, true);
    campus.addEdge("Entry", "BoysWashroom1", 2, true);
    campus.addEdge("Entry", "Hall2", 3, true);
    campus.addEdge("Hall2", "Office2", 4, true);
    campus.addEdge("Office2", "Lab1", 4, true);
    campus.addEdge("Office2", "LiftStair", 3, true);
    campus.addEdge("Lab1", "BoysWashroom2", 3, true);
    campus.addEdge("BoysWashroom2", "Out", 2, true);
    campus.addEdge("Lab2", "Class1", 3, true);
    campus.addEdge("Class1", "Class2", 3, true);
    campus.addEdge("Class2", "Class3", 3, true);
    campus.addEdge("Class3", "Class4", 3, true);
    campus.addEdge("Class4", "Class5", 3, true);
    campus.addEdge("Class5", "Lab1", 3, true);
    campus.addEdge("Entry", "SportsGround", 15, true);
    campus.addEdge("SportsGround", "Gate2", 4, true);
    campus.addEdge("LiftStair", "SeatingArea", 2, true);
    campus.addEdge("LiftStair", "Canteen", 5, true);
    campus.addEdge("GirlsWashroom2", "Out", 2, true);
}

int main() {

    CampusNavigator campus;
    loadDefaultCampus(campus);

    cout << "Welcome to the Intra Campus Navigator!\n";
    cout << "Ask me for the quickest way between any two places.\n";

    int choice = 0;

    do {
        cout << "\n=================================\n";
        cout << "     INTRA CAMPUS NAVIGATOR\n";
        cout << "=================================\n";
        cout << "1. See all locations\n";
        cout << "2. See campus connections\n";
        cout << "3. Find the shortest route\n";
        cout << "4. Add a new location\n";
        cout << "5. Add a new connection\n";
        cout << "6. Search for a location\n";
        cout << "7. Exit\n\n";

        choice = readNumber("What would you like to do? (1-7): ");

        char first[NAME_LEN], second[NAME_LEN];

        switch (choice) {

            case 1:
                campus.displayLocations();
                pause();
                break;

            case 2:
                campus.displayConnections();
                pause();
                break;

            case 3:
                readText("Where are you now? (name or number): ", first, NAME_LEN);
                readText("Where do you want to go? (name or number): ", second, NAME_LEN);
                campus.shortestPath(first, second);
                pause();
                break;

            case 4:
                readText("Name of the new location: ", first, NAME_LEN);
                campus.addLocation(first);
                pause();
                break;

            case 5: {
                readText("First location (name or number): ", first, NAME_LEN);
                readText("Second location (name or number): ", second, NAME_LEN);
                int cost = readNumber("Distance between them: ");
                campus.addEdge(first, second, cost);
                pause();
                break;
            }

            case 6:
                readText("Location to search for (name or number): ", first, NAME_LEN);
                campus.searchLocation(first);
                pause();
                break;

            case 7:
                cout << "\nThanks for using Campus Navigator. Have a great day!\n";
                break;

            default:
                cout << "That's not on the menu. Please pick a number from 1 to 7.\n";
        }

    } while (choice != 7);

    return 0;
}

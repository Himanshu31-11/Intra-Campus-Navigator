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

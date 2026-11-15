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

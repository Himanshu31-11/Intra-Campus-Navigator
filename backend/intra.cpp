#include <winsock2.h>   // must be first on Windows
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <string>
using namespace std;

const int MAXN = 30;  // max locations
const int MAXE = 10;  // max edges per location

struct Edge {
    int to;
    int cost;
};

struct Location {
    char name[30];
    Edge edges[MAXE];
    int edgeCount;
};

Location graph[MAXN];
int totalLocations = 0;

string toStr(int x) {
    ostringstream o;
    o << x;
    return o.str();
}

/* ---------------- Graph and Dijkstra (your original logic) ---------------- */

int findLocation(const char name[]) {
    for (int i = 0; i < totalLocations; i++) {
        if (strcmp(graph[i].name, name) == 0)
            return i;
    }
    return -1;
}

void addLocation(const char name[]) {
    if (totalLocations >= MAXN)
        return;
    strncpy(graph[totalLocations].name, name, 29);
    graph[totalLocations].name[29] = '\0';
    graph[totalLocations].edgeCount = 0;
    totalLocations++;
}

void addEdge(const char u[], const char v[], int cost) {
    int a = findLocation(u);
    int b = findLocation(v);

    if (a == -1 || b == -1)
        return;
    if (graph[a].edgeCount >= MAXE || graph[b].edgeCount >= MAXE)
        return;

    graph[a].edges[graph[a].edgeCount].to = b;
    graph[a].edges[graph[a].edgeCount].cost = cost;
    graph[a].edgeCount++;

    graph[b].edges[graph[b].edgeCount].to = a;
    graph[b].edges[graph[b].edgeCount].cost = cost;
    graph[b].edgeCount++;
}

// Returns 0 = success, 1 = invalid location, 2 = no route
int shortestPath(const char start[], const char destination[],
                 int path[], int &pathSize, int &total) {
    int source = findLocation(start);
    int dest = findLocation(destination);

    if (source == -1 || dest == -1)
        return 1;

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
            if (visited[i] == 0 && distance[i] < minDistance) {
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

            if (visited[next] == 0 &&
                distance[current] + cost < distance[next]) {
                distance[next] = distance[current] + cost;
                previous[next] = current;
            }
        }
    }

    if (distance[dest] == INT_MAX)
        return 2;

    int temp[MAXN];
    int count = 0;
    int current = dest;

    while (current != -1) {
        temp[count++] = current;
        current = previous[current];
    }

    pathSize = 0;
    for (int i = count - 1; i >= 0; i--)
        path[pathSize++] = temp[i];

    total = distance[dest];
    return 0;
}

void buildCampus() {
    addLocation("Gate1");
    addLocation("Entry");
    addLocation("MacLab");
    addLocation("ERPCell");
    addLocation("Library");
    addLocation("GirlsWashroom1");
    addLocation("Hall1");
    addLocation("OfficeRoom1");
    addLocation("Lab2");
    addLocation("GirlsWashroom2");
    addLocation("BoysWashroom1");
    addLocation("Hall2");
    addLocation("Office2");
    addLocation("Lab1");
    addLocation("BoysWashroom2");
    addLocation("Out");
    addLocation("LiftStair");
    addLocation("Class1");
    addLocation("Class2");
    addLocation("Class3");
    addLocation("Class4");
    addLocation("Class5");
    addLocation("SportsGround");
    addLocation("Gate2");
    addLocation("SeatingArea");
    addLocation("Canteen");

    addEdge("Gate1", "Entry", 3);
    addEdge("Entry", "MacLab", 2);
    addEdge("MacLab", "ERPCell", 3);
    addEdge("ERPCell", "Library", 3);
    addEdge("ERPCell", "GirlsWashroom1", 2);
    addEdge("GirlsWashroom1", "Hall1", 4);
    addEdge("Hall1", "OfficeRoom1", 2);
    addEdge("OfficeRoom1", "Lab2", 4);
    addEdge("Lab2", "GirlsWashroom2", 2);
    addEdge("OfficeRoom1", "LiftStair", 3);
    addEdge("Entry", "BoysWashroom1", 2);
    addEdge("Entry", "Hall2", 3);
    addEdge("Hall2", "Office2", 4);
    addEdge("Office2", "Lab1", 4);
    addEdge("Office2", "LiftStair", 3);
    addEdge("Lab1", "BoysWashroom2", 3);
    addEdge("BoysWashroom2", "Out", 2);
    addEdge("Lab2", "Class1", 3);
    addEdge("Class1", "Class2", 3);
    addEdge("Class2", "Class3", 3);
    addEdge("Class3", "Class4", 3);
    addEdge("Class4", "Class5", 3);
    addEdge("Class5", "Lab1", 3);
    addEdge("Entry", "SportsGround", 15);
    addEdge("SportsGround", "Gate2", 4);
    addEdge("LiftStair", "SeatingArea", 2);
    addEdge("LiftStair", "Canteen", 5);
    addEdge("GirlsWashroom2", "Out", 2);
}

/* ---------------- JSON helpers ---------------- */

string graphToJson() {
    string json = "{\"locations\":[";
    for (int i = 0; i < totalLocations; i++) {
        if (i > 0) json += ",";
        json += "\"" + string(graph[i].name) + "\"";
    }
    json += "],\"edges\":[";

    bool first = true;
    for (int i = 0; i < totalLocations; i++) {
        for (int j = 0; j < graph[i].edgeCount; j++) {
            int to = graph[i].edges[j].to;
            if (to < i) continue;   // each undirected edge only once
            if (!first) json += ",";
            first = false;
            json += "{\"from\":\"" + string(graph[i].name) +
                    "\",\"to\":\"" + string(graph[to].name) +
                    "\",\"cost\":" + toStr(graph[i].edges[j].cost) + "}";
        }
    }
    json += "]}";
    return json;
}

/* ---------------- Small web server helpers ---------------- */

string urlDecode(const string &s) {
    string out;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '%' && i + 2 < s.size()) {
            out += (char)strtol(s.substr(i + 1, 2).c_str(), NULL, 16);
            i += 2;
        } else if (s[i] == '+') {
            out += ' ';
        } else {
            out += s[i];
        }
    }
    return out;
}

string getParam(const string &query, const string &key) {
    string search = key + "=";
    size_t pos = 0;
    while (pos < query.size()) {
        size_t end = query.find('&', pos);
        if (end == string::npos) end = query.size();
        string part = query.substr(pos, end - pos);
        if (part.compare(0, search.size(), search) == 0)
            return urlDecode(part.substr(search.size()));
        pos = end + 1;
    }
    return "";
}

bool endsWith(const string &s, const string &suffix) {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

string contentTypeFor(const string &path) {
    if (endsWith(path, ".html")) return "text/html; charset=utf-8";
    if (endsWith(path, ".css"))  return "text/css; charset=utf-8";
    if (endsWith(path, ".js"))   return "application/javascript; charset=utf-8";
    if (endsWith(path, ".json")) return "application/json";
    if (endsWith(path, ".png"))  return "image/png";
    if (endsWith(path, ".jpg"))  return "image/jpeg";
    if (endsWith(path, ".svg"))  return "image/svg+xml";
    return "application/octet-stream";
}

bool readFile(const string &path, string &content) {
    ifstream f(path.c_str(), ios::binary);
    if (!f) return false;
    ostringstream ss;
    ss << f.rdbuf();
    content = ss.str();
    return true;
}

void sendResponse(SOCKET client, int status, const string &type, const string &body) {
    string text = (status == 200) ? "OK" : (status == 400 ? "Bad Request" : "Not Found");
    string head = "HTTP/1.1 " + toStr(status) + " " + text + "\r\n";
    head += "Content-Type: " + type + "\r\n";
    head += "Content-Length: " + toStr((int)body.size()) + "\r\n";
    head += "Access-Control-Allow-Origin: *\r\n";
    head += "Connection: close\r\n\r\n";

    send(client, head.c_str(), (int)head.size(), 0);

    size_t sent = 0;
    while (sent < body.size()) {
        int n = send(client, body.c_str() + sent, (int)(body.size() - sent), 0);
        if (n <= 0) break;
        sent += n;
    }
}

void handleClient(SOCKET client) {
    char buffer[4096];
    int n = recv(client, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0) return;
    buffer[n] = '\0';

    string request(buffer);
    size_t sp1 = request.find(' ');
    size_t sp2 = (sp1 == string::npos) ? string::npos : request.find(' ', sp1 + 1);
    if (sp1 == string::npos || sp2 == string::npos) {
        sendResponse(client, 400, "application/json", "{\"error\":\"Bad request\"}");
        return;
    }

    string target = request.substr(sp1 + 1, sp2 - sp1 - 1);
    string path = target, query = "";
    size_t q = target.find('?');
    if (q != string::npos) {
        path = target.substr(0, q);
        query = target.substr(q + 1);
    }

    /* GET /api/graph */
    if (path == "/api/graph") {
        sendResponse(client, 200, "application/json", graphToJson());
        return;
    }

    /* GET /api/route?start=Gate1&dest=Library */
    if (path == "/api/route") {
        string start = getParam(query, "start");
        string dest = getParam(query, "dest");

        if (start.empty() || dest.empty()) {
            sendResponse(client, 400, "application/json", "{\"error\":\"Missing start or dest\"}");
            return;
        }

        int route[MAXN];
        int pathSize = 0;
        int total = 0;
        int status = shortestPath(start.c_str(), dest.c_str(), route, pathSize, total);

        if (status == 1) {
            sendResponse(client, 404, "application/json", "{\"error\":\"Invalid campus location!\"}");
            return;
        }
        if (status == 2) {
            sendResponse(client, 404, "application/json", "{\"error\":\"No route found!\"}");
            return;
        }

        string json = "{\"path\":[";
        for (int i = 0; i < pathSize; i++) {
            if (i > 0) json += ",";
            json += "\"" + string(graph[route[i]].name) + "\"";
        }
        json += "],\"totalCost\":" + toStr(total) + "}";
        sendResponse(client, 200, "application/json", json);
        return;
    }

    /* Everything else: serve files from ../frontend */
    if (path == "/") path = "/index.html";

    if (path.find("..") != string::npos) {
        sendResponse(client, 404, "text/plain", "Not found");
        return;
    }

    string content;
    if (readFile("../frontend" + path, content)) {
        sendResponse(client, 200, contentTypeFor(path), content);
    } else {
        sendResponse(client, 404, "text/plain", "Not found");
    }
}

/* ---------------- main ---------------- */

int main() {
    buildCampus();

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "Could not start Winsock.\n";
        return 1;
    }

    SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == INVALID_SOCKET) {
        cout << "Could not create socket.\n";
        return 1;
    }

    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8080);

    if (::bind(server, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "Could not use port 8080. Is another program using it?\n";
        return 1;
    }

    listen(server, 10);

    cout << "===== INTRA CAMPUS NAVIGATOR =====\n";
    cout << "Server running. Open http://localhost:8080 in your browser.\n";
    cout << "Press Ctrl+C to stop.\n";

    while (true) {
        SOCKET client = accept(server, NULL, NULL);
        if (client == INVALID_SOCKET)
            continue;
        handleClient(client);
        closesocket(client);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
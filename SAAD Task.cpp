#include<iostream>
#include<vector>
#include<unordered_map>
#include<queue>

using namespace std;

struct Task {
    string name;
    vector<string> dependencies;
    int duration;
    int Estart = 0;
    int Efinish = 0;
    int Lstart = 0;
    int Lfinish = 0;
};

void InputTask(vector<Task>& tasks, unordered_map<string, vector<string>>& adjList);
void displaytasks(const vector<Task>& tasks);
void displayAdjL(const vector<Task>& tasks, const unordered_map<string, vector<string>>& adjList);
void topologicalSort(const vector<Task>& tasks, unordered_map<string, vector<string>>& adjList);

int main() {
    vector<Task> task;
    unordered_map<string, vector<string>> adjL;

    InputTask(task, adjL);
    displaytasks(task);
    displayAdjL(task, adjL);

    // Perform topological sorting
    topologicalSort(task, adjL);

    return 0;
}

void InputTask(vector<Task>& tasks, unordered_map<string, vector<string>>& adjList) {
    int taskNo;
    cout << "enter number of tasks :";
    cin >> taskNo;
    for (int i = 0; i < taskNo; i++) {
        Task task;
        int dependenciesNo;

        cout << "enter task " << i + 1 << " name : ";
        cin >> task.name;

        cout << "enter task duration :  ";
        cin >> task.duration;

        cout << "enter number of dependencies : ";
        cin >> dependenciesNo;

        for (int j = 0; j < dependenciesNo; j++) {
            string dep;
            cout << "enter name of dependency " << j + 1 << " : ";
            cin >> dep;
            task.dependencies.push_back(dep);
            adjList[dep].push_back(task.name);  // Reverse the edge for topological sorting
        }
        tasks.push_back(task);
    }
}

void displaytasks(const vector<Task>& tasks) {
    cout << "task details: \n";
    for (const auto& task : tasks) {
        cout << "name: " << task.name
            << "\nduration: " << task.duration
            << "\ndependencies: ";
        if (task.dependencies.empty()) {
            cout << "none";
        }
        else {
            for (const auto& dep : task.dependencies) {
                cout << dep << " ";
            }
        }
        cout << endl;
    }
}

void displayAdjL(const vector<Task>& tasks, const unordered_map<string, vector<string>>& adjList) {
    cout << "adjacency list: \n";
    for (const auto& task : tasks) {
        cout << task.name << ": ";
        auto it = adjList.find(task.name);
        if (it != adjList.end() && !it->second.empty()) {
            for (const auto& dep : it->second) {
                cout << dep << " ";
            }
        }
        else {
            cout << "none";
        }
        cout << endl;
    }
}

void topologicalSort(const vector<Task>& tasks, unordered_map<string, vector<string>>& adjList) {
    // Step 1: Compute indegree for all tasks
    unordered_map<string, int> indegree;
    for (const auto& task : tasks) {
        indegree[task.name] = 0;  // Initialize indegree to 0
    }

    // Fill indegree by counting dependencies
    for (const auto& task : tasks) {
        for (const string& dep : task.dependencies) {
            indegree[task.name]++;
        }
    }

    // Step 2: Find tasks with no dependencies (indegree 0)
    queue<string> q;
    for (const auto& task : tasks) {
        if (indegree[task.name] == 0) {
            q.push(task.name);
        }
    }

    // Step 3: Process tasks in topological order
    vector<string> topoOrder;
    while (!q.empty()) {
        string currentTask = q.front();
        q.pop();
        topoOrder.push_back(currentTask);

        // Reduce the indegree of all neighbors
        for (const auto& neighbor : adjList[currentTask]) {
            indegree[neighbor]--;
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    // Step 4: Check if topological sorting is possible (i.e., no cycle)
    if (topoOrder.size() != tasks.size()) {
        cout << "The graph contains a cycle, topological sorting not possible.\n";
        return;
    }

    // Step 5: Output the topological order
    cout << "Topological Sort Order: ";
    for (const auto& task : topoOrder) {
        cout << task << " ";
    }
    cout << endl;
}

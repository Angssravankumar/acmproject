#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Job {
    int process_id;
    int arrival_time;
    int burst_time;
};

bool compareByArrival(const Job &a, const Job &b) {
    return a.arrival_time < b.arrival_time;
}

int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream outputFile("output.txt");
    int num_processes;
    inputFile >> num_processes;
    std::vector<Job> jobs(num_processes);
    for (int i = 0; i < num_processes; ++i) {
        inputFile >> jobs[i].process_id >> jobs[i].arrival_time >> jobs[i].burst_time;
    }

    std::sort(jobs.begin(), jobs.end(), compareByArrival);

    std::vector<int> waiting_time(num_processes), turnaround_time(num_processes), response_time(num_processes);
    int current_time = 0;
    for (int i = 0; i < num_processes; ++i) {
        if (current_time < jobs[i].arrival_time) {
            current_time = jobs[i].arrival_time;
        }
        waiting_time[i] = current_time - jobs[i].arrival_time;
        turnaround_time[i] = waiting_time[i] + jobs[i].burst_time;
        response_time[i] = waiting_time[i];
        current_time += jobs[i].burst_time;
    }

    double avg_waiting_time = 0, avg_turnaround_time = 0, avg_response_time = 0;
    for (int i = 0; i < num_processes; ++i) {
        avg_waiting_time += waiting_time[i];
        avg_turnaround_time += turnaround_time[i];
        avg_response_time += response_time[i];
    }
    avg_waiting_time /= num_processes;
    avg_turnaround_time /= num_processes;
    avg_response_time /= num_processes;

    outputFile << avg_waiting_time << std::endl;
    outputFile << avg_turnaround_time << std::endl;
    outputFile << avg_response_time << std::endl;

    return 0;
}

#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

//an Event struct to represent events in the simulation (the structure for an event in the simulation, with attributes for the time of the event, type of event, and the machine_id)
struct Event {
    double time;
    int type; // 0 = process, 1 = breakdown, 2 = repair
    int machine_id;

    // overloading operator for priority queue to work as a min-heap
    bool operator<(const Event& other) const {
        return time > other.time;
    }
};

// class for manufacturing system
class ManufacturingSystem {
public:
    // constructor to initialize the system with these parameters 
    ManufacturingSystem(int num_machines, double process_time, double failure_rate, double repair_time) {
        this->num_machines = num_machines;
        this->process_time = process_time;
        this->failure_rate = failure_rate;
        this->repair_time = repair_time;
        this->current_time = 0.0;

        machines.resize(num_machines, true); // All machines start to working with corresponding to num_machines
    }


    //run function the simulation
    void runSimulation(double simulation_time) {
        priority_queue<Event> event_queue;

        // schedule initial process events for all the machines in events
        for (int i = 0; i < num_machines; ++i) {
            schedule_event(event_queue, process_time, 0, i);
        }

        // process events until the simulation time is reached
        while (!event_queue.empty() && event_queue.top().time < simulation_time) {
            Event event = event_queue.top();
            event_queue.pop();
            current_time = event.time;
            handle_event(event_queue, event);
        }

        cout << "Simulation finished at time " << current_time << endl;
    }

private:
    int num_machines;
    double process_time;
    double failure_rate;
    double repair_time;
    double current_time;
    vector<bool> machines; //boolean vector which represent the Machine states: True = working, False = broken

    // function to schedule an event (Schedules an event by creating an event object and pushing it into the event queue)
    void schedule_event(priority_queue<Event>& event_queue, double delay, int type, int machine_id) {
        Event event;
        event.time = current_time + delay;
        event.type = type;
        event.machine_id = machine_id;
        event_queue.push(event);
    }

    // function to handle an event (Handles events based on their type (process completion, breakdown, repair) and schedules subsequent events accordingly)
    void handle_event(priority_queue<Event>& event_queue, Event& event) {
        switch (event.type) {
            case 0: // process complete operaiton
                if (machines[event.machine_id]) {
                    cout << "Machine " << event.machine_id << " completed a process at time " << current_time << endl;
                    // we should determine if the machine breaks down after completing the process
                    if (rand() / double(RAND_MAX) < failure_rate) {
                        schedule_event(event_queue, 0, 1, event.machine_id); //  go to schedule breakdown
                    } else {
                        schedule_event(event_queue, process_time, 0, event.machine_id); // go to schedule next process
                    }
                }
                break;
            case 1: // breakdown operation
                cout << "Machine " << event.machine_id << " broke down at time " << current_time << endl;
                machines[event.machine_id] = false;
                schedule_event(event_queue, repair_time, 2, event.machine_id); // go to schedule repair
                break;
            case 2: // repair operation
                cout << "Machine " << event.machine_id << " repaired at time " << current_time << endl;
                machines[event.machine_id] = true;
                schedule_event(event_queue, process_time, 0, event.machine_id); // go to schedule next process
                break;
        }
    }
};

int main() {
    srand(time(0)); //random number generation

    // simulation parameters
    int num_machines = 5;
    double process_time = 10.0;
    double failure_rate = 0.1;
    double repair_time = 2.0;
    double simulation_time = 100.0;

    // create and run the manufacturing system simulation
    ManufacturingSystem system(num_machines, process_time, failure_rate, repair_time);
    system.runSimulation(simulation_time);

    return 0;
}

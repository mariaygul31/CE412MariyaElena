//  NAME: MARIYA ELENA AYGÜL
//  STUDENT ID:20191701055
#include <iostream>
#include <vector>
#include <random>

using namespace std;

// structure of politicans
struct Politician {
    int age; 
    int service_years; 
    string position; 
    Politician(int a, int s, string p) : age(a), service_years(s), position(p) {} 
};

// function that calculates penalties for re-elected Consuls
int reelectionPenaltyCounter(const vector<Politician>& politicians) {
    int penalty = 0;
    for (int i = 0; i < politicians.size(); ++i) {
        if (politicians[i].position == "Consul" && politicians[i].service_years < 10) {
            penalty -= 10;
        }
    }
    return penalty;
}

//function to count politicians position
int countPosition(const vector<Politician>& politicians, const string& position) {
    int count = 0;
    for (int i = 0; i < politicians.size(); ++i) {
        if (politicians[i].position == position) {
            count++;
        }
    }
    return count;
}

// fill position of politicians by using age and service requirements
void applyPosition(vector<Politician>& politicians, const string& position, int num_positions, int min_age, int min_serviceYears, int interval) {
    int count = countPosition(politicians, position);
    for (int i = count; i < num_positions; ++i) {
        politicians.push_back(Politician(min_age, 0, position));
    }
}

// function to calculate Political Stability Index
int calculatePoliticalStabilityIndex(const vector<Politician>& politicians) {
    int psi = 100;  // starting value of political stability index

    // Calculation of penalties for missing positions
    int unfilledPositionsPenalty = (20 - countPosition(politicians, "Quaestor")) * (-5) + (10 - countPosition(politicians, "Aedile")) * (-5) + (8 - countPosition(politicians, "Praetor")) * (-5) + (2 - countPosition(politicians, "Consul")) * (-5);

    // Penalty calculation for re-elected Consuls is done by using the reelectionPenaltyCounter function.
    int reelectionPenalty = reelectionPenaltyCounter(politicians);

    psi += unfilledPositionsPenalty + reelectionPenalty;
    return psi;
}

//function that prints the age distribution at a specific position of politicians
void calculateAgeDistribution(const vector<Politician>& politicians, const string& position) {
    cout << position << ": ";
    for (int i = 0; i < politicians.size(); ++i) {
        if (politicians[i].position == position) {
            cout << politicians[i].age << " ";
        }
    }
    cout << endl;
}

// funtion of Annual Influx
void simulateForYear(vector<Politician>& politicians) {
    // calculating new candidates
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> distribution_new_candidates(15, 5);
    int numNewCandidates = round(distribution_new_candidates(gen));
    for (int i = 0; i < numNewCandidates; ++i) {
        politicians.push_back(Politician(30, 0, "Quaestor"));
    }

    // calculating aging and life expectancy control
    normal_distribution<double> distribution_age(55, 10);
    for (int i = 0; i < politicians.size();) {
        politicians[i].age++;
        if (politicians[i].age > distribution_age(gen)) {
            politicians.erase(politicians.begin() + i);
        } else {
            ++i;
        }
    }

    applyPosition(politicians, "Consul", 2, 42, 10, 2);
    applyPosition(politicians, "Praetor", 8, 39, 2, 0);
    applyPosition(politicians, "Aedile", 10, 36, 2, 0);
    applyPosition(politicians, "Quaestor", 20, 30, 0, 0);
}


// execute the simulation by using this function
void simulateProject(int numYears) {
    int psi; // Political Stability Index
    vector<Politician> politicians; // list of politicians

    for (int year = 1; year <= numYears; ++year) {
        simulateForYear(politicians);
    }

    //prints the measurements 
    cout << "1) End-of-Simulation PSI: " << calculatePoliticalStabilityIndex(politicians) << endl;
    
    cout << "2) Annual Fill Rate:" << endl;
    cout << "Quaestor: " << countPosition(politicians, "Quaestor") / ((double)numYears * 100) << "%" << endl;
    cout << "Aedile: " << countPosition(politicians, "Aedile") / ((double)numYears * 100) << "%" << endl;
    cout << "Praetor: " << countPosition(politicians, "Praetor") / ((double)numYears * 100) << "%" << endl;
    cout << "Consul: " << countPosition(politicians, "Consul") / ((double)numYears * 100) << "%" << endl;

    cout << "3) Age Distribution:" << endl;
    calculateAgeDistribution(politicians, "Quaestor");
    calculateAgeDistribution(politicians, "Aedile");
    calculateAgeDistribution(politicians, "Praetor");
    calculateAgeDistribution(politicians, "Consul");
}

int main() {
    simulateProject(200);
    return 0;
}

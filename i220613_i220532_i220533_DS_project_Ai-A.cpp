

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Order 
{
    string name;
    int location;
    int delivery_time_limit;
};

struct Restaurant 
{
    string name;
    int location;
    Order orders[50]; // Maximum 100 orders per restaurant
    int num_orders;
};

int calculateRouteTime(Order orders[], int num_orders) ;

void Routes_planning(Restaurant restaurants[], int riders, int num_restaurants);

int main() 
{
    ofstream outputFile("input.txt");
    if (!outputFile) 
    {
        cerr << "Unable to create output file." << endl;
        return 1;
    }

    int num_test_cases;
    cout << "Enter the number of test cases: ";
    cin >> num_test_cases;
    outputFile << num_test_cases << endl;

    for (int t = 0; t < num_test_cases; ++t) 
    {
        int N, riders, restaurants;
        cout << "Enter grid size, number of riders, and number of restaurants for test case " << t + 1 << ": ";
        cin >> N >> riders >> restaurants;
        outputFile << N << " " << riders << " " << restaurants << endl;

        Restaurant restaurant_list[10]; // Maximum 10 restaurants
        int num_restaurants = 0;

        for (int r = 0; r < restaurants; ++r) 
        {
            cout << "Enter restaurant name, location, and number of orders for restaurant " << r + 1 << ": ";
            cin >> restaurant_list[num_restaurants].name >> restaurant_list[num_restaurants].location >> restaurant_list[num_restaurants].num_orders;
            outputFile << restaurant_list[num_restaurants].name << " " << restaurant_list[num_restaurants].location << " " << restaurant_list[num_restaurants].num_orders << endl;

            for (int o = 0; o < restaurant_list[num_restaurants].num_orders; ++o) 
            {
                cout << "Enter order name, location, and delivery time limit for order " << o + 1 << " of restaurant " << r + 1 << ": ";
                cin >> restaurant_list[num_restaurants].orders[o].name >> restaurant_list[num_restaurants].orders[o].location >> restaurant_list[num_restaurants].orders[o].delivery_time_limit;
                outputFile << restaurant_list[num_restaurants].orders[o].name << " " << restaurant_list[num_restaurants].orders[o].location << " " << restaurant_list[num_restaurants].orders[o].delivery_time_limit << endl;
            }
            num_restaurants++;
        }
    }

    outputFile.close();

    ifstream inputFile("input.txt");
    if (!inputFile) 
    {
        cerr << "Unable to open input file." << endl;
        return 1;
    }

    inputFile >> num_test_cases;

    for (int t = 0; t < num_test_cases; ++t) 
    {
        int N, riders, restaurants;
        inputFile >> N >> riders >> restaurants;

        Restaurant restaurant_list[10]; // Maximum 10 restaurants
        int num_restaurants = 0;

        for (int r = 0; r < restaurants; ++r) 
        {
            inputFile >> restaurant_list[num_restaurants].name >> restaurant_list[num_restaurants].location;
            inputFile >> restaurant_list[num_restaurants].num_orders;

            for (int o = 0; o < restaurant_list[num_restaurants].num_orders; ++o) 
            {
                inputFile >> restaurant_list[num_restaurants].orders[o].name >> restaurant_list[num_restaurants].orders[o].location >> restaurant_list[num_restaurants].orders[o].delivery_time_limit;
            }
            num_restaurants++;
        }

        Routes_planning(restaurant_list, riders, num_restaurants);
    }
    return 0;
}


int calculateRouteTime(Order orders[], int num_orders) 
{
    int graph[num_orders][num_orders];                          // Graph Initialization
    for (int i = 0; i < num_orders; ++i) 
    {
        for (int j = 0; j < num_orders; ++j) 
        {
            if (i == j) 
            {
                graph[i][j] = 0;
            } 
            else 
            {
                graph[i][j] = abs(orders[i].location - orders[j].location);    // Calculating Distance
            }
        }
    }

    int distances[num_orders];                  // Initialization Of Dijkstra Algorithm
    int previous[num_orders];
    for (int i = 0; i < num_orders; ++i) 
    {
        distances[i] = INT_MAX;
        previous[i] = -1;
    }
    distances[0] = 0;

    for (int i = 0; i < num_orders; ++i)            // Dijkstra Algorithm
    {
        int u = -1;                                     // selecting next vertex
        for (int j = 0; j < num_orders; ++j) 
        {
            if (distances[j] < INT_MAX && (u == -1 || distances[j] < distances[u])) 
            {
                u = j;
            }
        }

        for (int v = 0; v < num_orders; ++v)            // updating distances to neighbour vertices
        {
            if (graph[u][v] != 0 && distances[u] + graph[u][v] < distances[v]) 
            {
                distances[v] = distances[u] + graph[u][v];
                previous[v] = u;
            }
        }
    }

    int total_time = 0;                             // Total Time Calculation
    for (int i = 0; i < num_orders; ++i) 
    {
        total_time += distances[i];
    }

    return total_time;
}

void Routes_planning(Restaurant restaurants[], int riders, int num_restaurants) 
{
    int total_delivery_time = 0;
    for (int i = 0; i < num_restaurants; ++i) 
    {
        cout << "Rider " << (i % riders) + 1 << ": ";
        cout << restaurants[i].location << " ";
        for (int j = 0; j < restaurants[i].num_orders; ++j) 
        {
            cout << restaurants[i].orders[j].location << " ";
        }
        cout << "= " << calculateRouteTime(restaurants[i].orders, restaurants[i].num_orders) << " time units" << endl;
        total_delivery_time += calculateRouteTime(restaurants[i].orders, restaurants[i].num_orders);
    }
    cout << "Total: " << total_delivery_time << " time units" << endl;
}
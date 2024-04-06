#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <algorithm>
#include <set>


const int INF = 1e8;
int counter = 0;

class City
{
public:
    int N; // количество перекрестков
    int M; // количество улиц

    std::vector<std::vector<std::pair<int, int>>> graph;
    std::vector<std::vector<int>> buses;
    std::vector<std::vector<int>> distances;
    std::vector<std::vector<int>> next;

    City() {
        std::cin >> N >> M;
        graph.resize(N);
        distances.resize(N, std::vector<int>(N, INF));
        next.resize(N, std::vector<int>(N, -1));

        for(int i = 0; i < M; ++i) {
            int u, v, k;
            std::cin >> u >> v >> k;
            graph[u].push_back(std::make_pair(v, k));
        }
      get_shortest_paths();
    }

    void get_shortest_paths()
    {
      for(int i = 0; i < N; ++i)
      {
        for(const auto &edge: graph[i])
        {
            int v = edge.first;
            int w = edge.second;
            distances[i][v] = w;
            next[i][v] = v;
        }
      }
      for (int i = 0; i < N; ++i) 
      {
        distances[i][i] = 0;
        next[i][i] = i;
      }

      for(int k = 0; k < N; ++k)
      {
        for(int i = 0; i < N; ++i)
        {
            for(int j = 0; j < N; ++j)
            {
                if(distances[i][k] != INF && distances[k][j] != INF && distances[i][j] > distances[i][k] + distances[k][j])
                {
                    distances[i][j] = distances[i][k] + distances[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}





  void print_graph()
  {
    for(int i = 0; i < N; ++i)
    {
      for(int j = 0; j < graph[i].size(); ++j)
        {
          std::cout << "street: " << i << graph[i][j].first << " ->> " << "time = " << graph[i][j].second << std::endl;
        }
    }
  }

};

void get_user_input(City &myCity); 



int main() {
  City myCity;


  std::thread input(get_user_input, std::ref(myCity));
  input.join();
  
  
}




void get_user_input(City &myCity)
{
  std::string command;
  std::cin >> command;

  if(command == "add_bus")
  {
    int k;
    std::cin >> k;
    std::vector<int> bus_route;
    std::set<std::pair<int, int>> unique_crossroads;
    int start_crossroad;
    for(int i = 0; i <= k; ++i) 
      {
        int crossroad;
        std::cin >> crossroad;
        if(crossroad >= 0 || crossroad < myCity.N)
        {
          bus_route.push_back(crossroad);
          if(i == 0) start_crossroad = crossroad;
          if(i != 0) unique_crossroads.insert(std::make_pair(myCity.distances[start_crossroad][crossroad], crossroad));
        }
        else
        {
          std::cerr << "Crossroad " << crossroad << " doesn't exist.\n";
        }
      }
    //добавляем маршрут в вектор маршрутов автобусов
    myCity.buses.push_back(bus_route);
    std::pair<int, int> min_crossroad  = *unique_crossroads.begin();
    std::cout << "The closest destination from crossroad " << start_crossroad << " is " << min_crossroad.second << std::endl;
    std::cout << "the minimum time required for the bus to reach from " << start_crossroad << "to " << min_crossroad.second << " is " << min_crossroad.first << std::endl;
    counter += 1;
  }
  else 
  {
    std::cerr << "Unknown command: " << command << "\n";
  }
  
}
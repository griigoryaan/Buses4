#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <algorithm>
#include <set>
#include <queue>

const int INF = 1e8;
int counter = 0;
const int threads_number = 2; 

class City
{
public:
    int N; 
    int M; 

    std::vector<std::vector<std::pair<int, int>>> graph;
    std::vector<std::vector<int>> buses;
    std::vector<std::vector<int>> distances;
    std::vector<std::vector<int>> next;

    City() {
        std::cin >> N >> M;
        graph.resize(N);
        distances.resize(N, std::vector<int>(N, INF));
        next.resize(N, std::vector<int>(N, -1));
        for(int i = 0; i < M; ++i) 
        {
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

    std::vector<int> get_common_streets(int bus1, int bus2) 
    {
    std::vector<int> stops_bus1 = buses[bus1];
    std::vector<int> stops_bus2 = buses[bus2];
    int start_stop_bus1 = stops_bus1[0];
    int start_stop_bus2 = stops_bus2[0];
    std::vector<int> parent_bus1 = dijkstra(start_stop_bus1);
    std::vector<int> parent_bus2 = dijkstra(start_stop_bus2);
    std::vector<int> common_streets;
    for (int stop : stops_bus2) 
    {
        int current_stop = stop;
        while (current_stop != -1) 
        {
            if (std::find(stops_bus1.begin(), stops_bus1.end(), current_stop) != stops_bus1.end()) 
            {
                common_streets.push_back(current_stop);
                break;
            }
            current_stop = parent_bus2[current_stop];
        }
    }
  return common_streets;
}




//just for us in order to check the graph 
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

private:
    std::vector<int> dijkstra(int start) 
    {
      std::vector<int> dist(N, INF);
      std::vector<int> parent(N, -1);
      dist[start] = 0;
      std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
      pq.push({0, start});
      while (!pq.empty()) 
      {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();
        if (d > dist[u]) continue;
        for (auto& edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return parent;
}
};

void get_user_input(City &myCity); 
void get_user_input_2 (City &myCity);
void get_user_input_3(City &myCity);


int main() {
  City myCity;

  for(int i = 0; i < threads_number; ++i) 
  {
    std::thread input(get_user_input, std::ref(myCity));
    input.join();
  }

  std::cout << "before ubdating:" << "\n";
  for(int i = 0; i < myCity.buses.size(); ++i)
  {
      std::cout << "bus number " << i << ":" << "\n";
      for(int j = 1; j < myCity.buses[i].size(); ++j)
      {
          std::cout << "duration from " << myCity.buses[i][0] << " to " << myCity.buses[i][j] << " - " << myCity.distances[myCity.buses[i][0]][myCity.buses[i][j]]<< "\n";
      }
      std::cout << "\n";
  }

  std::cout << "after ubdating:" << "\n";
  std::thread input_second(get_user_input_2, std::ref(myCity));
  input_second.join();


  std::thread input_third(get_user_input_3, std::ref(myCity));
  input_third.join();

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
    std::cout << "the minimum time required for the bus to reach from " << start_crossroad << " to " << min_crossroad.second << " is " << min_crossroad.first << std::endl;
    counter += 1;
  }
  else 
  {
    std::cerr << "Unknown command: " << command << "\n";
  }

}
void get_user_input_2 (City &myCity)
{
    std::string command;
    std::cin >> command;

    if(command == "Construct_crossroad")
    {
        // Добавляем новый перекресток в город
        myCity.graph.push_back(std::vector<std::pair<int, int>>());
        myCity.N += 1;
        int k;
        std::cin >> k;
        myCity.M += k;
        // Пересчитываем размеры векторов distances и next
        myCity.distances.resize(myCity.N, std::vector<int>(myCity.N, INF));
        myCity.next.resize(myCity.N, std::vector<int>(myCity.N, -1));

        for(int i = 0; i < k; ++i) 
        {
            int u, v;
            std::cin >> u >> v ;
            if(u >= 0 || u < myCity.N)
            {
                myCity.graph[u].push_back(std::make_pair(myCity.N-1, v));
            }
            else
            {
                std::cerr << "Crossroad " << u << " doesn't exist.\n";
            }
        }
        int m;
        std::cin >> m;
        myCity.M += m;
        for(int i = 0; i < m; ++i)
        {
            int u, v;
            std::cin >> u >> v;
            if(u >= 0 || u < myCity.N)
            {
                myCity.graph[myCity.N-1].push_back(std::make_pair(u, v));
            }
            else
            {
                std::cerr << "Crossroad " << u << " doesn't exist.\n";
            }
        }

        // Вызываем метод get_shortest_paths для пересчета кратчайших расстояний
        myCity.get_shortest_paths();
        // Выводим информацию о маршрутах автобусов
        for(int i = 0; i < myCity.buses.size(); ++i)
        {
            std::cout << "bus number " << i << ":" << "\n";
            for(int j = 1; j < myCity.buses[i].size(); ++j)
            {
                std::cout << "duration from " << myCity.buses[i][0] << " to " << myCity.buses[i][j] << " - " << myCity.distances[myCity.buses[i][0]][myCity.buses[i][j]]<< "\n";
            }
            std::cout << "\n";
        }
    }
    else 
    {
        std::cerr << "Unknown command: " << command << "\n";
    }
}


void get_user_input_3(City &myCity) {
    std::string command;
    std::cin >> command;

    if (command == "common_street") {
        int idx1, idx2;
        std::cin >> idx1 >> idx2;

        // Проверяем корректность введенных индексов
        if (idx1 < 0 || idx1 >= myCity.buses.size() || idx2 < 0 || idx2 >= myCity.buses.size()) {
            std::cerr << "Invalid bus indices.\n";
            return;
        }

        // Получаем общие улицы между маршрутами автобусов
        std::vector<int> common_streets = myCity.get_common_streets(idx1, idx2);

        // Выводим общие улицы
        std::cout << "Common streets between bus " << idx1 << " and bus " << idx2 << ":\n";
        for (int street : common_streets) {
            std::cout << street << std::endl;
        }
    } else {
        std::cerr << "Unknown command: " << command << "\n";
    }
}


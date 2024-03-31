#include <iostream>
#include <vector>
#include <thread>
#include <string>


class City
{
public:
  int N;
  int M; 
  std::vector<std::vector<std::pair<int, int>>> graph;
  std::vector<std::vector<int>> buses;


  City()
  {
    std::cin >> N >> M;
    graph.resize(N);
    for(int i = 0; i < M; ++i)
      {
        int u, v, k;
        std::cin >> u >> v >> k;
        graph[u].push_back(std::make_pair(v, k));
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
    for(int i = 0; i < k; ++i)
      {
        int crossroad;
        std::cin >> crossroad;
        //проверяем, что перекресток существует
        if(crossroad >= 0 || crossroad < myCity.N)
        {
          bus_route.push_back(crossroad);
        }
        else
        {
          std::cerr << "Crossroad " << crossroad << " doesn't exist.\n";
        }
      }
    //добавляем маршрут в вектор маршрутов автобусов
    myCity.buses.push_back(bus_route);
  }
  else 
  {
    std::cerr << "Unknown command: " << command << "\n";
  }
  
}
import math

from aco_tsp_algorithm import ACO_Graph, ACO
from aco_tsp_plot import aco_plot

def load_tsp(file_name: str, struct: list, points: list) -> int:
    """
    :param file_name: nazwa pliku zawirającego instancje TSP
    :param: struct: nazwa listy pod którą ma być zapisana instacja [{index,x,y}]
    :param: points: nazwa listy do punktów wykresu

    Wczytanie instancji TSP do struktury danych listy słowników,
    zwrócenie ilości wierzchołków. Wczytuje również współrzędne do wykresu [[x,y]]
    """

    i = 0  # obsługa odczytania pierwszej lini
    vertex_number = 0  # ilości wierzchołków

    file = open(file_name, "r")
    lines_raw = file.readlines()
    file.close()

    for line in lines_raw:
        if i == 0:
            vertex_number = int(line.rstrip("\n"))
            i = i + 1
            continue

        line_tmp = line.rstrip("\n").split(" ")
        struct_tmp = {"index": int(line_tmp[0]), "x": int(line_tmp[1]), "y": int(line_tmp[2])}
        points.append([line_tmp[1],line_tmp[2]])
        struct.append(struct_tmp)

    return vertex_number


def calculate_distances(city_a: list, city_b: list) -> float:
    """
    :param city_a: dict zawierający wszystjie punkty index, x, y
    :param city_a: dict zawierający wszystjie punkty index, x, y

    Wyznaczenie wszystkich odległoś pomiędzy miastem A -> B
    """

    return math.sqrt((city_a["x"] - city_b["x"]) ** 2 + (city_a["y"] - city_b["y"]) ** 2)



if __name__ == "__main__":
    """Algorytm ACO dla TSP"""

    # główna tablica z miastami[{index,x,y}...]
    cities = []
    # tablic a do punktów wykresu [[x,y]...]
    plot_points = []
    # podanie nazwy pliku z instancją TSP
    tsp_file_name = input("Podaj nazwę pliku z instancją TSP: ")
    # result_file_name = input("Podaj nazwę pliku wynikowego: ")
    # wczytanie instancji TSP do listy, zwrócenie liczny wierzchołków
    cities_number = load_tsp(tsp_file_name, cities, plot_points)

    # wyliczenie tablicy odległości dla każdego z miast
    distances_cities = []
    for a in range(cities_number):
        tmp = []
        for b in range(cities_number):
            tmp.append(calculate_distances(cities[a], cities[b]))
        distances_cities.append(tmp)

    # wybór parametró dla metaherustyki
    aco = ACO(30, 100, 10.0, 1.0, 0.5, 10, 2)
    graph = ACO_Graph(distances_cities, cities_number)
    path, cost = aco.solve_tsp(graph)
    cost = round(cost)

    print('długość: {}'.format(cost))
    aco_plot(plot_points, path)

    path = [x + 1 for x in path]
    print('ścieżka: {}'.format(path))

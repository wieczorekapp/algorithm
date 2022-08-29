import math
import time

from aco_tsp_plot import aco_plot

def load_tsp(file_name, struct, points: list):
    """
    Wczytanie instancji TSP do struktury danych listy słowników,
    zwrócenie ilości wierzchołków
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
        struct_tmp = {"index": int(line_tmp[0]), "X": int(line_tmp[1]), "Y": int(line_tmp[2]), "available": True}
        points.append([line_tmp[1],line_tmp[2]])
        struct.append(struct_tmp)

    return vertex_number


def calculate_vertex(index, vertex_number, struct):
    """
    Wyznaczenie do którego z nieodwiedzonych wierzchołków jest najbliżej.
    Zwrócenie jesgo indeksu oraz dystansu zaokrąglonego do 2 miejsc po przecinku.
    Odwiedzone wierzchołki tzn available == False nie sąbrane pod uwagę
    """

    min_value = 0
    min_index = 0

    for i in range(vertex_number):
        if not struct[i].get("available"):
            # pominięcie bez obliczeń odwiedzonych wierzchołków lub samego siebie
            continue

        tmp = math.sqrt((struct[i].get("X") - struct[index].get("X")) ** 2  + (struct[i].get("Y") - struct[index].get("Y")) ** 2)


        if tmp < min_value or min_value == 0:
            min_value = tmp
            min_index = i

    struct[min_index]["available"] = False
    return min_index, min_value


if __name__ == "__main__":
    """Algorytm zachłanny dla TSP"""

    plot_points = []

    struct = [] # główna struktura z danymi dla instancji
    file_name = input("Podaj nazwę pliku: ")
    vertex_number = load_tsp(file_name, struct, plot_points)

    # punktem startu jest wierzchołek o indeksie 1[0 w tablicy]
    start_index = 0
    vartex_acctula_index = 0
    result_v = []
    result_d = 0

    result_v.append(struct[vartex_acctula_index].get("index"))
    struct[0]["available"] = False
    time_start = time.time()

    for i in range(vertex_number - 1):
        vartex_acctula_index, vertex_distance_tmp = calculate_vertex(vartex_acctula_index, vertex_number, struct)

        # budowanie wyniku
        result_v.append(struct[vartex_acctula_index].get("index"))
        result_d = result_d + vertex_distance_tmp

    result_d = result_d + math.sqrt((struct[0].get("X") - struct[vartex_acctula_index].get("X")) ** 2  + (struct[0].get("Y") - struct[vartex_acctula_index].get("Y")) ** 2)

    time_end = time.time()
    time_all = round(time_end - time_start)
    result_d = round(result_d)


    result_v.append(result_v[0])
    print('cost: {}, path: {}'.format(result_d, result_v))

    result_v = [x - 1 for x in result_v]
    aco_plot(plot_points, result_v)

   # with open("result.txt", "w") as file:
   #     file.write(f"Liczba wierzcholkow: {vertex_number}\n")
   #     file.write(f"Dlugosc drogi: {result_d}\n")
   #     file.write(f"Czas obliczen: {time_all}\n")

   #     for i in result_v:
   #         file.write(f"{i}\n")


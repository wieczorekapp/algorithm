# biblioteka std do funkcji losowych
import random
from timeit import default_timer as timer

class ACO_Graph:
    """
    Klasa zawierająca strukturę potrzebną do ACO, to znaczy
    listę z odległościami z każdego wierzchołka do każdego innego, listę wierzchołków
    oraz współczynnik feromonów na każdej krawędzi w tym grafie(pomiędzy każdą parą miast)
    """

    def __init__(self, distances_cities: list, cities_number: int):
        """
        :param distances_cities: listę z odległościami z każdego wierzchołka do każdego innego
        :param cities_number: liczba miast
        """

        self.distances_cities = distances_cities
        self.cities_number = cities_number
        self.pheromone = []

        # zainicjalizowanie listy z wartościami feromonu, początkowo takie same dla każdej krawędzi
        for a in range(self.cities_number):
            tmp = []
            for b in range(self.cities_number):
                tmp.append(1/(cities_number * cities_number)) # UWAGA możliwe, że inna wartość, dyskusyjne !!!
            self.pheromone.append(tmp)


class ACO:
    """
    Klasa implementująca metaheurystykę Ant Colony System
    Przyjmuje parametry wywołania metaheurystki, zawiera metodę rozwiązującą
    przy pomocy zadanych wartości. Aktualizuje po generacji warości feromonu w grafie dla kolejnej generacji
    """

    def __init__(self, ant_count: int, generations: int, alpha: float, beta: float, rho: float, q: int, strategy: int):
        """
        :param ant_count: liczba mrówek w każdej generacji przechodzących przez graf
        :param generations: liczba "pokoleń mrówek", późnijsze pokolenie ma wiedzę w postaci wartości feromonu po wcześnijszym pokoleniu
        :param alpha: znaczenie wartości feromonu przy działąniu algorytmu wyboru
        :param beta: zanczenie informacji o długości danej odległości między miastem A -> B
        :param rho: współczynnik przez jaki są przemnożone wszytkie wartości feromonu przy aktualizacji feromonu po generacji, współcznnik wiettrzenia feromonu
        :param q: wartość intensywności feromonu, który zostwia po sobie mrówka jeśli wybierze daną krawędź
        :param strategy: 2 => Q / odległość między A -> B;
                         1 => Q;
                         0 => Q/ długość całej ścieżki wytyczonej;
        """

        self.ant_count = ant_count
        self.generations = generations
        self.alpha = alpha
        self.beta = beta
        self.rho = rho
        self.Q = q
        self.update_strategy = strategy


    def solve_tsp(self, graph: ACO_Graph):
        """
        :param graph: przekazujemy dla jakiej instancji ma zostać rozwiązany TSP dla zainicjalizowanych parametrów

        Metoda inicjalizująca rozpoczęcie działania metaheurystki
        """

        # początkowo jako odniesienie nieskończoność by akceptować pierwszy wynik bez znacznia wielkości
        best_distance = float('inf')
        # list zawierjąca najlepsze w danym momencie znane rozwiązanie
        best_path = []
        # start pomiaru czasu
        time_start = timer()

        # iteracja równa ilości ustalonych generacji
        for gen in range(self.generations):
            # ograniczenie czasu działania algorytmu
            time_control = timer()
            time_condition = round(time_control - time_start)
            print(f"Czas {time_condition} s")

            # zwracamy dotychczasowy najlepszy wynik
            if time_condition > 300:
                return best_path, best_distance

            print(f"Numer generacji {gen}")
            # wygenerowanie odpowiedniej liczby mrówek
            ants = [Ant(self, graph) for i in range(self.ant_count)]

            for ant in ants:
                # każda mrówka przechodzi przez cały graf
                for i in range(graph.cities_number - 1):
                    # metoda wybierająca do któego z wierzchołków skierować się
                    ant.choose_city()
                # doliczenie do długości odcinka powrotu na początek z ostatniego wierzchołka
                ant.total_cost += graph.distances_cities[ant.visited[-1]][ant.visited[0]]
                # sprawdzenie czy znalezione rozwiązanie jest lepsze od do tej pory najlepszego
                if ant.total_cost < best_distance:
                    best_distance = ant.total_cost
                    # lista odwiedzonych wartości przez mrówkę jest równoznaczna z przebytą przez nią ścieżką
                    best_path = [] + ant.visited

                # każda mrówka wyznacza wartości feromonu jakie "naniesie" jej przejście na odpowiednich krawędziach
                ant.update_pheromone_local()

            # wykonanie aktulizacji globalnej po całej generacji mrówek(dodanie sumy feromonu do odpowidnich krawędzi)
            self.update_pheromone(graph, ants)
            # TESTOWE
            #print('generacja #{}, najlepszy dystans: {}, path: {}'.format(gen, best_distance, best_path))

        # dodanie krawędzi powrotu dla najlepszego odnalezionego raozwiązania po n generacjach koloni mrówek
        best_path.append(best_path[0])
        # zwrócenie najlpszego cyklu i jego długość
        return best_path, best_distance

    def update_pheromone(self, graph: ACO_Graph, ants: list):
        """
        :param graph: dla jakiego grafu zaktualizować wartość listy feromonów
        :param ants: lista mrówek które przeszły wa danej generacji graf, na poodstawie ich list feromonów aktualizacja

        Metoda dodatkowo wykorzystuje współczynik rho wietrzenia feromonu, przez który zawsze wszystkie wartości są przemnożone
        jako, że jest to ułamek to skaluje to wartości
        """

        for i, row in enumerate(graph.pheromone):
            for j, col in enumerate(row):
                # wietrzenie feromonu zganie z parametrem
                graph.pheromone[i][j] *= self.rho
                # dla danej krawędzi dodanie wartości feromonów dla każdej mróki co przeszła w danej generacji krawędzią
                for ant in ants:
                    graph.pheromone[i][j] += ant.pheromone_local[i][j]


class Ant:
    """
    Klasa implementująca pojedynczą mrówkę, jej przejście przez graf
    oraz wygenerowanie wartości feromonu dla jej przejścia
    """

    def __init__(self, aco: ACO, graph: ACO_Graph):
        """
        :param aco: obiekt ACO z parametrami
        :param gaph: przeszukiwana instancja
        """

        self.colony = aco # Informacje o parametrtach ACO
        self.graph = graph # Jaki graf jest przeszukiwany
        self.total_cost = 0.0 # długość ścieżki dla mróki
        # lista dzięki której wiem, które miasta od razu odrzucamy, zawiera również przyrastjące rozwiązanie dla danej mróki
        self.visited = []
        self.pheromone_local = []  # lokalna tablica feromonu lda danej mrówki
        # tablica które wierzchołki są dostępne do odwiedzenia, początkowo są wszystkie
        self.availabled = [i for i in range(graph.cities_number)]
        # informacja heurystyczna dla odległości względem każdego miasta(dla siebie samego jest to 0)
        self.eta = [[0 if i == j else 1 / graph.distances_cities[i][j] for j in range(graph.cities_number)] for i in
                    range(graph.cities_number)]
        # wybieramy losowo punkt w którym mróka zaczyna poszukiwać ścieżkę
        start = random.randint(0, graph.cities_number - 1)

        self.visited.append(start) # dodanie miasta do listy odwiedzonych(rozwiązania)
        self.current = start # obecnym miastem jest wylosowane(potrzebne do dalszego wyszukiwania)
        self.availabled.remove(start) # usuwamy miasto z listy dostępnych w kolejnej iteracji





    def choose_city(self):
        denominator = 0
        # wyliczenie minaowanika spośró dostępnych jeszcze miast
        for i in self.availabled:
            denominator += self.graph.pheromone[self.current][i] ** self.colony.alpha * self.eta[self.current][
                                                                                            i] ** self.colony.beta

        # tablica prawdopodobiwństaw do udania się do danego wierzchołka, początkowo same 0
        probabilities = [0 for i in range(self.graph.cities_number)]
        for i in range(self.graph.cities_number):
            try:
                self.availabled.index(i)
                probabilities[i] = self.graph.pheromone[self.current][i] ** self.colony.alpha * \
                    self.eta[self.current][i] ** self.colony.beta / denominator
            except ValueError:
                pass  # obsługa wyjątku poprzez brak akcji, sytuacja jeśli wierzchołek jest wybrany, prawdopodonieństwo wtedy 0

        # zmienna pod która podstawimy indeks miasta do któego sięudamy dzięki
        selected = 0
        # losowa wartość z zakresu [0,1)
        rand = random.random()
        for i, probability in enumerate(probabilities):
            # w każdej iteracji odejmujemy prawdopodobieństwo udania się do danego wierzchołka
            # odwiedzone, tzn 0 wierzchołki nie mogą byćwięc odwidzne bo nigdy nie zminą wartości logicznej nieróności
            rand -= probability
            if rand <= 0:
                # pierwszyszy spełnijące nieróność miasto jest wybrane
                selected = i
                break
        # usunięcie miasta z listy dostępnych w kolejnej iteracji
        self.availabled.remove(selected)
        # dodanie do listy odwiedzonych(wykluczonych przy szukaniu), kolejna część wyniku dla danej mrówki
        self.visited.append(selected)
        # dodanie koszt dsytansu pomiędzy beiżacy miastem a wybranym dla danego grafu
        self.total_cost += self.graph.distances_cities[self.current][selected]
        # zmiana bieżaćego wierzchołka od którego będzeimy dalej szukać
        self.current = selected


    def update_pheromone_local(self):
        """
        Wyznaczenie według wybranej strategi lokalnych wartości feromonu naniesionych przez mrówkę
        """

        self.pheromone_local = [[0 for j in range(self.graph.cities_number)] for i in range(self.graph.cities_number)]

        # przesunięcie bo porównujemy parami a-1 i a
        for a in range(1, len(self.visited)):
            i = self.visited[a - 1]
            j = self.visited[a]
            if self.colony.update_strategy == 1:
                self.pheromone_local[i][j] = self.colony.Q
            elif self.colony.update_strategy == 2:
                self.pheromone_local[i][j] = self.colony.Q / self.graph.distances_cities[i][j]
            else:
                self.pheromone_local[i][j] = self.colony.Q / self.total_cost
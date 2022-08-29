import random

if __name__ == "__main__":
    """
    Program generujący losową instancję dla TSP.
    Można wybrać nazwę instancji zapisanej do pliku,
    ilość wierzchołków wykoloswnych(wszystkie są unikatowe i nie krawędziowe),
    oraz wymiar boku kwadratowej powierzchni
    """

    file_name = input("Podaj nazwę pliku: ")
    vertex_number = int(input("Ilość wierzchołków: "))
    area_side = int(input("Długość boku przestrzeni[kwadratowa]: "))

    X = 0
    Y = 0

    # pomocnicze zmienne do ustalenie unikalności punktu na płaszczyźnie
    generated = []
    tmp = ()

    with open(file_name, "w") as file:
        for i in range(vertex_number + 1):
            # obsługa pierwzego wiersza pliku z instancją
            if i == 0:
                file.write(f"{vertex_number}\n")

            else:
                # losowanie współrzędnych
                X = random.randint(1, area_side)
                Y = random.randint(1, area_side)
                tmp = (X, Y)

                # punkty nie będą się pokrywać
                while tmp in generated:
                    X = random.randint(1, area_side)
                    Y = random.randint(1, area_side)
                    tmp = (X, Y)

                generated.append(tmp)
                file.write(f"{i} {X} {Y}\n")

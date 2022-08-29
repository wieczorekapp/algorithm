import operator

import matplotlib.pyplot as plt


def aco_plot(points, path: list):
    x = []
    y = []
    for point in points:
        x.append(int(point[0]))
        y.append(int(point[1]))
    # noinspection PyUnusedLocal
    #y = list(map(operator.sub, [max(y) for i in range(len(points))], y))
    plt.plot(x, y, 'co')

    for a in range(1, len(path)):
        i = int(path[a - 1])
        j = int(path[a])
        # print(f"i:{i} j:{j}")
        # noinspection PyUnresolvedReferences
        plt.arrow(x[i], y[i], x[j] - x[i], y[j] - y[i], color='r', length_includes_head=True)

    # noinspection PyTypeChecker
    plt.xlim(0, max(x) * 1.1)
    # noinspection PyTypeChecker
    plt.ylim(0, max(y) * 1.1)
    plt.show()
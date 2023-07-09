import matplotlib.pyplot as plt
from argparse import ArgumentParser

def parse_segments_set(path_to_file):
    with open(path_to_file) as _file:
        line = _file.readline()

    list_of_coordinates = line.split(' ')
    x_coords = []
    y_coords = []

    for idx in range(0, len(list_of_coordinates), 4):
        x_coords.append(float(list_of_coordinates[idx]))
        x_coords.append(float(list_of_coordinates[idx + 2]))
        y_coords.append(float(list_of_coordinates[idx + 1]))
        y_coords.append(float(list_of_coordinates[idx + 3]))

    return x_coords, y_coords

def main():
    parser = ArgumentParser()
    parser.add_argument('-i', '--input', required=True, type=str, help='Path to file with segments set')
    args = parser.parse_args()

    x_coords, y_coords = parse_segments_set(args.input)
    for idx in range(0, len(x_coords), 2):
        plt.plot([x_coords[idx], x_coords[idx + 1]], [y_coords[idx], y_coords[idx + 1]])
        plt.text((x_coords[idx] + x_coords[idx + 1]) / 2, (y_coords[idx] + y_coords[idx + 1]) / 2, str(idx // 2))
    plt.show()

if __name__ == '__main__':
    main()

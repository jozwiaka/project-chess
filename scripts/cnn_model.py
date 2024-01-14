import sys


def read_data():
    args = sys.argv
    if len(args) > 1:
        return str(args[1])


def predict_move():
    return "E4"


print(read_data())
print(predict_move())
